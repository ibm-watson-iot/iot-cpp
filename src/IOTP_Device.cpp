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
 *******************************************************************************/

#include <iostream>
#include <string>
#include "IOTP_Device.h"
//#include "IOTP_Client.h"
#include "IOTP_ResponseHandler.h"
#include "json/json.h"
#include "mqtt/async_client.h"

namespace Watson_IOTP {

//	/**
//	 * A device publishes message to this topic to become a managed device.
//	 */
//	const std::string& DEVICE_MANAGE_TOPIC() {
//		static std::string* str = new std::string("iotdevice-1/mgmt/manage");
//		return *str;
//	}
//
//	/**
//	 * A device publishes message to this topic when it no longer needs to be managed.
//	 */
//	const std::string& DEVICE_UNMANAGE_TOPIC() {
//		static std::string* str = new std::string("iotdevice-1/mgmt/unmanage");
//		return *str;
//	}
//
//	/**
//	 * Incoming message from the server to request the device:
//	 *   - to update its location
//	 *   - to check or validate firmware installed
//	 */
//	const std::string& SERVER_UPDATE_TOPIC() {
//		static std::string* str = new std::string("iotdm-1/device/update");
//		return *str;
//	}
//
//	/**
//	 * A device publishes message to this topic to report location change.
//	 */
//	const std::string& DEVICE_UPDATE_LOCATION_TOPIC() {
//		static std::string* str = new std::string("iotdevice-1/device/update/location");
//		return *str;
//	}
//
//	/**
//	 * A device publishes message to this topic to respond to device actions.
//	 */
//	const std::string& DEVICE_REPONSE_TOPIC() {
//		static std::string* str = new std::string("iotdevice-1/response");
//		return *str;
//	}
//
//	/**
//	 * Incoming message from the server to request the device to perform a reboot.
//	 * If this operation can be initiated immediately, set rc to 202.
//	 * If the reboot attempt fails, set rc to 500 and set the message field accordingly.
//	 * If reboot is not supported, set rc to 501 and optionally set the message field accordingly.
//	 * The reboot action is considered complete when the device sends a Manage device request following its reboot.
//	 */
//	const std::string& SERVER_DEVICE_REBOOT_TOPIC() {
//		static std::string* str = new std::string("iotdm-1/mgmt/initiate/device/reboot");
//		return *str;
//	}
//
//	/**
//	 * Incoming message from the server to request the device to perform factory reset.
//	 * The response code should be set to 202 if this action can be initiated immediately.
//	 * If the factory reset attempt fails, set rc to 500 and set the message field accordingly.
//	 * If the factory reset action is not supported, set rc to 501 and optionally set the message field accordingly.
//	 * The factory reset action is considered complete when the device sends a Manage device request following its factory reset.
//	 */
//	const std::string& SERVER_FACTORY_RESET_TOPIC() {
//		static std::string* str = new std::string("iotdm-1/mgmt/initiate/device/factory_reset");
//		return *str;
//	}
//
//	/**
//	 *  A device publishes message to this topic to notify the IoT Platform:
//	 *    - Firmware download:
//	 *        The device needs to report to the IoT Platform the status of the download.
//	 *        This is possible by publishing a message to the iotdevice-1/notify-topic,
//	 *        where the mgmt.firmware.state is set to either 1 (Downloading) or 2 (Downloaded)
//	 *    - Firmware update:
//	 *        In order to finish the firmware update request the device has to report its
//	 *        update status to the IoT Platform via a status message published on
//	 *        iotdevice-1/notify-topic. Once firmware update is completed,
//	 *        mgmt.firmware.updateStatus should be set to 0 (Success),
//	 *        mgmt.firmware.state should be set to 0 (Idle),
//	 *        downloaded firmware image can be deleted from the device and
//	 *        deviceInfo.fwVersion should be set to the value of mgmt.firmware.version.
//	 */
//	const std::string& DEVICE_NOTIFY_TOPIC() {
//		static std::string* str = new std::string("iotdevice-1/notify");
//		return *str;
//	}
//
//	/**
//	 * Incoming message from the IoT Platform.
//	 *   - Firmware download:
//	 *
//	 */
//	const std::string& SERVER_OBSERVE_TOPIC() {
//		static std::string* str = new std::string("iotdm-1/observe");
//		return *str;
//	}
//
//	const std::string& SERVER_CANCEL_TOPIC() {
//		static std::string* str = new std::string("iotdm-1/cancel");
//		return *str;
//	}
//
//	const std::string& SERVER_FIRMWARE_DOWNLOAD_TOPIC() {
//		static std::string* str = new std::string("iotdm-1/mgmt/initiate/firmware/download");
//		return *str;
//	}
//
//	const std::string& SERVER_FIRMWARE_UPDATE_TOPIC() {
//		static std::string* str = new std::string("iotdm-1/mgmt/initiate/firmware/update");
//		return *str;
//	}
//
//	//Diagnostic topics
//	const std::string& DEVICE_ADD_ERROR_CODES_TOPIC() {
//		static std::string* str = new std::string("iotdevice-1/add/diag/errorCodes");
//		return *str;
//	}
//
//	const std::string& DEVICE_CLEAR_ERROR_CODES_TOPIC() {
//		static std::string* str = new std::string("iotdevice-1/clear/diag/errorCodes");
//		return *str;
//	}
//
//	const std::string& DEVICE_ADD_DIAG_LOG_TOPIC() {
//		static std::string* str = new std::string("iotdevice-1/add/diag/log");
//		return *str;
//	}
//
//	const std::string& DEVICE_CLEAR_DIAG_LOG_TOPIC() {
//		static std::string* str = new std::string("iotdevice-1/clear/diag/log");
//		return *str;
//	}


