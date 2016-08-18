cmake_minimum_required(VERSION 2.8)
include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/lib")

add_executable(sampleQuickstart sampleQuickstart.cpp)
add_executable(sampleDevice sampleDevice.cpp)
add_executable(sampleGateway sampleGateway.cpp)

target_link_libraries(sampleQuickstart  IOTP_DeviceClient IOTP_ResponseHandler)
target_link_libraries(sampleGateway  IOTP_GatewayClient IOTP_DeviceActionHandler IOTP_DeviceFirmwareHandler IOTP_ResponseHandler)
target_link_libraries(sampleDevice  IOTP_DeviceClient IOTP_DeviceActionHandler IOTP_DeviceFirmwareHandler IOTP_DeviceAttributeHandler IOTP_ResponseHandler)



#set(HEADER_FILES Clients.h Heap.h LinkedList.h Log.h Messages.h MQTTAsync.h MQTTClient.h MQTTClientPersistence.h MQTTPacket.h MQTTPacketOut.h MQTTPersistenceDefault.h MQTTPersistence.h MQTTProtocolClient.h MQTTProtocol.h MQTTProtocolOut.h SocketBuffer.h Socket.h SSLSocket.h StackTrace.h Thread.h Tree.h utf-8.h mqtt/async_client.h mqtt/callback.h mqtt/client.h mqtt/connect_options.h mqtt/delivery_token.h mqtt/exception.h mqtt/iaction_listener.h mqtt/iclient_persistence.h mqtt/ipersistable.h mqtt/message.h mqtt/token.h mqtt/topic.h)
#add_library(sampleDevice.cpp mqtt.cpp.iotf.dm.test.cpp ${HEADER_FILES})

