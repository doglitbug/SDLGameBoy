#pragma once
#include "common.h"

class mmu {
public:
    mmu();
    ~mmu();

    /// @brief Read a byte from the specified address
    /// @param address as WORD
    /// @return BYTE
    BYTE readByte(WORD address);

    /// @brief Read a word from the specofied address
    /// @param address as WORD
    /// @return WORD
    WORD readWord(WORD address);

private:

};
