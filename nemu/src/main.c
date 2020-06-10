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
	size_t len = 0;
	ssize_t read;
	char *line = NULL;
	int fileline = 1;

	while((read = getline(&line, &len, fp)) != EOF){
		assert(strlen(line) < 65536);
		strcat(inputexprbuf, line);
		char x[20];
		sscanf(inputexprbuf, "%s", x);
		int x_len = strlen(x);
		sscanf(inputexprbuf, "%u", &result);
		if(result == expr(line + x_len+1, &success)){
			printf("success\n");
			memset(inputexprbuf, 0, sizeof(inputexprbuf));
		}
		else{
			printf("error occurs in %d\n", fileline);
			break;
		}
		fileline++;
	}
	fclose(fp);

  return 0;
}
