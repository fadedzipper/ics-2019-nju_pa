#include "nemu.h"
#include <stdio.h>
#include <string.h>

const char *regsl[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

extern CPU_state cpu;
void isa_reg_display() {
	printf("\nriscv32 registers\n");
	for(int i = 0; i < 32; i++){
		if(i % 4 == 0) printf("\n");
		printf("%5s -> %08x", regsl[i], cpu.gpr[i]._32);
	}
	printf("pc -> %08x\n", cpu.pc);
}

uint32_t isa_reg_str2val(const char *s, bool *success) {
	if(s[1] <= '9' && s[1] >= '0'){
		int x = 0;
		sscanf(s+1, "%d", &x);
		if(x < 0 || x > 31){
			*success = false;
			return 0;
		}
		return cpu.gpr[x]._32;

	}
	else{
		int i;
		for(i = 0; i < 32; i++){
			if(strcmp(regsl[i], s+1) == 0){
				break;
			}
		}
		if(i == 32){
			*success = false;
			return 0;
		}
		return cpu.gpr[i]._32;
	}
	return 0;
}
