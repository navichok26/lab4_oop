#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include "Encryptor.h"

namespace fs = std::filesystem;

void process_folder(const std::string& folder, const std::string& password, bool encrypt) {
    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.is_regular_file()) {
            std::string filepath = entry.path().string();
            std::ifstream in(filepath, std::ios::binary);
            std::vector<byte> data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            in.close();

            std::vector<byte> result;
            if (encrypt) {
                result = Encryptor::getInstance().encrypt(data, password);
                std::ofstream out(filepath + ".enc", std::ios::binary);
                out.write((char*)result.data(), result.size());
                out.close();
                fs::remove(filepath);
            } else {
                // Проверяем, что файл оканчивается на ".enc"
                if (filepath.size() >= 4 && filepath.substr(filepath.size() - 4) == ".enc") {
                    result = Encryptor::getInstance().decrypt(data, password);
                    std::string outpath = filepath.substr(0, filepath.size() - 4); // remove ".enc"
                    std::ofstream out(outpath, std::ios::binary);
                    out.write((char*)result.data(), result.size());
                    out.close();
                    fs::remove(filepath);
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Использование: " << argv[0] << " <путь_к_папке> <пароль> <encrypt|decrypt>\n";
        return 1;
    }

    std::string folder = argv[1];
    std::string password = argv[2];
    std::string mode = argv[3];

    if (mode == "encrypt") {
        process_folder(folder, password, true);
        std::cout << "Шифрование завершено.\n";
    } else if (mode == "decrypt") {
        process_folder(folder, password, false);
        std::cout << "Дешифрование завершено.\n";
    } else {
        std::cout << "Неизвестный режим. Используйте encrypt или decrypt.\n";
        return 1;
    }
    return 0;
}