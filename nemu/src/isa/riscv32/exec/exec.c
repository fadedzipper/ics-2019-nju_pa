#include "cpu/exec.h"
#include "all-instr.h"

/* load instructions exeute means */
static OpcodeEntry load_table [8] = {
  EXW(lb, 1), EXW(lh, 2), EXW(ld, 4), EMPTY, EXW(ld, 1), EXW(ld, 2), EMPTY, EMPTY
};

static make_EHelper(load) {
  decinfo.width = load_table[decinfo.isa.instr.funct3].width;
  idex(pc, &load_table[decinfo.isa.instr.funct3]);
}

/* store instructions exeute means */
static OpcodeEntry store_table [8] = {
  EXW(st, 1), EXW(st, 2), EXW(st, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(store) {
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  idex(pc, &store_table[decinfo.isa.instr.funct3]);
}

/* I-type serial instructions exeute means */
static OpcodeEntry imm_serial_SRLI_table[2] = {
	EXW(srli, 4), EXW(srai, 4)
};

static make_EHelper(imm_serial_SRLI){
	decinfo.width = imm_serial_SRLI_table[GETBIT30(decinfo.isa.instr.val)].width;
	idex(pc, &imm_serial_SRLI_table[GETBIT30(decinfo.isa.instr.val)]);
}

static OpcodeEntry imm_serial_table[8] = {
	EXW(addi, 4), EXW(slli, 4), EXW(slti, 4), EXW(sltiu, 4), EXW(xori, 4), EXW(imm_serial_SRLI, 4), EXW(ori, 4), EXW(andi, 4) 
};

static make_EHelper(imm_serial){
	decinfo.width = imm_serial_table[decinfo.isa.instr.funct3].width;
	idex(pc, &imm_serial_table[decinfo.isa.instr.funct3]);
}

/* R-type serial instructions exeute means */
static OpcodeEntry reg_serial_ADD_table [2] = {
	EXW(and, 4), EXW(sub, 4)
};

static make_EHelper(reg_serial_ADD){
	decinfo.width = reg_serial_ADD_table[GETBIT30(decinfo.isa.instr.val)].width;
	idex(pc, &reg_serial_ADD_table[GETBIT30(decinfo.isa.instr.val)]);
}

static OpcodeEntry reg_serial_SRL_table [2] = {
	EXW(srl, 4), EXW(sra, 4)
};

static make_EHelper(reg_serial_SRL){
	decinfo.width = reg_serial_SRL_table[GETBIT30(decinfo.isa.instr.val)].width;
	idex(pc, &reg_serial_SRL_table[GETBIT30(decinfo.isa.instr.val)]);
}

static OpcodeEntry reg_serial_table [8] = {
	EXW(reg_serial_ADD, 4), EXW(sll, 4), EXW(slt, 4), EXW(sltu, 4), EXW(xor, 4), EXW(reg_serial_SRL, 4), EXW(or, 4), EXW(and, 4)
};

static make_EHelper(reg_serial){
	decinfo.width = reg_serial_table[decinfo.isa.instr.funct3].width;
	idex(pc, &reg_serial_table[decinfo.isa.instr.funct3]);
}

/* B-type serial instrucions exeute means */
static OpcodeEntry B_serial_table [8] = {
	EXW(beq, 4), EXW(bne, 4), EMPTY, EMPTY, EXW(blt, 4), EXW(bge, 4), EXW(bltu, 4), EXW(bgeu, 4)
};

static make_EHelper(B_serial){
	decinfo.width = B_serial_table[decinfo.isa.instr.funct3].width;
	idex(pc, &B_serial_table[decinfo.isa.instr.funct3]);
}

/* total instructions find exeute function */
static OpcodeEntry opcode_table [32] = {
  /* b00 */ IDEX(ld, load), EMPTY, EMPTY, EMPTY, IDEX(I, imm_serial), IDEX(U, auipc), EMPTY, EMPTY,
  /* b01 */ IDEX(st, store), EMPTY, EMPTY, EMPTY, IDEX(R, reg_serial), IDEX(U, lui), EMPTY, EMPTY,
  /* b10 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b11 */ IDEX(B, B_serial), IDEX(I, jalr), EX(nemu_trap), IDEX(J, jal), EMPTY, EMPTY, EMPTY, EMPTY,
};

void isa_exec(vaddr_t *pc) {
  decinfo.isa.instr.val = instr_fetch(pc, 4);
  assert(decinfo.isa.instr.opcode1_0 == 0x3);
  idex(pc, &opcode_table[decinfo.isa.instr.opcode6_2]);
}
