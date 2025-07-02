#include "cartridge.h"

cartridge::cartridge() {
    m_fileSize = 0;
    SDL_Log("Cartridge initialized");
}

cartridge::~cartridge() {
    if (m_fileSize > 0) {
        delete(m_cartridgeData);
    }
    SDL_Log("Cartridge destroyed");
}

BYTE cartridge::readByte(const WORD address) {
    if (address > m_fileSize) {
        SDL_Log("Cartridge address out of range");
        return 0x00;
    }

    return m_cartridgeData[address];
}

WORD cartridge::readWord(const WORD address) {
    return readByte(address) | (readByte(address + 1) << 8);
}

BYTE cartridge::writeByte(WORD address, BYTE value) {
    NOTYET
}

WORD cartridge::writeWord(WORD address, BYTE value) {
    NOTYET
}


bool cartridge::loadCartridgeFromFile(const std::string &filename) {
    //TODO Assume valid filename and that it has been checked?
    size_t fileSize = 0;
    void *buffer = SDL_LoadFile(filename.c_str(), &fileSize);
    if (buffer == nullptr || fileSize == 0) {
        SDL_Log("Failed to load cartridge file: %s: %s", filename.c_str(), SDL_GetError());
        SDL_free(buffer);
        return false;
    }

    m_fileSize = fileSize;
    m_cartridgeData = new BYTE[fileSize];
    memcpy(m_cartridgeData, buffer, fileSize);

    SDL_free(buffer);
    return true;
}
