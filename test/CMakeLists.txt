cmake_minimum_required(VERSION 2.8)
include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/lib")

add_executable(test_deviceclient test_deviceclient.cpp)
add_test(test_deviceclient test_deviceclient)

add_executable(test_gatewayclient test_gatewayclient.cpp)
add_test(test_gatewayclient test_gatewayclient)

target_link_libraries(test_deviceclient IOTP_DeviceClient cpptest)
target_link_libraries(test_gatewayclient IOTP_GatewayClient cpptest)
