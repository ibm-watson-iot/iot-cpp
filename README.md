
C++ Client Library - Introduction
============================================

This C++ Client Library can be used to simplify interactions with the [IBM Watson IoT Platform](https://internetofthings.ibmcloud.com). Supported platforms are OSX and Linux.

Supported Features
------------------

| Feature   |      Supported?      | Description |
|----------|:-------------:|:-------------|
| [Device connectivity](https://console.ng.bluemix.net/docs/services/IoT/devices/libraries/mbedcpp.html) |  &#10004; | Connect your device(s) to Watson IoT Platform with ease using this library. [Click here](https://console.ng.bluemix.net/docs/services/IoT/devices/libraries/mbedcpp.html#library_use) for detailed information on how devices can publish events and handle commands.|
| [Gateway connectivity](https://github.com/ibm-watson-iot/iot-cpp/blob/master/docs/cpp_cli_for_gateway.rst) |    &#10004;   | Connect your gateway(s) to Watson IoT Platform with ease using this library. [Click here](https://github.com/ibm-watson-iot/iot-cpp/blob/master/docs/cpp_cli_for_gateway.rst) for detailed information on how gateways can publish events and handle commands for itself and for the attached devices. |
| [Device Management](https://github.com/ibm-watson-iot/iot-cpp/blob/master/docs/cpp_cli_for_manageddevice.rst) | &#10004; | Connects your device/gateway as managed device/gateway to Watson IoT Platform. [Click here](https://github.com/ibm-watson-iot/iot-cpp/blob/master/docs/cpp_cli_for_manageddevice.rst) for more information on how to perform device management operations like firmware update, reboot, location update and diagnostics update for a device.|
| [Event/Command publish using MQTT](https://console.ng.bluemix.net/docs/services/IoT/reference/mqtt/index.html)| &#10004; | Enables device/gateway/application to publish messages using MQTT. Refer to [Device](https://console.ng.bluemix.net/docs/services/IoT/devices/libraries/mbedcpp.html#publishing_events) and  [Gateway](https://github.com/ibm-watson-iot/iot-cpp/blob/master/docs/cpp_cli_for_gateway.rst)  section for more information. |
| [SSL/TLS support](https://console.ng.bluemix.net/docs/services/IoT/reference/security/index.html) | &#10004; | Enables Device/Gateway Client to connect to Watson IoT Platform securely using SSL/TLS feature.|
| [Client side Certificate based authentication](https://console.ng.bluemix.net/docs/services/IoT/reference/security/RM_security.html) | &#10008; |Support to be added in the near future|
| [Device Management Extension(DME)](https://console.ng.bluemix.net/docs/services/IoT/devices/device_mgmt/custom_actions.html) | &#10008; | Support to be added in the near future|
| Auto reconnect | &#10008; |Support to be added in the near future|

Dependencies
------------

1.  [Paho MQTT C++ Client]

  [Paho MQTT C++ Client]: https://eclipse.org/paho/clients/cpp/
  
2.  [Paho MQTT C Client]

  [Paho MQTT C Client]: https://eclipse.org/paho/clients/c/

3.  [A C++ library for interacting with JSON - jsoncpp]

  [A C++ library for interacting with JSON - jsoncpp]: https://github.com/open-source-parsers/jsoncpp
  
4.  [Log library for C++ - log4cpp]

  [Log library for C++ - log4cpp]: https://sourceforge.net/projects/log4cpp/
 
5.  [A C++ Unit Testing Framework - CppTest]

    [A C++ Unit Testing Framework - CppTest]: http://cpptest.sourceforge.net/
    
6.  [CMake Build Tool]

  [CMake Build Tool]: https://cmake.org/
 
Installing the Dependencies
---------------------------
Run the [setup.sh](https://github.com/ibm-watson-iot/iot-cpp/blob/master/setup.sh) file in the iot-cpp directory. This script installs the required dependencies and copies the dependencies into the lib directory after making necessary changes.

Note:
If the dependencies were not able to be installed with the script on the device, manually pull the dependencies from the links mentioned in [setup.sh](https://github.com/ibm-watson-iot/iot-cpp/blob/master/setup.sh) and follow the steps for making the necessary changes.

Build Instructions 
------------------

1.  Install [CMake](https://cmake.org/install/)
2.  git clone https://github.com/ibm-watson-iot/iot-cpp.git
3.  cd iot-cpp
4.  sh setup.sh 
5.  mkdir build ; cd build 
6.  cmake ..
7.  make

All the output files will be created under the build directory.

Running samples
---------------

1. cp iot-cpp/src/log4cpp.properties iot-cpp/build
2. Update device.cfg and gateway.cfg properties files present in iot-cpp/samples
3. cd iot-cpp/build
4. Execute Device Sample: ./samples/sampleDevice
5. Execute Gateway Sample: ./samples/sampleGateway

Detailed Documentation
----------------------

- The [Device section] (docs/cpp_cli_for_devices.rst) contains information on how devices publish events and handle commands using the C++ ibmiotf Client Library. 
- The [Managed Device section] (docs/cpp_cli_for_manageddevice.rst) contains information on how devices can connect to the Watson IoT Platform Device Management service using C++ ibmiotf Client Library and perform device management operations like firmware update, location update, and diagnostics update.
- The [Gateway section] (docs/cpp_cli_for_gateway.rst) contains information on how gateways publish events and handle commands for itself and for the attached devices using the C++ ibmiotf Client Library. 
- The [Gateway Management section] (docs/cpp_cli_for_managedgateway.rst) contains information on how to connect the gateway as Managed Gateway to IBM Watson IoT Platform and manage the attached devices.

----



License
---------------------------
The library is shipped with Eclipse Public License. For more information about the public licensing, see the [License file](https://github.com/amprasanna/iot-cpp/blob/master/LICENSE).
