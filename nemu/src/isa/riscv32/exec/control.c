#include "cpu/exec.h"

make_EHelper(jal){
	rtl_li(&s0, cpu.pc + 4);

	rtl_sr(id_dest->reg, &s0, 4);

	rtl_j(id_src->val + cpu.pc);
}

make_EHelper(jalr){
	rtl_li(&s1, cpu.pc + 4);

	rtl_sr(id_dest->reg, &s1, 4);

	rtl_add(&s0, &id_src->val, &id_src2->val);

	rtl_jr(&s0);
}
