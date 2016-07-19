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
 *    Hari Prasada reddy - Impelemented changes to add Client library functionality
 *******************************************************************************/

#ifndef IOTF_REPLYMESSAGE_H_
#define IOTF_REPLYMESSAGE_H_

#include "json/json.h"

namespace Watson_IOTP {

/**
 * IOTP_ReplyMessage is designed to be used internally within the Device Management library.
 */
class IOTP_ReplyMessage {
public:
	typedef std::shared_ptr<IOTP_ReplyMessage> ptr_t;
	IOTP_ReplyMessage(std::string topic, Json::Value& payload) : mTopic(topic), mPayload(payload), mQos(1) {}
	IOTP_ReplyMessage(std::string topic, Json::Value& payload, int qos) : mTopic(topic), mPayload(payload), mQos(qos) {}
	std::string getTopic() { return mTopic; }
	Json::Value& getPayload() { return mPayload; }
	int getQos() { return mQos; }

private:
	std::string mTopic;
	Json::Value mPayload;
	int mQos;
};
typedef IOTP_ReplyMessage::ptr_t iotp_reply_message_ptr;
}



#endif /* IOTF_REPLYMESSAGE_H_ */
