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

#include <iostream>
#include <string>
#include <memory>
#include <string>
#include <typeinfo>
#include <thread>
#include "IOTP_Client.h"
#include "IOTP_DeviceFirmwareHandler.h"
#include "IOTP_ReplyMessage.h"
#include "mqtt/message.h"
#include "json/json.h"
#include "IOTP_TopicDefinitions.h"

namespace Watson_IOTP {

//	extern const std::string& SERVER_FIRMWARE_DOWNLOAD_TOPIC;
//	extern const std::string& SERVER_FIRMWARE_UPDATE_TOPIC;
//	extern const std::string& SERVER_UPDATE_TOPIC;
//	extern const std::string& SERVER_OBSERVE_TOPIC;
//	extern const std::string& SERVER_CANCEL_TOPIC;
//	extern const std::string& DEVICE_REPONSE_TOPIC;
//	extern const std::string& DEVICE_NOTIFY_TOPIC;


	IOTP_FirmwareInfo::IOTP_FirmwareInfo(const char* name,
				const char* version,
				const char* uri,
				const char* verifier) :
						mName((name == NULL) ? "" : name),
						mVersion((version == NULL) ? "" : version),
						mURI((uri == NULL) ? "" : uri),
						mVerifier((verifier == NULL) ? "" : verifier),
						mState(FIRMWARE_INFO_STATE_IDLE),
						mUpdateStatus(-1),
						mUpdatedDateTime(-1) {
		setJsonValue();
	}

	IOTP_FirmwareInfo::IOTP_FirmwareInfo(const char* name,
			const char* version,
			const char* uri,
			const char* verifier,
			int state,
			int updateStatus,
			time_t updatedDateTime) :
					mName((name == NULL) ? "" : name),
					mVersion((version == NULL) ? "" : version),
					mURI((uri == NULL) ? "" : uri),
					mVerifier((verifier == NULL) ? "" : verifier),
					mState(state),
					mUpdateStatus(updateStatus),
					mUpdatedDateTime(updatedDateTime) {
		setJsonValue();
	}

	void IOTP_FirmwareInfo::set_name(const std::string name) {
		mName = name;
	}
	void IOTP_FirmwareInfo::set_version(const std::string version) {
		mVersion = version;
	}
	void IOTP_FirmwareInfo::set_uri(const std::string uri) {
		mURI = uri;
	}
	void IOTP_FirmwareInfo::set_verifier(const std::string verifier) {
		mVerifier = verifier;
	}
	void IOTP_FirmwareInfo::set_state(int state) {
		mState = state;
		setJsonValue();
	}
	void IOTP_FirmwareInfo::set_update_status(int updateStatus) {
		mUpdateStatus = updateStatus;
		setJsonValue();
	}
	void IOTP_FirmwareInfo::set_updated_date_time(time_t updatedDateTime) {
		mUpdatedDateTime = updatedDateTime;
		setJsonValue();
	}

	const Json::Value& IOTP_FirmwareInfo::toJsonValue() const {
			return json;
	}

	void IOTP_FirmwareInfo::setJsonValue() {
			char updatedDateTimeBuf[sizeof "YYYY-MM-DDTHH:MM:SSZ"];
			if (!mName.empty())
				json["name"] = mName;
			if (!mVersion.empty())
				json["version"] = mVersion;
			if (!mURI.empty())
				json["url"] = mURI;
			if (!mVerifier.empty())
				json["verifier"] = mVerifier;
			json["state"] = mState;
			if (mUpdateStatus != -1) {
				json["updateStatus"] = mUpdateStatus;
			}
			if (mUpdatedDateTime != -1) {
				strftime(updatedDateTimeBuf, sizeof updatedDateTimeBuf, "%FT%TZ", gmtime(&mUpdatedDateTime));
				json["updatedDateTime"] = updatedDateTimeBuf;
			}
		}

