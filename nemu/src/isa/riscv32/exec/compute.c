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
