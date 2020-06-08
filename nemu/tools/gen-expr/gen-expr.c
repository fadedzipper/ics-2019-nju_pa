#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>

// this should be enough
static char buf[65536];
int choose(int n)
{
    int r = rand();
    return r % n;
}

static char *gen_num()
{
    char *str = malloc(40);
    int num = choose(0x3fffffff);
    sprintf(str, "%du", num);
    return str;
}

static char *gen_rand_op()
{
    int x = choose(4);
    char *str = NULL;
    switch (x) {
    case 0:
	str = strdup("+");
	break;
    case 1:
	str = strdup("-");
	break;
    case 2:
	str = strdup("*");
	break;
    case 3:
	str = strdup("/");
	break;
    }
    return str;
}

static char *insert_blank()
{
    int x = choose(10) % 2;
    char *str = NULL;
    str = (x == 0) ? strdup(" ") : strdup("");
    return str;
}

static char *gen_rand_expr()
{
    char *str = malloc(65536);
    memset(str, 0, 65536);
    switch (choose(3)) {
    case 0:{
	    char *case1 = gen_num();
	    strcat(str, case1);
	    free(case1);
	    break;
	}
    case 1:{
	    char *case2 = strdup("(");
	    char *case3 = gen_rand_expr();
	    char *case4 = strdup(")");
	    int len = strlen(case3);
	    if (len + 5 > 65536) {
		free(case2);
		free(case4);
		free(str);
		return case3;
	    }
	    char *blank = insert_blank();

	    strcat(str, case2);
	    strcat(str, blank);
	    strcat(str, case3);
	    strcat(str, blank);
	    strcat(str, case4);
	    strcat(str, blank);

	    free(case2);
	    free(case3);
	    free(case4);
	    free(blank);
	    break;
	}
    default:{
	    char *case5 = gen_rand_expr();
	    char *case6 = gen_rand_op();
	    char *case7 = gen_rand_expr();
	    int len5 = strlen(case5);
	    int len7 = strlen(case7);
	    if (len5 + len7 + 40 > 65536) {
		free(str);
		free(case6);
		free(case7);
		return case5;
	    }
	    char *blank = insert_blank();

	    strcat(str, case5);
	    strcat(str, blank);
	    strcat(str, case6);
	    strcat(str, blank);
	    strcat(str, "((");
	    strcat(str, case7);
	    strcat(str, ") / 1000000u + 1u)");
	    strcat(str, blank);

	    free(case5);
	    free(case6);
	    free(case7);
	    free(blank);
	    break;
	}
    }
    return str;
}

static char code_buf[65536];
static char *code_format =
    "#include <stdio.h>\n"
    "int main() { "
    "  unsigned result = %s; "
    "  printf(\"%%u\", result); " "  return 0; " "}";

int main(int argc, char *argv[])
{
    int seed = time(0);
    srand(seed);
    int loop = 1;
    if (argc > 1) {
	sscanf(argv[1], "%d", &loop);
    }
    int i;
    for (i = 0; i < loop; i++) {
	char *str = gen_rand_expr();
	strcat(buf, str);

	sprintf(code_buf, code_format, buf);

	FILE *fp = fopen("/tmp/.code.c", "w");
	assert(fp != NULL);
	fputs(code_buf, fp);
	fclose(fp);

	int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
	if (ret != 0)
	    continue;

	fp = popen("/tmp/.expr", "r");
	assert(fp != NULL);

	int result;
	int x = fscanf(fp, "%d", &result);
	assert(x == 1);
	pclose(fp);

	printf("%u %s\n", result, buf);
	memset(buf, 0, sizeof(buf));
	free(str);
    }
    return 0;
}
