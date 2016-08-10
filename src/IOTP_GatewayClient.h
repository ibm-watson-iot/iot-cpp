/*
 * IOTP_GatewayClient.cpp
 *
 *  Created on: 28-Jul-2016
 *      Author: amit
 */

#include "IOTP_Client.h"

namespace Watson_IOTP {

class IOTP_GatewayClient: public IOTP_Client {

public:
	IOTP_GatewayClient(Properties& prop);
//	bool connect() throw (mqtt::exception, mqtt::security_exception);
//	bool connect(mqtt::iaction_listener& cb) throw (mqtt::exception, mqtt::security_exception);
	/**
	* Function used to Publish events from the device to the IBM Watson IoT service
	* @param eventType - Type of event to be published e.g status, gps
	* @param eventFormat - Format of the event e.g json
	* @param data - Payload of the event
	* @param QoS - qos for the publish event. Supported values : 0, 1, 2
	*
	* @return void
	*/
	void publishGatewayEvent(char *eventType, char *eventFormat, const char* data, int qos);

	/**
	* Function used to Publish events from the device to the IBM Watson IoT service
	* @param eventType - Type of event to be published e.g status, gps
	* @param eventFormat - Format of the event e.g json
	* @param data - Payload of the event
	* @param QoS - qos for the publish event. Supported values : 0, 1, 2
	* @param iaction_listener& cb - call back function for action listner
	* @return void
	*/
	void publishGatewayEvent(char *eventType, char *eventFormat, const char* data, int qos,  mqtt::iaction_listener& cb);

	/**
	* Function used to Publish events from the device to the IBM Watson IoT service
	* @param eventType - Type of event to be published e.g status, gps
	* @param eventFormat - Format of the event e.g json
	* @param data - Payload of the event
	* @param QoS - qos for the publish event. Supported values : 0, 1, 2
	*
	* @return void
	*/
	void publishDeviceEvent(char* deviceType, char* deviceId, char *eventType, char *eventFormat, const char* data, int qos);

	/**
	* Function used to Publish events from the device to the IBM Watson IoT service
	* @param eventType - Type of event to be published e.g status, gps
	* @param eventFormat - Format of the event e.g json
	* @param data - Payload of the event
	* @param QoS - qos for the publish event. Supported values : 0, 1, 2
	* @param iaction_listener& cb - call back function for action listner
	* @return void
	*/
	void publishDeviceEvent(char* deviceType, char* deviceId, char *eventType, char *eventFormat,
			const char* data, int qos, mqtt::iaction_listener& cb);

	/**
	 * Function used to subscribe commands from the IBM Watson IoT service
	 * @return bool
	 * returns true if commands are subscribed successfully else false
	 */
	bool subscribeGatewayCommands();

	/**
	 * Function used to subscribe commands from the IBM Watson IoT service
	 * @return bool
	 * returns true if commands are subscribed successfully else false
	 */
	bool subscribeDeviceCommands(char* deviceType, char* deviceId);


	~IOTP_GatewayClient(){}

private:
	void InitializeMqttClient();
};


} /* namespace Watson_IOTP */
