# Network-System

Basic C++ TCP echo server and client.

Donwload: Cmake

Build (CMake)
- Create build: `cmake -S . -B build`
- Build server and client: `cmake --build build --config Release`

Usage
- Run server: `build/Release/server.exe`
- Run client: `build/Release/client.exe`

Custom
- C1: Set host and port in folder `config`
- C2: Run server, client with a argv host eg. `build/Release/client.exe 192.168.10.1`

Project Structure
```bath
Network-System/
 ├── client/
 │   ├── main.cpp
 │   └── services/
 │       └── tcp.cpp
 │
 ├── server/
 │   ├── main.cpp
 │   └── services/
 │       └── tcp.cpp
 │
 ├── config/
 │   ├── host.h
 │   ├── port.h
 │   ├── tcp.h
 │   └── config.h
 │
 ├── common/
 │   ├── utils.h
 │   └── protocol.h
 │
 ├── CMakeLists.txt
 └── README.md
```

Notes
- Windows links against `Ws2_32` automatically via CMake.
- The server echoes back whatever the client sends.
