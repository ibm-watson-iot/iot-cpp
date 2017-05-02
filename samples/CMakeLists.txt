cmake_minimum_required(VERSION 2.8)
include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/lib")

set(LIBS
    IOTP_DeviceActionHandler
    IOTP_DeviceFirmwareHandler
    IOTP_DeviceAttributeHandler
    IOTP_ResponseHandler
    )

add_executable(sampleQuickstart sampleQuickstart.cpp)
add_executable(sampleDevice sampleDevice.cpp)
add_executable(sampleGateway sampleGateway.cpp)
add_executable(sampleManagedDevice sampleManagedDevice.cpp)
add_executable(sampleManagedGateway sampleManagedGateway.cpp)

target_link_libraries(sampleQuickstart  IOTP_DeviceClient IOTP_ResponseHandler)
target_link_libraries(sampleDevice  IOTP_DeviceClient)
target_link_libraries(sampleGateway  IOTP_GatewayClient)
target_link_libraries(sampleManagedGateway  IOTP_GatewayClient ${LIBS})
target_link_libraries(sampleManagedDevice  IOTP_DeviceClient ${LIBS})
