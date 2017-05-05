cmake_minimum_required(VERSION 2.8)
project(ibmiotf-cpp)
enable_testing()

option (run_tests "set run_tests to ON if build tests should be run, set to OFF to skip tests" ON)
SET(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage -fPIC -std=c++0x -pthread ${CMAKE_CXX_FLAGS} -I/usr/local/include ")
SET(CMAKE_C_FLAGS "-g -O0 -Wall -W -fprofile-arcs -ftest-coverage -fPIC ${CMAKE_C_FLAGS} ")
SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage ${CMAKE_EXE_LINKER_FLAGS} -L/usr/local/lib ")

add_definitions(-DOPENSSL)
SET(OPENSSL_SEARCH_PATH "" CACHE PATH "Directory containing OpenSSL libraries and includes")

IF (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
      SET(OPENSSL_SEARCH_PATH "/usr/local/opt/openssl")
ENDIF (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")

IF (WIN32)
      SET(OPENSSL_SEARCH_PATH "C:/OpenSSL-Win64")
ENDIF ()

set(OPENSSL_INCLUDE_DIR ${OPENSSL_SEARCH_PATH}/include)
FIND_LIBRARY(OPENSSL_LIB NAMES ssl libssl ssleay32 HINTS ${OPENSSL_SEARCH_PATH}/lib
                ${OPENSSL_SEARCH_PATH}/lib64)
FIND_LIBRARY(OPENSSLCRYPTO_LIB NAMES crypto libcrypto libeay32
                HINTS ${OPENSSL_SEARCH_PATH}/lib ${OPENSSL_SEARCH_PATH}/lib64)

MESSAGE(STATUS "OpenSSL hints: ${OPENSSL_SEARCH_PATH}")
MESSAGE(STATUS "OpenSSL headers found at ${OPENSSL_INCLUDE_DIR}")
MESSAGE(STATUS "OpenSSL library found at ${OPENSSL_LIB}")
MESSAGE(STATUS "OpenSSL Crypto library found at ${OPENSSLCRYPTO_LIB}")

IF (WIN32)
  ADD_DEFINITIONS ( -D_CRT_SECURE_NO_WARNINGS )
ELSE (WIN32)
  IF (APPLE)
    ADD_DEFINITIONS ( -DNDEBUG -DLOG4CPP_HAVE_SSTREAM )
    ADD_DEFINITIONS ( -DNDEBUG -DLOG4CPP_HAVE_UNISTD_H )
  ELSE (APPLE)
    ADD_DEFINITIONS ( -pthread -DNDEBUG -DLOG4CPP_HAVE_SSTREAM )
    ADD_DEFINITIONS ( -DNDEBUG -DLOG4CPP_HAVE_UNISTD_H )
  ENDIF (APPLE)
ENDIF (WIN32)

SET ( JSON_LIBRARY "jsoncpp" )
SET ( MQTT_CPP_LIBRARY "mqtt3cpp" )
SET ( MQTT_C_LIBRARY "mqtt3c" )
SET ( LOG4CPP_LIBRARY_NAME "log4cpp" )

add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(samples)

if (run_tests)
    add_subdirectory(test)
endif ()
