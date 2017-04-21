cmake_minimum_required(VERSION 2.8)
include_directories ("${PROJECT_SOURCE_DIR}/lib")
INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})

add_library(IOTP_Client IOTP_Client.cpp)
add_library(IOTP_DeviceClient SHARED IOTP_DeviceClient.cpp)
add_library(IOTP_GatewayClient SHARED IOTP_GatewayClient.cpp)
add_library(IOTP_Device IOTP_Device.cpp)
add_library(IOTP_DeviceActionHandler IOTP_DeviceActionHandler.cpp)
add_library(IOTP_DeviceFirmwareHandler IOTP_DeviceFirmwareHandler.cpp)
add_library(IOTP_DeviceAttributeHandler IOTP_DeviceAttributeHandler.cpp)
add_library(IOTP_ResponseHandler IOTP_ResponseHandler.cpp)

set(SYSTEM_LIBS ${THREAD_LIBS_SYSTEM} ${OPENSSL_LIB} ${OPENSSLCRYPTO_LIB} ${LIBS_SYSTEM})

set(COMMON_LIBS IOTP_Client IOTP_Device IOTP_DeviceActionHandler IOTP_DeviceFirmwareHandler
                IOTP_DeviceAttributeHandler IOTP_ResponseHandler ${JSON_LIBRARY} ${MQTT_CPP_LIBRARY}
                ${MQTT_C_LIBRARY} ${LOG4CPP_LIBRARY_NAME}
        )

TARGET_LINK_LIBRARIES (IOTP_DeviceClient ${COMMON_LIBS} ${SYSTEM_LIBS})
TARGET_LINK_LIBRARIES (IOTP_GatewayClient ${COMMON_LIBS} ${SYSTEM_LIBS})
