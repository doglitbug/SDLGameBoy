#pragma once
#include <string>

#include "cartridge.h"
#include "common.h"

class mmu {
public:
    mmu();
    ~mmu();

    void loadCartridge(const std::string &fileLocation);

    /// @brief Read a byte from the specified address
    /// @param address as WORD
    /// @return BYTE
    BYTE readByte(WORD address);

    /// @brief Read a word from the specified address
    /// @param address as WORD
    /// @return WORD
    WORD readWord(WORD address);

    /// @brief Write a byte to the specified address
    /// @param address as WORD
    /// @param value BYTE to write
    void writeByte(WORD address, BYTE value);

    /// @brief Write a word to the specified address
    /// @param address as WORD
    /// @param value WORD to write
    void writeWord(WORD address, WORD value);

private:
    friend class Engine;
    //Pointer to cartridge (in Engine)
    cartridge *p_cartridge;

    //Internal BIOS
    bool m_biosEnabled;
    void loadBIOS();
    BYTE bios[0x100];
};

/* General memory map
0x0000 - 0x3FFF : 16kB ROM Bank #0           } 32kB Cartridge
0x4000 - 0x7FFF : 18kB Switchable ROM bank   }
0x8000 - 0x9FFF : 8kB Video RAM
0xA000 - 0xBFFF : 8kB switchable RAM bank
0xC000 - 0xDFFF : 8kB internal RAM
0xE000 - 0xFDFF : Echo of 8kB internal RAM
0xFE00 - 0xFE9F : Sprite Attrib memory (OAM)
0xFEA0 - 0xFEFF : Empty but unusable for I/O
0xFF00 - 0xFF4B : I/O ports
0xFF4C - 0xFF7F : Empty but unusable for I/O
0xFF80 - 0xFFFE : Internal RAM
0xFFFF - 0xFFFF : Interrupt Enable Register
 **/
