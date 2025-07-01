#include "mmu.h"

mmu::mmu() {
}

mmu::~mmu() {
}

BYTE mmu::readByte(WORD address) {
    //TODO Implement
    return 0x12;
}

WORD mmu::readWord(WORD address) {
    return 0x4321;
}
