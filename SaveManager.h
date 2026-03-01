#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <string>

struct SaveData
{
    int highScore = 0;
};

class SaveManager
{
private:
    static std::string getSavePath();

    static int encrypt(int value);
    static int decrypt(int value);

    static int calcChecksum(int highScore);

public:
    static void save(const SaveData& data);
    static SaveData load();
};

#endif

