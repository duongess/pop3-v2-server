# Network-System

Basic C++ TCP echo server and client.

Build (CMake)
- Create build: `cmake -S . -B build`
- Build server and client: `cmake --build build --config Release`

Usage
- Start server (default port 5050): `build/Release/server.exe`
  - Custom port: `server 6000`
- Run client (default connects to 127.0.0.1:5050): `build/Release/client.exe ${host-server}` (`build\Release\client.exe localhost 6000`)
  - Custom host/port/message: `client 127.0.0.1 5050 "Hello"`

Notes
- Windows links against `Ws2_32` automatically via CMake.
- The server echoes back whatever the client sends.
