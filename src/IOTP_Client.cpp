/*******************************************************************************
 * Copyright (c) 2016 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Mike Tran - initial API and implementation and/or initial documentation
 *    Hari Prasada Reddy - Added functionalities/documentation to standardize with other client libraries
 *******************************************************************************/

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <typeinfo>
#include <utility>
//#include <vector>

#include "IOTP_Client.h"
#include "IOTP_ReplyMessage.h"
#include "mqtt/connect_options.h"
#include "mqtt/delivery_token.h"
#include "mqtt/message.h"
#include "mqtt/token.h"
#include "json/json.h"
#include "mqtt/async_client.h"
#include "mqtt/exception.h"
#include "IOTP_TopicDefinitions.h"

namespace Watson_IOTP {

//	const std::string& SERVER_RESPONSE_TOPIC{
//		static std::string* str = new std::string ("iotdm-1/response");
//		return *str;
//	}

	const unsigned long DEFAULT_TIMEOUT() {
		static unsigned long default_timeout = 10000;
		return default_timeout;
	}

	//IOTP_Callback methods
	IOTP_Client::IOTF_Callback::IOTF_Callback(IOTP_Client* iotf_client) : mArrivedMessages(0), user_callback(nullptr), mClient(iotf_client) {}

	IOTP_Client::IOTF_Callback::~IOTF_Callback() {
		if (mMessages.size() > 0) {
			std::cout << typeid(*this).name() << " Number of messages not processed " << mMessages.size() << std::endl;
			mMessages.clear();
		}

		if (mHandlers.size() > 0) {
			std::cout << typeid(*this).name() << " Number of handlers not removed " << mHandlers.size() << std::endl;
			mHandlers.clear();
		}

		if (mSubscriptions.size() > 0) {
			std::cout << typeid(*this).name() << " Number of subscriptions not removed " << mSubscriptions.size() << std::endl;
			mSubscriptions.clear();
		}
	}

	void IOTP_Client::IOTF_Callback::connection_lost(const std::string& cause) {
		std::cout << typeid(*this).name() << " Connection lost" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;
	}

	void IOTP_Client::IOTF_Callback::message_arrived(const std::string& topic, mqtt::message_ptr msg) {
		Json::Value jsonPayload;
		Json::Reader reader;
		iotp_message_handler_ptr handler = nullptr;
		iotp_reply_message_ptr reply;

		//guard g1(mLock);
		mArrivedMessages++;

		std::pair <std::multimap<std::string,iotp_message_handler_ptr>::iterator, std::multimap<std::string,iotp_message_handler_ptr>::iterator> ret;
		ret = mHandlers.equal_range(topic);
		for (std::multimap<std::string,iotp_message_handler_ptr>::iterator it=ret.first; it!=ret.second; ++it) {
			handler = it->second;
			if (reader.parse(msg->get_payload(), jsonPayload)) {
				reply = handler->message_arrived(topic, jsonPayload);
			} else {
				reply = handler->message_arrived(topic, msg);
			}

			//If the handler requested to send a reply to the IoT Platform,
			//put the reply message on the queue to be sent.
			if (reply != nullptr) {
				mClient->IOTF_send_reply(reply);
			}
		}

		if (handler == nullptr && user_callback) {
			std::cout << "calling user_callback\n";
			std::string::size_type pos = topic.find("cmd/");	//"iot-2/cmd/+/fmt/+"
			std::string::size_type nxtpos = topic.find_first_of('/', pos+4);
			std::string command = topic.substr(pos+4,nxtpos-(pos+4));
			pos = topic.find("fmt/",nxtpos+1);
			nxtpos = topic.find_first_of('/', pos+4);
			std::string format = topic.substr(pos+4,nxtpos);
			std::string payload = msg->get_payload();

			Command cmd(command, format, payload);

			user_callback->processCommand(cmd);
		}
	}

	void IOTP_Client::IOTF_Callback::delivery_complete(mqtt::idelivery_token_ptr tok) {
		//std::cout << "Delivery complete for token: " << (tok ? tok->get_message_id() : -1) << std::endl;
	}

	void IOTP_Client::IOTF_Callback::set_callback(CommandCallback* cb) {
		user_callback = cb;
	}

	std::vector<std::string> IOTP_Client::IOTF_Callback::get_subscriptions() {
		return mSubscriptions;
	}

	void IOTP_Client::IOTF_Callback::add_subscription(std::string topic) {
		mSubscriptions.push_back(topic);
	}

