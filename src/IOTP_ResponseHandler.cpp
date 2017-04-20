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
#include <algorithm>
#include <typeinfo>

#include "json/json.h"
#include "IOTP_ResponseHandler.h"

namespace Watson_IOTP {

	IOTP_ResponseHandler::IOTP_ResponseHandler() { }

	IOTP_ResponseHandler::~IOTP_ResponseHandler() {
		for(auto it = mResponseMessages.begin(); it != mResponseMessages.end(); ++it) {
			mResponseMessages.erase(it);
		}
	}

	iotp_reply_message_ptr IOTP_ResponseHandler::message_arrived(const std::string& topic, Json::Value& jsonPayload) {
		guard g(mLock);

		std::string reqId = jsonPayload.get("reqId", "").asString();
		if (reqId.empty() == false) {
			mResponseMessages[reqId] = jsonPayload;
		} else {
			std::cout << typeid(*this).name() << "Message arrived at TOPIC " << topic << " without a reqId field." << std::endl;
		}

		g.unlock();
		mCond.notify_all();
		return nullptr;
	}

	iotp_reply_message_ptr IOTP_ResponseHandler::message_arrived(const std::string& topic, mqtt::const_message_ptr msg) {
		Json::Value jsonPayload;
		Json::Reader reader;

		if (reader.parse(msg->get_payload(), jsonPayload)) {
			message_arrived(topic, jsonPayload);
		} else {
			std::cout << typeid(*this).name() << " Message arrived at TOPIC " << topic << " is not a JSON string." << std::endl;
		}
		return nullptr;
	}

	Json::Value const IOTP_ResponseHandler::wait_for_response(long timeout, const std::string& reqId) {
		Json::Value jsonPayload;
		guard g(mLock);
		if (mCond.wait_for(g, std::chrono::milliseconds(timeout))
				== std::cv_status::timeout)
			return jsonPayload;
		auto search = mResponseMessages.find(reqId);
		if(search != mResponseMessages.end()) {
			jsonPayload = search->second;
			mResponseMessages.erase(search);
		}
		return jsonPayload;
	}


}
