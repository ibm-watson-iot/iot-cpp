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

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>
#include <map>
#include <cstring>
#include "mqtt/async_client.h"
#include "json/json.h"
#include "IOTP_Device.h"
#include "IOTP_Client.h"
#include "IOTP_DeviceClient.h"
#include "IOTP_DeviceActionHandler.h"
#include "IOTP_DeviceFirmwareHandler.h"
#include "IOTP_DeviceAttributeHandler.h"

using namespace Watson_IOTP;

int fillDeviceData(const std::string& filePath, iotf_device_data_ptr& deviceDataPtr);
int InitializeProperties(const std::string& filePath, Properties& prop);

const std::string TEST_PAYLOADS("{"
		"\"payloads\":["
					"\"Hello World!\","
					"\"Hi there!\","
					"\"Is anyone listening?\","
					"\"Someone is always listening.\""
					"],"
		"\"qos\":1"
		"}");

inline void sleep(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

/////////////////////////////////////////////////////////////////////////////

/**
 * A base action listener.
 */
class SampleActionListener : public virtual mqtt::iaction_listener
{
protected:
	virtual void on_failure(const mqtt::itoken& tok) {
		std::cout << "\n\tListener: Failure on token: "
			<< tok.get_message_id() << std::endl;
	}

	virtual void on_success(const mqtt::itoken& tok) {
		std::cout << "\n\tListener: Success on token: "
			<< tok.get_message_id() << std::endl;
	}
};

/**
 * A derived action listener for publish events.
 */
class SampleDeliveryActionListener : public SampleActionListener
{
	bool mDone;

	virtual void on_failure(const mqtt::itoken& tok) {
		SampleActionListener::on_failure(tok);
		mDone = true;
	}

	virtual void on_success(const mqtt::itoken& tok) {
		SampleActionListener::on_success(tok);
		mDone = true;
	}

public:
	SampleDeliveryActionListener() : mDone(false) {}
	bool is_done() const { return mDone; }
};


/////////////////////////////////////////////////////////////////////////////
class SampleDeviceAction : public IOTP_DeviceActionHandler {

public:

	SampleDeviceAction() : mReboot(false), mFactoryReset(false) {}
	iotp_device_action_response_ptr reboot() {
		std::cout << "MyDeviceAction: reboot " << std::endl;
		IOTP_DeviceActionResponse rsp(DEVICE_ACTION_REBOOT_OK);
		mReboot = true;
		return std::make_shared<IOTP_DeviceActionResponse>(rsp);
	}

	iotp_device_action_response_ptr factory_reset() {
		std::cout << "MyDeviceAction: factory_reset " << std::endl;
		IOTP_DeviceActionResponse rsp(DEVICE_ACTION_FACTORY_RESET_OK);
		mFactoryReset = true;
		return std::make_shared<IOTP_DeviceActionResponse>(rsp);
	}

	bool getReboot() { return mReboot; }
	bool getFactoryReset() { return mFactoryReset; }

private:
	bool mReboot;
	bool mFactoryReset;

};

///////////////////////////////////////////////////////////////////
class SampleFirmwareAction : public IOTP_DeviceFirmwareHandler {

public:
	SampleFirmwareAction() : state(FIRMWARE_INFO_STATE_IDLE), update_status(FIRMWARE_INFO_UPDATE_STATUS_NOT_SET) {}

	~SampleFirmwareAction() {
		if (mFirmwareDownload.joinable())
			mFirmwareDownload.join();
		if (mFirmwareUpdate.joinable())
			mFirmwareUpdate.join();
	}

	virtual iotp_firmware_action_response_ptr verify(iotp_firmware_info_ptr fwinfo)  {
		std::cout << "ENTRY verify\n";
		std::cout << "Version: " << fwinfo->get_version() << " Name: " << fwinfo->get_name() << std::endl;
		IOTP_FirmwareActionResponse rsp(204);
		iotp_firmware_action_response_ptr ptr = std::make_shared<IOTP_FirmwareActionResponse>(rsp);
		return ptr;
	}

	virtual iotp_firmware_action_response_ptr is_ready_for_download(iotp_firmware_info_ptr fwinfo) {
		std::cout << "ENTRY is_ready_for_download\n";
		IOTP_FirmwareActionResponse rsp(200);
		iotp_firmware_action_response_ptr ptr = std::make_shared<IOTP_FirmwareActionResponse>(rsp);
		return ptr;
	}

	virtual iotp_firmware_action_response_ptr download(iotp_firmware_info_ptr fwinfo) {
		std::cout << "ENTRY download\n";
		mFirmwareDownload = std::thread(&SampleFirmwareAction::pretend_firmware_download, this);
		IOTP_FirmwareActionResponse rsp(202);
		iotp_firmware_action_response_ptr ptr = std::make_shared<IOTP_FirmwareActionResponse>(rsp);
		return ptr;
	}

	virtual unsigned int firmware_download_timeout() { return 20; }

	virtual void report_download_progress(iotp_firmware_info_ptr fwinfo) {
		fwinfo->set_state(state);
		return;
	}

	virtual iotp_firmware_action_response_ptr update(iotp_firmware_info_ptr fwinfo) {
		mFirmwareUpdate = std::thread(&SampleFirmwareAction::pretend_firmware_update, this);
		IOTP_FirmwareActionResponse rsp(202);
		iotp_firmware_action_response_ptr ptr = std::make_shared<IOTP_FirmwareActionResponse>(rsp);
		return ptr;
	}

	virtual unsigned int firmware_update_timeout() {return 45;};

	virtual void report_update_progress(iotp_firmware_info_ptr fwinfo) {
		fwinfo->set_state(state);
		fwinfo->set_update_status(update_status);
		fwinfo->set_updated_date_time(update_timestamp);
		return;
	}

private:
	void pretend_firmware_download() {
		std::cout << "ENTRY pretend_firmware_download\n";
		state = FIRMWARE_INFO_STATE_DOWNLOADING;
		sleep((firmware_download_timeout() / 2) * 1000);
		state = FIRMWARE_INFO_STATE_DOWNLOADED;
		std::cout << "EXIT pretend_firmware_download\n";
	}

	void pretend_firmware_update() {
		std::cout << "ENTRY pretend_firmware_update\n";
		update_status = FIRMWARE_INFO_UPDATE_STATUS_IN_PROGRESS;
		sleep((firmware_update_timeout() / 2) * 1000);
		state = FIRMWARE_INFO_STATE_IDLE;
		update_status = FIRMWARE_INFO_UPDATE_STATUS_SUCCESS;
		time(&update_timestamp);
		std::cout << "EXIt pretend_firmware_update\n";
	}


	int state;
	int update_status;
	time_t update_timestamp;
	std::thread mFirmwareDownload;
	std::thread mFirmwareUpdate;

};

class SampleDeviceAttributeAction : public IOTP_DeviceAttributeHandler {
public:
	SampleDeviceAttributeAction(){}
	~SampleDeviceAttributeAction() {}

	virtual bool UpdateLocation(iotf_device_location_ptr& locationPtr)  {
		std::cout << "ENTRY Update Location\n";
		std::cout << "Longitude: " << locationPtr->getLongitude() << " Latitude: " << locationPtr->getLatitude() << std::endl;
			//iotp_device_attribute_update_response_ptr ptr = std::make_shared<IOTP_DeviceAttributeUpdateResponse>(200);
		return true;
	}
};

//Implement the CommandCallback class to provide the way in which you want the command to be handled
class MyCommandCallback: public CommandCallback{
	/**
	 * This method is invoked by the library whenever there is command matching the subscription criteria
	 */
	void processCommand(Command& cmd){
		std::cout<<"Received Command \n"
			<<"Command Name:"<<cmd.getCommandName()<<"\t format:"<<cmd.getFormat()<<" \t payload:"<<cmd.getPayload()<<"\n";
	}
};

int main(int argc, char **argv) {
	Json::FastWriter fastWriter;
	std::string jsonMessage;
	mqtt::message_ptr pubmsg;
	mqtt::idelivery_token_ptr pubtok;
	mqtt::itoken_ptr subtok;
	SampleActionListener listener;
	SampleDeliveryActionListener deliveryListener;
	bool success = false;
	//int ret = 0;

	Properties prop;
	Properties quickProp;
	Json::Value root;
	Json::Reader reader;

	std::chrono::time_point<std::chrono::system_clock> now;
	now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	std::cout << "Current time: " << std::ctime(&now_time);


	if(argc > 1) {
		std::cout<<"Initializing properties for registered client"<<std::endl;
		if(!InitializeProperties(argv[1],prop))
		{
			std::cout<<"Failed initializing properties from configuration file"<<std::endl;
			return 1;
		}
	}
	else {
		std::cout << "Usage: sampleDevice <Path of configuration file in json format>" << std::endl;
		return 1;
	}


	try {

		//Qick start mode
		quickProp.setorgId("quickstart");
		quickProp.setdeviceType("devicetest");
		quickProp.setdeviceId("haritestdevice");


		std::cout<<"Creating IoTP Client with properties for quickstart mode"<<std::endl;
		IOTP_DeviceClient quickClient(quickProp);
		std::cout << "Connecting quick start client to Watson IoT platform" << std::endl;
		success = quickClient.connect();
		std::flush(std::cout);
		if (!success){
			std::cout<<"Connection failed\n";
			return 1;
		}

		std::cout<<"Connection successful"<<std::endl;

		jsonMessage = "{\"Data\": {\"Temp\": \"34\" } }"; //fastWriter.write(jsonPayload);
		std::cout << "Publishing event:" << std::endl << jsonMessage << std::endl
				<< std::flush;
		// First publish event without listner.
		quickClient.publishEvent("status", "json", jsonMessage.c_str(), 1);
		std::cout<<"Published success\n";
		quickClient.disconnect();
		std::cout<<"Disconnected qucik start client"<<std::endl;

		//Registered device flow properties reading from configuration file in json format
		std::cout<<"Creating IoTP Client with properties"<<std::endl;
		IOTP_DeviceClient client(prop);
		client.setKeepAliveInterval(90);
		std::cout<<"Connecting client to Watson IoT platform"<<std::endl;
		success = client.connect();
		std::cout<<"Connected client to Watson IoT platform"<<std::endl;
		std::flush(std::cout);
		if(!success)
			return 1;

		MyCommandCallback myCallback;
		client.setCommandHandler(&myCallback);
		Json::Value jsonPayload;
		Json::Value jsonText;

		jsonMessage = "{\"Data\": {\"Temp\": \"34\" } }";//fastWriter.write(jsonPayload);
		std::cout << "Publishing event:" << std::endl << jsonMessage << std::endl << std::flush;
		// First publish event without listner.
		client.publishEvent("status", "json", jsonMessage.c_str(), 1);

		//Publish event with listner
		std::cout << "Publishing event with listner:" << std::endl << jsonMessage << std::endl << std::flush;
		client.publishEvent("status1", "json", jsonMessage.c_str(), 1, listener);
		//Disconnect device client
		client.disconnect();
		std::cout << "Disconnected registered client\n";

		//Creating Device data from json file
		iotf_device_data_ptr deviceDataPtr;
		fillDeviceData(argv[1], deviceDataPtr);

		//creating action handler
		iotp_device_action_handler_ptr actionPtr = std::make_shared<SampleDeviceAction>();
		//creating firmware handler
		iotp_device_firmware_handler_ptr fwPtr = std::make_shared<SampleFirmwareAction>();
		//creating device attribute handler
		iotp_device_attribute_handler_ptr devAttributePtr = std::make_shared<SampleDeviceAttributeAction>();


//		std::string deviceType = prop.getdeviceType().c_str();
//		std::string deviceId = prop.getdeviceId().c_str();

		std::cout<<"Creating Managed Client\n";
		IOTP_DeviceClient managedClient(prop, deviceDataPtr, actionPtr, fwPtr, devAttributePtr);

		std::cout<<"Managed client created"<<std::endl;
		success = managedClient.connect();
		if (!success)
			return 1;

		managedClient.setCommandHandler(&myCallback);
		managedClient.setLifetime(3600);

		int count = 0;
		while (count < 100) {
			success = managedClient.manage();
			if (success)
				break;
			count++;
			std::cout << "Attempt #" << count << " Failed to register with IOTF as a managed device" << std::endl;
		}

		if (success == true) {

			std::cout << "THE DEVICE IS NOW MANAGED\n";

			// update location tests
			double x,y,z;

			std::cout<<"Requesting to update device location"<<std::endl;
			for (x=10.0, y=20.2, z=30.3; x < 20; x *=x, y *=y, z *=z) {
				IOTP_DeviceLocation deviceLocation(x,y,z);
				success = managedClient.update_device_location(deviceLocation);
				if (success == false)
					std::cout << "Failed to update device location to IOTF" << std::endl;
				else
					std::cout<< "Updated device location"<<std::endl;
			}


			std::cout<<"Adding Error code to the Watson IoT Platform"<<std::endl;
			success = managedClient.addErrorCodes(100);
			if (success == false)
				std::cout << "Failed to Add to error codes to Watson IoT platform" << std::endl;
			else
				std::cout<< "Adding error codes succeeded	"<<std::endl;

			std::cout<<"Requesting Watson IoT platform clear their error codes"<<std::endl;
			success = managedClient.clearErrorCodes();
			if (success == false)
				std::cout << "Failed to clear error codes" << std::endl;
			else
				std::cout<< "Clearing error codes succeeded"<<std::endl;

			std::cout<<"Add diagnostic logs to the Watson IoT Platform"<<std::endl;

			std::string logMessage = "Test Log Message";
			std::string timestamp = "2016-07-05T08:15:30-05:00";
			int sev = 1;
			std::string data = "Data";
			IOTP_DeviceLog deviceLog(logMessage, timestamp, sev, data);
			success = managedClient.addLogs(deviceLog);
			if (success == false)
				std::cout << "Failed Adding logs to the Watson IoT Platform" << std::endl;
			else
				std::cout<< "Adding logs succeeded"<<std::endl;

			std::cout<<"Clearing logs from the Watson IoT Platform"<<std::endl;
			success = managedClient.clearLogs();
			if (success == false)
				std::cout << "Failed to Clear logs" << std::endl;
			else
				std::cout<< "Clearing logs succeeded"<<std::endl;

			std::cout<<"Waiting for things to do...Press [Enter] to unregister and disconnect . . .";
			std::cin.get();

			if (static_cast<SampleDeviceAction*>(actionPtr.get())->getReboot() == false) {

				std::cout<<"Coming to Unmanage"<<std::endl;
				// If reboot was called, do not call unmange (is it a bug in WIoTP?)
				success = managedClient.unmanage();
				if (success == false) {
					std::cout << "Failed to unmanage from IOTF" << std::endl;
				}
			}

		}


		// Disconnect
		std::cout << "Disconnecting..." << std::flush;
		managedClient.disconnect();
		std::cout << "OK" << std::endl;

	}
	catch (const mqtt::exception& exc) {
		std::cerr << "Error: " << exc.what() << std::endl;
		return 1;
	}

 	return 0;
}

void fillDeviceData(const std::string& filePath, iotf_device_data_ptr& deviceDataPtr) {
	Json::Reader reader;
	Json::Value root;
	std::filebuf fb;
	if (fb.open(filePath, std::ios::in)) {
		std::istream is(&fb);
		if (!reader.parse(is, root)) {
			std::cout << "Failed to parse test configuration from input file "
					<< filePath << std::endl;
			fb.close();
			return 0;
		}
		fb.close();
	}

	const Json::Value devInfo = root["DeviceInfo"];
	const Json::Value devLocation = root["DeviceLocation"];
	const Json::Value devMetaData = root["MetaData"];

	iotf_device_info_ptr deviceInfoPtr = std::make_shared < IOTP_DeviceInfo
			> (devInfo);

	iotf_device_location_ptr deviceLocationPtr = std::make_shared
			< IOTP_DeviceLocation > (devLocation);

	iotp_firmware_info_ptr deviceFirmwarePtr = std::make_shared
			< IOTP_FirmwareInfo > ("name", "version", "uri", "verifier");
	iotf_device_metadata_ptr deviceMetaDataPtr = std::make_shared
			< IOTP_DeviceMetadata > (devMetaData);
	deviceDataPtr =
			std::make_shared < IOTP_DeviceData
					> (deviceInfoPtr, deviceLocationPtr, deviceFirmwarePtr, deviceMetaDataPtr);

	return 1;
}
int InitializeProperties(const std::string& filePath,Properties& prop) {
	Json::Reader reader;
	Json::Value root;
	std::filebuf fb;
	if (fb.open(filePath, std::ios::in)) {
		std::istream is(&fb);
		if (!reader.parse(is, root)) {
			std::cout << "Failed to parse test configuration from input file "
					<< filePath << std::endl;
			fb.close();
			return 0;
		}
		fb.close();
	}

	std::string org = root.get("Organization-ID", "").asString();
	if (org.size() == 0) {
		std::cout << "Failed to get Organization-ID from test configuration."
				<< std::endl;
		return 0;
	}

	std::string domain = root.get("domain", "").asString();
	if (domain.size() == 0) {
		std::cout << "Failed to get domain from test configuration."
				<< std::endl;
		return 0;
	}


	std::string deviceType = root.get("Device-Type", "").asString();
	if (deviceType.size() == 0) {
		std::cout << "Failed to get Device-Type from test configuration."
				<< std::endl;
		return 0;
	}

	std::string deviceId = root.get("Device-ID", "").asString();
	if (deviceId.size() == 0) {
		std::cout << "Failed to get Device-ID from test configuration."
				<< std::endl;
		return 0;
	}

	std::string username = root.get("Authentication-Method", "").asString();
	if (username.size() == 0) {
		std::cout
				<< "Failed to get Authentication-Method from test configuration."
				<< std::endl;
		return 0;
	}

	std::string password = root.get("Authentication-Token", "").asString();
	if (password.size() == 0) {
		std::cout
				<< "Failed to get Authentication-Token from test configuration."
				<< std::endl;
		return 0;
	}

	prop.setorgId(org);
	prop.setdomain(domain);
	prop.setdeviceType(deviceType);
	prop.setdeviceId(deviceId);
	prop.setauthMethod(username);
	prop.setauthToken(password);

	return 1;
}
