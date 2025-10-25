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
  server/storage/db.cpp
  server/storage/dbConnection.cpp
  server/storage/table.cpp
  server/storage/user.cpp
  server/storage/mail.cpp
)
target_include_directories(storage PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/server/storage
  ${CMAKE_CURRENT_SOURCE_DIR}/sqlite
)
target_link_libraries(storage PUBLIC sqlite3_vendor)

add_library(state
  client/state/db.cpp
  client/state/dbConnection.cpp
  client/state/table.cpp
  client/state/account.cpp
)
target_include_directories(state PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/client/state
  ${CMAKE_CURRENT_SOURCE_DIR}/sqlite
)
target_link_libraries(state PUBLIC sqlite3_vendor)

# ====== Server ======
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
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/server/storage)
target_link_libraries(server PRIVATE storage)

# ====== Client ======
add_executable(client
  client/main.cpp
  client/client.cpp
  client/utils.cpp
  common/protocol.cpp
  common/utils.cpp
)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/client/state)
target_link_libraries(client PRIVATE state)

if (WIN32)
  target_link_libraries(server PRIVATE Ws2_32)
  target_link_libraries(client PRIVATE Ws2_32)
endif()

# Nếu dùng std::filesystem với MinGW cũ:
if (MINGW)
  target_link_libraries(server PRIVATE stdc++fs)
  target_link_libraries(client PRIVATE stdc++fs)
endif()
