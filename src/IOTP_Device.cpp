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
#include <string.h>

namespace Watson_IOTP {

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

	IOTP_DeviceLocation::IOTP_DeviceLocation(const Json::Value& value) {
		char measuredDateTimeBuf[sizeof "YYYY-MM-DDTHH:MM:SSZ"];
		mLatitude = value.get("latitude", 0.0).asDouble();
		mLongitude = value.get("longitude", 0.0).asDouble();
		mElevation = value.get("elevation", 0.0).asDouble();
		mAccuracy = value.get("accuracy", 0.0).asDouble();
		strcpy(measuredDateTimeBuf , value.get("measuredDateTime", "").asString().data());
		if(strcmp(measuredDateTimeBuf,""))
			strptime(measuredDateTimeBuf, "%FT%TZ", gmtime(&measuredDateTime));
		else
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
	}

	IOTP_DeviceLog::IOTP_DeviceLog(std::string& msg, std::string& time, int severity, std::string data)
	:mMessage (msg), mMeasuredDateTime (time), mSeverity (severity), mData (data) {
		setJsonValue();
	}

	void IOTP_DeviceLog::setLogMessage(std::string& msg) {
		setLogInfo(msg, mMeasuredDateTime, mSeverity, mData);
	}
	void IOTP_DeviceLog::setLogEntryTime(std::string& time){
		setLogInfo(mMessage, time, mSeverity, mData);
	}
	void IOTP_DeviceLog::setLogSeverity(int& sev) {
		setLogInfo(mMessage, mMeasuredDateTime, sev, mData);
	}
	void IOTP_DeviceLog::setLogdata(std::string& data) {
		setLogInfo(mMessage, mMeasuredDateTime, mSeverity, data);
	}
	void IOTP_DeviceLog::setLogInfo(std::string& msg, std::string& time, int& sev, std::string& data) {
		mMessage = msg;
		mMeasuredDateTime = time;
		mSeverity = sev;
		mData = data;
		setJsonValue();
	}
	const Json::Value& IOTP_DeviceLog::toJsonValue() const {
		return json;
	}

	void IOTP_DeviceLog::setJsonValue() {
		//char measuredDateTimeBuf[sizeof "YYYY-MM-DDTHH:MM:SSZ"];
		json["message"] = mMessage;
		json["severity"] = mSeverity;
		json["data"] = mData;
		//strftime(measuredDateTimeBuf, sizeof measuredDateTimeBuf, "%FT%TZ", gmtime(&mMeasuredDateTime));
		json["timestamp"] = mMeasuredDateTime;
	}

}