	void IOTP_Client::IOTF_Callback::add_subscription(std::string topic, iotp_message_handler_ptr handler) {
		mSubscriptions.push_back(topic);
		mHandlers.insert(std::pair<std::string,iotp_message_handler_ptr>(topic, handler));
	}

	void IOTP_Client::IOTF_Callback::remove_subscription(std::string topic) {
		std::vector<std::string>::iterator it;
		it = std::find(mSubscriptions.begin(), mSubscriptions.end(), topic);
		if (it != mSubscriptions.end()) {
			mSubscriptions.erase(it);
		}
	}

	void IOTP_Client::IOTF_Callback::remove_subscription(std::string topic, iotp_message_handler_ptr handler) {
		for (std::multimap<std::string, iotp_message_handler_ptr>::iterator it=mHandlers.begin(); it!=mHandlers.end(); ++it) {
			if ((*it).first == topic && (*it).second == handler) {
				mHandlers.erase(it);
				break;
			}
		}
	}

	bool IOTP_Client::IOTF_Callback::check_subscription(std::string topic) {
		std::vector<std::string>::iterator it;
		it = std::find(mSubscriptions.begin(), mSubscriptions.end(), topic);
		return (it != mSubscriptions.end());
	}

	bool IOTP_Client::IOTF_Callback::check_subscription(std::string topic, iotp_message_handler_ptr handler) {
		bool found = false;
		for (std::multimap<std::string, iotp_message_handler_ptr>::iterator it=mHandlers.begin(); it!=mHandlers.end(); ++it) {
			if ((*it).first == topic && (*it).second == handler) {
				found = true;
				break;
			}
		}
		return found;
	}

	int IOTP_Client::IOTF_Callback::get_arrived_messages() { return mArrivedMessages; }

	// IOTF_ActionCallback methods
	IOTP_Client::IOTF_ActionCallback::IOTF_ActionCallback() : mSuccess(false) {}

	/**
	 * This method is invoked when an action fails.
	 * @param asyncActionToken
	 */
	void IOTP_Client::IOTF_ActionCallback::on_failure(const mqtt::itoken& asyncActionToken) {
		mSuccess = false;
	}
	/**
	 * This method is invoked when an action has completed successfully.
	 * @param asyncActionToken
	 */
	void IOTP_Client::IOTF_ActionCallback::on_success(const mqtt::itoken& asyncActionToken) {
		mSuccess= true;
	}

	bool IOTP_Client::IOTF_ActionCallback::success() {
		return mSuccess;
	}

	//IOTP_Client Initializing parameters
	void IOTP_Client::InitializeProperties(Properties& prop) {
		std::string serverURI = "tcp://" + prop.getorgId() + ".messaging."+prop.getdomain()+":1883";
		std::string clientId = "d:" + prop.getorgId() + ":" + prop.getdeviceType() + ":" + prop.getdeviceId();
		pasync_client = new mqtt::async_client(serverURI, clientId);
		mExit = false;
		mReqCounter = 0;
		connectOptions.set_clean_session(true);
		std::string usrName = prop.getauthMethod();
		connectOptions.set_user_name(usrName);

		std::string passwd = prop.getauthToken();
		connectOptions.set_password(passwd);
		mActionHandler = nullptr;
		mFirmwareHandler = nullptr;
		mResponseHandler = std::make_shared<IOTP_ResponseHandler> ();
		mReplyThread = std::thread(&IOTP_Client::_send_reply, this);
	}

	// IOTF_Client constructors and methods
	IOTP_Client::IOTP_Client(Properties& prop) {
		InitializeProperties(prop);
	}

	// IOTF_Client constructors and methods
	IOTP_Client::IOTP_Client(Properties& prop, Watson_IOTP::IOTP_DeviceInfo* deviceInfo,
		iotp_device_action_handler_ptr& actionHandler, iotp_device_firmware_handler_ptr& firmwareHandler) {
		InitializeProperties(prop);
		mDeviceInfo  = deviceInfo;
		mActionHandler = actionHandler;
		mFirmwareHandler = firmwareHandler;
	}

	// IOTF_Client constructors and methods
	IOTP_Client::IOTP_Client(Properties& prop, Watson_IOTP::IOTP_DeviceInfo* deviceInfo) :
		mDeviceInfo (deviceInfo) {
		InitializeProperties(prop);
	}

	IOTP_Client::~IOTP_Client() {
		mExit = true;
		mReplyThread.join();
		delete pasync_client;
	}


