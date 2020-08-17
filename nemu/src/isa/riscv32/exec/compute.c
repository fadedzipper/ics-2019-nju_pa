#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

make_EHelper(auipc){
	rtl_add(&id_src->val, &id_src->val, &cpu.pc);
	rtl_sr(id_dest->reg, &id_src->val, 4);

	print_asm_template2(auipc);
}

make_EHelper(addi){
	rtl_add(&id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);

	print_asm_template3(addi);
}


make_EHelper(slti){
	rtl_setrelop(RELOP_LT, &id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);

	print_asm_template2(slti);
}

make_EHelper(sltiu){
	rtl_setrelop(RELOP_LTU, &id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);

	print_asm_template2(sltiu);
}

make_EHelper(xori){
	rtl_xor(&id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);
	
	print_asm_template2(xori);
}

make_EHelper(ori){
	rtl_or(&id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);
	print_asm_template2(ori);
}

make_EHelper(andi){
	rtl_and(&id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);
	print_asm_template2(andi);
}

make_EHelper(slli){
	rtl_li(&s0, 31);  //  31 = 11111b
	rtl_and(&id_src2->val, &id_src2->val, &s0);

	rtl_shl(&id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);
	print_asm_template2(slli);
}

make_EHelper(srli){
	rtl_li(&s0, 31);  //  31 = 11111b
	rtl_and(&id_src2->val, &id_src2->val, &s0);

	rtl_shr(&id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);
	print_asm_template2(srli);
}

make_EHelper(srai){
	rtl_li(&s0, 31);  //  31 = 11111b
	rtl_and(&id_src2->val, &id_src2->val, &s0);

	rtl_sar(&id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);
	print_asm_template2(srai);
}

make_EHelper(add){

	rtl_add(&id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);

	print_asm_template3(add);

}
make_EHelper(sub){
	rtl_sub(&id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);

	print_asm_template3(sub);
}

make_EHelper(sll){
	rtl_shl(&id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);

	print_asm_template3(sll);
}

make_EHelper(slt){
	rtl_setrelop(RELOP_LT, &id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);

	print_asm_template3(slt);
}

make_EHelper(sltu){
	rtl_setrelop(RELOP_LTU, &id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);

	print_asm_template3(sltu);
	
}

make_EHelper(xor){
	rtl_xor(&id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);
	
	print_asm_template3(xor);
	
}

make_EHelper(srl){
	rtl_shr(&id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);

	print_asm_template3(srl);
	
}

make_EHelper(sra){
	rtl_sar(&id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);

	print_asm_template3(sra);
}

make_EHelper(or){
	rtl_or(&id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);

	print_asm_template3(or);
	
}

make_EHelper(and){
	rtl_and(&id_dest->val, &id_src->val, &id_src2->val);
	rtl_sr(id_dest->reg, &id_dest->val, 4);

	print_asm_template3(and);
}
