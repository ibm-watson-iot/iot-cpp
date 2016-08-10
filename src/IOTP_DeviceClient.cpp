/*
 * IOTP_DeviceClient.cpp
 *
 *  Created on: 01-Aug-2016
 *      Author: amit
 */

#include "IOTP_DeviceClient.h"
#include "IOTP_TopicDefinitions.h"
#include <iostream>

namespace Watson_IOTP {

// IOTP_DeviceClient constructors and methods
IOTP_DeviceClient::IOTP_DeviceClient(Properties& prop) :mDevAttributeHandler (nullptr),
		IOTP_Client(prop){
	InitializeMqttClient();
}

// IOTP_DeviceClient constructors and methods
IOTP_DeviceClient::IOTP_DeviceClient(Properties& prop, iotf_device_data_ptr& deviceData) :
		mDevAttributeHandler (nullptr), mDeviceData(deviceData), IOTP_Client(prop)  {
	InitializeMqttClient();
}

// IOTP_DeviceClient constructors and methods
IOTP_DeviceClient::IOTP_DeviceClient(Properties& prop, iotf_device_data_ptr& deviceData,
		iotp_device_action_handler_ptr& actionHandler,
		iotp_device_firmware_handler_ptr& firmwareHandler,
		iotp_device_attribute_handler_ptr& devAttributeHandler) :IOTP_Client(prop,
				actionHandler, firmwareHandler), mDeviceData(deviceData),
mDevAttributeHandler(devAttributeHandler){
	InitializeMqttClient();
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
void IOTP_DeviceClient::publishEvent(char *eventType, char *eventFormat, const char* data, int qos) {
	std::string publishTopic= "iot-2/evt/"+std::string(eventType)+"/fmt/"+std::string(eventFormat);
	std::string payload = data;
	mqtt::message_ptr pubmsg = std::make_shared < mqtt::message > (data);
	pubmsg->set_qos(qos);
	mqtt::idelivery_token_ptr delivery_tok = this->publishTopic(publishTopic, pubmsg);
	//delivery_tok->wait_for_completion(DEFAULT_TIMEOUT());
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
void IOTP_DeviceClient::publishEvent(char *eventType, char *eventFormat, const char* data, int qos,  mqtt::iaction_listener& cb) {
		std::string publishTopic= "iot-2/evt/"+std::string(eventType)+"/fmt/"+std::string(eventFormat);
		std::string payload = data;
		mqtt::message_ptr pubmsg = std::make_shared < mqtt::message > (data);
		pubmsg->set_qos(qos);
		mqtt::idelivery_token_ptr delivery_tok = this->publishTopic(publishTopic, pubmsg, NULL, cb);
}

/**
 * Function used to subscribe commands from the IBM Watson IoT service
 * @return bool
 * returns true if commands are subscribed successfully else false
 */
bool IOTP_DeviceClient::subscribeCommands() {
	std::string topic = "iot-2/cmd/+/fmt/+";
	int qos = 1;
	bool ret = this->subscribeTopic(topic, qos);
	return ret;
}

bool IOTP_DeviceClient::manage() {
	if (this->subscribeCommandHandler(SERVER_RESPONSE_TOPIC, mResponseHandler)) {
		Json::Value jsonManageData;

		jsonManageData["lifetime"] = mLifetime;
		jsonManageData["supports"]["deviceActions"] = supportDeviceActions();
		jsonManageData["supports"]["firmwareActions"] = supportFirmwareActions();
		//jsonManageData["deviceInfo"] = mDeviceInfo->toJsonValue();
		jsonManageData["deviceInfo"] = mDeviceData->getDeviceInfo()->toJsonValue();

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
				if (mDevAttributeHandler != nullptr) {
					this->subscribeCommandHandler(SERVER_UPDATE_TOPIC, mDevAttributeHandler);
					//Need to add the support if required
					//this->subscribeCommandHandler(SERVER_OBSERVE_TOPIC, mDevAttributeHandler);
					//this->subscribeCommandHandler(SERVER_CANCEL_TOPIC, mDevAttributeHandler);
				}

			}

			return (rc == 200);
		}
	}
	return false;
}

bool IOTP_DeviceClient::unmanage() {
	Json::Value nullData;
	std::string reqId = send_message(DEVICE_UNMANAGE_TOPIC, nullData);
	this->unsubscribeCommands(SERVER_RESPONSE_TOPIC);
	return (!reqId.empty());
}

bool IOTP_DeviceClient::update_device_location(IOTP_DeviceLocation& deviceLocation) {
	return pushManageMessage(DEVICE_UPDATE_LOCATION_TOPIC, deviceLocation.toJsonValue());
//		if (this->subscribeCommandHandler(SERVER_RESPONSE_TOPIC, mResponseHandler)) {
//			std::string reqId = send_message(DEVICE_UPDATE_LOCATION_TOPIC, deviceLocation.toJsonValue());
//			if (!reqId.empty()) {
//				Json::Value res = mResponseHandler->wait_for_response(DEFAULT_TIMEOUT(), reqId);
//				int rc = res.get("rc", -1).asInt();
//				return (rc == 200);
//			}
//		}
//		return false;
}

/**
 * Function used to add diagnostic error codes of the device to the IBM Watson IoT platform
 * @return bool
 */
bool IOTP_DeviceClient::addErrorCodes(int num) {
	Json::Value jsonPayload;
	jsonPayload["errorCode"] = num;
	return pushManageMessage(DEVICE_ADD_ERROR_CODES_TOPIC, jsonPayload);
}

/**
 * Function used to clear diagnostic error codes of the device from the IBM Watson IoT platform
 * @return void
 */
bool IOTP_DeviceClient::clearErrorCodes() {
	Json::Value jsonPayload;
	return pushManageMessage(DEVICE_CLEAR_ERROR_CODES_TOPIC, jsonPayload);
}

/**
 * Function used to add diagnostic logs of the device to the IBM Watson IoT platform
 * @return bool
 */
bool IOTP_DeviceClient::addLogs(IOTP_DeviceLog& deviceLog) {
	return pushManageMessage(DEVICE_ADD_DIAG_LOG_TOPIC, deviceLog.toJsonValue());
}

/**
 * Function used to clear diagnostic logs of the device from the IBM Watson IoT platform
 * @return void
 */
bool IOTP_DeviceClient::clearLogs() {
	Json::Value jsonPayload;
	return pushManageMessage(DEVICE_CLEAR_DIAG_LOG_TOPIC, jsonPayload);
}

void IOTP_DeviceClient::InitializeMqttClient() {
	std::string serverURI = "tcp://" + mProperties.getorgId() + ".messaging."+mProperties.getdomain()+":1883";
	std::string clientId = "d:" + mProperties.getorgId() + ":" + mProperties.getdeviceType() + ":" + mProperties.getdeviceId();
	pasync_client = new mqtt::async_client(serverURI, clientId);
}

} /* namespace Watson_IOTP */
