#pragma once

#include "common.h"
#include <string>
#include <map>

#include "mmu.h"

class cpu;

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

    /// @brief Dispatch am opcode
    /// @param instr Opcode to test
    void dispatch(int instr);


    /// @brief Logical exclusive OR n with register A, result in A
    /// @param value register or value
    void do_xor(const BYTE &value);

    /// @brief Perform one CPU cycle
    void tick(float deltaTime);



    void nop();                     //0x00
    void ld_bc_nn(WORD operand);    //0x01

    void ld_de_nn(WORD operand);    //0x11

    void ld_hl_nn(WORD operand);    //0x21

    void ld_sp_nn(WORD operand);    //0x31

    void xor_b();                   //0xA8
    void xor_c();                   //0xA9
    void xor_d();                   //0xAA
    void xor_e();                   //0xAB
    void xor_h();                   //0xAC
    void xor_l();                   //0xAD
    void xor_hl();                  //0xAE
    void xor_a();                   //0xAF

    void xor_n(BYTE operand);       //0xEE


private:
    ///@todo This stuff is backwards, might have to switch endianness depending on target architecture?
    struct registers {
        union {
            struct {
                union {
                    struct {
                        bool _d: 1;//TODO Delete manual padding?
                        bool _c: 1;
                        bool _b: 1;
                        bool _a: 1;
                        bool carry: 1;
                        bool halfCarry: 1;
                        bool negative: 1;
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

    friend class Engine;
    registers m_reg;
    std::map<int, instruction> m_instr;
    //Pointer to MMU (in Engine)
    mmu *p_mmu;
};