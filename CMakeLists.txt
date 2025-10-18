cmake_minimum_required(VERSION 3.15)

project(NetworkSystem CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/server)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/client)

add_executable(server
  server/main.cpp
  server/services/tcp.cpp
  server/services/pop3-v2.cpp
  server/session-manager.cpp
  server/service-manager.cpp
  server/server.cpp
  server/utils.cpp
  common/protocol.cpp
  common/utils.cpp
)

add_executable(client
  client/main.cpp
  client/client.cpp
  client/utils.cpp
  common/protocol.cpp
  common/utils.cpp
)

if (WIN32)
  target_link_libraries(server PRIVATE Ws2_32)
  target_link_libraries(client PRIVATE Ws2_32)
endif()
