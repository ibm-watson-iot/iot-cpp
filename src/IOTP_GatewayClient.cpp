/*
 * IOTP_GatewayClient.cpp
 *
 *  Created on: 28-Jul-2016
 *      Author: amit
 */

#include "IOTP_GatewayClient.h"
#include <iostream>
//#include "IOTF_ActionCallback.h"

namespace Watson_IOTP {

IOTP_GatewayClient::IOTP_GatewayClient(Properties& prop): IOTP_Client(prop) {
	InitializeMqttClient();
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
	bool ret = IOTP_Client::connect();
	subscribeGatewayCommands();
	return ret;
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
	std::string publishTopic= "iot-2/type/"+std::string(mProperties.getdeviceType())+"/id/"+std::string(mProperties.getdeviceId())+"/evt/"+std::string(eventType)+"/fmt/"+std::string(eventFormat);
	std::string payload = data;
	mqtt::message_ptr pubmsg = std::make_shared < mqtt::message > (data);
	pubmsg->set_qos(qos);
	mqtt::idelivery_token_ptr delivery_tok = this->publishTopic(publishTopic, pubmsg);
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
void IOTP_GatewayClient::publishGatewayEvent(char *eventType, char *eventFormat, const char* data, int qos,  mqtt::iaction_listener& cb) {
		std::string publishTopic= "iot-2/type/"+std::string(mProperties.getdeviceType())+"/id/"+std::string(mProperties.getdeviceId())+"/evt/"+std::string(eventType)+"/fmt/"+std::string(eventFormat);
		std::string payload = data;
		mqtt::message_ptr pubmsg = std::make_shared < mqtt::message > (data);
		pubmsg->set_qos(qos);
		mqtt::idelivery_token_ptr delivery_tok = this->publishTopic(publishTopic, pubmsg, NULL, cb);
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
	std::string publishTopic= "iot-2/type/"+std::string(deviceType)+"/id/"+std::string(deviceId)+"/evt/"+std::string(eventType)+"/fmt/"+std::string(eventFormat);
	std::string payload = data;
	mqtt::message_ptr pubmsg = std::make_shared < mqtt::message > (data);
	pubmsg->set_qos(qos);
	mqtt::idelivery_token_ptr delivery_tok = this->publishTopic(publishTopic, pubmsg);
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
void IOTP_GatewayClient::publishDeviceEvent(char* deviceType, char* deviceId, char *eventType, char *eventFormat,
		const char* data, int qos, mqtt::iaction_listener& cb) {
	std::string publishTopic = "iot-2/type/" + std::string(deviceType) + "/id/"
			+ std::string(deviceId) + "/evt/" + std::string(eventType) + "/fmt/"
			+ std::string(eventFormat);
	std::string payload = data;
	mqtt::message_ptr pubmsg = std::make_shared < mqtt::message > (data);
	pubmsg->set_qos(qos);
	mqtt::idelivery_token_ptr delivery_tok = this->publishTopic(
			publishTopic, pubmsg, NULL, cb);
}

/**
 * Function used to subscribe commands from the IBM Watson IoT service
 * @return bool
 * returns true if commands are subscribed successfully else false
 */
bool IOTP_GatewayClient::subscribeGatewayCommands() {
	std::string topic = "iot-2/type/"+std::string(mProperties.getdeviceType())+"/id/"+std::string(mProperties.getdeviceId())+"/cmd/+/fmt/+";
	int qos = 1;
		bool ret = this->subscribeTopic(topic, qos);
		return ret;
}

/**
 * Function used to subscribe commands from the IBM Watson IoT service
 * @return bool
 * returns true if commands are subscribed successfully else false
 */
bool IOTP_GatewayClient::subscribeDeviceCommands(char* deviceType, char* deviceId) {
	std::string topic = "iot-2/type/"+std::string(deviceType)+"/id/"+std::string(deviceId)+"/cmd/+/fmt/+";
	int qos = 1;
		bool ret = this->subscribeTopic(topic, qos);
		return ret;
}

void IOTP_GatewayClient::InitializeMqttClient() {
	std::string serverURI = "tcp://" + mProperties.getorgId() + ".messaging."
			+ mProperties.getdomain() + ":1883";
	std::string clientId = "g:" + mProperties.getorgId() + ":" + mProperties.getdeviceType()
			+ ":" + mProperties.getdeviceId();
	pasync_client = new mqtt::async_client(serverURI, clientId);
}

} /* namespace Watson_IOTP */
