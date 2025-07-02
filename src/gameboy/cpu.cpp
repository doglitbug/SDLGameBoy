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

    m_instr[0xA8] = {"XOR B",0,4,&cpu::xor_b};
    m_instr[0xA9] = {"XOR C",0,4,&cpu::xor_c};
    m_instr[0xAA] = {"XOR D",0,4,&cpu::xor_d};
    m_instr[0xAB] = {"XOR E",0,4,&cpu::xor_e};
    m_instr[0xAC] = {"XOR H",0,4,&cpu::xor_h};
    m_instr[0xAD] = {"XOR L",0,4,&cpu::xor_l};
    m_instr[0xAE] = {"XOR (HL)",0,8,&cpu::xor_hl};
    m_instr[0xAF] = {"XOR A",0,4,&cpu::xor_a};

    m_instr[0xEE] = {"XOR *",1,8,nullptr};m_instr[0xEE].execute.byteOperand = &cpu::xor_n;


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

void cpu::do_xor(const BYTE &value) {
    m_reg.a ^= value;

    m_reg.zero = m_reg.a == 0;
    m_reg.negative = false;
    m_reg.halfCarry = false;
    m_reg.carry = false;
}


// 0x00
void cpu::nop() {
    SDL_Log("NOP");
};

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

void cpu::xor_b() {
    do_xor(m_reg.b);
}

void cpu::xor_c() {
    do_xor(m_reg.c);
}

void cpu::xor_d() {
    do_xor(m_reg.d);
}

void cpu::xor_e() {
    do_xor(m_reg.e);
}

void cpu::xor_h() {
    do_xor(m_reg.h);
}

void cpu::xor_l() {
    do_xor(m_reg.l);
}

void cpu::xor_hl() {
    do_xor(p_mmu->readByte(m_reg.hl));
}

void cpu::xor_a() {
    do_xor(m_reg.a);
}

void cpu::xor_n(const BYTE operand) {
    do_xor(operand);
}
