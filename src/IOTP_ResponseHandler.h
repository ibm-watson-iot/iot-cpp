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

#ifndef IOTF_RESPONSEHANDLER_H_
#define IOTF_RESPONSEHANDLER_H_

#include <mutex>
#include <condition_variable>

#include "IOTP_MessageHandler.h"

namespace Watson_IOTP {

class IOTP_ResponseHandler : public IOTP_MessageHandler {
public:
	typedef std::shared_ptr<IOTP_ResponseHandler> ptr_t;

	IOTP_ResponseHandler();

	~IOTP_ResponseHandler();

	iotp_reply_message_ptr message_arrived(const std::string& topic, Json::Value& jsonPayload);

	iotp_reply_message_ptr message_arrived(const std::string& topic, mqtt::const_message_ptr msg);

	Json::Value const wait_for_response(long timeout, const std::string& reqId);

private:
	typedef std::unique_lock<std::mutex> guard;
	mutable std::mutex mLock;
	mutable std::condition_variable mCond;
	std::map<std::string, Json::Value> mResponseMessages;
};
typedef IOTP_ResponseHandler::ptr_t iotp_response_handler_ptr;
}



#endif /* IOTF_RESPONSEHANDLER_H_ */
