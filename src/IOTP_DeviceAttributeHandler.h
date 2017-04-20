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
 *******************************************************************************/

#ifndef SRC_IOTPDEVICEATTRIBUTEHANDLER_H_
#define SRC_IOTPDEVICEATTRIBUTEHANDLER_H_

#include "IOTP_MessageHandler.h"
#include "IOTP_Device.h"
#include "json/json.h"

namespace Watson_IOTP {

class IOTP_DeviceAttributeUpdateResponse {
public:
	/** Pointer type for this object */
	typedef std::shared_ptr<IOTP_DeviceAttributeUpdateResponse> ptr_t;
	IOTP_DeviceAttributeUpdateResponse(int rc, std::string message) : mRC(rc), mMessage(message) {}
	IOTP_DeviceAttributeUpdateResponse(int rc) : mRC(rc) {}
	int get_rc() {return mRC;}
	std::string get_message() {return mMessage;}

private:
	int mRC;
	std::string mMessage;
};
typedef IOTP_DeviceAttributeUpdateResponse::ptr_t iotp_device_attribute_update_response_ptr;

/**
 * DeviceAttributeUpdteHandler is an abstract class.
 * Derived class must implement UpdateDeviceLocation() method.
 */

class IOTP_DeviceAttributeHandler : public IOTP_MessageHandler {
public:
	/** Pointer type for this object */
	typedef std::shared_ptr<IOTP_DeviceAttributeHandler> ptr_t;

	virtual ~IOTP_DeviceAttributeHandler() {}
	iotp_reply_message_ptr message_arrived(const std::string& topic, Json::Value& jsonPayload);
	iotp_reply_message_ptr message_arrived(const std::string& topic, mqtt::const_message_ptr msg);

	/**
	 * If this operation can be initiated immediately, set rc to 202.
	 * If the reboot attempt fails, set rc to 500 and set the message field accordingly.
	 * If reboot is not supported, set rc to 501 and optionally set the message field accordingly.
	 *
	 * The reboot action is considered complete when the device sends a Manage device request following reboot.
	 */
	virtual bool UpdateLocation(iotf_device_location_ptr&) = 0;

	/**
	 * A response must be sent back indicating whether or not the factory reset action can be initiated.
	 * The response code should be set to 202 if this action can be initiated immediately.
	 * If the factory reset attempt fails, set rc to 500 and set the message field accordingly.
	 * If the factory reset action is not supported, set rc to 501 and optionally set the message field accordingly.
	 *
	 * The factory reset action is considered complete when the device sends a Manage device request following its factory reset.
	 */
	//virtual bool UpdateMetadata() = 0;

	/**
	 * A response must be sent back indicating whether or not the factory reset action can be initiated.
	 * The response code should be set to 202 if this action can be initiated immediately.
	 * If the factory reset attempt fails, set rc to 500 and set the message field accordingly.
	 * If the factory reset action is not supported, set rc to 501 and optionally set the message field accordingly.
	 *
	 * The factory reset action is considered complete when the device sends a Manage device request following its factory reset.
	 */
	//virtual bool UpdateDeviceInfo() = 0;
};
typedef IOTP_DeviceAttributeHandler::ptr_t iotp_device_attribute_handler_ptr;
}

#endif /* SRC_IOTPDEVICEATTRIBUTEHANDLER_H_ */
