#include "cpu.h"

#include <SDL3/SDL_log.h>

cpu::cpu() {
    //All data here to be taken from http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf for consistency (Pandocs?)
    //Backup https://izik1.github.io/gbops/ then https://rgbds.gbdev.io/docs/v0.9.2/gbz80.7
    m_instr[0x00] = {"NOP", 0, 4, &cpu::nop};
    m_instr[0x01] = {"LD BC,u16", 2, 12, nullptr};m_instr[0x01].execute.wordOperand = &cpu::ld_bc_nn;
    m_instr[0x02] = {"LD (BC),A", 0, 8, &cpu::ld_pbc_a};

    m_instr[0x06] = {"LD B,u8", 1, 8, nullptr};m_instr[0x06].execute.byteOperand = &cpu::ld_b_n;

    m_instr[0x0A] = {"LD A,(BC)",0,8,&cpu::ld_a_pbc};

    m_instr[0x0C] = {"INC C",0,4,&cpu::inc_c};

    m_instr[0x0E] = {"LD C,u8", 1, 8, nullptr};m_instr[0x0E].execute.byteOperand = &cpu::ld_c_n;

    m_instr[0x11] = {"LD DE,u16", 2, 12, nullptr};m_instr[0x11].execute.wordOperand = &cpu::ld_de_nn;
    m_instr[0x12] = {"LD (DE),A", 0, 8, &cpu::ld_pde_a};

    m_instr[0x16] = {"LD D,u8", 1, 8, nullptr};m_instr[0x16].execute.byteOperand = &cpu::ld_d_n;

    m_instr[0x1A] = {"LD A,(DE)",0,8,&cpu::ld_a_pde};

    m_instr[0x1E] = {"LD E,u8", 1, 8, nullptr};m_instr[0x1E].execute.byteOperand = &cpu::ld_e_n;

    m_instr[0x20] = {"JP NZ,i8",1,8,nullptr};m_instr[0x20].execute.byteOperand = &cpu::jp_nz;
    m_instr[0x21] = {"LD HL,u16", 2, 12, nullptr};m_instr[0x21].execute.wordOperand = &cpu::ld_hl_nn;

    m_instr[0x26] = {"LD H,u8", 1, 8, nullptr};m_instr[0x26].execute.byteOperand = &cpu::ld_h_n;

    m_instr[0x2E] = {"LD L,u8", 1, 8, nullptr};m_instr[0x2E].execute.byteOperand = &cpu::ld_l_n;

    m_instr[0x31] = {"LD SP,u16", 2, 12, nullptr};m_instr[0x31].execute.wordOperand = &cpu::ld_sp_nn;
    m_instr[0x32] = {"LDD (HL),A", 0, 8, &cpu::ldd_hl_a};

    m_instr[0x3E] = {"LD A,u8", 1, 8, nullptr};m_instr[0x3E].execute.byteOperand = &cpu::ld_a_n;

    m_instr[0x47] = {"LD B,A", 0, 4, &cpu::ld_b_a};

    m_instr[0x4F] = {"LD C,A", 0, 4, &cpu::ld_c_a};

    m_instr[0x57] = {"LD D,A", 0, 4, &cpu::ld_d_a};

    m_instr[0x5F] = {"LD E,A", 0, 4, &cpu::ld_e_a};

    m_instr[0x67] = {"LD H,A", 0, 4, &cpu::ld_h_a};

    m_instr[0x6F] = {"LD L, A", 0, 4, &cpu::ld_l_a};

    m_instr[0x77] = {"LD (HL),A", 0, 8, &cpu::ld_phl_a};
    m_instr[0x78] = {"LD A,B", 0, 4, &cpu::ld_a_b};
    m_instr[0x79] = {"LD A,C", 0, 4, &cpu::ld_a_c};
    m_instr[0x7A] = {"LD A,D", 0, 4, &cpu::ld_a_d};
    m_instr[0x7B] = {"LD A,E", 0, 4, &cpu::ld_a_e};
    m_instr[0x7C] = {"LD A,H", 0, 4, &cpu::ld_a_h};
    m_instr[0x7D] = {"LD A,L", 0, 4, &cpu::ld_a_l};
    m_instr[0x7E] = {"LD A,(HL)",0,8,&cpu::ld_a_phl};
    m_instr[0x7F] = {"LD A,A", 0, 4, &cpu::ld_a_a};

    m_instr[0xA8] = {"XOR B",0,4,&cpu::xor_b};
    m_instr[0xA9] = {"XOR C",0,4,&cpu::xor_c};
    m_instr[0xAA] = {"XOR D",0,4,&cpu::xor_d};
    m_instr[0xAB] = {"XOR E",0,4,&cpu::xor_e};
    m_instr[0xAC] = {"XOR H",0,4,&cpu::xor_h};
    m_instr[0xAD] = {"XOR L",0,4,&cpu::xor_l};
    m_instr[0xAE] = {"XOR (HL)",0,8,&cpu::xor_phl};
    m_instr[0xAF] = {"XOR A",0,4,&cpu::xor_a};

    m_instr[0xC5] = {"PUSH BC", 0, 16, &cpu::push_bc};

    m_instr[0xCB] = {"CB Prefix", 1, 0, nullptr};m_instr[0xCB].execute.byteOperand = &cpu::dispatchCB;

    m_instr[0xCD] = {"CALL u16",2,24,nullptr};m_instr[0xCD].execute.wordOperand = &cpu::call_nn;

    m_instr[0xD5] = {"PUSH DE", 0, 16, &cpu::push_de};

    m_instr[0xE0] = {"LD (FF00+u8),A",1,12,nullptr};m_instr[0xE0].execute.byteOperand = &cpu::ldh_n_a;

    m_instr[0xE2] = {"LD (FF00+C),A",0,8,&cpu::ldh_c_a};

    m_instr[0xE5] = {"PUSH HL", 0, 16, &cpu::push_hl};

    m_instr[0xEA] = {"LD u16,A",2,16,nullptr};m_instr[0xEA].execute.wordOperand = &cpu::ld_nn_a;

    m_instr[0xEE] = {"XOR *",1,8,nullptr};m_instr[0xEE].execute.byteOperand = &cpu::xor_n;

    m_instr[0xFA] = {"LD A,(nn)",2,16,nullptr};m_instr[0xFA].execute.wordOperand=&cpu::ld_a_pnn;

    m_instr[0xF5] = {"PUSH AF", 0, 16, &cpu::push_af};

    //CB Prefixed
    m_cbInstr[0x10] = {"RL B", 0, 8, &cpu::cb_rl_b};
    m_cbInstr[0x11] = {"RL C", 0, 8, &cpu::cb_rl_c};
    m_cbInstr[0x12] = {"RL D", 0, 8, &cpu::cb_rl_e};
    m_cbInstr[0x13] = {"RL E", 0, 8, &cpu::cb_rl_d};
    m_cbInstr[0x14] = {"RL H", 0, 8, &cpu::cb_rl_h};
    m_cbInstr[0x15] = {"RL L", 0, 8, &cpu::cb_rl_l};
    m_cbInstr[0x16] = {"RL (HL)", 0, 16, &cpu::cb_rl_phl};
    m_cbInstr[0x17] = {"RL A", 0, 8, &cpu::cb_rl_a};

    m_cbInstr[0x40] = {"BIT 0,B",0,8,&cpu::cb_test_0_b};
    m_cbInstr[0x41] = {"BIT 0,C",0,8,&cpu::cb_test_0_c};
    m_cbInstr[0x42] = {"BIT 0,D",0,8,&cpu::cb_test_0_d};
    m_cbInstr[0x43] = {"BIT 0,E",0,8,&cpu::cb_test_0_e};
    m_cbInstr[0x44] = {"BIT 0,H",0,8,&cpu::cb_test_0_h};
    m_cbInstr[0x45] = {"BIT 0,L",0,8,&cpu::cb_test_0_l};
    m_cbInstr[0x46] = {"BIT 0,(HL)",0,12,&cpu::cb_test_0_phl};
    m_cbInstr[0x47] = {"BIT 0,A",0,8,&cpu::cb_test_0_a};
    m_cbInstr[0x48] = {"BIT 1,B",0,8,&cpu::cb_test_1_b};
    m_cbInstr[0x49] = {"BIT 1,C",0,8,&cpu::cb_test_1_c};
    m_cbInstr[0x4A] = {"BIT 1,D",0,8,&cpu::cb_test_1_d};
    m_cbInstr[0x4B] = {"BIT 1,E",0,8,&cpu::cb_test_1_e};
    m_cbInstr[0x4C] = {"BIT 1,H",0,8,&cpu::cb_test_1_h};
    m_cbInstr[0x4D] = {"BIT 1,L",0,8,&cpu::cb_test_1_l};
    m_cbInstr[0x4E] = {"BIT 1,(HL)",0,12,&cpu::cb_test_1_phl};
    m_cbInstr[0x4F] = {"BIT 1,A",0,8,&cpu::cb_test_1_a};
    m_cbInstr[0x50] = {"BIT 2,B",0,8,&cpu::cb_test_2_b};
    m_cbInstr[0x51] = {"BIT 2,C",0,8,&cpu::cb_test_2_c};
    m_cbInstr[0x52] = {"BIT 2,D",0,8,&cpu::cb_test_2_d};
    m_cbInstr[0x53] = {"BIT 2,E",0,8,&cpu::cb_test_2_e};
    m_cbInstr[0x54] = {"BIT 2,H",0,8,&cpu::cb_test_2_h};
    m_cbInstr[0x55] = {"BIT 2,L",0,8,&cpu::cb_test_2_l};
    m_cbInstr[0x56] = {"BIT 2,(HL)",0,12,&cpu::cb_test_2_phl};
    m_cbInstr[0x57] = {"BIT 2,A",0,8,&cpu::cb_test_2_a};
    m_cbInstr[0x58] = {"BIT 3,B",0,8,&cpu::cb_test_3_b};
    m_cbInstr[0x59] = {"BIT 3,C",0,8,&cpu::cb_test_3_c};
    m_cbInstr[0x5A] = {"BIT 3,D",0,8,&cpu::cb_test_3_d};
    m_cbInstr[0x5B] = {"BIT 3,E",0,8,&cpu::cb_test_3_e};
    m_cbInstr[0x5C] = {"BIT 3,H",0,8,&cpu::cb_test_3_h};
    m_cbInstr[0x5D] = {"BIT 3,L",0,8,&cpu::cb_test_3_l};
    m_cbInstr[0x5E] = {"BIT 3,(HL)",0,12,&cpu::cb_test_3_phl};
    m_cbInstr[0x5F] = {"BIT 3,A",0,8,&cpu::cb_test_3_a};
    m_cbInstr[0x60] = {"BIT 4,B",0,8,&cpu::cb_test_4_b};
    m_cbInstr[0x61] = {"BIT 4,C",0,8,&cpu::cb_test_4_c};
    m_cbInstr[0x62] = {"BIT 4,D",0,8,&cpu::cb_test_4_d};
    m_cbInstr[0x63] = {"BIT 4,E",0,8,&cpu::cb_test_4_e};
    m_cbInstr[0x64] = {"BIT 4,H",0,8,&cpu::cb_test_4_h};
    m_cbInstr[0x65] = {"BIT 4,L",0,8,&cpu::cb_test_4_l};
    m_cbInstr[0x66] = {"BIT 4,(HL)",0,12,&cpu::cb_test_4_phl};
    m_cbInstr[0x67] = {"BIT 4,A",0,8,&cpu::cb_test_4_a};
    m_cbInstr[0x68] = {"BIT 5,B",0,8,&cpu::cb_test_5_b};
    m_cbInstr[0x69] = {"BIT 5,C",0,8,&cpu::cb_test_5_c};
    m_cbInstr[0x6A] = {"BIT 5,D",0,8,&cpu::cb_test_5_d};
    m_cbInstr[0x6B] = {"BIT 5,E",0,8,&cpu::cb_test_5_e};
    m_cbInstr[0x6C] = {"BIT 5,H",0,8,&cpu::cb_test_5_h};
    m_cbInstr[0x6D] = {"BIT 5,L",0,8,&cpu::cb_test_5_l};
    m_cbInstr[0x6E] = {"BIT 5,(HL)",0,12,&cpu::cb_test_5_phl};
    m_cbInstr[0x6F] = {"BIT 5,A",0,12,&cpu::cb_test_5_a};
    m_cbInstr[0x70] = {"BIT 6,B",0,8,&cpu::cb_test_6_b};
    m_cbInstr[0x71] = {"BIT 6,C",0,8,&cpu::cb_test_6_c};
    m_cbInstr[0x72] = {"BIT 6,D",0,8,&cpu::cb_test_6_d};
    m_cbInstr[0x73] = {"BIT 6,E",0,8,&cpu::cb_test_6_e};
    m_cbInstr[0x74] = {"BIT 6,H",0,8,&cpu::cb_test_6_h};
    m_cbInstr[0x75] = {"BIT 6,L",0,8,&cpu::cb_test_6_l};
    m_cbInstr[0x76] = {"BIT 6,(HL)",0,12,&cpu::cb_test_6_phl};
    m_cbInstr[0x77] = {"BIT 6,A",0,8,&cpu::cb_test_6_a};
    m_cbInstr[0x78] = {"BIT 7,B",0,8,&cpu::cb_test_7_b};
    m_cbInstr[0x79] = {"BIT 7,C",0,8,&cpu::cb_test_7_c};
    m_cbInstr[0x7A] = {"BIT 7,D",0,8,&cpu::cb_test_7_d};
    m_cbInstr[0x7B] = {"BIT 7,E",0,8,&cpu::cb_test_7_e};
    m_cbInstr[0x7C] = {"BIT 7,H",0,8,&cpu::cb_test_7_h};
    m_cbInstr[0x7D] = {"BIT 7,L",0,8,&cpu::cb_test_7_l};
    m_cbInstr[0x7E] = {"BIT 7,(HL)",0,12,&cpu::cb_test_7_phl};
    m_cbInstr[0x7F] = {"BIT 7,A",0,8,&cpu::cb_test_7_a};

    reset();
    SDL_Log("CPU initialized");
}

