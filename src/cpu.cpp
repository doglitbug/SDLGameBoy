#include "cpu.h"

#include <SDL3/SDL_log.h>

cpu::cpu() {
    m_instr[0x00] = {"NOP", 0, 0, &cpu::nop};
    m_instr[0x01] = {"NOP", 1, 0, nullptr};

    m_instr[0x01].execute.oneOperand = &cpu::nopByte;

    reset();
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

    //No operand
    OneOperand func = m_instr[0x01].execute.oneOperand;

    (this->*func)(0x33);
}

void cpu::nop(void) {
    SDL_Log("Nop nop mother fucker");
};

void cpu::nopByte(BYTE t) {
    SDL_Log("Argh %2x", t);
}
