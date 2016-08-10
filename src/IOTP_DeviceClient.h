/*
 * IOTP_DeviceClient.h
 *
 *  Created on: 01-Aug-2016
 *      Author: amit
 */

#ifndef SRC_IOTP_DEVICECLIENT_H_
#define SRC_IOTP_DEVICECLIENT_H_

#include "IOTP_Client.h"

namespace Watson_IOTP {

class IOTP_DeviceClient: public IOTP_Client {
public:
	/** Pointer type for this object */
	typedef std::shared_ptr<IOTP_DeviceClient> ptr_t;
	/**
	 * Constructor of an IOTF_Client.  The parameters are the same as
	 * the constructor of the base class, mqtt::callback.
	 */
	IOTP_DeviceClient(Properties& prop);
	//IOTP_Client(const std::string& uri, const std::string& clientId);
	//IOTP_Client(Properties& prop, Watson_IOTP::IOTP_DeviceInfo* deviceInfo);
	IOTP_DeviceClient(Properties& prop, iotf_device_data_ptr& deviceData);

	/**
	 * Constructor of an IOTF_Client.
	 * Required arguments are organization ID, device type and device ID.
	 */
	//IOTP_Client(Properties& prop, Watson_IOTP::IOTP_DeviceInfo* deviceInfo,
	IOTP_DeviceClient(Properties& prop, iotf_device_data_ptr& deviceData,
			iotp_device_action_handler_ptr& actionHandler, iotp_device_firmware_handler_ptr& firmwareHandler,
			iotp_device_attribute_handler_ptr& devAttribureHandler);

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
	iotp_device_attribute_handler_ptr mDevAttributeHandler;
	iotf_device_data_ptr mDeviceData;
	void InitializeMqttClient();

};
typedef IOTP_DeviceClient::ptr_t iotp_device_client_ptr;

} /* namespace Watson_IOTP */

#endif /* SRC_IOTP_DEVICECLIENT_H_ */
