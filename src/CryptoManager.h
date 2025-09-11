#pragma once
#include <string>
#include <vector>
#include <cryptopp/rsa.h>
#include <cryptopp/aes.h>
#include <cryptopp/gcm.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>

class CryptoManager {
public:
    CryptoManager(const std::string& myPrivFile, const std::string& peerPubFile);
    std::vector<uint8_t> encryptMessageToPeer(const std::string& plaintext);
    std::string decryptPacket(const std::vector<uint8_t>& packet);

private:
    CryptoPP::RSA::PrivateKey myPrivateKey;
    CryptoPP::RSA::PublicKey peerPublicKey;
};
