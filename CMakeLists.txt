cmake_minimum_required(VERSION 3.15)

# BẬT C luôn (vì có sqlite3.c)
project(NetworkSystem C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ====== Include paths ======
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/server)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/client)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/config)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/types)

# Guard để bắt thiếu file ngay khi configure
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/sqlite/sqlite3.c")
  message(FATAL_ERROR "Missing sqlite/sqlite3.c — hãy đặt đúng amalgamation vào thư mục sqlite/")
endif()

# ====== SQLite amalgamation -> static lib ======
add_library(sqlite3_vendor STATIC
  sqlite/sqlite3.c
)
target_include_directories(sqlite3_vendor PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/sqlite
)
target_compile_definitions(sqlite3_vendor PRIVATE
  SQLITE_OS_WIN=1
  SQLITE_THREADSAFE=1
  SQLITE_OMIT_LOAD_EXTENSION=1
)
if(MSVC)
  target_compile_options(sqlite3_vendor PRIVATE /W3)
else()
  target_compile_options(sqlite3_vendor PRIVATE -Wall -Wextra)
endif()

# ====== Storage layer ======
add_library(storage
  server/src/storage/db.cpp
  server/src/storage/dbConnection.cpp
  server/src/storage/table.cpp
  server/src/storage/user.cpp
  server/src/storage/mail.cpp
)
target_include_directories(storage PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/server/src/storage
  ${CMAKE_CURRENT_SOURCE_DIR}/sqlite
)
target_link_libraries(storage PUBLIC sqlite3_vendor)

add_library(state
  client/src/state/db.cpp
  client/src/state/dbConnection.cpp
  client/src/state/table.cpp
  client/src/state/account.cpp
)
target_include_directories(state PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/client/src/state
  ${CMAKE_CURRENT_SOURCE_DIR}/sqlite
)
target_link_libraries(state PUBLIC sqlite3_vendor)

# ====== Server ======
add_executable(server
  server/src/main.cpp
  server/src/services/tcp.cpp
  server/src/services/pop3-v2.cpp
  server/src/session-manager.cpp
  server/src/service-manager.cpp
  server/src/server.cpp
  server/src/utils.cpp
  common/protocol.cpp
  common/utils.cpp
)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/server/storage)
target_link_libraries(server PRIVATE storage)
target_compile_definitions(server 
  PUBLIC
    ${CMAKE_SOURCE_DIR}/server/include
)

# ====== Client ======
add_executable(client
  client/src/main.cpp
  client/src/client.cpp
  client/src/utils.cpp
  common/protocol.cpp
  common/utils.cpp
)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/client/state)
target_link_libraries(client PRIVATE state)
target_compile_definitions(client 
  PUBLIC
    ${CMAKE_SOURCE_DIR}/client/include
)

if (WIN32)
  target_link_libraries(server PRIVATE Ws2_32)
  target_link_libraries(client PRIVATE Ws2_32)
endif()

# Nếu dùng std::filesystem với MinGW cũ:
if (MINGW)
  target_link_libraries(server PRIVATE stdc++fs)
  target_link_libraries(client PRIVATE stdc++fs)
endif()
