cmake_minimum_required(VERSION 2.8)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})

add_library(${JSON_LIBRARY} jsoncpp.cpp)

add_library(${MQTT_CPP_LIBRARY} async_client.cpp client.cpp connect_options.cpp disconnect_options.cpp
                                iclient_persistence.cpp message.cpp response_options.cpp
                                ssl_options.cpp token.cpp topic.cpp will_options.cpp
                        )

add_library(${MQTT_C_LIBRARY} MQTTAsync.c Clients.c Heap.c LinkedList.c Log.c Messages.c
                              MQTTClient.c MQTTPacket.c MQTTPacketOut.c MQTTPersistence.c
                              MQTTPersistenceDefault.c MQTTProtocolClient.c MQTTProtocolOut.c
                              MQTTVersion.c SocketBuffer.c Socket.c SSLSocket.c StackTrace.c
                              Thread.c Tree.c utf-8.c
                        )

ADD_LIBRARY ( ${LOG4CPP_LIBRARY_NAME} Appender.cpp AppenderSkeleton.cpp AppendersFactory.cpp
              BufferingAppender.cpp FactoryParams.cpp LayoutsFactory.cpp LevelEvaluator.cpp
              Localtime.cpp PassThroughLayout.cpp TriggeringEventEvaluatorFactory.cpp
              LayoutAppender.cpp FileAppender.cpp DailyRollingFileAppender.cpp
              RollingFileAppender.cpp FixedContextCategory.cpp IdsaAppender.cpp
              OstreamAppender.cpp StringQueueAppender.cpp SyslogAppender.cpp
              RemoteSyslogAppender.cpp SimpleLayout.cpp BasicLayout.cpp PatternLayout.cpp
              Category.cpp CategoryStream.cpp HierarchyMaintainer.cpp Configurator.cpp
              BasicConfigurator.cpp SimpleConfigurator.cpp PropertyConfigurator.cpp
              PropertyConfiguratorImpl.cpp LoggingEvent.cpp Priority.cpp NDC.cpp Filter.cpp
              TimeStamp.cpp StringUtil.cpp Properties.cpp Win32DebugAppender.cpp
              NTEventLogAppender.cpp DllMain.cpp DummyThreads.cpp MSThreads.cpp
              OmniThreads.cpp PThreads.cpp PortabilityImpl.cpp AbortAppender.cpp
        )

IF (WIN32)
  TARGET_LINK_LIBRARIES (${LOG4CPP_LIBRARY_NAME} kernel32 user32 ws2_32 advapi32 )
  SET_TARGET_PROPERTIES(${LOG4CPP_LIBRARY_NAME} PROPERTIES LINK_FLAGS /NODEFAULTLIB:msvcrt )
ENDIF (WIN32)
