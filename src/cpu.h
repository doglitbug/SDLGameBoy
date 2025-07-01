#pragma once

#include <array>
#include <map>

#include "common.h"
#include <string>

class cpu;

///@todo This stuff is backwards, might have to switch endianness depending on target architecture?
struct registers {
    union {
        struct {
            union {
                struct {
                    bool _d: 1;
                    bool _c: 1;
                    bool _b: 1;
                    bool _a: 1;
                    bool carry: 1;
                    bool halfCarry: 1;
                    bool subtract: 1;
                    bool zero: 1;
                };

                BYTE flag;
            };

            BYTE a;
        };

        WORD af;
    };

    union {
        struct {
            BYTE c;
            BYTE b;
        };

        WORD bc;
    };

    union {
        struct {
            BYTE e;
            BYTE d;
        };

        WORD de;
    };

    union {
        struct {
            BYTE l;
            BYTE h;
        };

        WORD hl;
    };

    WORD sp;
    WORD pc;
};

//typedef void (*NoOperand)(const cpu &);
typedef void (cpu::*NoOperand)(void);
typedef void (cpu::*OneOperand)(BYTE);
typedef void (cpu::*TwoOperand)(WORD);

class cpu {
public:
    struct instruction {
        std::string disassembly;
        int operandLength;
        int cycles;
        union {
            NoOperand noOperand;
            OneOperand oneOperand;
            TwoOperand twoOperand;
        } execute;
    };

    cpu();

    ~cpu();

    /// @brief Set the CPU to a known state
    void reset();

    /// @brief Perform one CPU cycle
    void tick();

    /// @brief 0x00 NOP No operation
    void nop(void);

    void nopByte(BYTE i);

private:
    registers m_reg;
    std::map<int, instruction> m_instr;
};



