Update in progress

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
1. Go to the build directory under root directory (iot-cpp/build).
2. Run the command cmake ..
3. Run the command make
All the output files will be created under the build directory.

Running samples
---------------
Go to the samples directory under build (iot-cpp/build/samples)
Run device client (sampleDevice) with the following command from build directory
./samples/sampleDevice <Path to device configuration file in json format>


The documentation is divided into following sections:  

- The [Device section] (docs/c++_cli_for_devices.rst) contains information on how devices publish events and handle commands using the C++ ibmiotf Client Library. 
- The [Managed Device section] (docs/c++_cli_for_manageddevice.rst) contains information on how devices can connect to the Watson IoT Platform Device Management service using C++ ibmiotf Client Library and perform device management operations like firmware update, location update, and diagnostics update.
- The [Gateway section] (docs/c++_cli_for_gateway.rst) contains information on how gateways publish events and handle commands for itself and for the attached devices using the C++ ibmiotf Client Library. 
- The [Gateway Management section] (docs/c++_cli_for_managedgateway.rst) contains information on how to connect the gateway as Managed Gateway to IBM Watson IoT Platform and manage the attached devices.

----
