#include "cpu/exec.h"

/* unconditional jump instrution */
make_EHelper(jal){
	rtl_li(&s0, cpu.pc + 4);

	rtl_sr(id_dest->reg, &s0, 4);

	rtl_mv(&s1, &id_src->val);

	/* this operation could also be done in decode term just as same as B-type instrution*/
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

/* conditional branch instructions */
make_EHelper(beq){
	rtl_li(&s1, cpu.pc);
	rtl_add(&s0, &s1, &id_dest->val);
	rtl_jrelop(RELOP_EQ, &id_src->val, &id_src2->val, s0);
	
	print_asm_template3(beq);
}

make_EHelper(bne){
	rtl_li(&s1, cpu.pc);
	rtl_add(&s0, &s1, &id_dest->val);
	rtl_jrelop(RELOP_NE, &id_src->val, &id_src2->val, s0);
	
	print_asm_template3(bne);
	
}

make_EHelper(blt){
	rtl_li(&s1, cpu.pc);
	rtl_add(&s0, &s1, &id_dest->val);
	rtl_jrelop(RELOP_LT, &id_src->val, &id_src2->val, s0);
	
	print_asm_template3(blt);
}

make_EHelper(bge){
	rtl_li(&s1, cpu.pc);
	rtl_add(&s0, &s1, &id_dest->val);
	rtl_jrelop(RELOP_GE, &id_src->val, &id_src2->val, s0);
	
	print_asm_template3(bge);
	
}

make_EHelper(bltu){
	rtl_li(&s1, cpu.pc);
	rtl_add(&s0, &s1, &id_dest->val);
	rtl_jrelop(RELOP_LTU, &id_src->val, &id_src2->val, s0);
	
	print_asm_template3(bltu);
	
}

make_EHelper(bgeu){
	rtl_li(&s1, cpu.pc);
	rtl_add(&s0, &s1, &id_dest->val);
	rtl_jrelop(RELOP_GEU, &id_src->val, &id_src2->val, s0);
	
	print_asm_template3(bgeu);
	
}
