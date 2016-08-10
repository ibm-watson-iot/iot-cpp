======================================
C++ Client Library - Managed Device
======================================

Introduction
-------------

This client library describes how to use devices with the C++ WIoTP client library. For help with getting started with this module, see `C++ Client Library - Introduction <https://github.com/ibm-watson-iot/iot-cpp/blob/master/README.md>`__. 

This section contains information on how devices can connect to the Internet of Things Platform Device Management service using C++ and perform device management operations like firmware update, location update, and diagnostics update.

----

Device Management
-------------------------------------------------------------------------------
The `device management <https://docs.internetofthings.ibmcloud.com/devices/device_mgmt/index.html>`__ feature enhances the IBM Watson Internet of Things Platform service with new capabilities for managing devices. Device management makes a distinction between managed and unmanaged devices:

* **Managed Devices** are defined as devices which have a management agent installed. The management agent sends and receives device metadata and responds to device management commands from the IBM Watson Internet of Things Platform. 
* **Unmanaged Devices** are any devices which do not have a device management agent. All devices begin their lifecycle as unmanaged devices, and can transition to managed devices by sending a message from a device management agent to the IBM Watson Internet of Things Platform. 

----

Create DeviceData
------------------------------------------------------------------------
The `device model <https://docs.internetofthings.ibmcloud.com/reference/device_model.html>`__ describes the metadata and management characteristics of a device. The device database in the IBM Watson Internet of Things Platform is the master source of device information. Applications and managed devices are able to send updates to the database such as a location or the progress of a firmware update. Once these updates are received by the IBM Watson Internet of Things Platform, the device database is updated, making the information available to applications.

The device model in the WIoTP client library is represented as DeviceData and to create a DeviceData one needs to create the following objects,

* DeviceInfo (Optional)
* DeviceLocation (Optional, required only if the device wants to be notified about the location set by the application through Watson IoT Platform API)
* DeviceFirmware (Optional)
* DeviceMetadata (optional)

The following code snippet shows how to create the DeviceData object from the json file:

.. code:: C++

	void fillDeviceData(const std::string& filePath, iotf_device_data_ptr& deviceDataPtr) {
	Json::Reader reader;
	Json::Value root;
	std::filebuf fb;
	if (fb.open(filePath, std::ios::in)) {
		std::istream is(&fb);
		if (!reader.parse(is, root)) {
			std::cout << "Failed to parse test configuration from input file "
					<< filePath << std::endl;
			fb.close();
			return 0;
		}
		fb.close();
	}

	const Json::Value devInfo = root["DeviceInfo"];
	const Json::Value devLocation = root["DeviceLocation"];
	const Json::Value devMetaData = root["MetaData"];

	iotf_device_info_ptr deviceInfoPtr = std::make_shared < IOTP_DeviceInfo
			> (devInfo);

	iotf_device_location_ptr deviceLocationPtr = std::make_shared
			< IOTP_DeviceLocation > (devLocation);

	iotp_firmware_info_ptr deviceFirmwarePtr = std::make_shared
			< IOTP_FirmwareInfo > ("name", "version", "uri", "verifier");
	iotf_device_metadata_ptr deviceMetaDataPtr = std::make_shared
			< IOTP_DeviceMetadata > (devMetaData);
	deviceDataPtr =
			std::make_shared < IOTP_DeviceData > (deviceInfoPtr, deviceLocationPtr, deviceFirmwarePtr, deviceMetaDataPtr);
	}
	//Code will read the device data from json file
	//Creating Device data from json file
	iotf_device_data_ptr deviceDataPtr;
	fillDeviceData(argv[1], deviceDataPtr);

----

Construct ManagedDevice
-------------------------------------------------------------------------------
ManagedDevice - A device class that connects the device as managed device to IBM Watson Internet of Things Platform and enables the device to perform one or more Device Management operations. Also the ManagedDevice instance can be used to do normal device operations like publishing device events and listening for commands from application.

ManagedDevice exposes 2 different constructors to support different user patterns, 

**Constructor One**

Constructs a ManagedDevice instance by accepting the DeviceData and the following properties,

* Organization-ID - Your organization ID.
* Domain - (Optional) The messaging endpoint URL. By default the value is "internetofthings.ibmcloud.com"(Watson IoT Production server)
* Device-Type - The type of your device.
* Device-ID - The ID of your device.
* Authentication-Method - Method of authentication (The only value currently supported is "token"). 
* Authentication-Token - API key token

All these properties are required to interact with the IBM Watson Internet of Things Platform. 

The following code shows how to create a ManagedDevice instance:

.. code:: C++

	Properties prop;
	prop.setorgId("uguhsp");
	prop.setdeviceType("iotsample-arduino");
	prop.setdeviceId("00aabbccde03");
	prop.setauthMethod("token");
	prop.setauthToken("AUTH TOKEN FOR DEVICE");
	
	IOTP_DeviceClient managedClient(prop, deviceDataPtr);
 

