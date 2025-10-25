# ========================
# STAGE 1: BUILDER (Sử dụng CMake)
# Dùng để cài đặt các công cụ build (g++, CMake) và biên dịch mã nguồn C++
# ========================
FROM gcc:latest AS builder

# Cài đặt CMake và Build Essentials (bao gồm g++ và make)
# (Thực hiện trong một lệnh RUN để tối ưu hóa caching layer của Docker)
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    && rm -rf /var/lib/apt/lists/*

# Thiết lập thư mục làm việc bên trong container
WORKDIR /usr/src/app

# Copy tất cả mã nguồn, CMakeLists.txt và các file liên quan
COPY . .

# Quy trình Build với CMake
# Bước 1: Tạo thư mục build và chuyển vào đó
RUN mkdir build
RUN mkdir database
WORKDIR /usr/src/app/build

# Bước 2: Chạy CMake để tạo Makefile (trỏ lên thư mục cha chứa CMakeLists.txt)
RUN cmake .. -DCMAKE_C_FLAGS="-DSQLITE_OS_UNIX=1" -DCMAKE_CXX_FLAGS="-DSQLITE_OS_UNIX=1"

# Bước 3: Chạy Make để biên dịch
RUN make

# ========================
# STAGE 2: RUNTIME
# Chỉ chứa chương trình Server đã biên dịch
# ========================
FROM debian:stable-slim

# Cài đặt các thư viện C++ runtime cần thiết (libstdc++)
RUN apt-get update && apt-get install -y --no-install-recommends \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

# Thiết lập thư mục làm việc cho chương trình
WORKDIR /usr/app

# Chỉ COPY file thực thi network_server từ stage 'builder'
# (network_client bị loại bỏ để giữ image nhỏ gọn)
COPY --from=builder /usr/src/app/build/network_server .

# Cấu hình cổng cho Server (Giữ lại EXPOSE 21 theo yêu cầu)
EXPOSE 21

# Lệnh mặc định để chạy Server khi Container khởi động
CMD ["./network_server"]