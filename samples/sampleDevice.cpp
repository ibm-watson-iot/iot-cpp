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
 #include <csignal>
 #include <thread>

 #include "IOTP_DeviceClient.h"
 #include "Properties.h"

 using namespace std;
 using namespace Watson_IOTP;

 /**
  * A base action listener.
  */
 class SampleActionListener : public virtual mqtt::iaction_listener
 {
 protected:
 	virtual void on_failure(const mqtt::itoken& tok) {
 		cout << "\nListener: Failure on token: " << tok.get_message_id() << endl;
 	}

 	virtual void on_success(const mqtt::itoken& tok) {
 		cout << "\nListener: Success on token: " << tok.get_message_id() << endl;
 	}
 };

 //Implement the CommandCallback class to provide the way in which you want the command to be handled
 class MyCommandCallback: public CommandCallback{
 	/**
 	 * This method is invoked by the library whenever there is command matching the subscription criteria
 	 */
 	void processCommand(Command& cmd){
 		cout<<"Received Command: \nCommand Name:"<<cmd.getCommandName()<<"\t format:"
                    <<cmd.getFormat()<<" \t payload:"<<cmd.getPayload()<<"\n";
 	}
 };

 volatile int interrupt = 0;

 void signalHandler( int signum) {
    cout << "Interrupt signal (" << signum << ") received.\n";

    interrupt = 1;

 }

 int main(int argc, char **argv) {
        // register signal SIGINT and signal handler
        signal(SIGINT, signalHandler);

        Properties p;
        bool success = false;
        int rc = -1;
        SampleActionListener listener;
        MyCommandCallback myCallback;
        std::string jsonMessage;
        //Remove comments from below statements to initialize using properties from program
        //Make sure to provide right properties in below method calls
        /*p.setorgId("Org-ID");
        p.setdomain("internetofthings.ibmcloud.com");
        p.setdeviceType("type");
        p.setdeviceId("id");
        p.setauthMethod("token");
        p.setauthToken("password");
        p.setPort("8883");
        p.settrustStore("iot-cpp-home/IoTFoudnation.pem")*/

        try {
                //Remove comment from below statement to initialize DeviceClient using Properties Instance
                //IOTP_DeviceClient client(p);
                //Comment below statement if DeviceClient gets instantiated above using Properties Instance
                IOTP_DeviceClient client("../samples/device.cfg");
                success = client.connect();
                if(success){
                        client.setCommandHandler(&myCallback);
                        cout<<"\n\n PRESS CTRL-C to QUIT\n\n";
                        while(!interrupt)
                        {
                            jsonMessage = "{\"Data\": {\"Temp\": \"34\" } }";
                            cout << "Publishing event without listener :" << jsonMessage << "\n";
                            // First publish event without listener.
                            client.publishEvent("temp", "json", jsonMessage.c_str(), 0);
                            this_thread::sleep_for (chrono::seconds(1));
                            jsonMessage = "{\"Data\": {\"Temp\": \"44\" } }";
                            cout << "Publishing event with listener:" << jsonMessage << "\n";
                            // Next publish event with listener.
                            client.publishEvent("temp", "json", jsonMessage.c_str(), 1, listener);
                            this_thread::sleep_for (chrono::seconds(3));
                        }
                        client.disconnect();
                }
                else{
                        client.console.error("Failed connecting client to Watson IoT platform...");
                }

                rc = 0;
        }
        catch (const mqtt::exception& exc) {
		cout<<"Error: "<<exc.what();
                rc = -1;
	}

        return rc;
}
