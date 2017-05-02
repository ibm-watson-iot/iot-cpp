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
 *    Lokesh K Haralakatta - Unit Tests to test IOTP_GatewayClient code
 *******************************************************************************/

#include <cpptest.h>
#include <thread>

#include "IOTP_GatewayClient.h"
#include "Properties.h"

using namespace std;
using namespace Watson_IOTP;

class gatewayClientTest : public Test::Suite {
    private:
        void testInitializeGatewayClient();
        void testInitializeGatewayClientFromFile();
        void testConnectAndPubSub();

    public:
        gatewayClientTest( ) {
                TEST_ADD (gatewayClientTest::testInitializeGatewayClient);
                TEST_ADD (gatewayClientTest::testInitializeGatewayClientFromFile);
                TEST_ADD (gatewayClientTest::testConnectAndPubSub);
        }
};

/**
 * A base action listener.
 */
class SampleActionListener : public virtual mqtt::iaction_listener
{
protected:
       virtual void on_failure(const mqtt::itoken& tok) {
               std::cout << "\n\tListener: Failure on token: "
                       << tok.get_message_id() << std::endl;
       }

       virtual void on_success(const mqtt::itoken& tok) {
               std::cout << "\n\tListener: Success on token: "
                       << tok.get_message_id() << std::endl;
       }
};

class MyCommandCallback: public CommandCallback{
       void processCommand(Command& cmd){
               std::cout<<"Received Command \n"
                               <<"Device Type:"<<cmd.getDeviceType()<<"\t Device Id:"<<cmd.getDeviceId()<<"\t Command Name:"<<cmd.getCommandName()
                               <<"\t format:"<<cmd.getFormat()<<" \t payload:"<<cmd.getPayload()<<"\n";
       }
};

void gatewayClientTest:: testInitializeGatewayClient(){
        //Set properties instance
        Properties p;
        p.setorgId("testorg");
        p.setdomain("internetofthings.ibmcloud.com");
        p.setdeviceType("type");
        p.setdeviceId("id");
        p.setauthMethod("token");
        p.setauthToken("password");
        p.settrustStore("IoTFoundation.pem");

        //Create Gateway Client Instance using Properties Instance
        IOTP_GatewayClient client(p);

        //Retrieve Gateway Client Instance properties and verify
        TEST_ASSERT(client.getorgId().compare(p.getorgId()) == 0);
        TEST_ASSERT(client.getdomain().compare(p.getdomain()) == 0);
        TEST_ASSERT(client.getdeviceType().compare(p.getdeviceType()) == 0);
        TEST_ASSERT(client.getdeviceId().compare(p.getdeviceId()) == 0);
        TEST_ASSERT(client.getauthMethod().compare(p.getauthMethod()) == 0);
        TEST_ASSERT(client.getauthToken().compare(p.getauthToken()) == 0);
        TEST_ASSERT(client.gettrustStore().compare(p.gettrustStore()) == 0);
}

void gatewayClientTest:: testInitializeGatewayClientFromFile(){
        //Create Gateway Client Instance using test.cfg file
        IOTP_GatewayClient client("../test/test.cfg");

        //Retrieve Gateway Client Instance properties and verify
        TEST_ASSERT(client.getorgId().compare("testorg") == 0);
        TEST_ASSERT(client.getdomain().compare("internetofthings.ibmcloud.com") == 0);
        TEST_ASSERT(client.getdeviceType().compare("type") == 0);
        TEST_ASSERT(client.getdeviceId().compare("id") == 0);
        TEST_ASSERT(client.getauthMethod().compare("token") == 0);
        TEST_ASSERT(client.getauthToken().compare("password") == 0);
        TEST_ASSERT(client.gettrustStore().compare("IoTFoundation.pem") == 0);
}

void gatewayClientTest:: testConnectAndPubSub(){
        SampleActionListener listener;
        MyCommandCallback myCallback;
        std::string jsonMessage;

        //Create Gateway Client Instance using gateway.cfg file
        IOTP_GatewayClient client("../test/gateway.cfg");

        //Connect to IoTP
        TEST_ASSERT(client.connect() == true);

        //Status should be connected
        TEST_ASSERT(client.isConnected() == true);

        //Subscribing to Device Commands should be true
        TEST_ASSERT(client.subscribeDeviceCommands("attached","testGatewayPublish") == true);

        //Set command handler
        client.setCommandHandler(&myCallback);

        //Initialize Json message & Publish Gateway Event with and without  listener
        jsonMessage = "{\"Data\": {\"Temp\": \"34\" } }";
        client.publishGatewayEvent("utGatewayTemp", "json", jsonMessage.c_str(), 0);
        this_thread::sleep_for (chrono::seconds(1));
        client.publishGatewayEvent("utGatewayTemp", "json", jsonMessage.c_str(), 0,listener);
        this_thread::sleep_for (chrono::seconds(1));

        //Initialize Json message & Publish Device Event with and without  listener
        jsonMessage = "{\"Data\": {\"Temp\": \"44\" } }";
        client.publishDeviceEvent("attached","testGatewayPublish","utDeviceTemp",
                        "json", jsonMessage.c_str(), 0);
        this_thread::sleep_for (chrono::seconds(1));
        client.publishDeviceEvent("attached","testGatewayPublish","utDeviceTemp",
                        "json", jsonMessage.c_str(), 0,listener);
        this_thread::sleep_for (chrono::seconds(1));

        //Disconnect gateway client if connected
        if(client.isConnected())
                client.disconnect();
}

int main ( )
{
  gatewayClientTest tests;
  Test::TextOutput output(Test::TextOutput::Verbose);
  return tests.run(output);
}
