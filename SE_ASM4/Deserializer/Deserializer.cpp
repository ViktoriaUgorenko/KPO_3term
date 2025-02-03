#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdint>

#pragma pack(push, 1)
struct MetaData {
    char type[10];
    uint32_t size;
    char data[20];
};
#pragma pack(pop)

int main() {
    setlocale(LC_ALL, "ru");

    // Открытие файла для чтения
    std::ifstream inFile("D:\\БГТУ\\КПО3сем\\SE_ASM4\\Serializer\\serialized_data.bin", std::ios::binary);
    if (!inFile) {
        std::cerr << "Ошибка открытия файла для чтения.\n";
        return 1;
    }

    // Открытие файла для записи ассемблерного кода
    std::ofstream asmFile("deserialized_data.asm");
    if (!asmFile) {
        std::cerr << "Ошибка открытия файла для записи ассемблерного кода.\n";
        return 1;
    }

    asmFile << ".data\n"; // Начало секции данных

    MetaData meta;
    while (inFile.read(reinterpret_cast<char*>(&meta), sizeof(meta))) {
        asmFile << "; Тип данных: " << meta.type << "\n";
        if (strcmp(meta.type, "short") == 0) {
            short shortVar;
            std::memcpy(&shortVar, meta.data, sizeof(short));
            asmFile << "shortVar DW " << shortVar << "h\n"; // Записываем в HEX
        }
        else if (strcmp(meta.type, "uint16_t") == 0) {
            uint16_t unsignedShortVar;
            std::memcpy(&unsignedShortVar, meta.data, sizeof(uint16_t));
            asmFile << "unsignedShortVar DW " << unsignedShortVar << "h\n";
        }
    }

    asmFile.close();
    inFile.close();
    std::cout << "Десериализация завершена. Ассемблерный код сохранён в deserialized_data.asm\n";
    return 0;
}