cpu::~cpu() {
    SDL_Log("CPU destroyed");
}

void cpu::reset() {

}

void cpu::tick(float deltaTime) {
    const BYTE opCode = p_mmu->readByte(m_reg.pc);
    printDebug(opCode);
    m_reg.pc++;
    dispatch(opCode);

    if (m_reg.pc>0x98) {
        SDL_Log(".");
    }
}

void cpu::dispatch(const BYTE opCode) {
    //TODO Not working
    if (m_instr.find(opCode) == m_instr.end()) {
        SDL_Log("Missing OpCode 0x%02X", opCode);
        return;
    }

    if (m_instr[opCode].execute.noOperand == nullptr) {
        SDL_Log("Incomplete OpCode 0x%02X", opCode);
        return;
    }

    m_reg.pc += m_instr[opCode].operandLength; //Point to next instruction

    switch (m_instr[opCode].operandLength) {
        case 0:
            (this->*m_instr[opCode].execute.noOperand)();
            break;
        case 1:
            (this->*m_instr[opCode].execute.byteOperand)(p_mmu->readByte(m_reg.pc-1));
            break;
        case 2:
            (this->*m_instr[opCode].execute.wordOperand)(p_mmu->readWord(m_reg.pc-2));
            break;
        default:
            SDL_Log("Unknown operand length %d", m_instr[opCode].operandLength);
            break;
    }

        m_cycles += m_instr[opCode].cycles;
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
    m_cycles += m_cbInstr[opCode].cycles;
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

void cpu::doIncrement(BYTE &value) {
    m_reg.halfCarry = (((value & 0xF) + (1 & 0xF)) & 0x10) == 0x10;
    value++;
    //TODO Wrap to zero?
    m_reg.zero = (value == 0);
    m_reg.negative = false;
}

void cpu::doPush(const BYTE &value) {
    m_reg.sp--;
    p_mmu->writeByte(m_reg.sp, value);
}

void cpu::doPushWord(const WORD &value) {
    doPush(value & 0xFF);
    doPush(value >> 8);
}

void cpu::doPop(BYTE &value) {
    value=p_mmu->readByte(m_reg.sp);
    m_reg.sp++;
}

BYTE cpu::doRotateLeft(const BYTE &value) {
    const bool oldCarry = m_reg.carry;
    BYTE work = value;

    m_reg.carry = testBit(7, value);

    work = value << 1;

    if (oldCarry) {
        setBit(0, work);
    } else {
        resetBit(0, work);
    }

    m_reg.zero = work == 0;

    m_reg.negative = false;
    m_reg.halfCarry = false;

    return work;
}


// ReSharper disable once CppMemberFunctionMayBeStatic
// 0x00
void cpu::nop() {};

void cpu::ld_bc_nn(const WORD operand) {
    m_reg.bc = operand;
}

void cpu::ld_pbc_a() {
    p_mmu->writeByte(m_reg.bc, m_reg.a);
}

void cpu::inc_b() {
    doIncrement(m_reg.b);
}

void cpu::ld_b_n(const BYTE operand) {
    m_reg.b = operand;
}

void cpu::ld_a_pbc() {
    m_reg.a = p_mmu->readByte(m_reg.bc);
}

void cpu::inc_c() {
    doIncrement(m_reg.c);
}

void cpu::ld_c_n(const BYTE operand) {
    m_reg.c = operand;
}

void cpu::ld_de_nn(const WORD operand) {
    m_reg.de = operand;
}

void cpu::ld_pde_a() {
    p_mmu->writeByte(m_reg.de, m_reg.a);
}

void cpu::inc_d() {
    doIncrement(m_reg.d);
}

void cpu::ld_d_n(const BYTE operand) {
    m_reg.d = operand;
}

void cpu::ld_a_pde() {
     m_reg.a = p_mmu->readByte(m_reg.de);
}

void cpu::inc_e() {
    doIncrement(m_reg.e);
}

void cpu::ld_e_n(const BYTE operand) {
    m_reg.e = operand;
}

void cpu::jp_nz(const BYTE operand) {
    if (!m_reg.zero) {
        m_reg.pc += unsignedToSigned(operand);
        m_cycles += 4;
    }
}

void cpu::ld_hl_nn(const WORD operand) {
    m_reg.hl = operand;
}

void cpu::inc_h() {
    doIncrement(m_reg.h);
}

void cpu::ld_h_n(const BYTE operand) {
    m_reg.h = operand;
}

void cpu::inc_l() {
    doIncrement(m_reg.l);
}

void cpu::ld_l_n(const BYTE operand) {
    m_reg.l = operand;
}

void cpu::ld_sp_nn(const WORD operand) {
    m_reg.sp = operand;
}

void cpu::ldd_hl_a() {
    p_mmu->writeByte(m_reg.hl, m_reg.a);
    m_reg.hl--;
}

void cpu::inc_phl() {
    NOTYET
}

void cpu::inc_a() {
    doIncrement(m_reg.a);
}

void cpu::ld_a_n(const BYTE operand) {
    m_reg.a = operand;
}

void cpu::ld_b_a() {
    m_reg.b = m_reg.a;
}

void cpu::ld_c_a() {
    m_reg.c = m_reg.a;
}

void cpu::ld_d_a() {
    m_reg.d = m_reg.a;
}

void cpu::ld_e_a() {
    m_reg.e = m_reg.a;
}

void cpu::ld_h_a() {
    m_reg.h = m_reg.a;
}

void cpu::ld_l_a() {
    m_reg.l = m_reg.a;
}

void cpu::ld_phl_a() {
    p_mmu->writeByte(m_reg.hl, m_reg.a);
}

void cpu::ld_a_b() {
    m_reg.a = m_reg.b;
}

void cpu::ld_a_c() {
    m_reg.a = m_reg.c;
}

void cpu::ld_a_d() {
    m_reg.a = m_reg.d;
}

void cpu::ld_a_e() {
    m_reg.a = m_reg.e;
}

void cpu::ld_a_h() {
    m_reg.a = m_reg.h;
}

void cpu::ld_a_l() {
    m_reg.a = m_reg.l;
}

void cpu::ld_a_phl() {
    m_reg.a = p_mmu->readByte(m_reg.hl);
}

void cpu::ld_a_a() {
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

void cpu::xor_phl() {
    doXOR(p_mmu->readByte(m_reg.hl));
}

void cpu::xor_a() {
    doXOR(m_reg.a);
}

void cpu::push_bc() {
    doPushWord(m_reg.bc);
}

void cpu::call_nn(const WORD operand) {
    doPushWord(m_reg.pc);
    m_reg.pc = operand;
}

void cpu::push_de() {
    doPushWord(m_reg.de);
}

void cpu::ldh_n_a(const BYTE operand) {
    p_mmu->writeByte(0xFF00 + operand, m_reg.a);
}

void cpu::ldh_c_a() {
    p_mmu->writeByte(0xFF00 + m_reg.c, m_reg.a);
}

void cpu::push_hl() {
    doPushWord(m_reg.hl);
}

void cpu::ld_nn_a(const WORD operand) {
    p_mmu->writeByte(operand, m_reg.a);
}

void cpu::xor_n(const BYTE operand) {
    doXOR(operand);
}

void cpu::push_af() {
    doPushWord(m_reg.af);
}

void cpu::ld_a_pnn(const WORD operand) {
    m_reg.a = p_mmu->readByte(operand);
}

//CB

void cpu::cb_rl_b() {
    m_reg.b = doRotateLeft(m_reg.b);
}

void cpu::cb_rl_c() {
    m_reg.c = doRotateLeft(m_reg.c);
}

void cpu::cb_rl_d() {
    m_reg.d = doRotateLeft(m_reg.d);
}

void cpu::cb_rl_e() {
    m_reg.e = doRotateLeft(m_reg.e);
}

void cpu::cb_rl_h() {
    m_reg.h = doRotateLeft(m_reg.h);
}

void cpu::cb_rl_l() {
    m_reg.l = doRotateLeft(m_reg.l);
}

void cpu::cb_rl_phl() {
    NOTYET
}

void cpu::cb_rl_a() {
    m_reg.a = doRotateLeft(m_reg.a);
}

void cpu::cb_test_0_b() {
    doBitTest(0, m_reg.b);
}
void cpu::cb_test_0_c() {
    doBitTest(0, m_reg.c);
}
void cpu::cb_test_0_d() {
    doBitTest(0, m_reg.d);
}
void cpu::cb_test_0_e() {
    doBitTest(0, m_reg.e);
}
void cpu::cb_test_0_h() {
    doBitTest(0, m_reg.h);
}
void cpu::cb_test_0_l() {
    doBitTest(0, m_reg.l);
}
void cpu::cb_test_0_phl() {
    doBitTest(0, p_mmu->readByte(m_reg.hl));
}
void cpu::cb_test_0_a() {
    doBitTest(0, m_reg.a);
}
void cpu::cb_test_1_b() {
    doBitTest(1, m_reg.b);
}
void cpu::cb_test_1_c() {
    doBitTest(1, m_reg.c);
}
void cpu::cb_test_1_d() {
    doBitTest(1, m_reg.d);
}
void cpu::cb_test_1_e() {
    doBitTest(1, m_reg.e);
}
void cpu::cb_test_1_h() {
    doBitTest(1, m_reg.h);
}
void cpu::cb_test_1_l() {
    doBitTest(1, m_reg.l);
}
void cpu::cb_test_1_phl() {
    doBitTest(1, p_mmu->readByte(m_reg.hl));
}
void cpu::cb_test_1_a() {
    doBitTest(1, m_reg.a);
}
void cpu::cb_test_2_b() {
    doBitTest(2, m_reg.b);
}
void cpu::cb_test_2_c() {
    doBitTest(2, m_reg.c);
}
void cpu::cb_test_2_d() {
    doBitTest(2, m_reg.d);
}
void cpu::cb_test_2_e() {
    doBitTest(2, m_reg.e);
}
void cpu::cb_test_2_h() {
    doBitTest(2, m_reg.h);
}
void cpu::cb_test_2_l() {
    doBitTest(2, m_reg.l);
}
void cpu::cb_test_2_phl() {
    doBitTest(2, p_mmu->readByte(m_reg.hl));
}
void cpu::cb_test_2_a() {
    doBitTest(2, m_reg.a);
}
void cpu::cb_test_3_b() {
    doBitTest(3, m_reg.b);
}
void cpu::cb_test_3_c() {
    doBitTest(3, m_reg.c);
}
void cpu::cb_test_3_d() {
    doBitTest(3, m_reg.d);
}
void cpu::cb_test_3_e() {
    doBitTest(3, m_reg.e);
}
void cpu::cb_test_3_h() {
    doBitTest(3, m_reg.h);
}
void cpu::cb_test_3_l() {
    doBitTest(3, m_reg.l);
}
void cpu::cb_test_3_phl() {
    doBitTest(3, p_mmu->readByte(m_reg.hl));
}
void cpu::cb_test_3_a() {
    doBitTest(3, m_reg.a);
}
void cpu::cb_test_4_b() {
    doBitTest(4, m_reg.b);
}
void cpu::cb_test_4_c() {
    doBitTest(4, m_reg.c);
}
void cpu::cb_test_4_d() {
    doBitTest(4, m_reg.d);
}
void cpu::cb_test_4_e() {
    doBitTest(4, m_reg.e);
}
void cpu::cb_test_4_h() {
    doBitTest(4, m_reg.h);
}
void cpu::cb_test_4_l() {
    doBitTest(4, m_reg.l);
}
void cpu::cb_test_4_phl() {
    doBitTest(4, p_mmu->readByte(m_reg.hl));
}
void cpu::cb_test_4_a() {
    doBitTest(4, m_reg.a);
}
void cpu::cb_test_5_b() {
    doBitTest(5, m_reg.b);
}
void cpu::cb_test_5_c() {
    doBitTest(5, m_reg.c);
}
void cpu::cb_test_5_d() {
    doBitTest(5, m_reg.d);
}
void cpu::cb_test_5_e() {
    doBitTest(5, m_reg.e);
}
void cpu::cb_test_5_h() {
    doBitTest(5, m_reg.h);
}
void cpu::cb_test_5_l() {
    doBitTest(5, m_reg.l);
}
void cpu::cb_test_5_phl() {
    doBitTest(5, p_mmu->readByte(m_reg.hl));
}
void cpu::cb_test_5_a() {
    doBitTest(5, m_reg.a);
}
void cpu::cb_test_6_b() {
    doBitTest(6, m_reg.b);
}
void cpu::cb_test_6_c() {
    doBitTest(6, m_reg.c);
}
void cpu::cb_test_6_d() {
    doBitTest(6, m_reg.d);
}
void cpu::cb_test_6_e() {
    doBitTest(6, m_reg.e);
}
void cpu::cb_test_6_h() {
    doBitTest(6, m_reg.h);
}
void cpu::cb_test_6_l() {
    doBitTest(6, m_reg.l);
}
void cpu::cb_test_6_phl() {
    doBitTest(6, p_mmu->readByte(m_reg.hl));
}
void cpu::cb_test_6_a() {
    doBitTest(6, m_reg.a);
}
void cpu::cb_test_7_b() {
    doBitTest(7, m_reg.b);
}
void cpu::cb_test_7_c() {
    doBitTest(7, m_reg.c);
}
void cpu::cb_test_7_d() {
    doBitTest(7, m_reg.d);
}
void cpu::cb_test_7_e() {
    doBitTest(7, m_reg.e);
}
void cpu::cb_test_7_h() {
    doBitTest(7, m_reg.h);
}
void cpu::cb_test_7_l() {
    doBitTest(7, m_reg.l);
}
void cpu::cb_test_7_phl() {
    doBitTest(7, p_mmu->readByte(m_reg.hl));
}
void cpu::cb_test_7_a() {
    doBitTest(7, m_reg.a);
}

void cpu::printDebug(const BYTE opCode) {
    SDL_Log("0x%04X : [0x%02X] %s",m_reg.pc, opCode, m_instr[opCode].disassembly.c_str());
}
