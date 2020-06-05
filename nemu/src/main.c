#include <stdio.h>
#include "nemu.h"
#include <stddef.h>
int init_monitor(int, char *[]);
void ui_mainloop(int);
uint32_t expr(char *e, bool *success);


int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
//  int is_batch_mode = init_monitor(argc, argv);

  /* Receive commands from user. */
 // ui_mainloop(is_batch_mode);
	bool success = true;
	FILE *fp = NULL;
	fp = fopen("../tools/gen-expr/input", "r");
	assert(fp != NULL);
	unsigned int result = 0;
	char inputexprbuf[65536];
	int ret = 0;
	int line = 0;
	while((ret = fscanf(fp, "%u %s", &result, inputexprbuf) != EOF)){
		if(result == expr(inputexprbuf, &success)){
			printf("success\n");
		}
		else{
			printf("failed in %d line\n", ++line);
		}
	}

  return 0;
}
