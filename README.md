#  SecureChat – Encrypted Peer-to-Peer Chat

## Overview
**SecureChat** is a cross-platform Qt-based desktop application for **end-to-end encrypted peer-to-peer communication**.  
It allows two peers to exchange messages securely using asymmetric cryptography, TCP sockets, and a simple GUI.

###  Features
- End-to-End Encryption using a **CryptoManager** wrapper.
- Peer-to-Peer messaging (one acts as **server**, the other as **client**).
- Qt-based GUI with:
  -  Connect button
  - Send message box
  -  Disconnect button
  -  Live chat history
- Works across two systems on the same network (LAN or via port forwarding).

---

## Project Structure
```bash
SecureChat/
│── CMakeLists.txt # Build system configuration
│── README.md # Project documentation
│
├── include/ # Header files
│ ├── CryptoManager.h
│ ├── NetworkManager.h
│ └── MainWindow.h
│
├── src/ # Source files
│ ├── main.cpp
│ ├── CryptoManager.cpp
│ ├── NetworkManager.cpp
│ └── MainWindow.cpp
│
├── resources/ # (Optional) icons, UI assets
└── build/ # Build output (created after compilation)

```

##  Dependencies
Before building, make sure you have the following installed:

- **C++17 compiler** (g++, clang++, or MSVC)
- **Qt 6 / Qt 5** (Widgets + Network modules)
- **CMake ≥ 3.16**
- **nlohmann/json** (header-only library)

### Install on Ubuntu/Debian:
```bash
sudo apt update
sudo apt install build-essential cmake qtbase5-dev qt5-qmake \
                 qtmultimedia5-dev libqt5widgets5 \
                 libqt5network5 nlohmann-json3-dev

```
### Build Instructions
 1. Clone Repository
git clone https://github.com/<your-username>/SecureChat.git
cd SecureChat

2. Create Build Directory
mkdir build && cd build

 3. Configure CMake
cmake ..

 4. Compile
make -j4


This generates the SecureChatApp binary inside build/.



## Running the Application

You need two systems (or two terminals):

One runs as the Server (listening on a port).

The other connects as a Client.

Example Setup:
On System A (Server)
./SecureChatApp


Enter your private key and peer’s public key in code/config.

Start the server at a chosen port (e.g., 5000).

On System B (Client)
./SecureChatApp


Enter your private key and peer’s public key in code/config.

Click Connect, enter Server’s IP + Port (e.g., 192.168.1.5:5000).'



## Usage Guide

Launch app on both systems.

Server starts listening (net->startServer(listenPort)).

Client enters IP + Port of server and clicks Connect.

Both can send messages securely:

You: Hello
Peer: Hi!


Click Disconnect to close the connection.



##  GUI Overview

Connect – Establish connection with peer.

Send – Send encrypted message.

Disconnect – Close the connection.

Input Textbox - To type the message

Message Display – Shows ongoing chat history.


##  Security

SecureChat ensures **end-to-end encrypted peer-to-peer communication** using a hybrid encryption approach:

1. **Key Management**
   - Each peer has a **unique RSA public-private key pair** (manually generated beforehand).
   - Each peer stores:
     - Its **own private key** (never shared).
     - The **public key of the peer**.
   - Both keys are stored in a single **configuration file** (`config.json`) at the project root.

2. **Message Encryption Process**
   - For each outgoing message:
     - A fresh **random AES session key** is generated.
     - The message is encrypted using **AES** with this session key.
     - The AES session key is then encrypted using the **receiver’s RSA public key**.
     - Both the **encrypted message** and **encrypted session key** are sent over the socket.

3. **Message Decryption Process**
   - On receiving a message:
     - The AES session key is decrypted using the **receiver’s RSA private key**.
     - The message is then decrypted using this AES session key.
   - If decryption fails, the application displays:  
     `!! Decryption failed !!`

4. **Design Simplifications**
   - IP address, port number, RSA key sizes, and cryptographic parameters are stored in `config.json` — not entered via GUI.
   - This avoids user errors and ensures consistent configurations across both peers.



## Troubleshooting

CMake error about JSON → install nlohmann-json3-dev.

Compiler not found → install g++:

sudo apt install g++


Different cache dir error → delete build/ and re-run cmake:

rm -rf build && mkdir build && cd build && cmake ..


Connection issues → ensure:

Both systems are on same LAN

Firewall allows chosen port

Correct IP address is used


## Contributors
1. Tanya Singh (IIT2023171)
2. Anushka Chakravarti (IIT2023182)
3. Suman Kumari (IIT2023187)
4. Barre Shree Kruthi Reddy (IIT2023188)
5. Ambika Gavel (IIT2023193)
