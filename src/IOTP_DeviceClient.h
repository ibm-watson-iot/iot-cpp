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

#ifndef SRC_IOTP_DEVICECLIENT_H_
#define SRC_IOTP_DEVICECLIENT_H_

#include "IOTP_Client.h"

namespace Watson_IOTP {

class IOTP_DeviceClient: public IOTP_Client {
public:
	/* Pointer type for this object */
	typedef std::shared_ptr<IOTP_DeviceClient> ptr_t;
	/*
	 * Constructor of an IOTP_Client.  The parameters are:
	 * Properties Instance
	 * log4cpp properties file path
	 */
	IOTP_DeviceClient(Properties& prop,std::string logPropertiesFile="log4cpp.properties");

	/*
	 * Constructor of an IOTP_Client.  The parameters are:
	 * Properties Instance
	 * deviceData Instance
	 * log4cpp properties file path
	 */
	IOTP_DeviceClient(Properties& prop, iotf_device_data_ptr& deviceData,
					std::string logPropertiesFile="log4cpp.properties");

	/*
	* Constructor of an IOTP_Client.  The parameters are:
	* Properties Instance
	* deviceData Instance
	* deviceAction handler
	* firmwareHandler
	* devAttribureHandler
	* log4cpp properties file path
	*/
	IOTP_DeviceClient(Properties& prop, iotf_device_data_ptr& deviceData,
			iotp_device_action_handler_ptr& actionHandler,
			iotp_device_firmware_handler_ptr& firmwareHandler,
			iotp_device_attribute_handler_ptr& devAttribureHandler,
			std::string logPropertiesFile="log4cpp.properties");

	/*
	* Constructor of an IOTP_Client.  The parameters are:
	* WIoTP configuration file path
	* log4cpp properties file path
	*/
	IOTP_DeviceClient(const std::string& filePath, std::string logPropertiesFile="log4cpp.properties");

	~IOTP_DeviceClient(){}

	/**
	 * Connect to Watson IoT Platform messaging server using default options.
	 *
	 * @return bool
	 * returns true if connection succeeds else fasle
	 * @throw MQTT exceptions
	 */
	bool connect()
		throw(mqtt::exception, mqtt::security_exception) ;

	/**
	* Function used to Publish events from the device to the IBM Watson IoT service
	* @param eventType - Type of event to be published e.g status, gps
	* @param eventFormat - Format of the event e.g json
	* @param data - Payload of the event
	* @param QoS - qos for the publish event. Supported values : 0, 1, 2
	*
	* @return void
	*/
	void publishEvent(char *eventType, char *eventFormat, const char* data, int qos);
	/**
	* Function used to Publish events from the device to the IBM Watson IoT service
	* @param eventType - Type of event to be published e.g status, gps
	* @param eventFormat - Format of the event e.g json
	* @param data - Payload of the event
	* @param QoS - qos for the publish event. Supported values : 0, 1, 2
	* @param iaction_listener& cb - call back function for action listner
	* @return void
	*/
	void publishEvent(char *eventType, char *eventFormat, const char* data, int qos,  mqtt::iaction_listener& cb);

	/**
	 * Function used to subscribe commands from the IBM Watson IoT service
	 * @return bool
	 * returns true if commands are subscribed successfully else false
	 */
	bool subscribeCommands();

	void setDeviceData(iotf_device_data_ptr& deviceData) { mDeviceData = deviceData; }
	iotf_device_data_ptr getDeviceData() const { return mDeviceData; }
	int getLifetime() const { return mLifetime; }
	void setLifetime(int lifetime) {mLifetime = lifetime;}

	bool manage();
	bool unmanage();
	bool update_device_location(Watson_IOTP::IOTP_DeviceLocation& deviceLocation);

	/**
	 * Function used to add diagnostic error codes of the device to the IBM Watson IoT platform
	 * @return bool
	 */
	bool addErrorCodes(int);

	/**
	 * Function used to clear diagnostic error codes of the device from the IBM Watson IoT platform
	 * @return void
	 */
	bool clearErrorCodes();

	/**
	 * Function used to add diagnostic logs of the device to the IBM Watson IoT platform
	 * @return bool
	 */
	bool addLogs(Watson_IOTP::IOTP_DeviceLog& deviceLog);

	/**
	 * Function used to clear diagnostic logs of the device from the IBM Watson IoT platform
	 * @return void
	 */
	bool clearLogs();



private:
	int mLifetime;
	std::string mServerURI;
	std::string mClientID;
	iotp_device_attribute_handler_ptr mDevAttributeHandler;
	iotf_device_data_ptr mDeviceData;
	bool InitializeMqttClient();

};
typedef IOTP_DeviceClient::ptr_t iotp_device_client_ptr;

} /* namespace Watson_IOTP */

#endif /* SRC_IOTP_DEVICECLIENT_H_ */