	/**
	 * Function sets the "keep Alive" interval.
	 *
	 * @param keepAliveInterval
	 * @return void
	 *
	 */
	void IOTP_Client::setKeepAliveInterval(int keepAliveInterval) {
		connectOptions.set_keep_alive_interval(keepAliveInterval);
	}

	/**
	 * Connect to Watson IoT Platform messaging server using default options.
	 *
	 * @return int
	 * returns 1 if connection succeeds else 0
	 * @throw MQTT exceptions
	 */
	bool IOTP_Client::connect()
		throw(mqtt::exception, mqtt::security_exception) {
		IOTF_ActionCallback action;

		std::string usrName = connectOptions.get_user_name();
		if(usrName.compare("token") != 0) {
			std::cout<<"wrong auth-Method supplied.Platform supports auth-method:\"token\""<<std::endl;
			return false;
		}
		else
		{
			usrName = "use-token-auth";
			connectOptions.set_user_name(usrName);
		}

		mqtt::itoken_ptr conntok = pasync_client->connect(connectOptions, NULL, action);
		conntok->wait_for_completion(DEFAULT_TIMEOUT());
		if (action.success()) {
			callback_ptr = set_callback();
		}

		if (conntok ->is_complete() == false)
			return false;

		return true;
	}

	/**
	 * Connect to Watson IoT Platform messaging server using default options.
	 *
	 * @param cb Action listener callback
	 * @return int
	 * returns 1 if connection succeeds else 0
	 * @throw MQTT exceptions
	 */
	bool IOTP_Client::connect(mqtt::iaction_listener& cb)
					throw(mqtt::exception, mqtt::security_exception) {
		IOTF_ActionCallback action;
		mqtt::itoken_ptr conntok = pasync_client->connect(connectOptions, NULL, action);
		conntok->wait_for_completion(DEFAULT_TIMEOUT());

		if (action.success() == true) {
			cb.on_success(*conntok);
			callback_ptr = set_callback();
		} else {
			cb.on_failure(*conntok);
		}

		if (conntok->is_complete() == false)
			return false;

		return true;
	}


	IOTP_Client::iotf_callback_ptr IOTP_Client::set_callback() {
		IOTF_Callback* cb = new IOTF_Callback(this);
		pasync_client->set_callback(*cb);
		return iotf_callback_ptr(cb);
	}

	/**
	 * Function to set the set the command handler
	 *
	 * @param cb Command callback pointer
	 * @return void
	 */
	void IOTP_Client::setCommandHandler(CommandCallback* cb) {
			callback_ptr->set_callback(cb);
	}

	/**
	* Function used to Publish events from the device to the IBM Watson IoT service
	* @param eventType - Type of event to be published e.g status, gps
	* @param eventFormat - Format of the event e.g json
	* @param data - Payload of the event
	* @param QoS - qos for the publish event. Supported values : 0, 1, 2
	*
	* @return void
	*/
	void IOTP_Client::publishEvent(char *eventType, char *eventFormat, const char* data, int qos) {
		std::string publishTopic= "iot-2/evt/"+std::string(eventType)+"/fmt/"+std::string(eventFormat);
		std::string payload = data;
		mqtt::message_ptr pubmsg = std::make_shared < mqtt::message > (data);
		pubmsg->set_qos(qos);
		mqtt::idelivery_token_ptr delivery_tok = pasync_client->publish(publishTopic, pubmsg);
		delivery_tok->wait_for_completion(DEFAULT_TIMEOUT());
		mqtt::message_ptr msgPtr = delivery_tok->get_message();
	}

	/**
	* Function used to Publish events from the device to the IBM Watson IoT service
	* @param eventType - Type of event to be published e.g status, gps
	* @param eventFormat - Format of the event e.g json
	* @param data - Payload of the event
	* @param QoS - qos for the publish event. Supported values : 0, 1, 2
	* @param iaction_listener& cb - call back function for action listner
	* @return void
	*/
	void IOTP_Client::publishEvent(char *eventType, char *eventFormat, const char* data, int qos,  mqtt::iaction_listener& cb) {
			std::string publishTopic= "iot-2/evt/"+std::string(eventType)+"/fmt/"+std::string(eventFormat);
			std::string payload = data;
			mqtt::message_ptr pubmsg = std::make_shared < mqtt::message > (data);
			pubmsg->set_qos(qos);
			mqtt::idelivery_token_ptr delivery_tok = pasync_client->publish(publishTopic, pubmsg, NULL, cb);
	}

