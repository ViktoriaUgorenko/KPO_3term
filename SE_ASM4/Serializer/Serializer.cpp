#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdint>

//Устанавливает выравнивание на 1 байт, убирая дополнительные байты выравнивания.
#pragma pack(push, 1) // Упаковать структуру без выравнивания
struct MetaData {
    char type[10];     // Тип данных
    uint32_t size;     // Размер данных в байтах
    char data[20];     // Сериализованные данные
};
//Возвращает настройки выравнивания к значениям, которые были до вызова push.
#pragma pack(pop)

int main() {
    setlocale(LC_ALL, "ru");

    // Исходные данные
    short shortVar = 3;              // Пример переменной short
    uint16_t unsignedShortVar = 8;  // Пример беззнакового целого литерала (2 байта)

    // Открытие файла для записи
    std::ofstream outFile("serialized_data.bin", std::ios::binary);
    if (!outFile) {
        std::cerr << "Ошибка открытия файла для записи.\n";
        return 1;
    }

    MetaData shortMeta;
    strcpy_s(shortMeta.type, "short");
    shortMeta.size = sizeof(shortVar);
    std::memcpy(shortMeta.data, &shortVar, sizeof(shortVar));
    outFile.write(reinterpret_cast<char*>(&shortMeta), sizeof(shortMeta));

    MetaData unsignedShortMeta;
    strcpy_s(unsignedShortMeta.type, "uint16_t");
    unsignedShortMeta.size = sizeof(unsignedShortVar);
    std::memcpy(unsignedShortMeta.data, &unsignedShortVar, sizeof(unsignedShortVar));
    outFile.write(reinterpret_cast<char*>(&unsignedShortMeta), sizeof(unsignedShortMeta));

    outFile.close();
    std::cout << "Сериализация завершена. Данные сохранены в serialized_data.bin\n";
    return 0;
}
