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

#ifndef DEVICERESPONSEMESSAGE_H_
#define DEVICERESPONSEMESSAGE_H_

#include "json/json.h"

namespace Watson_IOTP {

class IOTP_DeviceResponseMessage {

public:
	/** Pointer type for this object */
	typedef std::shared_ptr<IOTP_DeviceResponseMessage> ptr_t;

	IOTP_DeviceResponseMessage(int rc) : mRC(rc), mMessage("") { setJsonValue(); }
	IOTP_DeviceResponseMessage(int rc, std::string& message) : mRC(rc), mMessage(message) {setJsonValue();}
	const Json::Value& toJsonValue() const{
		return json;
	}

private:
	void setJsonValue() {
		json["rc"] = mRC;
		if (!mMessage.empty())
			json["message"] = mMessage;
	};
	int mRC;
	Json::Value json;
	std::string mMessage;
};

typedef IOTP_DeviceResponseMessage::ptr_t iotp_device_reponse_msg_ptr;

}



#endif /* DEVICERESPONSEMESSAGE_H_ */
