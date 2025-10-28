# Network-System: Basic C++ POP3 V2 Echo Server

This project implements a basic **POP3 (Post Office Protocol version 3)** echo server using C++ and TCP sockets. The server is designed to listen for connections and simply **echo back** any data received from the client.

## 🚀 Key Features

  * **Core Function:** POP3 V2 Echo Server.
  * **Networking:** Utilizes fundamental TCP/IP sockets.
  * **Build System:** CMake.
  * **Windows Note:** Automatically links against the `Ws2_32` library via CMake.
  * **Cross-Platform Support:** Includes necessary helper libraries/configurations for Linux and macOS (e.g., handling socket differences).
  * **Database:** Uses **SQLite** for data persistence (e.g., user and mail storage).
  * **Deployment Endpoint:** `interchange.proxy.rlwy.net:16785`

-----

## 🛠️ Setup and Build

### 1\. Prerequisites

You must have **gpp 15+** and **CMake** installed to build the project.

**CMake Installation on Windows (using winget):**

```bash
winget install Kitware.CMake
```

### 2\. Build Process

Execute the following commands from the project root directory:

1.  **Create Build Directory:** Generate the necessary build files (e.g., Makefiles or Visual Studio projects) within the `build/` directory.
    ```bash
    cmake -S . -B build
    ```
2.  **Build Server:** Compile the source code to create the `server.exe` executable in the `build/` directory.
    ```bash
    cmake --build build --config Release
    ```

-----

## 💻 Usage

### Running the Server

After a successful build, start the server using the generated executable:

```bash
build/server.exe
```

The server will begin listening for incoming client connections on the configured port.

### Connecting the Client

Use the dedicated client (available at [https://github.com/duongess/pop3-v2-client](https://github.com/duongess/pop3-v2-client)) to connect to the server and test the echo functionality.

The client sends data, and the server echoes the exact data back.

-----

## 🐳 Docker Deployment

To containerize the server application:

1.  **Build Docker Image:** (Assuming a `Dockerfile` exists in the root directory)
    ```bash
    docker build -t pop3-echo-server .
    ```
2.  **Create and Run Container:** Start a container and map the necessary port to the host.
    ```bash
    docker run -d --name pop3-service -p 21000:21 pop3-echo-server
    ```
    *Note: Ensure the mapped port `21000` matches the port configured in your server's `config/` files.*

-----

## 📂 Project Structure

The server-focused project is organized for modularity and clear separation of concerns:

```bash
Server/
│
├───common/               # Shared utilities and protocol definitions
│       console.h
│       protocol.cpp
│       protocol.h
│       utils.cpp
│       utils.h
│
├───config/               # Configuration files (host, port, etc.)
│
├───database/             # SQLite database files
│       POP3V2.db
│       POP3V2.dev.db
│
├───server/               # Main server implementation files
│   ├───include/          # Server header files (interfaces)
│   │       db.h
│   │       dbConnection.h
│   │       pop3-v2.h
│   │       server.h
│   │       service-manager.h
│   │       session-manager.h
│   │       tcp.h
│   │       mail.h, user.h, table.h ...
│   │
│   └───src/              # Server source files (implementation)
│       │   main.cpp      # Server entry point
│       │   server.cpp
│       ├───services/     # Protocol and networking handlers
│       │       pop3-v2.cpp
│       │       tcp.cpp
│       │
│       └───storage/      # Database interaction (ORM/DAO)
│               db.cpp
│               dbConnection.cpp
│               mail.cpp
│               user.cpp
│
├───sqlite/               # SQLite library/source files (if static linking)
│
└───types/                # Custom data type definitions
```