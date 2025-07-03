#include "mmu.h"

mmu::mmu() {
    m_biosEnabled = true;
    loadBIOS();
    SDL_Log("MMU initialized");
}

mmu::~mmu() {
    SDL_Log("MMU destroyed");
}

void mmu::loadCartridge(const std::string &fileLocation) {
    p_cartridge->loadCartridgeFromFile(fileLocation);
}

BYTE mmu::readByte(const WORD address) {
    switch (address & 0xF000) {
        case 0x0000:
            if (m_biosEnabled && address <=0x100) {
                return bios[address];
            }
        case 0x1000:
        case 0x2000:
        case 0x3000:
            return p_cartridge->readByte(address);

        default:
            NOTYET
            break;
    }
    return 0x00;
}

WORD mmu::readWord(const WORD address) {
    return readByte(address) | (readByte(address + 1) << 8);
}

void mmu::writeByte(WORD address, BYTE value) {
    //NOTYET
}

void mmu::writeWord(WORD address, WORD value) {
    //NOTYET
}

void mmu::loadBIOS() {
    size_t fileSize = 0;
    void *buffer = SDL_LoadFile("assets/gb_bios.bin", &fileSize);
    if (buffer == nullptr || fileSize == 0) {
        SDL_Log("Failed to load BIOS file: %s", SDL_GetError());
        SDL_free(buffer);
        return;
    }
    memcpy(bios, buffer, 0x100);
    SDL_free(buffer);
}
