#include "CryptoManager.h"
#include <cryptopp/rsa.h>
#include <cryptopp/pssr.h>
#include <cryptopp/osrng.h>
#include <cryptopp/aes.h>
#include <cryptopp/gcm.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <fstream>

CryptoManager::CryptoManager(const std::string& myPrivFile, const std::string& peerPubFile) {
    CryptoPP::FileSource fs1(myPrivFile.c_str(), true);
    myPrivateKey.Load(fs1);

    CryptoPP::FileSource fs2(peerPubFile.c_str(), true);
    peerPublicKey.Load(fs2);
}

std::vector<uint8_t> CryptoManager::encryptMessageToPeer(const std::string& plaintext) {
    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::SecByteBlock aesKey(32);
    prng.GenerateBlock(aesKey, aesKey.size());

    // AES-GCM Encryption
    CryptoPP::SecByteBlock iv(12);
    prng.GenerateBlock(iv, iv.size());
    std::string cipherText;
    CryptoPP::GCM<CryptoPP::AES>::Encryption enc;
    enc.SetKeyWithIV(aesKey, aesKey.size(), iv, iv.size());
    CryptoPP::StringSource ss(plaintext, true,
        new CryptoPP::AuthenticatedEncryptionFilter(enc,
            new CryptoPP::StringSink(cipherText)
        )
    );

    // Encrypt AES key with peer RSA public key
    std::string encryptedKey;
    CryptoPP::RSAES_OAEP_SHA_Encryptor e(peerPublicKey);
    CryptoPP::StringSource ss2(aesKey, aesKey.size(), true,
        new CryptoPP::PK_EncryptorFilter(prng, e,
            new CryptoPP::StringSink(encryptedKey)
        )
    );

    // Combine: encryptedKey + iv + cipherText
    std::vector<uint8_t> packet;
    packet.insert(packet.end(), encryptedKey.begin(), encryptedKey.end());
    packet.insert(packet.end(), iv.begin(), iv.end());
    packet.insert(packet.end(), cipherText.begin(), cipherText.end());
    return packet;
}

std::string CryptoManager::decryptPacket(const std::vector<uint8_t>& packet) {
    CryptoPP::AutoSeededRandomPool prng;

    size_t rsaSize = myPrivateKey.GetModulus().ByteCount();
    std::string encryptedKey(packet.begin(), packet.begin() + rsaSize);
    std::string iv(packet.begin() + rsaSize, packet.begin() + rsaSize + 12);
    std::string cipherText(packet.begin() + rsaSize + 12, packet.end());

    // Decrypt AES key
    std::string aesKeyStr;
    CryptoPP::RSAES_OAEP_SHA_Decryptor d(myPrivateKey);
    CryptoPP::StringSource ss(encryptedKey, true,
        new CryptoPP::PK_DecryptorFilter(prng, d,
            new CryptoPP::StringSink(aesKeyStr)
        )
    );
    CryptoPP::SecByteBlock aesKey((const CryptoPP::byte*)aesKeyStr.data(), aesKeyStr.size());

    // Decrypt AES-GCM
    std::string recovered;
    CryptoPP::GCM<CryptoPP::AES>::Decryption dec;
    dec.SetKeyWithIV(aesKey, aesKey.size(), (const CryptoPP::byte*)iv.data(), iv.size());
    CryptoPP::StringSource ss2(cipherText, true,
        new CryptoPP::AuthenticatedDecryptionFilter(dec,
            new CryptoPP::StringSink(recovered)
        )
    );
    return recovered;
}
