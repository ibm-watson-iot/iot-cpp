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
 *    Lokesh Haralakatta - Updates to match with latest mqtt lib changes
 *******************************************************************************/

#ifndef DEVICEFIRMWAREHANDLER_H_
#define DEVICEFIRMWAREHANDLER_H_

#include <thread>
#include "IOTP_MessageHandler.h"
#include "json/json.h"

namespace Watson_IOTP {


#define FIRMWARE_INFO_STATE_IDLE 0
#define FIRMWARE_INFO_STATE_DOWNLOADING 1
#define FIRMWARE_INFO_STATE_DOWNLOADED 2
#define FIRMWARE_DOWNLOAD_DEFAULT_TIMEOUT 60

#define FIRMWARE_INFO_UPDATE_STATUS_NOT_SET -1
#define FIRMWARE_INFO_UPDATE_STATUS_SUCCESS 0
#define FIRMWARE_INFO_UPDATE_STATUS_IN_PROGRESS 1
#define FIRMWARE_INFO_UPDATE_STATUS_OUT_OF_MEMORY 2
#define FIRMWARE_INFO_UPDATE_STATUS_CONNECTION_LOST 3
#define FIRMWARE_INFO_UPDATE_STATUS_VERIFICATION_FAILED 4
#define FIRMWARE_INFO_UPDATE_STATUS_UNSUPPORTED_IMAGE 5
#define FIRMWARE_INFO_UPDATE_STATUS_INVALID_URL 6
#define FIRMWARE_UPDATE_DEFAULT_TIMEOUT 300

/**
 * FirmwareInfo class describes the information of firmware.
 * The information is used during download and firmware actions.
 */
class IOTP_FirmwareInfo {
public:
	/** Pointer type for this object */
	typedef std::shared_ptr<IOTP_FirmwareInfo> ptr_t;

	IOTP_FirmwareInfo(const char* name,
			const char* version,
			const char* uri,
			const char* verifier);

	IOTP_FirmwareInfo(const char* name,
			const char* version,
			const char* uri,
			const char* verifier,
			int state,
			int updateStatus,
			time_t updatedDateTime);
	void set_name(const std::string name);
	const std::string get_name() {return mName;}
	void set_version(const std::string version);
	const std::string get_version() {return mVersion;}
	void set_uri(const std::string uri);
	const std::string get_uri() {return mURI;}
	void set_verifier(const std::string verifier);
	const std::string get_verifier() { return mVerifier;}
	void set_state(int state);
	const int get_state() {return mState;}
	void set_update_status(int updateStatus);
	const int get_update_status() {return mUpdateStatus;}
	void set_updated_date_time(time_t updatedDateTime);
	const time_t get_updated_date_time() {return mUpdatedDateTime;}
	const Json::Value& toJsonValue() const;

private:
	void setJsonValue();
	std::string mName;
	std::string mVersion;
	std::string mURI;
	std::string mVerifier;
	int mState;
	int mUpdateStatus;
	time_t mUpdatedDateTime;
	Json::Value json;
};

typedef IOTP_FirmwareInfo::ptr_t iotp_firmware_info_ptr;

/**
 * Response to send back to Watson IoT Platform.
 */
class IOTP_FirmwareActionResponse {
public:
	/** Pointer type for this object */
	typedef std::shared_ptr<IOTP_FirmwareActionResponse> ptr_t;
	IOTP_FirmwareActionResponse(int rc, std::string message) : mRC(rc), mMessage(message) {}
	IOTP_FirmwareActionResponse(int rc) : mRC(rc) {}
	int get_rc() {return mRC;}
	std::string get_message() {return mMessage;}

private:
	int mRC;
	std::string mMessage;
};
typedef IOTP_FirmwareActionResponse::ptr_t iotp_firmware_action_response_ptr;


/**
 * DeviceFirmwareHandler is an abstract class.
 * Derived class must implement the virtual methods below.
 */
class IOTP_DeviceFirmwareHandler : public IOTP_MessageHandler {
public:
	/** Pointer type for this object */
	typedef std::shared_ptr<IOTP_DeviceFirmwareHandler> ptr_t;

	IOTP_DeviceFirmwareHandler();
	virtual ~IOTP_DeviceFirmwareHandler();
	iotp_reply_message_ptr message_arrived(const std::string& topic, Json::Value& jsonPayload);
	iotp_reply_message_ptr message_arrived(const std::string& topic, mqtt::const_message_ptr msg);

	/**
	 * The device is to validate that the firmware differs from the current installed firmware.
	 * If there is a difference, the derived class is to return 204.
	 */
	virtual iotp_firmware_action_response_ptr verify(iotp_firmware_info_ptr fwinfo) = 0;

	/**
	 * Verifies if the device is ready to start the firmware download.
	 * When the download can be started immediately, set rc to 200 (Ok).
	 */
	virtual iotp_firmware_action_response_ptr is_ready_for_download(iotp_firmware_info_ptr) = 0;

	/**
	 * This request tells a device to actually start the firmware download.
	 * If the action can be initiated immediately, the derived class is to return 202.
	 */
	virtual iotp_firmware_action_response_ptr download(iotp_firmware_info_ptr) = 0;

	/**
	 * Returns the timeout value in seconds for firmware download.
	 * The derived class should override this method accordingly.
	 */
	virtual unsigned int firmware_download_timeout() { return FIRMWARE_DOWNLOAD_DEFAULT_TIMEOUT;}

	/**
	 * When called, the device is to report the progress of firmware download.
	 * Where the mgmt.firmware.state is set to either 1 (Downloading) or 2 (Downloaded).
	 * @see set_state(int state) method to set the state.
	 */
	virtual void report_download_progress(iotp_firmware_info_ptr fwinfo) = 0;

	/**
	 * This request tells a device to actually start the firmware update.
	 * If the action can be initiated immediately, the derived class is to return 202.
	 */
	virtual iotp_firmware_action_response_ptr update(iotp_firmware_info_ptr fwinfo) = 0;

	/**
	 * Returns the timeout value in seconds for firmware update process.
	 * The derived class should override this method accordingly.
	 */
	virtual unsigned int firmware_update_timeout() { return FIRMWARE_UPDATE_DEFAULT_TIMEOUT;}

	/**
	 * When called, the device is to report the progress of firmware update.
	 * Once firmware update is completed:
	 * 	mgmt.firmware.updateStatus should be set to 0 (Success),
	 * 	mgmt.firmware.state should be set to 0 (Idle),
	 * 	downloaded firmware image can be deleted from the device
	 * 	and deviceInfo.fwVersion should be set to the value of mgmt.firmware.version.
	 */
	virtual void report_update_progress(iotp_firmware_info_ptr fwinfo) = 0;

private:
	IOTP_FirmwareInfo fwinfo;
	std::thread mDownloadProgress;
	std::thread mUpdateProgress;
	void check_download_progress();
	void check_update_progress();

};
typedef IOTP_DeviceFirmwareHandler::ptr_t iotp_device_firmware_handler_ptr;
}




#endif /* DEVICEFIRMWAREHANDLER_H_ */
