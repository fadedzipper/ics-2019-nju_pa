#include "nemu.h"
#include "monitor/diff-test.h"
#include "cpu/decode.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
	int i;
	for(i = 0; i < 32; i++){
		if(ref_r->gpr[i]._32 != cpu.gpr[i]._32){
			return false;
		}
	}
	if(ref_r->pc != pc){
		return false;
	}
	return true;
}

void isa_difftest_attach(void) {
	switch(decinfo.isa.instr.pad3){ 
		case 0x67 : // jalr need DUT (1,2), ref exec 1 step.dut exec 2 step.
			difftest_skip_dut(1, 2);
			break;

		case 0x6b : // nemu_trap
			difftest_skip_ref();
			break;

		default :
			break;
	}
	
}