	/**
	 * Function used to Publish messages on a specific topic to the IBM Watson IoT service
	 * @param topic - topic on which message is sent
	 * @param message - message to be posted
	 * @return mqtt::idelivery_token_ptr
	 */
	mqtt::idelivery_token_ptr IOTP_Client::publish(std::string topic, mqtt::message_ptr message) {
		mqtt::idelivery_token_ptr delivery_tok = pasync_client->publish(topic, message);
		return delivery_tok;
	}

	/**
	 * Function used to subscribe commands from the IBM Watson IoT service
	 * @return bool
	 * returns true if commands are subscribed successfully else false
	 */
	bool IOTP_Client::subscribeCommands() {
		std::string topic = "iot-2/cmd/+/fmt/+";
		int qos = 1;
		if (callback_ptr->check_subscription(topic) == false) {
			mqtt::itoken_ptr tok = pasync_client->subscribe(topic, qos);
			tok->wait_for_completion(DEFAULT_TIMEOUT());
			if (tok->is_complete()) {
				callback_ptr->add_subscription(topic);
				return true;
			}
		} else {
			return true;
		}
		//If we come here, something is wrong.
		return false;
	}

	/**
	 * Function used to subscribe handler for each topic from the IBM Watson IoT service
	 * @return bool
	 * returns true if topic is subscribed successfully else false
	 */
	bool IOTP_Client::subscribeCommandHandler(const std::string& topic, iotp_message_handler_ptr handler) {
		int qos =1;
		if (callback_ptr->check_subscription(topic, handler) == false) {
			mqtt::itoken_ptr tok = pasync_client->subscribe(topic, qos);
			tok->wait_for_completion(DEFAULT_TIMEOUT());
			if (tok->is_complete()) {
				callback_ptr->add_subscription(topic, handler);
				handler->mClient = this;
				return true;
			}
		} else {
			return true;
		}
		//If we come here, something is wrong.
		return false;
	}

	/**
	 * Function used to unsubscribe topic from the IBM Watson IoT service
	 * @return bool
	 * returns true if topic is unsubscribed successfully else false
	 */
	bool IOTP_Client::unsubscribeCommands(const std::string& topic) {
		if (callback_ptr->check_subscription(topic) == true) {
			mqtt::itoken_ptr tok = pasync_client->unsubscribe(topic);
			tok->wait_for_completion(DEFAULT_TIMEOUT());
			if (tok->is_complete()) {
				callback_ptr->remove_subscription(topic);
				return true;
			}
		}
		return false;
	}

	/**
	 * Function used to disconnect the device from the IBM Watson IoT service
	 * @return void
	 */
	void IOTP_Client::disconnect() throw(mqtt::exception) {
		mqtt::itoken_ptr conntok = pasync_client->disconnect();
		conntok->wait_for_completion();
	}

	/**
	 * Static helper method to convert a Json object to string.
	 */
	std::string IOTP_Client::jsonValueToString(Json::Value& jsonValue) {
		Json::FastWriter fastWriter;
		return fastWriter.write(jsonValue);
	}

	/**
	 * Put the reply message on the queue, then wake up the publish thread.
	 */
	void IOTP_Client::IOTF_send_reply(iotp_reply_message_ptr reply) {
		IOTP_ReplyMessage newReply(reply->getTopic(), reply->getPayload(), reply->getQos());
		iotp_reply_message_ptr replyPtr = std::make_shared<IOTP_ReplyMessage>(newReply);
		std::unique_lock<std::mutex> lck(this->mLock);
		this->mReplyMsgs.push(replyPtr);
		this->mCond.notify_one();
	}

	void IOTP_Client::_send_reply() {
		long timeout = DEFAULT_TIMEOUT();
		while (mExit == false) {
			std::unique_lock<std::mutex> lck(mLock);
			if (mCond.wait_for(lck, std::chrono::milliseconds(timeout)) != std::cv_status::timeout) {
				if (mReplyMsgs.empty() == false) {
					iotp_reply_message_ptr reply = mReplyMsgs.front();
					mReplyMsgs.pop();
					std::string topic(reply->getTopic());
					std::string jsonMessage(jsonValueToString(reply->getPayload()));
					//std::cout << "Sending TOPIC " << topic << " PAYLOAD " << jsonMessage << std::endl;
					mqtt::idelivery_token_ptr pubtok = pasync_client->publish(topic, (void*)jsonMessage.data(), jsonMessage.size(), reply->getQos(), false);
					pubtok->wait_for_completion(DEFAULT_TIMEOUT());
				} else {
					std::cout << "_send_reply: notified but queue is empty. Something went wrong." << std::endl;
				}
			}
			lck.unlock();
		}
	}

