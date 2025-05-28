#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include "Encryptor.h"

namespace fs = std::filesystem;

void process_folder(const std::string& folder, const std::string& password) {
    for (const auto& entry : fs::recursive_directory_iterator(folder)) {
        if (entry.is_regular_file()) {
            std::string filepath = entry.path().string();
            std::ifstream in(filepath, std::ios::binary);
            std::vector<byte> data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            in.close();

            std::vector<byte> result;
            // if (encrypt) {
            result = Encryptor::getInstance().encrypt(data, password);
            std::ofstream out(filepath, std::ios::binary | std::ios::trunc);
            out.write((char*)result.data(), result.size());
            out.close();
            // } else {
            //     result = Encryptor::getInstance().decrypt(data, password);
            //     std::ofstream out(filepath, std::ios::binary | std::ios::trunc);
            //     out.write((char*)result.data(), result.size());
            //     out.close();
            // }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Использование: " << argv[0] << " <путь_к_папке> <пароль>\n";
        return 1;
    }

    std::string folder = argv[1];
    std::string password = argv[2];

    // if (mode == "encrypt") {
    process_folder(folder, password);
    std::cout << "Шифрование завершено.\n";
    // } else if (mode == "decrypt") {
    //     process_folder(folder, password, false);
    //     std::cout << "Дешифрование завершено.\n";
    return 0;
}