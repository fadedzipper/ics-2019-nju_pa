#include <dlfcn.h>

#include "nemu.h"
#include "monitor/monitor.h"

void (*ref_difftest_memcpy_from_dut)(paddr_t dest, void *src, size_t n) = NULL;
void (*ref_difftest_getregs)(void *c) = NULL;
void (*ref_difftest_setregs)(const void *c) = NULL;
void (*ref_difftest_exec)(uint64_t n) = NULL;

static bool is_skip_ref = false;
static int skip_dut_nr_instr = 0;
static bool is_detach = false;

void qemu_reg_display(CPU_state *ref_r){
	extern const char *regsl[];
	printf("\nqemu registers \n");
	for(int i = 0; i < 32; i++){
		if(i % 4 == 0) printf("\n");
		printf("%5s -> %08x", regsl[i], ref_r->gpr[i]._32);
	}
	printf("\nqemu pc -> %08x\n", ref_r->pc);
}

void exec_instrs_abort_in_pc(CPU_state *ref, vaddr_t ori_pc){
    extern void isa_reg_display(void);
    isa_reg_display();
	qemu_reg_display(ref);

    nemu_state.state = NEMU_ABORT;
    nemu_state.halt_pc = ori_pc;
}

// this is used to let ref skip instructions which
// can not produce consistent behavior with NEMU
void difftest_skip_ref() {
  is_skip_ref = true;
  // If such an instruction is one of the instruction packing in QEMU
  // (see below), we end the process of catching up with QEMU's pc to
  // keep the consistent behavior in our best.
  // Note that this is still not perfect: if the packed instructions
  // already write some memory, and the incoming instruction in NEMU
  // will load that memory, we will encounter false negative. But such
  // situation is infrequent.
  skip_dut_nr_instr = 0;
}

// this is used to deal with instruction packing in QEMU.
// Sometimes letting QEMU step once will execute multiple instructions.
// We should skip checking until NEMU's pc catches up with QEMU's pc.
// The semantic is
//   Let REF run `nr_ref` instructions first.
//   We expect that DUT will catch up with REF within `nr_dut` instructions.
void difftest_skip_dut(int nr_ref, int nr_dut) {
  skip_dut_nr_instr += nr_dut;

  while (nr_ref-- > 0) {
	printf("ref exec 1 step %s %d\n", __FILE__, __LINE__);
    ref_difftest_exec(1);
  }
}

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc);
void isa_difftest_attach();

void difftest_attach() {
#ifndef DIFF_TEST
  return;
#endif

  is_detach = false;
  is_skip_ref = false;
  skip_dut_nr_instr = 0;

  isa_difftest_attach();
}

void init_difftest(char *ref_so_file, long img_size) {
#ifndef DIFF_TEST
  return;
#endif

  assert(ref_so_file != NULL);

  void *handle;
  handle = dlopen(ref_so_file, RTLD_LAZY | RTLD_DEEPBIND);
  assert(handle);

  ref_difftest_memcpy_from_dut = dlsym(handle, "difftest_memcpy_from_dut");
  assert(ref_difftest_memcpy_from_dut);

  ref_difftest_getregs = dlsym(handle, "difftest_getregs");
  assert(ref_difftest_getregs);

  ref_difftest_setregs = dlsym(handle, "difftest_setregs");
  assert(ref_difftest_setregs);

  ref_difftest_exec = dlsym(handle, "difftest_exec");
  assert(ref_difftest_exec);

  void (*ref_difftest_init)(void) = dlsym(handle, "difftest_init");
  assert(ref_difftest_init);

  Log("Differential testing: \33[1;32m%s\33[0m", "ON");
  Log("The result of every instruction will be compared with %s. "
      "This will help you a lot for debugging, but also significantly reduce the performance. "
      "If it is not necessary, you can turn it off in include/common.h.", ref_so_file);

  ref_difftest_init();
  ref_difftest_memcpy_from_dut(PC_START, guest_to_host(IMAGE_START), img_size);
  char *mainargs = guest_to_host(0);
  ref_difftest_memcpy_from_dut(PC_START - IMAGE_START, mainargs, strlen(mainargs) + 1);
  ref_difftest_setregs(&cpu);
}

static void checkregs(CPU_state *ref, vaddr_t pc) {
  if (!isa_difftest_checkregs(ref, pc)) {
    extern void isa_reg_display(void);
    isa_reg_display();
	printf("pc compared to qemu -> %08x\n", pc);
	qemu_reg_display(ref);

    nemu_state.state = NEMU_ABORT;
    nemu_state.halt_pc = pc;
  }
}

void difftest_step(vaddr_t ori_pc, vaddr_t next_pc) {

	/* printf("get into defftest_step\n"); */

	CPU_state ref_r;

 	/* printf("is_detach = %d in ori_pc = %08x\n", is_detach, ori_pc); */
 	/* printf("skip_dut_nr_instr = %d in ori_pc = %08x\n", skip_dut_nr_instr, ori_pc); */
 	/* printf("is_skip_ref = %d in ori_pc = %08x\n", is_skip_ref, ori_pc); */

	// jalr dont have any abnormal action ???
 	/* if(skip_dut_nr_instr == 0) */
 	/*   difftest_attach(); */

 	if (is_detach) return;

 	if (skip_dut_nr_instr > 0) {
 	  ref_difftest_getregs(&ref_r);
 	  if (ref_r.pc == next_pc) {

		/* printf("get into checkregs %s :%s in %d line\n", __FILE__, __func__, __LINE__); */

 	    checkregs(&ref_r, next_pc);
 	    skip_dut_nr_instr = 0;
 	    return;
 	  }
 	  skip_dut_nr_instr --;
 	  if (skip_dut_nr_instr == 0)
 	    panic("can not catch up with ref.pc = %x at pc = %x", ref_r.pc, ori_pc);
 	  return;
 	}
 	/* printf("1\n"); */

 	if (is_skip_ref) {
 	  // to skip the checking of an instruction, just copy the reg state to reference design
 	  ref_difftest_setregs(&cpu);
 	  is_skip_ref = false;
 	  return;
 	}
	/* printf("2\n"); */

 	ref_difftest_exec(1);
 	ref_difftest_getregs(&ref_r);

	/* printf("get into checkregs %s :%s in %d line\n", __FILE__, __func__, __LINE__); */

 	checkregs(&ref_r, next_pc);

	printf("test in pc %08x pass, now riscv32_pc %08x, qemu_pc %08x\n", \
			ori_pc, cpu.pc, ref_r.pc);
	vaddr_t abort_pc = 0x80100080;
	if(ori_pc == abort_pc){
		exec_instrs_abort_in_pc(&ref_r, abort_pc);
	}
}

void difftest_detach() {
  is_detach = true;
}