**Constructor Two**

Construct a ManagedDevice instance by accepting the DeviceData and the device action handler, firmware handler and device attribute handler. This constructor helps in calling the handlers when a particular manage request is sent for the device. construction for this type type is explained as follows:

.. code:: C++
	
	//Creating Device data from json file
	iotf_device_data_ptr deviceDataPtr;
	fillDeviceData(argv[2], deviceDataPtr);

	//creating action handler
	iotp_device_action_handler_ptr ptr = std::make_shared<SampleDeviceAction>();
	//creating firmware handler
	iotp_device_firmware_handler_ptr fwPtr = std::make_shared<SampleFirmwareAction>();
	//creating device attribute handler
	iotp_device_attribute_handler_ptr devAttributePtr = std::make_shared<SampleDeviceAttributeAction>();
	std::cout<<"Creating Managed Client\n";
	IOTP_DeviceClient managedClient(prop, deviceDataPtr, ptr, fwPtr, devAttributePtr);
	
----

Manage	
------------------------------------------------------------------
The device can invoke manage() method to participate in device management activities. The manage request will initiate a connect request internally if the device is not connected to the IBM Watson Internet of Things Platform already:

.. code:: C++

	managedDevice.manage();
	
Before calling manage manage request the life time can be set. By default lifetime will be set for 3600Secs (1 hour)

* *lifetime* The length of time in seconds within which the device must send another **Manage device** request in order to avoid being reverted to an unmanaged device and marked as dormant. If set to 0, the managed device will not become dormant. When set, the minimum supported setting is 3600 (1 hour).
* *supportFirmwareActions* Tells whether the device supports firmware actions or not. The device must add a firmware handler to handle the firmware requests.
* *supportDeviceActions* Tells whether the device supports Device actions or not. The device must add a Device action handler to handle the reboot and factory reset requests.


Refer to the `documentation <https://docs.internetofthings.ibmcloud.com/devices/device_mgmt/index.html#/manage-device#manage-device>`__ for more information about the manage operation.

----

Unmanage
-----------------------------------------------------

A device can invoke sendUnmanageRequest() method when it no longer needs to be managed. The IBM Watson Internet of Things Platform will no longer send new device management requests to this device and all device management requests from this device will be rejected other than a **Manage device** request.

.. code:: C++

	managedDevice.unmanage();

Refer to the `documentation <https://docs.internetofthings.ibmcloud.com/devices/device_mgmt/index.html#/unmanage-device#unmanage-device>`__ for more information about the Unmanage operation.

----

Location Update
-----------------------------------------------------

Devices that can determine their location can choose to notify the IBM Watson Internet of Things Platform about location changes. The Device can invoke one of the overloaded updateLocation() method to update the location of the device. 

.. code:: C++

    	// update the location with latitude, longitude and elevation
    	IOTP_DeviceLocation deviceLocation(x,y,z);
	success = managedClient.update_device_location(deviceLocation);
	if (success == false)
		std::cout << "Failed to update device location to IOTF" << std::endl;
	else
		std::cout<< "Updated device location"<<std::endl;

Refer to the `documentation <https://docs.internetofthings.ibmcloud.com/devices/device_mgmt/index.html#/update-location#update-location>`__ for more information about the Location update.

----

Append/Clear ErrorCodes
-----------------------------------------------

Devices can choose to notify the IBM Watson Internet of Things Platform about changes in their error status. The Device can invoke  addErrorCode() method to add the current errorcode to Watson IoT Platform.

.. code:: C++

	int rc = managedDevice.addErrorCodes(300);

Also, the ErrorCodes can be cleared from IBM Watson Internet of Things Platform by calling the clearErrorCodes() method as follows:

.. code:: C++

	int rc = managedDevice.clearErrorCodes();

----

Append/Clear Log messages
-----------------------------
Devices can choose to notify the IBM Watson Internet of Things Platform about changes by adding a new log entry. Log entry includes a log messages, its timestamp and severity, as well as an optional base64-encoded binary diagnostic data. The Devices can invoke addLog() method to send log messages,

.. code:: C++
	// An example Log event
	std::string logMessage = "Test Log Message";
	std::string timestamp = "2016-07-05T08:15:30-05:00";
	int sev = 1;
	std::string data = "Data";
	IOTP_DeviceLog deviceLog(logMessage, timestamp, sev, data);
	success = managedClient.addLogs(deviceLog);
	if (success == false)
		std::cout << "Failed Adding logs to the Watson IoT Platform" << std::endl;
	else
		std::cout<< "Adding logs succeeded"<<std::endl;
	
Also, the log messages can be cleared from IBM Watson Internet of Things Platform by calling the clearLogs() method as follows:

