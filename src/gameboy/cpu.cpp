#include "cpu.h"

#include <SDL3/SDL_log.h>

cpu::cpu() {
    //All data here to be taken from http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf for consistency
    m_instr[0x00] = {"NOP", 0, 0, &cpu::nop};
    m_instr[0x01] = {"LD BC, 0x%04X", 2, 0, nullptr};m_instr[0x01].execute.wordOperand = &cpu::ld_bc_nn;

    m_instr[0x11] = {"LD DE, 0x%04X", 2, 0, nullptr};m_instr[0x31].execute.wordOperand = &cpu::ld_de_nn;

    m_instr[0x21] = {"LD HL, 0x%04X", 2, 0, nullptr};m_instr[0x31].execute.wordOperand = &cpu::ld_hl_nn;

    m_instr[0x31] = {"LD SP, 0x%04X", 2, 0, nullptr};m_instr[0x31].execute.wordOperand = &cpu::ld_sp_nn;




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