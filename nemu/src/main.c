#include <stdio.h>
#include "nemu.h"
#include <stddef.h>
#include <errno.h>
int init_monitor(int, char *[]);
void ui_mainloop(int);


int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
//    int is_batch_mode = init_monitor(argc, argv);
//
//  /* Receive commands from user. */
//	ui_mainloop(is_batch_mode);
	void init_regex();
	uint32_t expr(char *, bool *);

	init_regex();
	bool success = true;

	FILE *fp = fopen("/home/mff/ics2019/nemu/tools/gen-expr/input", "r");
	assert(fp != NULL);
	unsigned int result = 0;
	char inputexprbuf[65536];
	int ret = 0;
	int line = 1;

	while((ret = fscanf(fp, "%u %s", &result, inputexprbuf)) != EOF){
		if(result == expr(inputexprbuf, &success)){
			printf("success\n");
			memset(inputexprbuf, 0, sizeof(inputexprbuf));
		}
		else{
			printf("failed in %d line\n", line);
			break;
		}
		line++;
	}
	fclose(fp);

  return 0;
}
