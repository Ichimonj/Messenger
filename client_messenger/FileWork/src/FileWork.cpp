#include "FileWork.hpp"

void record(ofstream& file, uint64_t type){
    if (!file.write(reinterpret_cast<char*>(&type), sizeof(uint64_t)))
    {
        throw exception("Ошибка записи в файл");
    }
}
void record(ofstream& file, string& type){
    size_t typeSize = type.size();
    try
    {
        record(file, (uint64_t)typeSize);
    }
    catch (const std::exception& e)
    {
        throw e;
    }

    if (!file.write(reinterpret_cast<char*>(&type), typeSize))
    {
        throw exception("Ошибка записи в файл");
    }
}

void record(ofstream& file, char* type)
{
    if (!file.write(reinterpret_cast<char*>(type), sizeof(char)))
    {
        throw exception("Ошибка чтения из файла");
    }
}

void reading(ifstream& file, uint64_t* type){
    if (!file.read(reinterpret_cast<char*>(type), sizeof(uint64_t)))
    {
        throw exception("Ошибка чтения из файла");
    }
}

void reading(ifstream& file, string* type){
    uint64_t typeSize;
    try
    {
        reading(file, &typeSize);
    }
    catch (const std::exception& e)
    {
        throw e;
    }

    type->resize(typeSize);
    if (!file.read(reinterpret_cast<char*>(type), typeSize))
    {
        throw exception("Ошибка чтения из файла");
    }
}

void reading(ifstream& file, char* type)
{
    if (!file.read(reinterpret_cast<char*>(type), sizeof(char)))
    {
        throw exception("Ошибка чтения из файла");
    }
}
