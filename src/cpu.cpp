#include "cpu.h"

#include <SDL3/SDL_log.h>

cpu::cpu() {
    m_instr[0x00] = {"NOP", 0, 0, &cpu::nop};
    m_instr[0x01] = {"LD BC, 0x%04X", 2, 0, nullptr};
    m_instr[0x01].execute.wordOperand = &cpu::ld_bc_nn;

    reset();
    test(0);
    test(1);
    test(0x99);
}

cpu::~cpu() {
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

void cpu::test(const int instr) {
    if (m_instr.find(instr) == m_instr.end()) {
        SDL_Log("Unimplemented OpCode 0x%02X", instr);
        return;
    }

    switch (m_instr[instr].operandLength) {
        case 0:
            (this->*m_instr[instr].execute.noOperand)();
            break;
        case 1:
            //TODO Read from MMU and inc PC

            (this->*m_instr[instr].execute.byteOperand)(p_mmu->readByte(1));
            break;
        case 2:
            //TODO Read from MMU and inc PC
            (this->*m_instr[instr].execute.wordOperand)(p_mmu->readWord(2));
            break;
        default:
            SDL_Log("Unknown operand length %d", m_instr[instr].operandLength);
            break;
    }
}

void cpu::tick() {
}

// 0x00
void cpu::nop() {
    SDL_Log("NOP");
};

void cpu::ld_bc_nn(const WORD nn) {
    m_reg.bc = nn;
}

// 0x10
