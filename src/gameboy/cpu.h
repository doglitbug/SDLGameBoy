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

    /// @brief Dispatch an opcode
    /// @param opCode Opcode
    void dispatch(BYTE opCode);
    void dispatchCB(BYTE opCode);

    /// @brief Logical exclusive OR n with register A, result in A
    /// @param value register or value
    void doXOR(const BYTE &value);

    /// Test bit in register
    /// @param bit to test
    /// @param value to test
    void doBitTest(int bit, const BYTE &value);

    /// @brief Perform one CPU cycle
    void tick(float deltaTime);

    //Normal opCodes
    void nop();                     //0x00
    void ld_bc_nn(WORD operand);    //0x01

    void ld_de_nn(WORD operand);    //0x11

    void jp_nz(BYTE operand);       //0x20
    void ld_hl_nn(WORD operand);    //0x21

    void ld_sp_nn(WORD operand);    //0x31
    void ldd_hl_a();                //0x32

    void xor_b();                   //0xA8
    void xor_c();                   //0xA9
    void xor_d();                   //0xAA
    void xor_e();                   //0xAB
    void xor_h();                   //0xAC
    void xor_l();                   //0xAD
    void xor_hl();                  //0xAE
    void xor_a();                   //0xAF

                                    //0xCB

    void xor_n(BYTE operand);       //0xEE

    //CB opCodes
    void cb_test_0_b();             //0x40
    void cb_test_0_c();             //0x41
    void cb_test_0_d();             //0x42
    void cb_test_0_e();             //0x43
    void cb_test_0_h();             //0x44
    void cb_test_0_l();             //0x45
    void cb_test_0_hl();            //0x46
    void cb_test_0_a();             //0x47
    void cb_test_1_b();             //0x48
    void cb_test_1_c();             //0x49
    void cb_test_1_d();             //0x4A
    void cb_test_1_e();             //0x4B
    void cb_test_1_h();             //0x4C
    void cb_test_1_l();             //0x4D
    void cb_test_1_hl();            //0x4E
    void cb_test_1_a();             //0x4F
    void cb_test_2_b();             //0x50
    void cb_test_2_c();             //0x51
    void cb_test_2_d();             //0x52
    void cb_test_2_e();             //0x53
    void cb_test_2_h();             //0x54
    void cb_test_2_l();             //0x55
    void cb_test_2_hl();            //0x56
    void cb_test_2_a();             //0x57
    void cb_test_3_b();             //0x58
    void cb_test_3_c();             //0x59
    void cb_test_3_d();             //0x5A
    void cb_test_3_e();             //0x5B
    void cb_test_3_h();             //0x5C
    void cb_test_3_l();             //0x5D
    void cb_test_3_hl();            //0x5E
    void cb_test_3_a();             //0x5F
    void cb_test_4_b();             //0x60
    void cb_test_4_c();             //0x61
    void cb_test_4_d();             //0x62
    void cb_test_4_e();             //0x63
    void cb_test_4_h();             //0x64
    void cb_test_4_l();             //0x65
    void cb_test_4_hl();            //0x66
    void cb_test_4_a();             //0x67
    void cb_test_5_b();             //0x68
    void cb_test_5_c();             //0x69
    void cb_test_5_d();             //0x6A
    void cb_test_5_e();             //0x6B
    void cb_test_5_h();             //0x6C
    void cb_test_5_l();             //0x6D
    void cb_test_5_hl();            //0x6E
    void cb_test_5_a();             //0x6F
    void cb_test_6_b();             //0x70
    void cb_test_6_c();             //0x71
    void cb_test_6_d();             //0x72
    void cb_test_6_e();             //0x73
    void cb_test_6_h();             //0x74
    void cb_test_6_l();             //0x75
    void cb_test_6_hl();            //0x76
    void cb_test_6_a();             //0x77
    void cb_test_7_b();             //0x78
    void cb_test_7_c();             //0x79
    void cb_test_7_d();             //0x7A
    void cb_test_7_e();             //0x7B
    void cb_test_7_h();             //0x7C
    void cb_test_7_l();             //0x7D
    void cb_test_7_hl();            //0x7E
    void cb_test_7_a();             //0x7F

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
    registers m_reg{};
    int m_cycles;
    bool pcChanged;
    std::map<int, instruction> m_instr;
    std::map<int, instruction> m_cbInstr;
    //Pointer to MMU (in Engine)
    mmu *p_mmu = nullptr;
};