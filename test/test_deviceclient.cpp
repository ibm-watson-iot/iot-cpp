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
 *    Lokesh K Haralakatta - Unit Tests to test IOTP_DeviceClient code
 *    Lokesh K Haralakatta - Added unit tests for custom port support
 *******************************************************************************/

#include <cpptest.h>
#include <thread>

#include "IOTP_DeviceClient.h"
#include "Properties.h"

using namespace std;
using namespace Watson_IOTP;

class deviceClientTest : public Test::Suite {
    private:
        void testInitializeDeviceClient();
        void testInitializeDeviceClientFromFile();
        void testConnectAndPublishInQSMode();
        void testConnectAndPublishInRegMode();
        void testConnectAndPublishWith443();

    public:
        deviceClientTest( ) {
                TEST_ADD (deviceClientTest::testInitializeDeviceClient);
                TEST_ADD (deviceClientTest::testInitializeDeviceClientFromFile);
                TEST_ADD (deviceClientTest::testConnectAndPublishInQSMode);
                TEST_ADD (deviceClientTest::testConnectAndPublishInRegMode);
                TEST_ADD (deviceClientTest::testConnectAndPublishWith443);
        }
};

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

void deviceClientTest:: testInitializeDeviceClient(){
        //Set properties instance
        Properties p;
        p.setorgId("testorg");
        p.setdomain("internetofthings.ibmcloud.com");
        p.setdeviceType("type");
        p.setdeviceId("id");
        p.setauthMethod("token");
        p.setauthToken("password");
        p.setPort(443);
        p.settrustStore("IoTFoundation.pem");

        //Create Device Client Instance using Properties Instance
        IOTP_DeviceClient client(p);

        //Retrieve Device Client Instance properties and verify
        TEST_ASSERT(client.getorgId().compare(p.getorgId()) == 0);
        TEST_ASSERT(client.getdomain().compare(p.getdomain()) == 0);
        TEST_ASSERT(client.getdeviceType().compare(p.getdeviceType()) == 0);
        TEST_ASSERT(client.getdeviceId().compare(p.getdeviceId()) == 0);
        TEST_ASSERT(client.getauthMethod().compare(p.getauthMethod()) == 0);
        TEST_ASSERT(client.getauthToken().compare(p.getauthToken()) == 0);
        TEST_ASSERT(client.gettrustStore().compare(p.gettrustStore()) == 0);
        TEST_ASSERT(client.getPort() == p.getPort());
}

void deviceClientTest:: testInitializeDeviceClientFromFile(){
        //Create Device Client Instance using test.cfg file
        IOTP_DeviceClient client("../test/test.cfg");

        //Retrieve Device Client Instance properties and verify
        TEST_ASSERT(client.getorgId().compare("testorg") == 0);
        TEST_ASSERT(client.getdomain().compare("internetofthings.ibmcloud.com") == 0);
        TEST_ASSERT(client.getdeviceType().compare("type") == 0);
        TEST_ASSERT(client.getdeviceId().compare("id") == 0);
        TEST_ASSERT(client.getauthMethod().compare("token") == 0);
        TEST_ASSERT(client.getauthToken().compare("password") == 0);
        TEST_ASSERT(client.gettrustStore().compare("IoTFoundation.pem") == 0);
        TEST_ASSERT(client.getPort() == 8883);
}

void deviceClientTest:: testConnectAndPublishInQSMode(){
        //Set properties instance
        Properties p;
        p.setorgId("quickstart");
        p.setdomain("internetofthings.ibmcloud.com");
        p.setdeviceType("type");
        p.setdeviceId("id");

        //Create Device Client Instance using Properties Instance
        IOTP_DeviceClient client(p);

        //Connect to IoTP
        TEST_ASSERT(client.connect() == true);

        //Publish Event in QS Mode
        client.publishEvent("unitTestQS","text","test message",0);

        if(client.isConnected())
                client.disconnect();

}

void deviceClientTest:: testConnectAndPublishInRegMode(){
        SampleActionListener listener;
        MyCommandCallback myCallback;
        std::string jsonMessage;

        //Create Device Client Instance using device.cfg file
        IOTP_DeviceClient client("../test/device.cfg");

        //Connect to IoTP
        TEST_ASSERT(client.connect() == true);

        //Status should be connected
        TEST_ASSERT(client.isConnected() == true);

        //Set command handler
        client.setCommandHandler(&myCallback);

        //Initialize Json Message to publishEvent
        jsonMessage = "{\"Data\": {\"Temp\": \"34\" } }";

        //Publish Event without listener
        client.publishEvent("unitTestTemp","json",jsonMessage.c_str(),0);
        this_thread::sleep_for (chrono::seconds(1));

        //Publish Event with listener
        client.publishEvent("unitTestTemp","json",jsonMessage.c_str(),0,listener);
        this_thread::sleep_for (chrono::seconds(1));

        //Disconnect gateway client if connected
        if(client.isConnected())
                client.disconnect();
}

void deviceClientTest:: testConnectAndPublishWith443(){
        SampleActionListener listener;
        MyCommandCallback myCallback;
        std::string jsonMessage;

        //Create Device Client Instance using device_443.cfg file
        IOTP_DeviceClient client("../test/device_443.cfg");

        //Connect to IoTP
        TEST_ASSERT(client.connect() == true);

        //Status should be connected
        TEST_ASSERT(client.isConnected() == true);

        //Set command handler
        client.setCommandHandler(&myCallback);

        //Initialize Json Message to publishEvent
        jsonMessage = "{\"Data\": {\"Temp\": \"34\" } }";

        //Publish Event without listener
        client.publishEvent("unitTestTemp","json",jsonMessage.c_str(),0);
        this_thread::sleep_for (chrono::seconds(1));

        //Publish Event with listener
        client.publishEvent("unitTestTemp","json",jsonMessage.c_str(),0,listener);
        this_thread::sleep_for (chrono::seconds(1));

        //Disconnect gateway client if connected
        if(client.isConnected())
                client.disconnect();
}

int main ( )
{
  deviceClientTest tests;
  Test::TextOutput output(Test::TextOutput::Verbose);
  return tests.run(output);
}
