Device Management update in progress

C++ Client Library - Introduction
============================================

This C++ Client Library can be used to simplify interactions with the [IBM Watson IoT Platform] (https://internetofthings.ibmcloud.com). 

Supported Features
------------------

| Feature   |      Supported?      | Description |
|----------|:-------------:|:-------------|
| [Device connectivity](https://console.ng.bluemix.net/docs/services/IoT/devices/libraries/mbedcpp.html) |  &#10004; | Connect your device(s) to Watson IoT Platform with ease using this library. [Click here](https://console.ng.bluemix.net/docs/services/IoT/devices/libraries/mbedcpp.html#library_use) for detailed information on how devices can publish events and handle commands.|
| [Gateway connectivity](https://github.com/ibm-watson-iot/iot-cpp/blob/master/docs/cpp_cli_for_gateway.rst) |    &#10004;   | Connect your gateway(s) to Watson IoT Platform with ease using this library. [Click here](https://github.com/ibm-watson-iot/iot-cpp/blob/master/docs/cpp_cli_for_gateway.rst) for detailed information on how gateways can publish events and handle commands for itself and for the attached devices. |
| [Device Management](https://github.com/ibm-watson-iot/iot-cpp/blob/master/docs/cpp_cli_for_manageddevice.rst) | &#10004; | Connects your device/gateway as managed device/gateway to Watson IoT Platform. [Click here](https://github.com/ibm-watson-iot/iot-cpp/blob/master/docs/cpp_cli_for_manageddevice.rst) for more information on how to perform device management operations like firmware update, reboot, location update and diagnostics update for a device.|
| [Event/Command publish using MQTT](https://console.ng.bluemix.net/docs/services/IoT/reference/mqtt/index.html)| &#10004; | Enables device/gateway/application to publish messages using MQTT. Refer to [Device](https://console.ng.bluemix.net/docs/services/IoT/devices/libraries/mbedcpp.html#publishing_events) and  [Gateway](https://github.com/ibm-watson-iot/iot-cpp/blob/master/docs/cpp_cli_for_gateway.rst)  section for more information. |
| [SSL/TLS support](https://console.ng.bluemix.net/docs/services/IoT/reference/security/index.html) | &#10008; | Support to be added in the near future|
| [Client side Certificate based authentication](https://console.ng.bluemix.net/docs/services/IoT/reference/security/RM_security.html) | &#10008; |Support to be added in the near future|
| [Device Management Extension(DME)](https://console.ng.bluemix.net/docs/services/IoT/devices/device_mgmt/custom_actions.html) | &#10008; | Support to be added in the near future|
| Auto reconnect | &#10008; |Support to be added in the near future|


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



License
---------------------------
The library is shipped with Eclipse Public License. For more information about the public licensing, see the [License file](https://github.com/amprasanna/iot-cpp/blob/master/LICENSE).