	// DeviceInfo
	IOTP_DeviceInfo::IOTP_DeviceInfo(const char* serialNumber,
			const char* manufacturer,
			const char* model,
			const char* deviceClass,
			const char* description,
			const char* fwVersion,
			const char* hwVersion,
			const char* descriptiveLocation) :
				mSerialNumber((serialNumber == NULL) ? "" : serialNumber ),
				mManufacturer((manufacturer == NULL) ? "" : manufacturer),
				mModel((model == NULL) ? "" : model),
				mDeviceClass((deviceClass == NULL) ? "" : deviceClass),
				mDescription((description == NULL) ? "" : description),
				mFwVersion((fwVersion == NULL) ? "" : fwVersion),
				mHwVersion((hwVersion == NULL) ? "" : hwVersion),
				mDescriptiveLocation((descriptiveLocation == NULL) ? "" : descriptiveLocation) {
		setJsonValue();
	};

	IOTP_DeviceInfo::IOTP_DeviceInfo(const Json::Value& value) {
		mSerialNumber = value.get("serialNumber", "").asString();
		mManufacturer = value.get("manufacturer", "").asString();
		mModel = value.get("model", "").asString();
		mDeviceClass = value.get("deviceClass", "").asString();
		mDescription = value.get("description", "").asString();
		mFwVersion = value.get("fwVersion", "").asString();
		mHwVersion = value.get("hwVersion", "").asString();
		mDescriptiveLocation = value.get("descriptiveLocation", "").asString();
		setJsonValue();
	}

	IOTP_DeviceInfo::IOTP_DeviceInfo(const IOTP_DeviceInfo& src) :
		mSerialNumber(src.mSerialNumber),
		mManufacturer(src.mManufacturer),
		mModel(src.mModel),
		mDeviceClass(src.mDeviceClass),
		mDescription(src.mDescription),
		mFwVersion(src.mFwVersion),
		mHwVersion(src.mHwVersion),
		mDescriptiveLocation(src.mDescriptiveLocation) {
		setJsonValue();
	};

