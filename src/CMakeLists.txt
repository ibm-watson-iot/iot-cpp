cmake_minimum_required(VERSION 2.8)
include_directories ("${PROJECT_SOURCE_DIR}/lib")
add_library(IOTP_Client SHARED IOTP_Client.cpp)
add_library(IOTP_Device IOTP_Device.cpp)
add_library(IOTP_DeviceActionHandler IOTP_DeviceActionHandler.cpp)
add_library(IOTP_DeviceFirmwareHandler IOTP_DeviceFirmwareHandler.cpp)
add_library(IOTP_ResponseHandler IOTP_ResponseHandler.cpp)
 

TARGET_LINK_LIBRARIES (IOTP_Client async_client client iclient_persistence jsoncpp message token topic Clients  LinkedList  Messages   MQTTClient  MQTTPacketOut    MQTTPersistenceDefault  MQTTProtocolOut  SocketBuffer  SSLSocket   Thread  utf-8 Heap     Log  MQTTAsync  MQTTPacket  MQTTPersistence  MQTTProtocolClient MQTTVersion Socket  StackTrace  Tree)
