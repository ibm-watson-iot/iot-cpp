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
#include "IOTP_GatewayClient.h"
#include <iostream>
//#include "IOTF_ActionCallback.h"

namespace Watson_IOTP {

// GatewayClient constructor with properties instance
IOTP_GatewayClient::IOTP_GatewayClient(Properties& prop,std::string logPropertiesFile):
	gatewayCMDTopic(""),deviceCMDTopic(""),IOTP_Client(prop,logPropertiesFile)
{

}

// GatewayClient constructor with properties file
IOTP_GatewayClient::IOTP_GatewayClient(const std::string& filePath,std::string logPropertiesFile):
	gatewayCMDTopic(""),deviceCMDTopic(""),IOTP_Client(filePath,logPropertiesFile)
{

}

/**
 * Connect to Watson IoT Platform messaging server using default options.
 *
 * @return bool
 * returns true if connection succeeds else fasle
 * @throw MQTT exceptions
 */
bool IOTP_GatewayClient::connect()
	throw(mqtt::exception, mqtt::security_exception) {
	std::string methodName = __PRETTY_FUNCTION__;
	logger.debug(methodName+" Entry: ");
	bool rc = false;
	if(InitializeMqttClient()){
		console.info("Gateway Client connecting to " + mServerURI +
			" with client-id " + mClientID);
		rc = IOTP_Client::connect();

		if(rc)
			subscribeGatewayCommands();
		else
			console.error("Gateway Client connecting to " + mServerURI + "failed !!!");
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
void IOTP_GatewayClient::publishGatewayEvent(char *eventType, char *eventFormat, const char* data, int qos) {
	std::string methodName = __PRETTY_FUNCTION__;
	logger.debug(methodName+" Entry: ");
	std::string publishTopic= "iot-2/type/"+std::string(mProperties.getdeviceType()) +
				"/id/"+std::string(mProperties.getdeviceId())+"/evt/" +
				std::string(eventType)+"/fmt/"+std::string(eventFormat);
	std::string payload = data;
	logger.debug("publishTopic - " + publishTopic);
	logger.debug("payload - " + payload);
	mqtt::message_ptr pubmsg = std::make_shared < mqtt::message > (data);
	pubmsg->set_qos(qos);
	logger.debug("Calling method publishTopic()...");
	mqtt::idelivery_token_ptr delivery_tok = this->publishTopic(publishTopic, pubmsg);
	delivery_tok->wait_for_completion(DEFAULT_TIMEOUT());
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
void IOTP_GatewayClient::publishGatewayEvent(char *eventType, char *eventFormat, const char* data, int qos,  mqtt::iaction_listener& cb) {
	std::string methodName = __PRETTY_FUNCTION__;
	logger.debug(methodName+" Entry: ");
	std::string publishTopic= "iot-2/type/"+std::string(mProperties.getdeviceType()) +
				"/id/"+std::string(mProperties.getdeviceId())+"/evt/" +
				std::string(eventType)+"/fmt/"+std::string(eventFormat);
	std::string payload = data;
	logger.debug("publishTopic - " + publishTopic);
	logger.debug("payload - " + payload);
	mqtt::message_ptr pubmsg = std::make_shared < mqtt::message > (data);
	pubmsg->set_qos(qos);
	logger.debug("Calling method publishTopic() with given callback...");
	mqtt::idelivery_token_ptr delivery_tok = this->publishTopic(publishTopic, pubmsg, NULL, cb);
	logger.debug(methodName+" Exit: ");
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
void IOTP_GatewayClient::publishDeviceEvent(char* deviceType, char* deviceId, char *eventType, char *eventFormat, const char* data, int qos) {
	std::string methodName = __PRETTY_FUNCTION__;
	logger.debug(methodName+" Entry: ");
	std::string publishTopic= "iot-2/type/"+std::string(deviceType)+"/id/" +
				std::string(deviceId)+"/evt/"+std::string(eventType) +
				"/fmt/"+std::string(eventFormat);
	std::string payload = data;
	logger.debug("publishTopic - " + publishTopic);
	logger.debug("payload - " + payload);
	mqtt::message_ptr pubmsg = std::make_shared < mqtt::message > (data);
	pubmsg->set_qos(qos);
	logger.debug("Calling method publishTopic() ...");
	mqtt::idelivery_token_ptr delivery_tok = this->publishTopic(publishTopic, pubmsg);
	delivery_tok->wait_for_completion(DEFAULT_TIMEOUT());
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
void IOTP_GatewayClient::publishDeviceEvent(char* deviceType, char* deviceId, char *eventType, char *eventFormat,
		const char* data, int qos, mqtt::iaction_listener& cb) {
	std::string methodName = __PRETTY_FUNCTION__;
	logger.debug(methodName+" Entry: ");
	std::string publishTopic = "iot-2/type/" + std::string(deviceType) + "/id/" +
				std::string(deviceId) + "/evt/" + std::string(eventType) +
				"/fmt/" + std::string(eventFormat);
	std::string payload = data;
	logger.debug("publishTopic - " + publishTopic);
	logger.debug("payload - " + payload);
	mqtt::message_ptr pubmsg = std::make_shared < mqtt::message > (data);
	pubmsg->set_qos(qos);
	logger.debug("Calling method publishTopic() with given callback...");
	mqtt::idelivery_token_ptr delivery_tok = this->publishTopic(publishTopic, pubmsg, NULL, cb);
	logger.debug(methodName+" Exit: ");
}

/**
 * Function used to subscribe commands from the IBM Watson IoT service
 * @return bool
 * returns true if commands are subscribed successfully else false
 */
bool IOTP_GatewayClient::subscribeGatewayCommands() {
	std::string methodName = __func__;
	logger.debug(methodName+" Entry: ");
	gatewayCMDTopic = "iot-2/type/"+std::string(mProperties.getdeviceType())+"/id/"+std::string(mProperties.getdeviceId())+"/cmd/+/fmt/+";
	int qos = 1;
	logger.debug("Calling subscribeTopic() for " + gatewayCMDTopic);
	bool rc = this->subscribeTopic(gatewayCMDTopic, qos);
	logger.debug(methodName+" Exit: ");
	return rc;
}

/**
 * Function used to subscribe commands from the IBM Watson IoT service
 * @return bool
 * returns true if commands are subscribed successfully else false
 */
bool IOTP_GatewayClient::subscribeDeviceCommands(char* deviceType, char* deviceId) {
	std::string methodName = __func__;
	logger.debug(methodName+" Entry: ");
	deviceCMDTopic = "iot-2/type/"+std::string(deviceType)+"/id/"+std::string(deviceId)+"/cmd/+/fmt/+";
	int qos = 1;
	logger.debug("Calling subscribeTopic() for " + deviceCMDTopic);
	bool rc = this->subscribeTopic(deviceCMDTopic, qos);
	logger.debug(methodName+" Exit: ");
	return rc;
}

/**
* Function used to disconnect from the IBM Watson IoT Service.
* Removes any command related subsriptions and calls the base class diconnect.
**/
void IOTP_GatewayClient::disconnect(){
	std::string methodName = __PRETTY_FUNCTION__;
	logger.debug(methodName+" Entry: ");
	if(gatewayCMDTopic.size() > 0){
		logger.debug("Calling unsubscribeCommands() for the topic - " + gatewayCMDTopic);
		unsubscribeCommands(gatewayCMDTopic);
	}

	if(deviceCMDTopic.size() >0){
		logger.debug("Calling unsubscribeCommands() for the topic - " + deviceCMDTopic);
		unsubscribeCommands(deviceCMDTopic);
	}

	IOTP_Client::disconnect();
	logger.debug(methodName+" Exit: ");
}

bool IOTP_GatewayClient::InitializeMqttClient() {
	std::string methodName = __func__;
	logger.debug(methodName+" Entry: ");
	bool rc = true;
	if(mProperties.getorgId().size() == 0){
		console.error(methodName+ ": Organization-ID can not be empty / null");
		rc = false;
	}
	else if(mProperties.getorgId().compare("quickstart") == 0 ||
		mProperties.getorgId().compare("QuickStart") == 0 ||
		mProperties.getorgId().compare("QUICKSTART") == 0){
		console.error(methodName+ ": Organization-ID can not be quickstart for gateway");
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
	else{
		std::string serverURI = "tcp://" + mProperties.getorgId() + ".messaging."
			+ mProperties.getdomain() + ":1883";
		std::string clientId = "g:" + mProperties.getorgId() + ":" + mProperties.getdeviceType()
			+ ":" + mProperties.getdeviceId();

		mServerURI = serverURI;
		mClientID = clientId;
		logger.debug("serverURI: " + serverURI);
		logger.debug("clientId: " + clientId);

		pasync_client = new mqtt::async_client(serverURI, clientId);
		logger.debug("Underlying async_client created...");
	}

	logger.debug(methodName+" Exit: ");
	return rc;
}

} /* namespace Watson_IOTP */
