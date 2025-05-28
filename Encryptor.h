#pragma once
#include <vector>
#include <string>

using byte = unsigned char;

class Encryptor {
private:
    Encryptor();
    Encryptor(const Encryptor&) = delete;
    Encryptor& operator=(const Encryptor&) = delete;
    std::vector<byte> generateKey(const std::string& password, size_t length);
    
    // Добавляем метод для инициализации S-box в RC4
    void initSBox(std::vector<byte>& sbox, const std::vector<byte>& key);

public:
    static Encryptor& getInstance();
    std::vector<byte> encrypt(const std::vector<byte>& data, const std::string& password);
    std::vector<byte> decrypt(const std::vector<byte>& data, const std::string& password);
};