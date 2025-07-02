#include "cpu.h"

#include <SDL3/SDL_log.h>

cpu::cpu() {
    //All data here to be taken from http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf for consistency (Pandocs?)
    //Backup https://izik1.github.io/gbops/ then https://rgbds.gbdev.io/docs/v0.9.2/gbz80.7
    m_instr[0x00] = {"NOP", 0, 4, &cpu::nop};
    m_instr[0x01] = {"LD BC, 0x%04X", 2, 12, nullptr};m_instr[0x01].execute.wordOperand = &cpu::ld_bc_nn;

    m_instr[0x11] = {"LD DE, 0x%04X", 2, 12, nullptr};m_instr[0x11].execute.wordOperand = &cpu::ld_de_nn;

    m_instr[0x21] = {"LD HL, 0x%04X", 2, 12, nullptr};m_instr[0x21].execute.wordOperand = &cpu::ld_hl_nn;

    m_instr[0x31] = {"LD SP, 0x%04X", 2, 12, nullptr};m_instr[0x31].execute.wordOperand = &cpu::ld_sp_nn;
    m_instr[0x32] = {"LDD (HL),A", 0, 8, &cpu::ldd_hl_a};

    m_instr[0xA8] = {"XOR B",0,4,&cpu::xor_b};
    m_instr[0xA9] = {"XOR C",0,4,&cpu::xor_c};
    m_instr[0xAA] = {"XOR D",0,4,&cpu::xor_d};
    m_instr[0xAB] = {"XOR E",0,4,&cpu::xor_e};
    m_instr[0xAC] = {"XOR H",0,4,&cpu::xor_h};
    m_instr[0xAD] = {"XOR L",0,4,&cpu::xor_l};
    m_instr[0xAE] = {"XOR (HL)",0,8,&cpu::xor_hl};
    m_instr[0xAF] = {"XOR A",0,4,&cpu::xor_a};

    m_instr[0xCB] = {"CB Prefix", 1, 0, nullptr};m_instr[0xCB].execute.byteOperand = &cpu::dispatchCB;

    m_instr[0xEE] = {"XOR *",1,8,nullptr};m_instr[0xEE].execute.byteOperand = &cpu::xor_n;

    //CB Prefixed
    m_cbInstr[0x00] = {"RLC B",1,8,nullptr};


    m_cbInstr[0x7C] = {"BIT 7, H",0,8,&cpu::cb_test_7_h};
    m_cbInstr[0x7D] = {"BIT 7, L",0,8,&cpu::cb_test_7_l};

    reset();
    SDL_Log("CPU initialized");
}

cpu::~cpu() {
    SDL_Log("CPU destroyed");
}

void cpu::reset() {
    m_reg.af = 0x00;
    m_reg.bc = 0x00;
    m_reg.de = 0x00;
    m_reg.hl = 0x00;
    m_reg.sp = 0x00;
    m_reg.pc = 0x00;
    //TODO And so on!
}

void cpu::tick(float deltaTime) {
    const BYTE opCode = p_mmu->readByte(m_reg.pc++);
    dispatch(opCode);
    //TODO cycles!
}

void cpu::dispatch(const int instr) {
    if (m_instr.find(instr) == m_instr.end()) {
        SDL_Log("Missing OpCode 0x%02X", instr);
        return;
    }

    if (m_instr[instr].execute.noOperand == nullptr) {
        SDL_Log("Incomplete OpCode 0x%02X", instr);
        return;
    }

        switch (m_instr[instr].operandLength) {
            case 0:
                (this->*m_instr[instr].execute.noOperand)();
                break;
            case 1:
                (this->*m_instr[instr].execute.byteOperand)(p_mmu->readByte(m_reg.pc));
                break;
            case 2:
                (this->*m_instr[instr].execute.wordOperand)(p_mmu->readWord(m_reg.pc));
                break;
            default:
                SDL_Log("Unknown operand length %d", m_instr[instr].operandLength);
                break;
        }

        m_reg.pc += m_instr[instr].operandLength;
}

void cpu::dispatchCB(const BYTE opCode) {
    if (m_cbInstr.find(opCode) == m_instr.end()) {
        SDL_Log("Missing CB OpCode 0x%02X", opCode);
        return;
    }

    if (m_cbInstr[opCode].execute.noOperand == nullptr) {
        SDL_Log("Incomplete CB OpCode 0x%02X", opCode);
        return;
    }

    (this->*m_cbInstr[opCode].execute.noOperand)();
}

void cpu::doXOR(const BYTE &value) {
    m_reg.a ^= value;

    m_reg.zero = m_reg.a == 0;
    m_reg.negative = false;
    m_reg.halfCarry = false;
    m_reg.carry = false;
}

void cpu::doBitTest(const int bit, const BYTE &value) {
    m_reg.zero = !testBit(bit, value);
    m_reg.negative = false;
    m_reg.halfCarry = true;
}


// ReSharper disable once CppMemberFunctionMayBeStatic
// 0x00
void cpu::nop() {};

void cpu::ld_bc_nn(const WORD operand) {
    m_reg.bc = operand;
}

void cpu::ld_de_nn(const WORD operand) {
    m_reg.de = operand;
}

void cpu::ld_hl_nn(const WORD operand) {
    m_reg.hl = operand;
}

void cpu::ld_sp_nn(const WORD operand) {
    m_reg.sp = operand;
}

void cpu::ldd_hl_a() {
    p_mmu->writeByte(m_reg.hl, m_reg.a);
    m_reg.hl--;
}

void cpu::xor_b() {
    doXOR(m_reg.b);
}

void cpu::xor_c() {
    doXOR(m_reg.c);
}

void cpu::xor_d() {
    doXOR(m_reg.d);
}

void cpu::xor_e() {
    doXOR(m_reg.e);
}

void cpu::xor_h() {
    doXOR(m_reg.h);
}

void cpu::xor_l() {
    doXOR(m_reg.l);
}

void cpu::xor_hl() {
    doXOR(p_mmu->readByte(m_reg.hl));
}

void cpu::xor_a() {
    doXOR(m_reg.a);
}

void cpu::xor_n(const BYTE operand) {
    doXOR(operand);
}


//CB
void cpu::cb_test_7_h() {
    doBitTest(7, m_reg.h);

}void cpu::cb_test_7_l() {
    doBitTest(7, m_reg.l);
}
