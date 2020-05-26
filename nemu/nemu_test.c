#include <stdint.h>
#include <stdio.h>
typedef struct {
  struct {
	union{
		uint32_t _32;
		uint16_t _16;
		uint8_t _8[2];
	};

  } gpr[8];

  /* Do NOT change the order of the GPRs' definitions. */

  /* In NEMU, rtlreg_t is exactly uint32_t. This makes RTL instructions
   * in PA2 able to directly access these registers.
   */


} CPU_state;
int main(){
	CPU_state cpu;
	cpu.gpr[0]._32 = 0x87654321;
	printf("%x\n", cpu.gpr[0]._16);
	printf("%x\n", cpu.gpr[0]._8[0]);
	printf("%x\n", cpu.gpr[0]._8[1]);
	printf("%x\n", cpu.gpr[0]._32);
	return 0;
}
