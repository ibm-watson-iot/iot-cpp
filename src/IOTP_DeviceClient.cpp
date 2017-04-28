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
 *    Hari Prasada Reddy - Initial implementation
 *    Lokesh Haralakatta - Updates to match with latest mqtt lib changes
 *    Lokesh Haralakatta - Added members to hold serverURI and clientID.
 *    Lokesh Haralakatta - Added logging feature using log4cpp.
 *******************************************************************************/

#include "IOTP_DeviceClient.h"
#include "IOTP_TopicDefinitions.h"
#include <iostream>

namespace Watson_IOTP {

// IOTP_DeviceClient constructor with properties instance
IOTP_DeviceClient::IOTP_DeviceClient(Properties& prop,std::string logPropertiesFile) :
	mDevAttributeHandler (nullptr), mDeviceData(nullptr),mLifetime(3600),
	IOTP_Client(prop,logPropertiesFile)
{

}

// IOTP_DeviceClient constructor with properties instance and deviceData
IOTP_DeviceClient::IOTP_DeviceClient(Properties& prop, iotf_device_data_ptr& deviceData,std::string logPropertiesFile) :
	mDevAttributeHandler (nullptr), mDeviceData(deviceData), mLifetime(3600),
	IOTP_Client(prop,logPropertiesFile)
{

}

// IOTP_DeviceClient constructor with properties and handlers
IOTP_DeviceClient::IOTP_DeviceClient(Properties& prop, iotf_device_data_ptr& deviceData,
		iotp_device_action_handler_ptr& actionHandler,
		iotp_device_firmware_handler_ptr& firmwareHandler,
		iotp_device_attribute_handler_ptr& devAttributeHandler,std::string logPropertiesFile) :
		IOTP_Client(prop,actionHandler, firmwareHandler,logPropertiesFile), mDeviceData(deviceData),
		mLifetime(3600), mDevAttributeHandler(devAttributeHandler)
{

}

// IOTP_DeviceClient constructor with properties file and log4cpp file
IOTP_DeviceClient::IOTP_DeviceClient(const std::string& filePath, std::string logPropertiesFile):
	mDevAttributeHandler (nullptr), mDeviceData(nullptr),mLifetime(3600),
	IOTP_Client(filePath,logPropertiesFile)
{

}

/**
 * Connect to Watson IoT Platform messaging server using default options.
 *
 * @return bool
 * returns true if connection succeeds else fasle
 * @throw MQTT exceptions
 */
bool IOTP_DeviceClient::connect()
	throw(mqtt::exception, mqtt::security_exception) {
	std::string methodName = __PRETTY_FUNCTION__;
	logger.debug(methodName+" Entry: ");
	bool rc = false;
	if(InitializeMqttClient()){
		if(mProperties.getuseCerts()){
			console.info("Device Client connecting using Client Certificates to " +
					mServerURI + " with client-id " + mClientID);
		}
		else {
			console.info("Device Client connecting to " + mServerURI + " with client-id " +
					mClientID);
		}

		rc = IOTP_Client::connect();

		if(rc){
		   if(mProperties.getorgId().compare("quickstart") != 0)
			subscribeCommands();
		}
		else
		   console.error("Device Client connecting to " + mServerURI + "failed !!!");
	}
	else {
		console.error("Initializing MQTT Client Failed...");
	}

	logger.debug(methodName+" Exit: ");
	return rc;
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
	std::string methodName = __PRETTY_FUNCTION__;
	logger.debug(methodName+" Entry: ");
	std::string publishTopic= "iot-2/evt/"+std::string(eventType)+"/fmt/"+std::string(eventFormat);
	std::string payload = data;
	logger.debug("publishTopic: " + publishTopic);
	logger.debug("payload: " + payload);
	mqtt::message_ptr pubmsg = std::make_shared < mqtt::message > (data);
	pubmsg->set_qos(qos);
	mqtt::idelivery_token_ptr delivery_tok = this->publishTopic(publishTopic, pubmsg);
	//delivery_tok->wait_for_completion(DEFAULT_TIMEOUT());
	mqtt::const_message_ptr msgPtr = delivery_tok->get_message();
	logger.debug(methodName+" Exit: ");
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
void IOTP_DeviceClient::publishEvent(char *eventType, char *eventFormat, const char* data, int qos,
	  				mqtt::iaction_listener& cb)
{
	std::string methodName = __PRETTY_FUNCTION__;
	logger.debug(methodName+" Entry: ");
	std::string publishTopic= "iot-2/evt/"+std::string(eventType)+"/fmt/"+std::string(eventFormat);
	std::string payload = data;
	logger.debug("publishTopic: " + publishTopic);
	logger.debug("payload: " + payload);
	mqtt::message_ptr pubmsg = std::make_shared < mqtt::message > (data);
	pubmsg->set_qos(qos);
	mqtt::idelivery_token_ptr delivery_tok = this->publishTopic(publishTopic, pubmsg, NULL, cb);
	logger.debug(methodName+" Exit: ");
}

/**
 * Function used to subscribe commands from the IBM Watson IoT service
 * @return bool
 * returns true if commands are subscribed successfully else false
 */
bool IOTP_DeviceClient::subscribeCommands() {
	std::string methodName = __func__;
	logger.debug(methodName+" Entry: ");
	int qos = 1;
	logger.debug("Calling subscribeTopic() for " + commandTopic);
	bool rc = this->subscribeTopic(commandTopic, qos);
	logger.debug(methodName+" Exit: ");
	return rc;
}

/**
* Function used to disconnect from the IBM Watson IoT Service.
* Removes any command related subsriptions and calls the base class diconnect.
**/
void IOTP_DeviceClient::disconnect(){
	std::string methodName = __PRETTY_FUNCTION__;
	logger.debug(methodName+" Entry: ");
	if(commandTopic.size() >0){
		logger.debug("Calling unsubscribeCommands() for the topic - " + commandTopic);
		unsubscribeCommands(commandTopic);
	}

	IOTP_Client::disconnect();
	logger.debug(methodName+" Exit: ");
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

bool IOTP_DeviceClient::InitializeMqttClient() {
	std::string methodName = __func__;
	logger.debug(methodName+" Entry: ");
	bool rc = true;
	if(mProperties.getorgId().size() == 0){
		console.error(methodName+ ": Organization-ID can not be empty / null");
		rc = false;
	}
	else if(mProperties.getdomain().size() == 0){
		console.error(methodName+ ": domain can not be empty / null");
		rc = false;
	}
	else if(mProperties.getdeviceType().size() == 0){
		console.error(methodName+ ": Device-Type can not be empty / null");
		rc = false;
	}
	else if(mProperties.getdeviceId().size() == 0){
		console.error(methodName+ ": Device-Id can not be empty / null");
		rc = false;
	}
	else if(mProperties.getorgId().compare("quickstart") != 0 &&
	 	mProperties.gettrustStore().size() == 0){
		console.error(methodName+ ": clientTrustStorePath can not be empty / null");
		rc = false;
	}
	else{
		if(mProperties.getorgId().compare("quickstart") != 0) {
			mServerURI = "ssl://" + mProperties.getorgId() + ".messaging." +
						mProperties.getdomain()+":8883";
		}
		else {
			mServerURI = "tcp://" + mProperties.getorgId() + ".messaging." +
						mProperties.getdomain()+":1883";
		}

		mClientID = "d:" + mProperties.getorgId() + ":" + mProperties.getdeviceType() +
						":" + mProperties.getdeviceId();

		logger.debug("serverURI: " + mServerURI);
		logger.debug("clientId: " + mClientID);

		pasync_client = new mqtt::async_client(mServerURI, mClientID);
		logger.debug("Underlying async_client created...");
	}

	logger.debug(methodName+" Exit: ");

	return rc;
}

} /* namespace Watson_IOTP */
