
#include "SaveManager.h"
#include <cstdlib>
#include <fstream>
#include <filesystem>

namespace
{
    constexpr int XOR_KEY = 0x5a5a5a5a;
}

std::string SaveManager::getSavePath()
{
    const char* home = std::getenv("HOME");
    std::string path = home ? home : ".";

    path += "/.local/share/Meteorite/";

    std::filesystem::create_directories(path);

    return path + "save.dat";
}

int SaveManager::encrypt(int value)
{
    return value ^ XOR_KEY;
}

int SaveManager::decrypt(int value)
{
    return value ^ XOR_KEY;
}

int SaveManager::calcChecksum(int highScore)
{
    return highScore * 37 + 12345;
}

void SaveManager::save(const SaveData& data)
{
    std::ofstream file(getSavePath(), std::ios::binary);
    if(!file)
        return;

    int encryptedScore = encrypt(data.highScore);
    int checksum = calcChecksum(data.highScore);

    file.write(reinterpret_cast<char*>(&encryptedScore), sizeof(int));
    file.write(reinterpret_cast<char*>(&checksum), sizeof(int));
}

SaveData SaveManager::load()
{
    SaveData data;
    std::ifstream file(getSavePath(), std::ios::binary);
    if(!file)
        return SaveData{};

    int encryptedScore = 0;
    int storedChecksum = 0;

    file.read(reinterpret_cast<char*>(&encryptedScore), sizeof(int));
    file.read(reinterpret_cast<char*>(&storedChecksum), sizeof(int));

    int decryptedScore = decrypt(encryptedScore);

    if(storedChecksum != calcChecksum(decryptedScore))
    {
        return SaveData{};
    }

    data.highScore = decryptedScore;
    return data;
}
