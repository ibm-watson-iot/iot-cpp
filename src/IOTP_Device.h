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

#ifndef DEVICEDATA_H_
#define DEVICEDATA_H_

#include "IOTP_DeviceActionHandler.h"
#include "IOTP_DeviceFirmwareHandler.h"
//#include "IOTP_Client.h"
#include "IOTP_ResponseHandler.h"


/**
 * Declaration of the following classes:
 * - DeviceInfo
 * - DeviceLocation
 * - IOTP_Device
 */
namespace Watson_IOTP {

	/**
	 * Device Info as specified in Watson IoT Platform Device Model, The following attributes provide the
	 * information about the device,
	 * <ul class="simple">
	 *   <li>serialNumber
	 *   <li>manufacturer
	 *   <li>model
	 *   <li>deviceClass
	 *   <li>description
	 *   <li>fwVersion
	 *   <li>hwVersion
	 *   <li>descriptiveLocation
	 */
	class IOTP_DeviceInfo
	{
	public:

		/** Pointer type for this object */
		typedef std::shared_ptr<IOTP_DeviceInfo> ptr_t;

		IOTP_DeviceInfo(const char* serialNumber,
				const char* manufacturer,
				const char* model,
				const char* deviceClass,
				const char* description,
				const char* fwVersion,
				const char* hwVersion,
				const char* descriptiveLocation);
		IOTP_DeviceInfo(const Json::Value& value);
		IOTP_DeviceInfo(const IOTP_DeviceInfo& src);
		const Json::Value& toJsonValue() const;

	private:
		void setJsonValue();
		std::string mSerialNumber,
			mManufacturer,
			mModel,
			mDeviceClass,
			mDescription,
			mFwVersion,
			mHwVersion,
			mDescriptiveLocation;
		Json::Value json;
	};

	typedef IOTP_DeviceInfo::ptr_t iotf_device_info_ptr;

	class IOTP_DeviceLocation {
	public:

		/** Pointer type for this object */
		typedef std::shared_ptr<IOTP_DeviceLocation> ptr_t;

		IOTP_DeviceLocation(double latitude, double longitude, double elevation);
		IOTP_DeviceLocation(double latitude, double longitude, double elevation, double accuracy);
		void setLatitude(double latitude);
		void setLongitude(double longitude);
		void setElevation(double elevation);
		void setLocation(double latitude, double longitude, double elevation);
		const Json::Value& toJsonValue() const;

	private:
		void setJsonValue();
		double mLatitude;
		double mLongitude;
		double mElevation;
		double mAccuracy;
		time_t measuredDateTime;
		Json::Value json;
	};

	typedef IOTP_DeviceLocation::ptr_t iotf_device_location_ptr;



	/*class IOTP_Device {
	public:

		/** Pointer type for this object */
	/*	typedef std::shared_ptr<IOTP_Device> ptr_t;

		IOTP_Device(IOTP_Client& client, const char* typeId, const char* deviceId, IOTP_DeviceInfo* deviceInfo);
		IOTP_Device(IOTP_Client& client, std::string& typeId, std::string& deviceId, IOTP_DeviceInfo* deviceInfo);
		IOTP_Device(IOTP_Client& client, std::string& typeId, std::string& deviceId, IOTP_DeviceInfo* deviceInfo,
				iotp_device_action_handler_ptr actionHandler, iotp_device_firmware_handler_ptr firmwareHandler);
		void setLifetime(int lifetime) {mLifetime = lifetime;}

		std::string getTypeId() const { return mTypeId; }
		std::string getDeviceId() const { return mDeviceId; }
		IOTP_DeviceInfo* getDeviceInfo() const { return mDeviceInfo; };
		int getLifetime() const { return mLifetime; }
		bool supportDeviceActions() const;
		bool supportFirmwareActions() const;

		bool manage();
		bool unmanage();
		bool update_device_location(IOTP_DeviceLocation& deviceLocation);

	private:
		std::string send_message(const std::string& topic, const Json::Value& data, int qos = 1);

		IOTP_Client& mClient;
		std::string mTypeId;
		std::string mDeviceId;
		int mLifetime;
		IOTP_DeviceInfo* mDeviceInfo;
		iotp_response_handler_ptr mResponseHandler;
		iotp_device_action_handler_ptr mActionHandler;
		iotp_device_firmware_handler_ptr mFirmwareHandler;
		int mReqCounter;
	};

	typedef IOTP_Device::ptr_t iotp_device_ptr;
*/
}

#endif /* DEVICEDATA_H_ */