.. code:: C++

	success = managedClient.clearLogs();
	
The device diagnostics operations are intended to provide information on device errors, and does not provide diagnostic information relating to the devices connection to the IBM Watson Internet of Things Platform.

Refer to the `documentation <https://docs.internetofthings.ibmcloud.com/devices/device_mgmt/index.html#/update-location#update-location>`__ for more information about the Diagnostics operation.

----


Device Actions
------------------------------------
The IBM Watson Internet of Things Platform supports the following device actions:

* Reboot
* Factory Reset

The device needs to do the following activities to support Device Actions:

**1. Inform server about the Device Actions support**

In order to perform Reboot and Factory Reset, the device needs to inform the IBM Watson Internet of Things Platform about its support first. This can be achieved by creating the IOTP_DeviceClient constructor with the action handler parameter,

.. code:: C++
	// Last parameter represents the device action support
    	IOTP_DeviceClient managedClient(prop, deviceDataPtr, actionPtr, fwPtr, devAttributePtr);

Once the support is informed to the DM server, the server then forwards the device action requests to the device.
	
**2. Create the Device Action Handler**

In order to support the device action, the device needs to create a handler. The handler must extend a IOTP_DeviceActionHandler class and provide implementation for the following methods:

.. code:: C++

	virtual iotp_device_action_response_ptr reboot() = 0;
	virtual iotp_device_action_response_ptr factory_reset() = 0;

**2.1 Sample implementation of handleReboot**

The implementation must create a separate thread and add a logic to reboot the device and report the status of the reboot via DeviceAction object. Upon receiving the request, the device first needs to inform the server about the support(or failure) before proceeding with the actual reboot. And if the device can not reboot the device or any other error during the reboot, the device can update the status along with an optional message. A sample reboot implementation for a Raspberry Pi device is shown below:

.. code:: java

	iotp_device_action_response_ptr reboot() {
		std::cout << "MyDeviceAction: reboot " << std::endl;
		IOTP_DeviceActionResponse rsp(DEVICE_ACTION_REBOOT_OK);
		mReboot = true;
		return std::make_shared<IOTP_DeviceActionResponse>(rsp);
	}
	
	In the similar lines factory_reset can be implemented.

**3. Add the handler to ManagedDevice**

The created handler needs to be added to the ManagedDevice instance so that the WIoTP client library invokes the corresponding method when there is a device action request from IBM Watson Internet of Things Platform.

.. code:: C++

	iotp_device_action_handler_ptr actionPtr = std::make_shared<SampleDeviceAction>();
	IOTP_DeviceClient managedClient(prop, deviceDataPtr, actionPtr, fwPtr, devAttributePtr);

----

Listen for Device attribute changes
-----------------------------------------------------------------

This WIoTP client library updates the corresponding objects whenever there is an update request from the IBM Watson Internet of Things Platform, these update requests are initiated by the application either directly or indirectly (Firmware Update) via the IBM Watson Internet of Things Platform ReST API. Apart from updating these attributes, the library provides a mechanism where the device can be notified whenever a device attribute is updated.

Attributes that can be updated by this operation are location, metadata, device information and firmware.

In order to get notified, the device needs to add a device attribute handler while creating managed device.

.. code:: C++

	//creating device attribute handler
	iotp_device_attribute_handler_ptr devAttributePtr = std::make_shared<SampleDeviceAttributeAction>();
	
Handler will be notified when ever platform sends an update request for the change in the device attributes:
Currently it is supported only with location update handler. Find the code below for the implementation of the handler. Custom handler needs to implemnt IOTP_DeviceAttributeHandler. 

.. code:: C++

	class SampleDeviceAttributeAction : public IOTP_DeviceAttributeHandler {
	public:
	SampleDeviceAttributeAction(){}
	~SampleDeviceAttributeAction() {}

	virtual bool UpdateLocation(iotf_device_location_ptr& locationPtr)  {
		std::cout << "ENTRY Update Location\n";
		std::cout << "Longitude: " << locationPtr->getLongitude() << " Latitude: " << locationPtr->getLatitude() << std::endl;
			//iotp_device_attribute_update_response_ptr ptr = std::make_shared<IOTP_DeviceAttributeUpdateResponse>(200);
		return true;
	}
};

----

Adding the handler to the managed device.

.. code:: C++

	iotp_device_attribute_handler_ptr devAttributePtr = std::make_shared<SampleDeviceAttributeAction>();
	IOTP_DeviceClient managedClient(prop, deviceDataPtr, actionPtr, fwPtr, devAttributePtr);

Refer to `this page <https://docs.internetofthings.ibmcloud.com/devices/device_mgmt/index.html#/update-device-attributes#update-device-attributes>`__ for more information about updating the device attributes.

----

