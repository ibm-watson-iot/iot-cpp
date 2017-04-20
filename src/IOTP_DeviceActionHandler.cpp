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
#include <memory>
#include <string>
#include <typeinfo>
#include "IOTP_Client.h"
#include "IOTP_DeviceActionHandler.h"
#include "IOTP_ReplyMessage.h"
#include "mqtt/message.h"
#include "IOTP_TopicDefinitions.h"

namespace Watson_IOTP {

//	extern const std::string& SERVER_DEVICE_REBOOT_TOPIC;
//	extern const std::string& SERVER_FACTORY_RESET_TOPIC;
//	extern const std::string& DEVICE_REPONSE_TOPIC;

	iotp_reply_message_ptr IOTP_DeviceActionHandler::message_arrived(const std::string& topic, Json::Value& jsonPayload) {

		iotp_device_action_response_ptr rsp;

		if (topic == SERVER_DEVICE_REBOOT_TOPIC) {
			rsp = reboot();
		} else if (topic == SERVER_FACTORY_RESET_TOPIC) {
			rsp = factory_reset();
		}

		std::cout<<"Rc:"<<rsp->get_rc()<<std::endl;
		std::cout<<"Msg:"<<rsp->get_message()<<std::endl;
		std::cout<<"ReqId"<<jsonPayload.get("reqId", "").asString()<<std::endl;
		Json::Value payload;
		payload["rc"] = rsp->get_rc();
		if (rsp->get_message().empty() == false) {
			payload["message"] = rsp->get_message();
		}
		payload["reqId"] = jsonPayload.get("reqId", "").asString();

		IOTP_ReplyMessage reply(DEVICE_REPONSE_TOPIC, payload);
		iotp_reply_message_ptr replyPtr = std::make_shared<IOTP_ReplyMessage>(reply);

//		std::cout<<"reply message:"<<payload["message"]<<std::endl;
//		std::cout<<"reply reqId:"<<payload["reqId"]<<std::endl;
//		std::cout<<"reply rc:"<<payload["rc"]<<std::endl;
		if (mClient != nullptr) {
			mClient->IOTF_send_reply(replyPtr);
			return nullptr;
		}
		return replyPtr;
	}

	iotp_reply_message_ptr IOTP_DeviceActionHandler::message_arrived(const std::string& topic, mqtt::const_message_ptr msg) {
		Json::Value jsonPayload;
		Json::Reader reader;
		iotp_reply_message_ptr replyPtr;

		std::cout<<"Message Arrived: msgPtr"<<std::endl;

		if (reader.parse(msg->get_payload(), jsonPayload)) {
			replyPtr = message_arrived(topic, jsonPayload);
		} else {
			std::cout << typeid(*this).name() << " Message arrived at TOPIC " << topic << " is not a JSON string." << std::endl;
		}
		return nullptr;
	}


}
