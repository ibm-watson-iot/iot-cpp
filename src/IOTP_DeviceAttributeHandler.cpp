/*
 * IOTPDeviceLocationUpdateHandler.cpp
 *
 *  Created on: 21-Jul-2016
 *      Author: amit
 */

#include "IOTP_DeviceAttributeHandler.h"
#include "IOTP_TopicDefinitions.h"
#include "IOTP_Device.h"

namespace Watson_IOTP {

iotp_reply_message_ptr IOTP_DeviceAttributeHandler::message_arrived(const std::string& topic, Json::Value& jsonPayload) {

	iotp_reply_message_ptr replyPtr = nullptr;
	iotp_device_attribute_update_response_ptr rsp;
	//iotf_device_location_ptr p = std::make_shared<IOTP_DeviceLocation> getDeviceLocation

	iotf_device_location_ptr p = std::make_shared<IOTP_DeviceLocation>(0.0,0.0,0.0);

	if (topic == SERVER_UPDATE_TOPIC) {
		/**
		 * Incoming request from the IoT Platform:
				Topic: iotdm-1/device/update
				Message:
				{
		   	   	   "reqId" : "f38faafc-53de-47a8-a940-e697552c3194",
		   	   	   "d" : {
		      	  	  "fields" : [{
		            	"field" : "mgmt.firmware",
		            		"value": {
                    			"latitude": number,
                    			"longitude": number,
                    			"elevation": number,
                    			"accuracy": number,
                    			"measuredDateTime": "string in ISO8601 format"
                			}
		         	 	 }
		      	  	  ]
		   	   	   }
				}
			 */
		Json::ArrayIndex index = 0;
		Json::Value v;
		Json::Value firstField = jsonPayload.get("d", v).get("fields", v).get(index, v);

		if (firstField.get("field", v).asString() == "location") {
			Json::Value value = firstField.get("value", v);
			p->setLatitude(value.get("latitude", 0.0).asDouble());
			p->setLongitude(value.get("longitude", 0.0).asDouble());
			p->setElevation(value.get("elevation", 0.0).asDouble());
		}

		// Call the derived class to perform Location update.
		bool rsp = UpdateLocation(p);
		return nullptr;
		//Please note: there is no reqId as no response by device is required.
	}
}

iotp_reply_message_ptr IOTP_DeviceAttributeHandler::message_arrived(const std::string& topic, mqtt::message_ptr msg) {

}
}
