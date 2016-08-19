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
 *    Hari Prasada Reddy - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_COMMAND_H_
#define SRC_COMMAND_H_

class Command{
private:
	std::string deviceType;
	std::string deviceId;
	std::string commandName;
	std::string format;
	std::string payload;
public:
	Command(std::string type, std::string id, std::string cmdName, std::string fmt, std::string strPayload){
		deviceType = type;
		deviceId = id;
		commandName = cmdName;
		format = fmt;
		payload = strPayload;
	}
	std::string getDeviceType(){
		return deviceType;
	}
	std::string getDeviceId(){
		return deviceId;
	}
	std::string getCommandName(){
		return commandName;
	}

	std::string getFormat() {
		return format;
	}

	std::string getPayload() {
		return payload;
	}
};

#endif /* SRC_COMMAND_H_ */
