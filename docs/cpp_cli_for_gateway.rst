C++ Client Library - Gateway 
============================

Introduction
------------
This Gateway Section describes how to use Gateway Client with the C++ ibmiotf client library. For help with getting started with this module, see `C++ Client Library - Introduction <https://github.com/ibm-watson-iot/iot-cpp/blob/master/README.md>`_.

Using Constructor to create GatewayClient instance with Properties Object
-------------------------------------------------------------------------------

The constructor builds the Gateway client instance by accepting a Properties object containing the following definitions:

* org - Your organization ID.
* domain - (Optional) The messaging endpoint URL. By default the value is "internetofthings.ibmcloud.com"(Watson IoT Production server)
* type - The type of your Gateway device.
* id - The ID of your Gateway.
* auth-method - Method of authentication (The only value currently supported is "token"). 
* auth-token - API key token.
* clientTrustStorePath - Path to Watson IoT Server Certificate.
* port - Port Number to use for connection. Two supported secure ports are 8883 and 443.

The Properties class has setter/getter methods to initialize the values which are used to interact with the Watson IoT Platform module. 

The following code snippet shows how to construct the GatewayClient instance using Properties Object:

.. code:: C++
    
    	//Initialize Properties Instance
  	Properties p;
	p.setorgId("Org-Id");
        p.setdomain("internetofthings.ibmcloud.com");
        p.setdeviceType("gType");
        p.setdeviceId("gId");
        p.setauthMethod("token");
        p.setauthToken("password");
        p.setPort(8883);
        p.settrustStore("iot-cpp-home/IoTFoudnation.pem")
	
	//Instantiate GatewayClient
	IOTP_GatewayClient client(p);



Using Constructor to create GatewayClient instance with Configuration File
--------------------------------------------------------------------------

Instead of including a Properties object directly, you can use a configuration file containing the name-value pairs for Properties. If you are using a configuration file containing a Properties object, use the following code format.

.. code:: C++

    //Pass configuration file path as argument
    IOTP_GatewayClient client("../samples/gateway.cfg");
	
 

The Gateway device configuration file must be in the following format:

::

    {
	"Organization-ID": $orgId,
	"domain": $domain,
	"Device-Type": $gatewayType,
	"Device-ID": $gatewayId,
	"Authentication-Method": $authMethod,
	"Authentication-Token": $authToken,
	"Port" : 8883 or 443,
	"clientTrustStorePath" : "iot-cpp-home/IoTFoundation.pem"
    }


Connecting to the Watson Internet of Things Platform
----------------------------------------------------

Connect to the Watson Internet of Things Platform by calling the **connect** method. Also, one can use the **setKeepAliveInterval(int)** method before calling connect() to set the MQTT "keep alive" interval. This value, measured in seconds, defines the maximum time interval between messages sent or received. It enables the client to detect if the server is no longer available, without having to wait for the TCP/IP timeout. The client will ensure that at least one message travels across the network within each keep alive period. In the absence of a data-related message during the time period, the client sends a very small "ping" message, which the server will acknowledge. A value of 0 disables keepalive processing in the client. The default value is 60 seconds.

.. code:: C++

    IOTP_GatewayClient client(prop);
    client.setKeepAliveInterval(80);
    client.connect();
    

As the GatewayClient connects to Watson IoT Platform, then it can:

* Publish events for itself and on behalf of devices connected behind the Gateway.
* Subscribe to commands for itself and on behalf of devices behind the Gateway.


Publishing events
-------------------------------------------------------------------------------
Events are the mechanism by which Gateways/devices publish data to the Watson IoT Platform. The Gateway/device controls the content of the event and assigns a name for each event it sends.

**The Gateway can publish events from itself and on behalf of any device connected via the Gateway**.

When an event is received by the IBM Watson IoT Platform the credentials of the connection on which the event was received are used to determine from which Gateway the event was sent. With this architecture it is impossible for a Gateway to impersonate another device.

Events can be published at any of the three `quality of service levels <../messaging/mqtt.html#/>`__ defined by the MQTT protocol.  By default events will be published as qos level 0.

Publish Gateway event 
~~~~~~~~~~~~~~~~~~~~~~
.. code:: C++
    
    	std::string jsonMessage;
    	jsonMessage = "{\"Data\": {\"Temp\": \"34\" } }";//fastWriter.write(jsonPayload);
	std::cout << "Publishing Gateway event:" << std::endl << jsonMessage << std::endl << std::flush;
	client.publishGatewayEvent("status", "json", jsonMessage.c_str(), 1);



Publish Device event
~~~~~~~~~~~~~~~~~~~~

The Gateway can publish events on behalf of any device connected via the Gateway by passing the appropriate typeId and deviceId based on the origin of the event. The device gets added automatically in IBM Watson IoT Platform when Gateway publishes any event/subscribes to any commands for the devices connected to it:

.. code:: C++

    std::string jsonMessage;
    jsonMessage = "{\"Data\": {\"Temp\": \"34\" } }";//fastWriter.write(jsonPayload);
    std::cout << "Publishing Device event:" << std::endl << jsonMessage << std::endl << std::flush;
    client.publishDeviceEvent("raspi", "pi1", "status", "json", jsonMessage.c_str(), 1);

One can use the overloaded publishDeviceEvent() method to publish the device event in the desired quality of service. Refer to `MQTT Connectivity for Gateways <https://docs.internetofthings.ibmcloud.com/gateways/mqtt.html>`__ documentation to know more about the topic structure used.


Handling commands
-------------------------------------------------------------------------------
The Gateway can subscribe to commands directed at the gateway itself and to any device connected via the gateway. When the Gateway client connects, it automatically subscribes to any commands for this Gateway. But to subscribe to any commands for the devices connected via the Gateway, use one of the overloaded subscribeToDeviceCommands() method, for example,

.. code:: C++

    //Automatically subscribes to any gateway commands
    client.connect()
    
    // subscribe to commands on behalf of device
    client.subscribeDeviceCommands(DEVICE_TYPE, DEVICE_ID);

To process specific commands you need to register a command callback method. The messages are returned as an instance of the Command class which has the following properties:


* std::string deviceType;
* std::string deviceId;
* std::string commandName;
* std::string format;
* std::string payload;


A sample implementation of the Command callback is shown below,

.. code:: C++

    class MyCommandCallback: public CommandCallback{
	void processCommand(Command& cmd){
		std::cout<<"Received Command \n"
		<<"Device Type:"<<cmd.getDeviceType()<<"\t Device Id:"<<cmd.getDeviceId()<<"\t Command Name:"<<cmd.getCommandName()
		<<"\t format:"<<cmd.getFormat()<<" \t payload:"<<cmd.getPayload()<<"\n";
		}
	};
    	
    	
  
Once the Command callback is added to the GatewayClient, the processCommand() method is invoked whenever any command is published on the subscribed criteria, The following snippet shows how to add the command call back into GatewayClient instance,

.. code:: C++

    	client.connect()
    	MyCommandCallback myCallback;
	client.setCommandHandler(&myCallback);
	client.subscribeDeviceCommands("raspi", "pi1");


Overloaded methods are available to control the command subscription. 

For complete code sample, refer to our `GatewaySample <https://github.com/ibm-watson-iot/iot-cpp/blob/master/samples/sampleGateway.cpp>`_ Program.

----
