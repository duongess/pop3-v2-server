cmake_minimum_required(VERSION 3.15)

# BẬT C luôn (vì có sqlite3.c)
project(NetworkSystem C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TẠO BIẾN DÙNG CHUNG: Dùng biến này thay cho ${CMAKE_CURRENT_SOURCE_DIR} cho gọn
set(ROOT_DIR ${CMAKE_SOURCE_DIR})

# Guard để bắt thiếu file ngay khi configure
if(NOT EXISTS "${ROOT_DIR}/sqlite/sqlite3.c")
  message(FATAL_ERROR "Missing sqlite/sqlite3.c — hãy đặt đúng amalgamation vào thư mục sqlite/")
endif()

# =================================================================================
# 1. SQLITE VENDOR LIBRARY (Khắc phục lỗi windows.h/sys/ioctl.h)
# =================================================================================
add_library(sqlite3_vendor STATIC
  sqlite/sqlite3.c
)
target_include_directories(sqlite3_vendor PUBLIC
  ${ROOT_DIR}/sqlite
)

# ĐỊNH NGHĨA MACRO ĐIỀU KIỆN
if(NOT WIN32)
    # LƯU Ý: Nếu không phải Windows, định nghĩa UNIX để tránh windows.h
    target_compile_definitions(sqlite3_vendor PRIVATE
        SQLITE_OS_UNIX=1
    )
endif()

target_compile_definitions(sqlite3_vendor PRIVATE
  SQLITE_THREADSAFE=1
  SQLITE_OMIT_LOAD_EXTENSION=1
)

if(MSVC)
  target_compile_options(sqlite3_vendor PRIVATE /W3)
else()
  target_compile_options(sqlite3_vendor PRIVATE -Wall -Wextra)
endif()

# =================================================================================
# 2. ĐỊNH NGHĨA CÁC ĐƯỜNG DẪN INCLUDE DÙNG CHUNG (GOM VÀO TARGET RIÊNG)
# =================================================================================

# Tạo một target interface chỉ để quản lý các đường dẫn include dùng chung
add_library(NetworkSystem_Headers INTERFACE)
target_include_directories(NetworkSystem_Headers INTERFACE
    ${ROOT_DIR}
    # Thêm các thư mục include chính của Server/Client để dễ gọi header
    ${ROOT_DIR}/server/include
    ${ROOT_DIR}/lib/include
)


# =================================================================================
# 3. STORAGE/STATE LAYERS (Link với NetworkSystem_Headers)
# =================================================================================

# ====== Storage layer ======
add_library(storage
  server/src/storage/db.cpp
  server/src/storage/dbConnection.cpp
  server/src/storage/table.cpp
  server/src/storage/user.cpp
  server/src/storage/mail.cpp
)
target_include_directories(storage PUBLIC
  ${ROOT_DIR}/server/src/storage 
)
target_link_libraries(storage PUBLIC sqlite3_vendor NetworkSystem_Headers)

# =================================================================================
# 4. SERVER EXECUTABLE
# =================================================================================

# ====== Server ======
file(GLOB_RECURSE SERVER_SOURCES "server/src/*.cpp")
file(GLOB LIB_SOURCES "lib/src/*.cpp")
add_executable(server
    ${SERVER_SOURCES}
    ${LIB_SOURCES}
)
# Server link đến storage và NetworkSystem_Headers
target_link_libraries(server PRIVATE storage NetworkSystem_Headers)


# =================================================================================
# 5. LIÊN KẾT HỆ THỐNG ĐA NỀN TẢNG
# =================================================================================

if (WIN32)
  # Link thư viện mạng Windows
  target_link_libraries(server PRIVATE Ws2_32)
endif()

if (MINGW)
  # Nếu dùng std::filesystem với MinGW cũ
  target_link_libraries(server PRIVATE stdc++fs)
endif()