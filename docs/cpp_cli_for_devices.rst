===============================================================================
C++ Client Library - Devices
===============================================================================

Introduction
-------------------------------------------------------------------------------

This client library describes how to use devices with the C++ ibmiotf client library. For help with getting started with this module, see `C++ Client Library - Introduction <https://github.com/ibm-watson-iot/iot-cpp/blob/master/README.md>`__. 

Constructor
-------------------------------------------------------------------------------

The constructor builds the client instance, and accepts a Properties object containing the following definitions:

* org - Your organization ID. (This is a required field. In case of quickstart flow, provide org as quickstart.)
* domain - (Optional) The messaging endpoint URL. By default the value is "internetofthings.ibmcloud.com"(Watson IoT Production server)
* type - The type of your device. (This is a required field.)
* id - The ID of your device. (This is a required field.
* auth-method - Method of authentication (This is an optional field, needed only for registered flow and the only value currently supported is "token"). 
* auth-token - API key token (This is an optional field, needed only for registered flow).


The Properties object creates definitions which are used to interact with the Watson IoT Platform module. 

The following code shows a device publishing events in a Quickstart mode.


.. code:: C++



	//Qick start mode
	prop.setorgId("quickstart");
	prop.setdeviceType("devicetest");
	prop.setdeviceId("haritestdevice");

	std::cout<<"Creating IoTP Client with properties for quickstart mode"<<std::endl;
	IOTP_DeviceClient quickClient(prop);
	std::cout << "Connecting quick start client to Watson IoT platform" << std::endl;
	success = quickClient.connect();
	std::flush(std::cout);
	if (!success){
		std::cout<<"Connection failed\n";
		return 1;
	}

	std::cout<<"Connection successful"<<std::endl;

	jsonMessage = "{\"Data\": {\"Temp\": \"34\" } }"; //fastWriter.write(jsonPayload);
	std::cout << "Publishing event:" << std::endl << jsonMessage << std::endl
			<< std::flush;
	// First publish event without listner.
	quickClient.publishEvent("status", "json", jsonMessage.c_str(), 1);
	std::cout<<"Published success\n";
	quickClient.disconnect();
	std::cout<<"Disconnected qucik start client"<<std::endl;
      ...

 


The following program shows a device publishing events in a registered flow


.. code:: C++


	//Registered device flow properties reading from configuration file in json format
	std::cout<<"Connecting client to Watson IoT platform"<<std::endl;
	success = client.connect();
	std::cout<<"Connected client to Watson IoT platform"<<std::endl;
	std::flush(std::cout);
	if(!success)
		return 1;

	MyCommandCallback myCallback;
	client.setCommandHandler(&myCallback);
	client.subscribeCommands();
	Json::Value jsonPayload;
	Json::Value jsonText;

	jsonMessage = "{\"Data\": {\"Temp\": \"34\" } }";//fastWriter.write(jsonPayload);
	std::cout << "Publishing event:" << std::endl << jsonMessage << std::endl << std::flush;
	// First publish event without listner.
	client.publishEvent("status", "json", jsonMessage.c_str(), 1);

	//Publish event with listner
	std::cout << "Publishing event with listner:" << std::endl << jsonMessage << std::endl << std::flush;
	client.publishEvent("status1", "json", jsonMessage.c_str(), 1, listener);
	//Disconnect device client
	client.disconnect();
	std::cout << "Disconnected registered client\n";

      ...



Using a configuration file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Instead of including a Properties object directly, you can use a configuration file containing the name-value pairs for Properties. If you are using a configuration file containing a Properties object, use the following code format.

.. code:: C++


	//Registered device flow properties reading from configuration file in json format
	std::cout<<"Creating IoTP Client with properties"<<std::endl;
	IOTP_DeviceClient client(prop);
	client.setKeepAliveInterval(90);
	std::cout<<"Connecting client to Watson IoT platform"<<std::endl;
	success = client.connect();
	std::cout<<"Connected client to Watson IoT platform"<<std::endl;
	std::flush(std::cout);
	if(!success)
		return 1;

	MyCommandCallback myCallback;
	client.setCommandHandler(&myCallback);
	Json::Value jsonPayload;
	Json::Value jsonText;

	jsonMessage = "{\"Data\": {\"Temp\": \"34\" } }";
	std::cout << "Publishing event:" << std::endl << jsonMessage << std::endl << std::flush;
	// First publish event without listner.
	client.publishEvent("status", "json", jsonMessage.c_str(), 1);
		
      ...

The content of the configuration file must be in the following format:

::

  {
	"org": $orgId,
	"domain": $domain,
	"type": $deviceType,
	"id": $deviceId,
	"auth-method": $authMethod,
	"auth-token": $authToken,
}


----

Connecting to the Watson IoT Platform
----------------------------------------------------

Connect to the Watson IoT Platform by calling the *connect* function. 

Also, one can use the setKeepAliveInterval(int) method before calling connect() to set the MQTT "keep alive" interval. This value, measured in seconds, defines the maximum time interval between messages sent or received. It enables the client to detect if the server is no longer available, without having to wait for the TCP/IP timeout. The client will ensure that at least one message travels across the network within each keep alive period. In the absence of a data-related message during the time period, the client sends a very small "ping" message, which the server will acknowledge. A value of 0 disables keepalive processing in the client. The default value is 60 seconds.

.. code:: C++

	IOTP_DeviceClient client(prop);
	client.setKeepAliveInterval(90);
	std::cout<<"Connecting client to Watson IoT platform"<<std::endl;
	success = client.connect();
    
After the successful connection to the IoTF service, the device client can perform the following operations, like publishing events and subscribe to device commands from application.

----


Publishing events
-------------------------------------------------------------------------------
Events are the mechanism by which devices publish data to the Watson IoT Platform. The device controls the content of the event and assigns a name for each event it sends.

When an event is received by the IBM IoT Foundation the credentials of the connection on which the event was received are used to determine from which device the event was sent. With this architecture it is impossible for a device to impersonate another device.

Events can be published at any of the three `quality of service levels <https://docs.internetofthings.ibmcloud.com/messaging/mqtt.html#/>` defined by the MQTT protocol.  By default events will be published as qos level 0.

Publish event using user-defined quality of service
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Events can be published at higher MQTT quality of servive levels, but these events may take slower than QoS level 0, because of the extra confirmation of receipt. Also Quickstart flow allows only Qos of 0

.. code:: C++

	std::string jsonMessage;
	jsonMessage = "{\"Data\": {\"Temp\": \"34\" } }"; 
	client.publishEvent("status", "json", jsonMessage.c_str(), 1);

----

Handling commands
-------------------------------------------------------------------------------
When the device client connects it automatically subscribes to any command for this device. To process specific commands you need to register a command callback method. 
The messages are returned as an instance of the Command class which has the following properties:

* std::string deviceType;
* std::string deviceId;
* std::string commandName;
* std::string format;
* std::string payload;


.. code:: C++


	//Implement the CommandCallback class to provide the way in which you want the command to be handled
	class MyCommandCallback: public CommandCallback{
		/**
	 	* This method is invoked by the library whenever there is command matching the subscription criteria
	 	*/
		void processCommand(Command& cmd){
			std::cout<<"Received Command \n"
			<<"Command Name:"<<cmd.getCommandName()<<"\t format:"<<cmd.getFormat()<<" \t payload:"<<cmd.getPayload()<<"\n";
		}
	};

	//Registered device flow properties reading from configuration file in json format
	std::cout<<"Creating IoTP Client with properties"<<std::endl;
	IOTP_DeviceClient client(prop);
	client.setKeepAliveInterval(90);
	std::cout<<"Connecting client to Watson IoT platform"<<std::endl;
	success = client.connect();
	std::cout<<"Connected client to Watson IoT platform"<<std::endl;
	std::flush(std::cout);
	if(!success)
		return 1;

	MyCommandCallback myCallback;
	client.setCommandHandler(&myCallback);
	

----