	/**
	 * DeviceFirmwareHandler is an abstract class.
	 *
	 * To initiate a firmware download using the REST API, issue a POST request to:
	 *     https://<org>.internetofthings.ibmcloud.com/api/v0002/mgmt/requests
	 *  The information provided is:
	 *     The action firmware/download
	 *     The URI for the firmware image
	 *     A list of devices to receive the image, with a maximum of 5000 devices
	 *     Optional verifier string to validate the image
	 *     Optional firmware name
	 *     Optional firmware version
	 *
	 *
	 * The device management server in the IoT Platform uses the Device Management Protocol
	 * to send a request to the devices, initiating the firmware download. There are
	 * multiple steps:
	 *
	 * 1. Firmware details update request sent on topic iotdm-1/device/update:
	 *    This request let the device validate if the requested firmware differs from the
	 *    currently installed firmware. If there is a difference, set rc to 204, which
	 *    translates to the status Changed.
	 *
	 * 2. Observation request for firmware download status iotdm-1/observe:
	 *    Verifies if the device is ready to start the firmware download.
	 *    When the download can be started immediately, set rc to 200 (Ok).
	 *
	 *    Also, the device must publish a message to iotdevice-1/notify topic, set
	 *      mgmt.firmware.state to 0 (Idle)
	 *      mgmt.firmware.updateStatus to 0 (Idle).
	 *
	 *
	 * 3. Initiate the download request sent on topic iotdm-1/mgmt/initiate/firmware/download:
	 *    This request tells a device to actually start the firmware download. If the action
	 *    can be initiated immediately, set rc to 202.
	 *
	 *    After a firmware download is initiated this way, the device needs to report to
	 *    the IoT Platform the status of the download. This is possible by publishing a message
	 *    to the iotdevice-1/notify-topic, where the mgmt.firmware.state is set to
	 *    either 1 (Downloading) or 2 (Downloaded).
	 */

	IOTP_DeviceFirmwareHandler::IOTP_DeviceFirmwareHandler() : fwinfo(NULL, NULL, NULL, NULL) {}

	IOTP_DeviceFirmwareHandler::~IOTP_DeviceFirmwareHandler() {
		if (mDownloadProgress.joinable())
			mDownloadProgress.join();
		if (mUpdateProgress.joinable())
			mUpdateProgress.join();
	}

	iotp_reply_message_ptr IOTP_DeviceFirmwareHandler::message_arrived(const std::string& topic, Json::Value& jsonPayload) {
		iotp_reply_message_ptr replyPtr = nullptr;
		iotp_firmware_action_response_ptr rsp;

		iotp_firmware_info_ptr p = std::make_shared<IOTP_FirmwareInfo>(fwinfo);

		if (topic == SERVER_UPDATE_TOPIC) {
			/**
			 * Incoming request from the IoT Platform:
					Topic: iotdm-1/device/update
					Message:
					{
			   	   	   "reqId" : "f38faafc-53de-47a8-a940-e697552c3194",
			   	   	   "d" : {
			      	  	  "fields" : [{
			            	"field" : "mgmt.firmware",
			            		"value" : {
			               	   	   "version" : "some firmware version",
			               	   	   "name" : "some firmware name",
			               	   	   "uri" : "some uri for firmware location",
			               	   	   "verifier" : "some validation code",
			               	   	   "state" : 0,
			               	   	   "updateStatus" : 0,
			               	   	   "updatedDateTime" : ""
			            		}
			         	 	 }
			      	  	  ]
			   	   	   }
					}
				 */
			Json::ArrayIndex index = 0;
			Json::Value v;
			Json::Value firstField = jsonPayload.get("d", v).get("fields", v).get(index, v);

			if (firstField.get("field", v).asString() == "mgmt.firmware") {
				Json::Value value = firstField.get("value", v);
				p->set_version(value.get("version", "").asString());
				p->set_name(value.get("name", "").asString());
				p->set_uri(value.get("uri", "").asString());
				p->set_verifier(value.get("verifier", "").asString());
			}

			// Call the derived class to perform version check.
			rsp = verify(p);

		} else if (topic == SERVER_OBSERVE_TOPIC) {
			/**
			 * Observation request for firmware download status iotdm-1/observe:
			 * Verifies if the device is ready to start the firmware download.
			 * When the download can be started immediately, set rc to 200 (Ok),
			 * mgmt.firmware.state to 0 (Idle) and mgmt.firmware.updateStatus to 0 (Idle).
			 * Here an example exchange between the IoT Platform and device:
			 * Incoming request from the IoT Platform:
			 * 	Topic: iotdm-1/observe
			 * 	Message:
			 * 	{
			 * 		"reqId" : "909b477c-cd37-4bee-83fa-1d568664fbe8",
			 * 			"d" : {
			 * 				"fields" : [ {
			 * 					"field" : "mgmt.firmware"
			 * 					}
			 * 				]
			 * 			}
			 * 	}
			 *
			 * 	Outgoing response from device:
			 * 	Topic: iotdevice-1/response
			 * 	Message:
			 * 	{
			 * 		"rc" : 200,
			 * 		"reqId" : "909b477c-cd37-4bee-83fa-1d568664fbe8"
			 * 	}
			 */
			rsp = is_ready_for_download(p);


		} else if (topic == SERVER_FIRMWARE_DOWNLOAD_TOPIC) {
			rsp = download(p);
			if (rsp->get_rc() == 202) {
				mDownloadProgress = std::thread(&IOTP_DeviceFirmwareHandler::check_download_progress, this);
			}
		} else if (topic == SERVER_FIRMWARE_UPDATE_TOPIC) {
			rsp = update(p);
			if (rsp->get_rc() == 202) {
				mUpdateProgress = std::thread(&IOTP_DeviceFirmwareHandler::check_update_progress, this);
			}
		} else if (topic == SERVER_CANCEL_TOPIC) {
			IOTP_FirmwareActionResponse response(200);
			rsp = std::make_shared<IOTP_FirmwareActionResponse>(response);
		}

		Json::Value payload;
		payload["rc"] = rsp->get_rc();
		if (rsp->get_message().empty() == false)
			payload["message"] = rsp->get_message();
		payload["reqId"] = jsonPayload.get("reqId", "").asString();

		IOTP_ReplyMessage reply(DEVICE_REPONSE_TOPIC, payload);
		replyPtr = std::make_shared<IOTP_ReplyMessage>(reply);
		return replyPtr;
	}

