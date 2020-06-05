#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

static char buf[65536];
int choose(int n){
	int r = rand();
	return r%n;
}

static char* gen_num(){
	char* str = (char *)malloc(35);
	assert(str != NULL);
	int num = choose(100);
	sprintf(str, "%d", num);
	assert(strlen(str) < 35);
	return str;
}

static char* gen(const char* x){
	char* str = (char*)malloc(5);
	assert(str != NULL);
	strcpy(str, x);
	assert(strlen(str) < 5);
	return str;
}

static char* gen_rand_op(){
	int x = choose(4);
	char* str = (char*)malloc(5);
	assert(str != NULL);
	switch (x) {
		case 0 : strcpy(str, "+");
				 break;
		case 1 : strcpy(str, "-");
				 break;
		case 2 : strcpy(str, "*");
				 break;
		case 3 : strcpy(str, "/");
				 break;
	}
	assert(strlen(str) < 2);
	return str;
}

static char* gen_rand_expr() {
	char* str = (char *)malloc(sizeof(65536));
	assert(str != NULL);
	switch (choose(3)) {
   		case 0: {
			char* case1 = gen_num(); 
			strcpy(str, case1);
			free(case1);
			assert(strlen(str)< 65536);
			break;
				}
    	case 1: {
			char* case2 = gen("("); 
			char* case3 = gen_rand_expr(); 
			char* case4 = gen(")"); 
			int len = strlen(case3);
			if(len + 5 > 65536){
				return case3;
			}
			sprintf(str, "%s", case2);
			sprintf(str+1, "%s", case3);
			sprintf(str+1+len, "%s", case4);
			assert(strlen(str) < 65536);
			break;
				}
    	default: {
			char* case5 = gen_rand_expr(); 
			char* case6 = gen_rand_op(); 
			char* case7 = gen_rand_expr();
			int len5 = strlen(case5);
			int len7 = strlen(case7);
			if(len5 + len7 +5> 65536){
				return case5;
			}
			sprintf(str, "%s", case5);
			sprintf(str+len5, "%s", case6);
			sprintf(str+len5+1, "%s", case7);
			assert(strlen(str) < 65536);
			break;
				 }
 	 }
	return str;
}

int main(void){
	int seed = time(0);
	srand(seed);
	char *str = strdup("i");

	return 0;
}
