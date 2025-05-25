#include "Encryptor.h"
#include <algorithm>
#include <functional>
#include <cstring>
#include <random>

// RC4 использует S-box размером 256 байт
constexpr int SBOX_SIZE = 256;

Encryptor::Encryptor() {}

// Метод для генерации ключа из пароля
std::vector<byte> Encryptor::generateKey(const std::string& password, size_t length) {
    // Используем пароль для инициализации ключа
    std::vector<byte> key(password.begin(), password.end());
    
    // Если пароль слишком короткий, повторяем его
    if (key.size() < length) {
        size_t original_size = key.size();
        key.resize(length);
        for (size_t i = original_size; i < length; ++i) {
            key[i] = key[i % original_size];
        }
    }
    
    // Если пароль слишком длинный, обрезаем его
    if (key.size() > length) {
        key.resize(length);
    }
    
    return key;
}

// Вспомогательный метод для инициализации S-box RC4
void Encryptor::initSBox(std::vector<byte>& sbox, const std::vector<byte>& key) {
    // Инициализация S-box (стандартный алгоритм RC4)
    for (int i = 0; i < SBOX_SIZE; ++i) {
        sbox[i] = i;
    }
    
    int j = 0;
    for (int i = 0; i < SBOX_SIZE; ++i) {
        j = (j + sbox[i] + key[i % key.size()]) % SBOX_SIZE;
        std::swap(sbox[i], sbox[j]);
    }
}

Encryptor& Encryptor::getInstance() {
    static Encryptor instance;
    return instance;
}

std::vector<byte> Encryptor::encrypt(const std::vector<byte>& data, const std::string& password) {
    if (data.empty()) return {};
    
    // Генерация ключа из пароля
    std::vector<byte> key = generateKey(password, 16); // 16 байт для RC4 достаточно
    
    // Инициализация S-box
    std::vector<byte> sbox(SBOX_SIZE);
    initSBox(sbox, key);
    
    // Создаем поток ключа и выполняем XOR с данными
    std::vector<byte> result(data.size());
    
    int i = 0, j = 0;
    for (size_t k = 0; k < data.size(); ++k) {
        // Генерация потока ключа
        i = (i + 1) % SBOX_SIZE;
        j = (j + sbox[i]) % SBOX_SIZE;
        std::swap(sbox[i], sbox[j]);
        byte stream_byte = sbox[(sbox[i] + sbox[j]) % SBOX_SIZE];
        
        // XOR с байтом данных
        result[k] = data[k] ^ stream_byte;
    }
    
    return result;
}

std::vector<byte> Encryptor::decrypt(const std::vector<byte>& data, const std::string& password) {
    // В RC4 шифрование и дешифрование идентичны, так как это поточный шифр, основанный на XOR
    return encrypt(data, password);
}