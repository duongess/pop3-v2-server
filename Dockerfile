# ===== STAGE 1: builder =====
FROM gcc:latest AS builder
RUN apt-get update && apt-get install -y --no-install-recommends build-essential cmake \
  && rm -rf /var/lib/apt/lists/*
WORKDIR /usr/src/app
COPY . .

# build ra đúng thư mục build/ để khớp với runtime
RUN cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=/usr/src/app/build \
    -DCMAKE_C_FLAGS="-DSQLITE_OS_UNIX=1" \
    -DCMAKE_CXX_FLAGS="-DSQLITE_OS_UNIX=1"
RUN cmake --build build -j

# ===== STAGE 2: runtime =====
FROM debian:stable-slim
RUN apt-get update && apt-get install -y --no-install-recommends libstdc++6 \
  && rm -rf /var/lib/apt/lists/*
WORKDIR /usr/app

# tạo 2 thư mục ngang hàng như local
RUN mkdir -p /usr/app/build

# copy đúng binary (đổi tên 'server' nếu target của bạn khác)
COPY --from=builder /usr/src/app/build/server /usr/app/build/server

# (khuyên) dùng cổng >1024; nếu vẫn 21 thì giữ nguyên
EXPOSE 21000

# giữ cùng cấu trúc đường dẫn như lúc bạn chạy trên Windows
CMD ["./build/server"]
