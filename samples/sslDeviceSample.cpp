/*******************************************************************************
 * Copyright (c) 2017 IBM Corp.
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
 *    Lokesh K Haralakatta
 *******************************************************************************/

 #include <iostream>

 #include "IOTP_DeviceClient.h"
 #include "Properties.h"
 #include <log4cpp/Category.hh>
 #include <log4cpp/PropertyConfigurator.hh>

 using namespace std;
 using namespace Watson_IOTP;

 int main(int argc, char **argv) {
        Properties p;
        bool success = false;
        int rc = -1;
        string initFileName = "../samples/log4cpp.properties";
	log4cpp::PropertyConfigurator::configure(initFileName);
        log4cpp::Category& logger = log4cpp::Category::getRoot();
        log4cpp::Category& console = log4cpp::Category::getInstance(std::string("clogger"));

        logger.debug("Setting the properties to connect to WIoTP");
        p.setorgId("quickstart");
        logger.debug("OrgId: "+ p.getorgId());
        //p.setdomain("messaging.internetofthings.ibmcloud.com");
        p.setdeviceType("elevator");
        logger.debug("DeviceType: "+ p.getdeviceType());
        p.setdeviceId("android");
        logger.debug("DeviceId: "+ p.getdeviceId());
        //p.setauthMethod("token");
        //p.setauthToken("password");
        logger.warn("Needs token to connect...");
        console.error("No credentials...");

        try {
                IOTP_DeviceClient client(p);
                console.info("Created IoTP Client with set properties...");

                success = client.connect();
                if(success){
                        console.info("Connected client to Watson IoT platform...");
                        client.disconnect();
                        console.info("Client disconnected from Watson IoT platform...");
                }
                else{
                        logger.warn("Failed connecting client to Watson IoT platform...");
                }

                rc = 0;
        }
        catch (const mqtt::exception& exc) {
		console.error("Error: ",exc.what());
                rc = -1;
	}

        log4cpp::Category::shutdown();
        return rc;
}
