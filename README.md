# Network-System: Basic C++ POP3 V2 Echo Server

This project implements a basic **POP3 (Post Office Protocol version 3)** echo server using C++ and TCP sockets. The server is designed to listen for connections and simply **echo back** any data received from the client.

## 🚀 Key Features

* **Core Function:** POP3 V2 Echo Server.
* **Networking:** Utilizes fundamental TCP/IP sockets.
* **Build System:** CMake (Modern and Cross-Platform).
* **Database:** Integrated **SQLite** for data persistence (user and mail storage) without requiring an external SQL server.
* **Cross-Platform Support:**
* **Windows:** Automatically links against the `Ws2_32` library.
* **Linux/macOS:** Fully supported via GCC/Clang with standard socket APIs.


* **Deployment:** Supports rapid deployment via Docker.

---

## 📂 Project Structure

The project is organized into modules to separate core networking libraries, server logic, and storage layers:

```text
.
├── lib/               # Shared libraries & Core utilities
│   ├── include/       # Headers: socket.h, cli.h, utils.h...
│   └── src/           # Implementation: TCP socket handling, CLI tools
│
├── server/            # Main Server Application
│   ├── include/       # Headers: User, Mail, ServerConfig...
│   └── src/           # Server implementation
│       ├── services/  # POP3 protocol handlers & Session management
│       └── storage/   # Database interactions (SQLite Wrappers)
│
├── sqlite/            # SQLite source files (Vendor code)
├── types/             # Common data type definitions
├── CMakeLists.txt     # Main build configuration
└── Dockerfile         # Docker build configuration

```

---

## 🛠️ Setup and Build

### 1. Prerequisites

* **C++ Compiler:** Must support **C++17** or higher (GCC, Clang, or MSVC).
* **CMake:** Version 3.15 or higher.

### 2. Build Instructions

#### 🐧 Linux (Ubuntu/Debian) & 🍎 macOS

1. **Install Dependencies:**
* **Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential cmake

```


* **macOS (via Homebrew):**
```bash
brew install cmake

```




2. **Build:**
Navigate to the project root and run:
```bash
# Generate build files
cmake -S . -B build

# Compile the project
cmake --build build --config Release

```


3. **Run the Server:**
```bash
./build/server

```



#### 🪟 Windows

1. **Install CMake:**
Using Winget or downloading from the official site:
```powershell
winget install Kitware.CMake

```


2. **Build:**
Open PowerShell or CMD at the project root:
```powershell
# Generate build files (Visual Studio solution or MinGW makefiles)
cmake -S . -B build

# Compile the executable
cmake --build build --config Release

```


3. **Run the Server:**
```powershell
# Path may vary depending on the generator (Release/Debug folders)
.\build\Release\server.exe
# Or if using MinGW/Ninja:
.\build\server.exe

```



---

## 🐳 Docker Deployment

You can containerize and run the application instantly without setting up a C++ environment.

1. **Build Docker Image:**
```bash
docker build -t pop3-echo-server .

```


2. **Run Container:**
Start the container and map the ports (e.g., mapping host port `21000` to container port `21`):
```bash
docker run -d --name pop3-service -p 21000:21 pop3-echo-server

```


*Note: Ensure the mapped port matches the configuration in your server config files.*

---

## 💻 Usage (Client)

Once the server is running and listening for connections:

1. **Connect:** Use the dedicated client available at [https://github.com/duongess/pop3-v2-client](https://github.com/duongess/pop3-v2-client).
2. **Interaction:** The client sends data (POP3 commands or text strings), and the server will echo the exact data back to verify the connection and protocol integrity.

---

## 📝 License

This project is open-source. Please refer to the `LICENSE` file in the root directory for more information.