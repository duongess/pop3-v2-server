# Network-System

Basic C++ TCP echo server and client.

Donwload: Cmake
```
winget install Kitware.CMake
```

Build (CMake)
- Create build: `cmake -S . -B build`
- Build server and client: `cmake --build build --config Release`

Usage
- Run server: `build/server.exe`
- Run client: `build/client.exe`

Custom
- O1: Set host and port in folder `config`
- O2: Run server, client with a argv host eg. `build/client.exe 192.168.10.1`

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