	void IOTP_DeviceInfo::setJsonValue() {
		if (!mSerialNumber.empty())
			json["serialNumber"] = mSerialNumber;
		if (!mManufacturer.empty())
			json["manufacturer"] = mManufacturer;
		if (!mModel.empty())
			json["model"] = mModel;
		if (!mDeviceClass.empty())
			json["deviceClass"] = mDeviceClass;
		if (!mDescription.empty())
			json["description"] = mDescription;
		if (!mFwVersion.empty())
			json["fwVersion"] = mFwVersion;
		if (!mHwVersion.empty())
			json["hwVersion"] = mHwVersion;
		if (!mDescriptiveLocation.empty())
			json["descriptiveLocation"] = mDescriptiveLocation;

	}

	const Json::Value& IOTP_DeviceInfo::toJsonValue() const{
		return json;
	}


	// DeviceLocation
	IOTP_DeviceLocation::IOTP_DeviceLocation(double latitude, double longitude, double elevation) :
		mLatitude(latitude),
		mLongitude(longitude),
		mElevation(elevation),
		mAccuracy(0) {
		time(&measuredDateTime);
		setJsonValue();
	}

	IOTP_DeviceLocation::IOTP_DeviceLocation(double latitude, double longitude, double elevation, double accuracy) :
		mLatitude(latitude),
		mLongitude(longitude),
		mElevation(elevation),
		mAccuracy(accuracy) {
		time(&measuredDateTime);
		setJsonValue();
	}

	void IOTP_DeviceLocation::setLatitude(double latitude) {
		setLocation(latitude, mLongitude, mElevation);
	}

	void IOTP_DeviceLocation::setLongitude(double longitude) {
		setLocation(mLatitude, longitude, mElevation);
	}

	void IOTP_DeviceLocation::setElevation(double elevation) {
		setLocation(mLatitude, mLongitude, elevation);
	}

	void IOTP_DeviceLocation::setLocation(double latitude, double longitude, double elevation) {
		mLatitude = latitude;
		mLongitude = longitude;
		mElevation = elevation;
		time(&measuredDateTime);
		setJsonValue();
	}

	const Json::Value& IOTP_DeviceLocation::toJsonValue() const {
		return json;
	}

	void IOTP_DeviceLocation::setJsonValue() {
		char measuredDateTimeBuf[sizeof "YYYY-MM-DDTHH:MM:SSZ"];
		json["latitude"] = mLatitude;
		json["longitude"] = mLongitude;
		json["elevation"] = mElevation;
		json["accuracy"] = mAccuracy;
		strftime(measuredDateTimeBuf, sizeof measuredDateTimeBuf, "%FT%TZ", gmtime(&measuredDateTime));
		json["measuredDateTime"] = measuredDateTimeBuf;
	};