	iotp_reply_message_ptr IOTP_DeviceFirmwareHandler::message_arrived(const std::string& topic, mqtt::const_message_ptr msg) {
		Json::Value jsonPayload;
		Json::Reader reader;
		iotp_reply_message_ptr replyPtr = nullptr;

		if (reader.parse(msg->get_payload(), jsonPayload)) {
			replyPtr = message_arrived(topic, jsonPayload);
		} else {
			std::cout << typeid(*this).name() << " Message arrived at TOPIC " << topic << " is not a JSON string." << std::endl;
		}
		return replyPtr;
	}

	void IOTP_DeviceFirmwareHandler::check_download_progress() {

		iotp_firmware_info_ptr p = std::make_shared<IOTP_FirmwareInfo>(fwinfo);

		unsigned int timeout = firmware_download_timeout() * 1000;
		unsigned int sleep_interval = timeout / 10;

		/**
		 * Check download progress 10 times.
		 * Send progress back to Watson IoT Platform.
		 *
		 * Topic: iotdevice-1/notify
		 * Message:
		 * {
		 * 	"reqId" : "123456789",
		 * 	"d" : {
		 * 		"fields" : [ {
		 * 			"field" : "mgmt.firmware",
		 * 			"value" : {
		 * 				"state" : 1
		 * 			}
		 * 		} ]
		 * 	}
		 * }
		 */
		for (int i=0; i<10; i++) {
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_interval));
			report_download_progress(p);

			Json::Value field1;
			field1["field"] = "mgmt.firmware";
			field1["value"]["state"] = p->get_state();
			Json::Value array;
			array.append(field1);
			Json::Value payload;
			payload["d"]["fields"] = array;

			// Do we need a reqId for notification??
			//payload["reqId"] = "";

			IOTP_ReplyMessage reply(DEVICE_NOTIFY_TOPIC, payload);
			iotp_reply_message_ptr replyPtr = std::make_shared<IOTP_ReplyMessage>(reply);

			if (mClient != nullptr) {
					mClient->IOTF_send_reply(replyPtr);
			}

			// Terminate this thread when download is complete.
			if (p->get_state() == FIRMWARE_INFO_STATE_DOWNLOADED) {
				break;
			}
		}

	}

	void IOTP_DeviceFirmwareHandler::check_update_progress() {
		iotp_firmware_info_ptr p = std::make_shared<IOTP_FirmwareInfo>(fwinfo);

		unsigned int timeout = firmware_update_timeout() * 1000;
		unsigned int sleep_interval = timeout / 10;

		/**
		 * Topic: iotdevice-1/notify
		 * Message:
		 * {
		 * 	"d" : {
		 * 		"fields": [
		 * 			{
		 * 				"field" : "mgmt.firmware",
		 * 				"value" : {
		 * 					"state" : 0,
		 * 					"updateStatus" : 0
		 * 				}
		 * 			}
		 * 		]
		 * 	}
		 * }
		 */
		for (int i=0; i<10; i++) {
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_interval));
			report_update_progress(p);

			Json::Value field1;
			field1["field"] = "mgmt.firmware";
			field1["value"]["state"] = p->get_state();
			field1["value"]["updateStatus"] = p->get_update_status();
			if (p->get_update_status() == FIRMWARE_INFO_UPDATE_STATUS_SUCCESS) {
				field1["value"]["updatedDateTime"] = p->toJsonValue().get("updatedDateTime","").asString();
			}
			Json::Value array;
			array.append(field1);
			Json::Value payload;
			payload["d"]["fields"] = array;

			IOTP_ReplyMessage reply(DEVICE_NOTIFY_TOPIC, payload);
			iotp_reply_message_ptr replyPtr = std::make_shared<IOTP_ReplyMessage>(reply);

			if (mClient != nullptr) {
					mClient->IOTF_send_reply(replyPtr);
			}

			// Terminate this thread when update is no longer in progress.
			if (p->get_update_status() != FIRMWARE_INFO_UPDATE_STATUS_IN_PROGRESS) {
				break;
			}
		}

	}


}
