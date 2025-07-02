#pragma once
#include <string>

#include "common.h"


class cartridge {
public:
    cartridge();
    ~cartridge();

    BYTE readByte(WORD address);
    WORD readWord(WORD address);
    BYTE writeByte(WORD address, BYTE value);
    WORD writeWord(WORD address, BYTE value);

    bool loadCartridgeFromFile(const std::string& filename);

private:
    int m_fileSize;
    BYTE *m_cartridgeData;
};
