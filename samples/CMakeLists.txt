cmake_minimum_required(VERSION 2.8)
include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/lib")

add_executable(sampleDevice sampleDevice.cpp)
target_link_libraries(sampleDevice  IOTP_Client IOTP_Device IOTP_DeviceActionHandler IOTP_DeviceFirmwareHandler IOTP_ResponseHandler)

