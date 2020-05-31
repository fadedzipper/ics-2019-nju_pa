#include <assert.h>
#include <string.h>
#include <stdio.h>
int main(void){
	char args[20] = "10 0x1234";
	char* x = strtok(args, " ");
	assert(x != NULL);
	char* addr = x + strlen(x) + 1;
    assert(addr > args);
	printf("%s \n%s\n", x, args);
	int startaddr, cnt;
	sscanf(addr, "%x", &startaddr);
	sscanf(x, "%d", &cnt);
	printf("%d, %x\n", cnt, startaddr);

	return 0;
}
