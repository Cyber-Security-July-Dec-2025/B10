cmake_minimum_required(VERSION 3.16)
project(SecureChat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Enable automatic moc, uic, and rcc for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

# Find Crypto++
find_library(CRYPTOPP_LIB NAMES cryptopp)

# Add executable and list all .cpp and .h files for classes with Q_OBJECT
add_executable(SecureChatApp
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/NetworkManager.cpp
    src/NetworkManager.h
    src/CryptoManager.cpp
    src/CryptoManager.h
)

# Link libraries
target_link_libraries(SecureChatApp PRIVATE Qt6::Widgets Qt6::Network ${CRYPTOPP_LIB})
