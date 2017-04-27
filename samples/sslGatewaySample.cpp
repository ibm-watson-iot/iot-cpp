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

 #include "IOTP_GatewayClient.h"
 #include "Properties.h"

 using namespace std;
 using namespace Watson_IOTP;

 int main(int argc, char **argv) {
        Properties p;
        bool success = false;
        int rc = -1;
        /*p.setorgId("Org-Id");
        p.setdomain("internetofthings.ibmcloud.com");
        p.setdeviceType("gType");
        p.setdeviceId("gId");
        p.setauthMethod("token");
        p.setauthToken("password");*/

        try {
                //IOTP_GatewayClient client(p);
                IOTP_GatewayClient client("../samples/gateway.cfg");
                success = client.connect();
                if(success){
                        client.subscribeDeviceCommands("attached","testGatewayPublish");
                        client.publishGatewayEvent("test", "text", "sample gateway data", 0);
                        client.publishDeviceEvent("attached","testGatewayPublish","test", "text", "sample gateway data", 0);
                        client.disconnect();
                }
                else{
                        client.console.error("Failed connecting gateway client to Watson IoT platform...");
                }

                rc = 0;
        }
        catch (const mqtt::exception& exc) {
		std::cerr << "Error: " << exc.what() << std::endl;
	}

        return rc;
}