	/////////////////////////////////////////////////////
	bool IOTP_Client::supportDeviceActions() const {
		return (!(mActionHandler == nullptr));
	}

	bool IOTP_Client::supportFirmwareActions() const {
		return (!(mFirmwareHandler == nullptr));
	}

	bool IOTP_Client::manage() {
		if (this->subscribeCommandHandler(SERVER_RESPONSE_TOPIC, mResponseHandler)) {
			Json::Value jsonManageData;

			jsonManageData["lifetime"] = mLifetime;
			jsonManageData["supports"]["deviceActions"] = supportDeviceActions();
			jsonManageData["supports"]["firmwareActions"] = supportFirmwareActions();
			jsonManageData["deviceInfo"] = mDeviceInfo->toJsonValue();

			std::string reqId = send_message(DEVICE_MANAGE_TOPIC, jsonManageData);
			if (!reqId.empty()) {
				Json::Value res = mResponseHandler->wait_for_response(DEFAULT_TIMEOUT(), reqId);
				int rc = res.get("rc", -1).asInt();
				if (rc == 200) {
					if (mFirmwareHandler != nullptr) {
						this->subscribeCommandHandler(SERVER_UPDATE_TOPIC, mFirmwareHandler);
						this->subscribeCommandHandler(SERVER_OBSERVE_TOPIC, mFirmwareHandler);
						this->subscribeCommandHandler(SERVER_CANCEL_TOPIC, mFirmwareHandler);
						this->subscribeCommandHandler(SERVER_FIRMWARE_DOWNLOAD_TOPIC, mFirmwareHandler);
						this->subscribeCommandHandler(SERVER_FIRMWARE_UPDATE_TOPIC, mFirmwareHandler);
					}
					if (mActionHandler != nullptr) {
						this->subscribeCommandHandler(SERVER_DEVICE_REBOOT_TOPIC, mActionHandler);
						this->subscribeCommandHandler(SERVER_FACTORY_RESET_TOPIC, mActionHandler);
					}
				}

				return (rc == 200);
			}
		}
		return false;
	}

	bool IOTP_Client::unmanage() {
		Json::Value nullData;
		std::string reqId = send_message(DEVICE_UNMANAGE_TOPIC, nullData);
		this->unsubscribeCommands(SERVER_RESPONSE_TOPIC);
		return (!reqId.empty());
	}

	bool IOTP_Client::update_device_location(IOTP_DeviceLocation& deviceLocation) {
		if (this->subscribeCommandHandler(SERVER_RESPONSE_TOPIC, mResponseHandler)) {
			std::string reqId = send_message(DEVICE_UPDATE_LOCATION_TOPIC, deviceLocation.toJsonValue());
			if (!reqId.empty()) {
				Json::Value res = mResponseHandler->wait_for_response(DEFAULT_TIMEOUT(), reqId);
				int rc = res.get("rc", -1).asInt();
				return (rc == 200);
			}
		}
		return false;
	}

	std::string IOTP_Client::send_message(const std::string& topic, const Json::Value& data, int qos) {
		bool success = false;
		Json::Value jsonManagePayload;
		time_t now;
		time(&now);

		//FIXME: This should work, but the g++ library on windows does not have to_string() functions enabled by default
		//std::string reqId = std::to_string(mReqCounter++);
		//Use sstream for now

		std::ostringstream ss;
		ss << ++mReqCounter + now;
		std::string reqId = ss.str();

		if (data.isNull() == false)
			jsonManagePayload["d"] = data;

		jsonManagePayload["reqId"] = reqId;

		std::string jsonMessage = IOTP_Client::jsonValueToString(jsonManagePayload);

		mqtt::message_ptr pubmsg = std::make_shared<mqtt::message>(jsonMessage);
		pubmsg->set_qos(qos);
		mqtt::idelivery_token_ptr pubtok = this->publish(topic, pubmsg);
		pubtok->wait_for_completion(DEFAULT_TIMEOUT());
		success = pubtok->is_complete();

		if (success == false) {
			std::cout << "send_message FAILED to send message to TOPIC " << topic << " PAYLOAD " << std::endl << jsonMessage << std::endl << std::flush;
			reqId.clear();
		}
		return reqId;
	}
	/////////////////////////////////////////////////////////////////////////

}

