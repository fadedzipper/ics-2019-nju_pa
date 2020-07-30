#include "cpu/exec.h"

make_EHelper(jal){
	rtl_li(&s0, cpu.pc + 4);

	rtl_sr(id_dest->reg, &s0, 4);

	rtl_mv(&s1, &id_src->val);

	rtl_shli(&s1, &s1, 1);

	rtl_j(s1 + cpu.pc);

	print_asm_template2(jal);
}

make_EHelper(jalr){
	rtl_li(&s1, cpu.pc + 4);

	rtl_sr(id_dest->reg, &s1, 4);

	rtl_add(&s0, &id_src->val, &id_src2->val);

	rtl_li(&s1, ~1u);

	rtl_and(&s0, &s0, &s1);

	rtl_jr(&s0);

	print_asm_template2(jalr);
}
