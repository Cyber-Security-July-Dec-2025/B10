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
│── CMakeLists.txt             # Build system configuration
│── Makefile                    # Optional convenience wrapper
│── README.md                   # Project documentation
│── config_alice.json           # Configuration for peer “Alice”
│── config_bob.json             # Configuration for peer “Bob”
│
├── include/                     # Header files
│   ├── CryptoManager.h
│   ├── NetworkManager.h
│   └── MainWindow.h
│
├── src/                         # Source files
│   ├── main.cpp
│   ├── CryptoManager.cpp
│   ├── NetworkManager.cpp
│   └── MainWindow.cpp
│
├── keys/                        # RSA key-pair files, public/private for each peer
│   ├── alice_private.pem
│   ├── alice_public.pem
│   ├── bob_private.pem
│   └── bob_public.pem
│
├── resources/                   # (Optional) icons, UI assets, etc.
│
└── build/                       # Build output directory (auto generated)


```

##  Dependencies
Before building, make sure you have the following installed:

- **C++17 compiler** (g++, clang++, or MSVC)
- **Qt 6 / Qt 5** (Widgets + Network modules)
- **CMake ≥ 3.16**
- **nlohmann/json** (header-only library)

### Install on Ubuntu/Debian:
```bash
# 1. Start WSL
wsl

# 2. Update package list
sudo apt update

# 3. Install compiler + build tools
sudo apt install -y build-essential cmake git

# 4. Install Qt6 (Widgets + Network + dev tools)
sudo apt install -y qt6-base-dev qt6-base-dev-tools

# 5. Install Crypto++ library
sudo apt install -y libcrypto++-dev libcrypto++-doc libcrypto++-utils


```
### Build Instructions
```bash
# Copy project into WSL home directory
cd ~
cp -r /mnt/c/Users/LENOVO/Downloads/SecureChat ~/SecureChat
cd ~/SecureChat

# Remove old build if exists
rm -rf build
mkdir build && cd build

# Configure & build
cmake ..
make -j$(nproc)

```

This generates the SecureChatApp binary inside build/.



## Running the Application
```bash
cd ..
./build/SecureChatApp config_bob.json
```

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


**Disconnect firewall if using mobile hotspot.


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
