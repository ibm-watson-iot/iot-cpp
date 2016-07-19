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

#ifndef IOTF_MESSAGEHANDLER_H_
#define IOTF_MESSAGEHANDLER_H_

#include "json/json.h"
#include "mqtt/message.h"
#include "IOTP_ReplyMessage.h"

namespace Watson_IOTP {

class IOTP_Client;

/**
 * IOTP_MessageHandler is an internal abstract class to be used within the Device Management library.
 */
class IOTP_MessageHandler {

public:
	typedef std::shared_ptr<IOTP_MessageHandler> ptr_t;

	IOTP_MessageHandler() : mClient(nullptr) {};
	virtual ~IOTP_MessageHandler() {};

	/**
	 * This method is called when a message arrives from the server.
	 * @param topic
	 * @param jsonMsg
	 */
	virtual iotp_reply_message_ptr message_arrived(const std::string& topic, Json::Value& jsonPayload) =0;

	/**
	 * This method is called when a message arrives from the server.
	 * @param topic
	 * @param msg
	 */
	virtual iotp_reply_message_ptr message_arrived(const std::string& topic, mqtt::message_ptr msg) =0;

	friend IOTP_Client;

protected:
	IOTP_Client* mClient;
};
typedef IOTP_MessageHandler::ptr_t iotp_message_handler_ptr;
}


#endif /* IOTF_MESSAGEHANDLER_H_ */
