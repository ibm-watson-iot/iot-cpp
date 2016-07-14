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

#ifndef DEVICEACTIONHANDLER_H_
#define DEVICEACTIONHANDLER_H_

#include "IOTP_MessageHandler.h"
#include "json/json.h"

namespace Watson_IOTP {

#define DEVICE_ACTION_REBOOT_OK 202
#define DEVICE_ACTION_REBOOT_FAILED 500
#define DEVICE_ACTION_REBOOT_NOT_SUPPORTED 501
#define DEVICE_ACTION_FACTORY_RESET_OK 202
#define DEVICE_ACTION_FACTORY_RESET_FAILED 500
#define DEVICE_ACTION_FACTORY_RESET_NOT_SUPPORTED 501

class IOTP_DeviceActionResponse {
public:
	/** Pointer type for this object */
	typedef std::shared_ptr<IOTP_DeviceActionResponse> ptr_t;
	IOTP_DeviceActionResponse(int rc, std::string message) : mRC(rc), mMessage(message) {}
	IOTP_DeviceActionResponse(int rc) : mRC(rc) {}
	int get_rc() {return mRC;}
	std::string get_message() {return mMessage;}

private:
	int mRC;
	std::string mMessage;
};
typedef IOTP_DeviceActionResponse::ptr_t iotp_device_action_response_ptr;

/**
 * DeviceActionHandler is an abstract class.
 * Derived class must implement reboot() and factory_reset() methods.
 */
class IOTP_DeviceActionHandler : public IOTP_MessageHandler {
public:
	/** Pointer type for this object */
	typedef std::shared_ptr<IOTP_DeviceActionHandler> ptr_t;

	virtual ~IOTP_DeviceActionHandler() {}
	iotp_reply_message_ptr message_arrived(const std::string& topic, Json::Value& jsonPayload);
	iotp_reply_message_ptr message_arrived(const std::string& topic, mqtt::message_ptr msg);

	/**
	 * If this operation can be initiated immediately, set rc to 202.
	 * If the reboot attempt fails, set rc to 500 and set the message field accordingly.
	 * If reboot is not supported, set rc to 501 and optionally set the message field accordingly.
	 *
	 * The reboot action is considered complete when the device sends a Manage device request following reboot.
	 */
	virtual iotp_device_action_response_ptr reboot() = 0;

	/**
	 * A response must be sent back indicating whether or not the factory reset action can be initiated.
	 * The response code should be set to 202 if this action can be initiated immediately.
	 * If the factory reset attempt fails, set rc to 500 and set the message field accordingly.
	 * If the factory reset action is not supported, set rc to 501 and optionally set the message field accordingly.
	 *
	 * The factory reset action is considered complete when the device sends a Manage device request following its factory reset.
	 */
	virtual iotp_device_action_response_ptr factory_reset() = 0;
};
typedef IOTP_DeviceActionHandler::ptr_t iotp_device_action_handler_ptr;
}



#endif /* DEVICEACTIONHANDLER_H_ */
