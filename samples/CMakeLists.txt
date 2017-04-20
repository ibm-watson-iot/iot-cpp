cmake_minimum_required(VERSION 2.8)
include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/lib")

set(LIBS
    IOTP_DeviceActionHandler
    IOTP_DeviceFirmwareHandler
    IOTP_DeviceAttributeHandler
    IOTP_ResponseHandler
    )

add_executable(sslDeviceSample sslDeviceSample.cpp)
add_executable(sampleQuickstart sampleQuickstart.cpp)
add_executable(sampleDevice sampleDevice.cpp)
add_executable(sampleGateway sampleGateway.cpp)

target_link_libraries(sslDeviceSample  IOTP_DeviceClient)

target_link_libraries(sampleQuickstart  IOTP_DeviceClient IOTP_ResponseHandler)
target_link_libraries(sampleGateway  IOTP_GatewayClient ${LIBS})
target_link_libraries(sampleDevice  IOTP_DeviceClient ${LIBS})
