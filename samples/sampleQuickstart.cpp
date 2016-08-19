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
#include <fstream>
#include <unistd.h>
#include <string>
#include <map>
#include "mqtt/async_client.h"
#include "json/json.h"
#include "IOTP_Device.h"
#include "IOTP_Client.h"
#include "IOTP_DeviceClient.h"

using namespace Watson_IOTP;
int InitializeProperties(const std::string& filePath,Properties& prop);
int main(int argc, char **argv) {

	Properties prop;	
	if(argc > 1) {
                std::cout<<"Initializing properties for registered client"<<std::endl;
                if(!InitializeProperties(argv[1],prop))
                {
                        std::cout<<"Failed initializing properties from configuration file"<<std::endl;
                        return 1;
                }
        }
        else {
                std::cout << "Usage: sampleDevice <Path of configuration file in json format>" << std::endl;
                return 1;
        }


        try {
	//Authentication is omitted in the quickstart flow
	//Quick start flow properties reading from configuration file in json format
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

	Json::Value jsonPayload;
	Json::Value jsonText;

	std::string jsonMessage = "{\"d\": {\"Temp\": \"34\" } }";//fastWriter.write(jsonPayload);
	for(int i=0; i<10; i++) {
		std::cout<<"Publishing event:"<<jsonMessage<<std::endl;	
		// publish event without listner.
		client.publishEvent("status", "json", jsonMessage.c_str(), 0);
		sleep(10);
	}
	//Disconnect device client
	client.disconnect();
	std::cout << "===================Disconnected quickstart client================\n";
	}
        catch (const mqtt::exception& exc) {
                std::cerr << "Error: " << exc.what() << std::endl;
                return 1;
        }

	return 0;
}

int InitializeProperties(const std::string& filePath,Properties& prop) {
        Json::Reader reader;
        Json::Value root;
        std::filebuf fb;
        if (fb.open(filePath, std::ios::in)) {
                std::istream is(&fb);
                if (!reader.parse(is, root)) {
                        std::cout << "Failed to parse test configuration from input file "
                                        << filePath << std::endl;
                        fb.close();
                        return 0;
                }
                fb.close();
        }

        std::string org = root.get("Organization-ID", "").asString();
        if (org.size() == 0) {
                std::cout << "Failed to get Organization-ID from test configuration."
                                << std::endl;
                return 0;
        }
        else
                prop.setorgId(org);


        std::string domain = root.get("domain", "").asString();
        if (domain.size() != 0)
                prop.setdomain(domain);


        std::string deviceType = root.get("Device-Type", "").asString();
        if (deviceType.size() == 0) {
                std::cout << "Failed to get Device-Type from test configuration."
                                << std::endl;
                return 0;
        }
        else
                prop.setdeviceType(deviceType);

        std::string deviceId = root.get("Device-ID", "").asString();
        if (deviceId.size() == 0) {
                std::cout << "Failed to get Device-ID from test configuration."
                                << std::endl;
                return 0;
        }
	else
		prop.setdeviceId(deviceId);

        if(org.compare("quickstart") != 0) {
                std::string username = root.get("Authentication-Method", "").asString();

                if (username.size() == 0) {
                        std::cout << "Failed to get Authentication-Method from test configuration."
                                << std::endl;
                        return 0;
                }
                else
                        prop.setauthMethod(username);

                std::string password = root.get("Authentication-Token", "").asString();
                if (password.size() == 0) {
                        std::cout << "Failed to get Authentication-Token from test configuration."
                                        << std::endl;
                        return 0;
                }
                else
                        prop.setauthToken(password);
        }


        return 1;
}

