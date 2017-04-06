Device Management update in progress

C++ Client Library - Introduction
============================================

This C++ Client Library can be used to simplify interactions with the [IBM Watson IoT Platform] (https://internetofthings.ibmcloud.com). 

Installing the Dependencies
---------------------------
Run the setup.sh file in the iot-cpp directory.
This script will install following Dependencies and it will copy the dependencies into the lib directory after making necessary changes.

Dependencies
------------

1.  [paho mqtt cpp Client]

  [paho mqtt cpp Client]: https://eclipse.org/paho/clients/cpp/
  Following version of paho MQTT library is used for this client library
  https://github.com/eclipse/paho.mqtt.cpp/archive/master.zip
  
2.  [paho mqtt c Client]
  https://github.com/eclipse/paho.mqtt.c/archive/master.zip

3.  jsoncpp
  https://github.com/open-source-parsers/jsoncpp/archive/master.zip

Note:
If the dependencies were not able to be installed with the script on the device, manually pull the dependencies from the links mentioned above and follow the steps in the setup.sh file for making the necessary changes.

Build Instructions 
------------------
For building the client library and the sample application cmake is required.
Install cmake by following the instrctions in the link https://cmake.org/install/
Once cmake is installed 

1.  Go to the build directory under root directory (iot-cpp/build).
2.  Run the command cmake ..
3.  Run the command make
All the output files will be created under the build directory.

Running samples
---------------
Execute quick start sample (sampleQuickstart) with the following command from build directory
./samples/sampleQuickstart <Path to quick start configuration file in json format>

Similarly device client (sampleDevice) can be executed with the following command from build directory
./samples/sampleDevice <Path to device configuration file in json format>

Similarly gateway client (sampleGateway) can be executed with the following command from build directory
./samples/sampleGateway <Path to gateway configuration file in json format>

The documentation is divided into following sections:  

- The [Device section] (docs/cpp_cli_for_devices.rst) contains information on how devices publish events and handle commands using the C++ ibmiotf Client Library. 
- The [Managed Device section] (docs/cpp_cli_for_manageddevice.rst) contains information on how devices can connect to the Watson IoT Platform Device Management service using C++ ibmiotf Client Library and perform device management operations like firmware update, location update, and diagnostics update.
- The [Gateway section] (docs/cpp_cli_for_gateway.rst) contains information on how gateways publish events and handle commands for itself and for the attached devices using the C++ ibmiotf Client Library. 
- The [Gateway Management section] (docs/cpp_cli_for_managedgateway.rst) contains information on how to connect the gateway as Managed Gateway to IBM Watson IoT Platform and manage the attached devices.

----

----

Supported Features
------------------

| Feature   |      Supported?      | Description |
|----------|:-------------:|:-------------|
| [Device connectivity](https://console.ng.bluemix.net/docs/services/IoT/devices/libraries/mbedcpp.html) |  &#10004; | Connect your device(s) to Watson IoT Platform with ease using this library. [Click here](https://console.ng.bluemix.net/docs/services/IoT/devices/libraries/mbedcpp.html#library_use) for detailed information on how devices can publish events and handle commands.|
| [Gateway connectivity](https://github.com/ibm-watson-iot/iot-cpp/blob/master/docs/cpp_cli_for_gateway.rst) |    &#10004;   | Connect your gateway(s) to Watson IoT Platform with ease using this library. [Click here](https://github.com/ibm-watson-iot/iot-cpp/blob/master/docs/cpp_cli_for_gateway.rst) for detailed information on how gateways can publish events and handle commands for itself and for the attached devices. |
| [Device Management](https://github.com/ibm-watson-iot/iot-cpp/blob/master/docs/cpp_cli_for_manageddevice.rst) | &#10004; | Connects your device/gateway as managed device/gateway to Watson IoT Platform. [Click here](https://github.com/ibm-watson-iot/iot-cpp/blob/master/docs/cpp_cli_for_manageddevice.rst) for more information on how to perform device management operations like firmware update, reboot, location update and diagnostics update for a device.|
| [Event/Command publish using MQTT](https://console.ng.bluemix.net/docs/services/IoT/reference/mqtt/index.html)| &#10004; | Enables device/gateway/application to publish messages using MQTT. Refer to [Device](https://console.ng.bluemix.net/docs/services/IoT/devices/libraries/java.html#publishing_events), [Gateway](https://console.ng.bluemix.net/docs/services/IoT/gateways/libraries/java_cli_gw.html#publishing_events) and [Application](https://console.ng.bluemix.net/docs/services/IoT/applications/libraries/java.html#publishing_events_devices) section for more information. |



Features that are expected to be supported, shortly
------------------

| Feature   |      Supported as of now?      | Description |
|----------|:-------------:|:-------------|
| [Application connectivity](https://console.ng.bluemix.net/docs/services/IoT/applications/libraries/java.html) | &#10008; | Connect your application(s) to Watson IoT Platform with ease using this library. [Click here](https://console.ng.bluemix.net/docs/services/IoT/applications/libraries/java.html) for detailed information on how applications can subscribe to device events and publish commands to devices. |
| [Watson IoT API](https://console.ng.bluemix.net/docs/services/IoT/reference/api.html) | &#10008; | Shows how applications can use this library to interact with the Watson IoT Platform through REST APIs. [Click here](docs/java_cli_for_api.rst) for more information. |
| [SSL/TLS support](https://console.ng.bluemix.net/docs/services/IoT/reference/security/index.html) | &#10008; | By default, this library connects your devices, gateways and applications **securely** to Watson IoT Platform registered service. Ports 8883(default one) and 443 support secure connections using TLS with the MQTT and HTTP protocol. Developers can use the [port setting](https://console.ng.bluemix.net/docs/services/IoT/devices/libraries/java.html#constructor) to change the port number to 443 incase port 8883 is blocked. Also, use the [WebSocket setting]((https://console.ng.bluemix.net/docs/services/IoT/devices/libraries/java.html#constructor)) in order to connect your device/gateway/application over WebSockets. <br> Also, note that the library uses port 1883(unsecured) to connect to the Quickstart service.|
| [Client side Certificate based authentication](https://console.ng.bluemix.net/docs/services/IoT/reference/security/RM_security.html) | &#10008; | [Client side Certificate based authentication](https://console.ng.bluemix.net/docs/services/IoT/reference/security/RM_security.html) not supported now and will be added soon.|
| [Device Management Extension(DME)](https://console.ng.bluemix.net/docs/services/IoT/devices/device_mgmt/custom_actions.html) | &#10008; | Provides support for custom device management actions.<br>1. [Click here](https://github.com/ibm-watson-iot/iot-java/blob/master/docs/java_cli_for_manageddevice.rst#device-management-extension-dme-packages) for more information about DME support for a Device. <br> 2. [Click here](https://github.com/ibm-watson-iot/iot-java/blob/master/docs/java_cli_for_api.rst#device-management-extensiondme) to know how to create and initiate a DME request from the application.|
| [Scalable Application](https://console.ng.bluemix.net/docs/services/IoT/applications/mqtt.html) | &#10008; | Provides support for load balancing for applications. [Click here](https://console.ng.bluemix.net/docs/services/IoT/applications/libraries/java.html#constructor) for more information about how to enable scalable application support using this library. |
| [Auto reconnect](https://github.com/eclipse/paho.mqtt.java/issues/9) | &#10008; | Enables device/gateway/application to automatically reconnect to Watson IoT Platform while they are in a disconnected state. To enable this feature, set [Automatic-Reconnect](https://console.ng.bluemix.net/docs/services/IoT/devices/libraries/java.html#constructor) option to true. |
| Websocket | &#10008; | Enables device/gateway/application to connect to Watson IoT Platform using WebSockets. To enable this feature, set [WebSocket](https://console.ng.bluemix.net/docs/services/IoT/devices/libraries/java.html#constructor) option to true. |
| [Event/Command publish using HTTP](https://console.ng.bluemix.net/docs/services/IoT/devices/api.html)| &#10008; |Enables device/gateway/application to publish messages using HTTP. Refer to [Device](https://console.ng.bluemix.net/docs/services/IoT/devices/libraries/java.html#publishing_events), [Gateway](https://console.ng.bluemix.net/docs/services/IoT/gateways/libraries/java_cli_gw.html#publishing_events) and [Application](https://console.ng.bluemix.net/docs/services/IoT/applications/libraries/java.html#publishing_events_devices) section for more information. |
