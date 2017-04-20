/*******************************************************************************
 * Copyright (c) 2016 IBM Corp.
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
 *    Hari Prasada Reddy - Initial implementation
 *    Lokesh Haralakatta - Updates to match with latest mqtt lib changes
 *******************************************************************************/
 
#ifndef SRC_IOTP_TOPICDEFINITIONS_H_
#define SRC_IOTP_TOPICDEFINITIONS_H_

namespace Watson_IOTP {
#define SERVER_RESPONSE_TOPIC "iotdm-1/response"
#define DEVICE_MANAGE_TOPIC "iotdevice-1/mgmt/manage"
#define DEVICE_UNMANAGE_TOPIC "iotdevice-1/mgmt/unmanage"
#define SERVER_UPDATE_TOPIC "iotdm-1/device/update"
#define DEVICE_UPDATE_LOCATION_TOPIC "iotdevice-1/device/update/location"
#define DEVICE_REPONSE_TOPIC "iotdevice-1/response"
#define SERVER_DEVICE_REBOOT_TOPIC "iotdm-1/mgmt/initiate/device/reboot"
#define SERVER_FACTORY_RESET_TOPIC "iotdm-1/mgmt/initiate/device/factory_reset"
#define DEVICE_NOTIFY_TOPIC	"iotdevice-1/notify"
#define SERVER_OBSERVE_TOPIC "iotdm-1/observe"
#define SERVER_CANCEL_TOPIC	"iotdm-1/cancel"
#define SERVER_FIRMWARE_DOWNLOAD_TOPIC "iotdm-1/mgmt/initiate/firmware/download"
#define SERVER_FIRMWARE_UPDATE_TOPIC "iotdm-1/mgmt/initiate/firmware/update"
#define DEVICE_ADD_ERROR_CODES_TOPIC "iotdevice-1/add/diag/errorCodes"
#define DEVICE_CLEAR_ERROR_CODES_TOPIC "iotdevice-1/clear/diag/errorCodes"
#define DEVICE_ADD_DIAG_LOG_TOPIC "iotdevice-1/add/diag/log"
#define DEVICE_CLEAR_DIAG_LOG_TOPIC "iotdevice-1/clear/diag/log"

}


#endif /* SRC_IOTP_TOPICDEFINITIONS_H_ */