	// IOTP_Device class

/*	IOTP_Device::IOTP_Device(IOTP_Client& client, const char* typeId, const char* deviceId, IOTP_DeviceInfo* deviceInfo) :
				mClient(client),
				mTypeId((typeId == NULL) ? "" : typeId ),
				mDeviceId((deviceId == NULL) ? "" : deviceId),
				mLifetime(3600),
				mDeviceInfo(deviceInfo),
				mActionHandler(nullptr),
				mFirmwareHandler(nullptr),
				mReqCounter(0) {
		mResponseHandler = std::make_shared<IOTP_ResponseHandler> ();
	};

	IOTP_Device::IOTP_Device(IOTP_Client& client, std::string& typeId, std::string& deviceId, IOTP_DeviceInfo* deviceInfo) :
		mClient(client),
		mTypeId(typeId),
		mDeviceId(deviceId),
		mLifetime(3600),
		mDeviceInfo(deviceInfo),
		mActionHandler(nullptr),
		mFirmwareHandler(nullptr),
		mReqCounter(0) {
		mResponseHandler = std::make_shared<IOTP_ResponseHandler> ();
	};

	IOTP_Device::IOTP_Device(IOTP_Client& client, std::string& typeId, std::string& deviceId, IOTP_DeviceInfo* deviceInfo,
			iotp_device_action_handler_ptr actionHandler, iotp_device_firmware_handler_ptr firmwareHandler) :
			mClient(client),
			mTypeId(typeId),
			mDeviceId(deviceId),
			mLifetime(3600),
			mDeviceInfo(deviceInfo),
			mActionHandler(actionHandler),
			mFirmwareHandler(firmwareHandler),
			mReqCounter(0) {
			mResponseHandler = std::make_shared<IOTP_ResponseHandler> ();
		};

	bool IOTP_Device::supportDeviceActions() const {
		return (!(mActionHandler == nullptr));
	}

	bool IOTP_Device::supportFirmwareActions() const {
		return (!(mFirmwareHandler == nullptr));
	}

	bool IOTP_Device::manage() {
		if (mClient.subscribeCommandHandler(SERVER_RESPONSE_TOPIC(), mResponseHandler)) {
			Json::Value jsonManageData;
			jsonManageData["lifetime"] = mLifetime;
			jsonManageData["supports"]["deviceActions"] = supportDeviceActions();
			jsonManageData["supports"]["firmwareActions"] = supportFirmwareActions();
			jsonManageData["deviceInfo"] = mDeviceInfo->toJsonValue();
			std::string reqId = send_message(DEVICE_MANAGE_TOPIC(), jsonManageData);
			if (!reqId.empty()) {
				Json::Value res = mResponseHandler->wait_for_response(DEFAULT_TIMEOUT(), reqId);
				int rc = res.get("rc", -1).asInt();
				if (rc == 200) {
					if (mFirmwareHandler != nullptr) {
						mClient.subscribeCommandHandler(SERVER_UPDATE_TOPIC(), mFirmwareHandler);
						mClient.subscribeCommandHandler(SERVER_OBSERVE_TOPIC(), mFirmwareHandler);
						mClient.subscribeCommandHandler(SERVER_CANCEL_TOPIC(), mFirmwareHandler);
						mClient.subscribeCommandHandler(SERVER_FIRMWARE_DOWNLOAD_TOPIC(), mFirmwareHandler);
						mClient.subscribeCommandHandler(SERVER_FIRMWARE_UPDATE_TOPIC(), mFirmwareHandler);
					}
					if (mActionHandler != nullptr) {
						mClient.subscribeCommandHandler(SERVER_DEVICE_REBOOT_TOPIC(), mActionHandler);
						mClient.subscribeCommandHandler(SERVER_FACTORY_RESET_TOPIC(), mActionHandler);
					}
				}

				return (rc == 200);
			}
		}
		return false;
	}

	bool IOTP_Device::unmanage() {
		Json::Value nullData;
		std::string reqId = send_message(DEVICE_UNMANAGE_TOPIC(), nullData);
		mClient.unsubscribeCommands(SERVER_RESPONSE_TOPIC());
		return (!reqId.empty());
	}

	bool IOTP_Device::update_device_location(IOTP_DeviceLocation& deviceLocation) {
		if (mClient.subscribeCommandHandler(SERVER_RESPONSE_TOPIC(), mResponseHandler)) {
			std::string reqId = send_message(DEVICE_UPDATE_LOCATION_TOPIC(), deviceLocation.toJsonValue());
			if (!reqId.empty()) {
				Json::Value res = mResponseHandler->wait_for_response(DEFAULT_TIMEOUT(), reqId);
				int rc = res.get("rc", -1).asInt();
				return (rc == 200);
			}
		}
		return false;
	}

	std::string IOTP_Device::send_message(const std::string& topic, const Json::Value& data, int qos) {
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
		mqtt::idelivery_token_ptr pubtok = mClient.publish(topic, pubmsg);
		pubtok->wait_for_completion(DEFAULT_TIMEOUT());
		success = pubtok->is_complete();

		if (success == false) {
			std::cout << "send_message FAILED to send message to TOPIC " << topic << " PAYLOAD " << std::endl << jsonMessage << std::endl << std::flush;
			reqId.clear();
		}
		return reqId;
	}*/

}


