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

#include <iostream>
//#include <fstream>
//#include <cstdlib>
#include <string>
//#include <thread>
//#include <chrono>
#include <map>
//#include <cstring>
#include "mqtt/async_client.h"
#include "json/json.h"
//#include "IOTP_Device.h"
#include "IOTP_Client.h"
//#include "IOTP_DeviceClient.h"
//#include "IOTP_DeviceActionHandler.h"
//#include "IOTP_DeviceFirmwareHandler.h"
//#include "IOTP_DeviceAttributeHandler.h"

using namespace Watson_IOTP;

int main()
{
	//Quick start flow properties reading from configuration file in json format
	//Authentication is omitted in the quickstart flow
	std::cout<<"==================quick start client Mode==========================="<<std::endl;
	std::cout<<"Creating Client with properties"<<std::endl;
	IOTP_DeviceClient client(prop);
	client.setKeepAliveInterval(90);
	std::cout<<"Connecting client to Watson IoT platform"<<std::endl;
	bool success = client.connect();
	std::cout<<"Connected client to Watson IoT platform"<<std::endl;
	std::flush(std::cout);
	if(!success)
		return 1;

	MyCommandCallback myCallback;
	client.setCommandHandler(&myCallback);
	Json::Value jsonPayload;
	Json::Value jsonText;

	std::string jsonMessage = "{\"Data\": {\"Temp\": \"34\" } }";//fastWriter.write(jsonPayload);
	std::cout << "Publishing event:" << std::endl << jsonMessage << std::endl << std::flush;
	// First publish event without listner.
	client.publishEvent("status", "json", jsonMessage.c_str(), 1);

	//Publish event with listner
	std::cout << "Publishing event with listner:" << std::endl << jsonMessage << std::endl << std::flush;
	client.publishEvent("status1", "json", jsonMessage.c_str(), 1, listener);
	//Disconnect device client
	client.disconnect();
	std::cout << "===================Disconnected quickstart client================\n";

	return 1;
}
