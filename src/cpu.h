#pragma once

#include "common.h"
#include <string>
#include <map>

#include "mmu.h"

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

typedef void (cpu::*NoOperand)();
typedef void (cpu::*ByteOperand)(BYTE);
typedef void (cpu::*WordOperand)(WORD);

class cpu {
public:
    struct instruction {
        std::string disassembly;
        int operandLength;
        int cycles;
        union {
            NoOperand noOperand;
            ByteOperand byteOperand;
            WordOperand wordOperand;
        } execute;
    };

    cpu();
    ~cpu();

    /// @brief Set the CPU to a known state
    void reset();

    /// @brief Testing only
    /// @param instr Opcode to test
    void test(int instr);

    /// @brief Perform one CPU cycle
    void tick();

    // 0x00
    void nop();
    void ld_bc_nn(WORD nn);

    // 0x10


private:
    friend class Engine;
    registers m_reg;
    std::map<int, instruction> m_instr;
    //Pointer to MMU (in Engine)
    mmu *p_mmu;
};



