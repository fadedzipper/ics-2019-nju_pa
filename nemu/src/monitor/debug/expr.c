#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

enum {
    TK_NOTYPE = 256,
    TK_EQ,
    TK_ADD,
    TK_LP,
    TK_RP,
    TK_MINUS,
    TK_STAR,
    TK_DIV,
    TK_INT,
	TK_DEREF,
	TK_HEXINT,
	TK_REGISTER_VALUE,
	TK_NONEQ,
	TK_AND,
	TK_PREFIXMINUS
	/* TODO: Add more token types */
};

static struct rule {
    char *regex;
    int token_type;
} rules[] = {

    /* TODO: Add more rules.
     * Pay attention to the precedence level of different rules.
	 * go down with lower priority
     */

    { " +", TK_NOTYPE},		// spaces
    { "\\+", TK_ADD},		// plus
    { "==", TK_EQ},		// equal
    { "\\(", TK_LP}, 
	{ "\\)", TK_RP}, 
	{ "-", TK_MINUS}, 
	{ "\\*", TK_STAR}, 
	{ "/", TK_DIV}, 
	{ "u", TK_NOTYPE},
	{ "\n", TK_NOTYPE},
	{ "!=", TK_NONEQ},
	{ "0x[0-9|a-z]+", TK_HEXINT},
	{ "[0-9]+", TK_INT},	// no (8 to 1) and this line must be down compared to TK_HEXINT
	{ "\\$[0-9|a-z]+", TK_REGISTER_VALUE},
	{ "&&", TK_AND}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
    int i;
    char error_msg[128];
    int ret;

    for (i = 0; i < NR_REGEX; i++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0) {
		    regerror(ret, &re[i], error_msg, 128);
		    panic("regex compilation failed: %s\n%s", error_msg,
			  rules[i].regex);
		}
    }
}

typedef struct token {
    int type;
    char str[35];
} Token;

static Token tokens[120] __attribute__ ((used)) = {};

static int nr_token __attribute__ ((used)) = 0;

static bool make_token(char *e){
    int position = 0;
    int i;
    regmatch_t pmatch;

    nr_token = 0;

    while (e[position] != '\0') {
	/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++) {
		    if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0
				&& pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
	
		//		Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;
	
				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */
				switch (rules[i].token_type) {
				case TK_INT:
				    tokens[nr_token].type = TK_INT;
					assert(substr_len < 33);
					strncpy(tokens[nr_token].str, substr_start, substr_len);
					tokens[nr_token].str[substr_len] = '\0';
					nr_token++;
				    break;

				case TK_HEXINT:
					tokens[nr_token].type = TK_HEXINT;
					assert(substr_len < 11);
					strncpy(tokens[nr_token].str, substr_start, substr_len);
					tokens[nr_token].str[substr_len] = '\0';
					nr_token++;
					break;

				case TK_REGISTER_VALUE:
					tokens[nr_token].type = TK_REGISTER_VALUE;
					assert(substr_len < 35);
					strncpy(tokens[nr_token].str, substr_start, substr_len);
					tokens[nr_token].str[substr_len] = '\0';
					nr_token++;
	
				case TK_NOTYPE:
					break;

				default:
				    tokens[nr_token].type = rules[i].token_type;
					nr_token++;
				    break;
				}
	
				break;
		    }
		}
	
		if (i == NR_REGEX) {
		    printf("no match at position %d\n%s\n%*.s^\n", position, e,
			   position, "");
		    return false;
		}
    }

    return true;
}

bool check_parentheses(int p, int q, bool * success)
{
    int flag = 0;
    int i;
    for (i = p; i <= q; i++) {
		if (tokens[i].type == TK_LP) {
		    flag++;
		}
		if (tokens[i].type == TK_RP) {
		    flag--;
		}
		if (flag < 0) {
		    *success = false;
		    return false;
		}
    }
    if (flag != 0) {
		*success = false;
		return false;
    }
    flag = 0;
    bool istrue = true;
    for (i = p; i <= q; i++) {
		if (tokens[i].type == TK_LP) {
		    flag++;
		}
		if (tokens[i].type == TK_RP) {
		    flag--;
		}
		if (i != q && flag == 0) {	//if contains '(' and ')' in p and q perspectively ,flag > 0
		    istrue = false;
		    break;
		}
    }
    if (istrue == true) {
		return true;
    }
	else {
		return false;
    }
}

int find_maincalsymbol(int p, int q)
{
    int STAR_levelpos = -1;	//high priority symbol
    int ADD_levelpos = -1;	//low priority symbol
	int EQ_levelpos = -1;
	int AND_levelpos = -1;

    int i, inLP = 0;
    for (i = p; i <= q; i++) {
		if (tokens[i].type == TK_LP) {
		    inLP++;
		}
		if (tokens[i].type == TK_RP) {
		    inLP--;
		}
		if (inLP > 0) {
		    continue;
		}
	
		/* go down with priority less*/
		if (tokens[i].type == TK_STAR || tokens[i].type == TK_DIV) {
		    STAR_levelpos = i;
		}
		if (tokens[i].type == TK_ADD || tokens[i].type == TK_MINUS) {
		    ADD_levelpos = i;
		}
		if (tokens[i].type == TK_NONEQ || tokens[i].type == TK_EQ){
			EQ_levelpos = i;
		}
		if (tokens[i].type == TK_AND){
			AND_levelpos = i;
		}
    }
	
	/* the if outside has higher priority to select ,and we need
	 * to find the lowest priority in whole expression according
	 * with reducity process*/
	uint32_t result = 0;
	if(AND_levelpos == -1){
		if(EQ_levelpos == -1){
			if(ADD_levelpos == -1){
				if(STAR_levelpos == -1){
					result = -1;
				}
				else{
					result = STAR_levelpos;
				}
			}
			else{
				result = ADD_levelpos;
			}
		}
		else{
			result = EQ_levelpos;
		}
	}
	else{
		result = AND_levelpos;
	}

	assert(result != -1);
	return result;
}


uint32_t eval(int p, int q, bool * success)
{
    if (p > q) {
		/* Bad expression */
		*success = false;
		return 0;
    }
	else if (p == q) {
		/* Single token.
		 * For now this token should be a number.
		 * Return the value of the number.
		 */
		uint32_t val = 0;
		switch (tokens[p].type){
			case TK_INT:
				val = (uint32_t) atoi(tokens[p].str);
				break;
			
			case TK_HEXINT:
				sscanf(tokens[p].str+2, "%x", &val);
				break;

			case TK_REGISTER_VALUE: {
				uint32_t isa_reg_str2val(const char *, bool *);
				val = isa_reg_str2val(tokens[p].str, success);
				break;
			}
		}
		return val;
    } 
	else if (check_parentheses(p, q, success) == true) {
		/* The expression is surrounded by a matched pair of parentheses.
		 * If that is the case, just throw away the parentheses.
		 */
		int val = eval(p + 1, q - 1, success);
		if(*success == false){
			return 0;
		}
		printf("check_parentheses position %x\n", val);
		return val;
    }
	else {
		if(*success == false){
			return 0;
		}
		if(p + 1 == q){
			uint32_t val = 0;
			if(tokens[p].type == TK_DEREF){
				uint32_t paddr_read(paddr_t, int);
				val = eval(p+1, q, success);
				if(*success == false){
					return 0;
				}
				val = paddr_read(val, 1);
				return val;
			}
			else if(tokens[p].type == TK_PREFIXMINUS){
				/* only set a minus number to its two's complement*/
				val = eval(p+1, q, success);
				if(*success == false){
					return 0;
				}
				val = ~val +1;
				return val;
			}
			else{
				/* now impossibly get other situation*/
				assert(0);
			}
		}
		else{
			int op = find_maincalsymbol(p, q);
			uint32_t val1 = eval(p, op - 1, success);
			if (*success == false) {
			    return 0;
			}
			uint32_t val2 = eval(op + 1, q, success);
			if (*success == false) {
			    return 0;
			}
		
			switch (tokens[op].type) {
			case TK_ADD:
			    return val1 + val2;
			case TK_MINUS:
			    return val1 - val2;
			case TK_STAR:
			    return val1 * val2;
			case TK_DIV:
				if(val2 == 0){
					*success = false;
					return 0;
				}
			    return val1 / val2;
			case TK_EQ:
				return (val1 == val2) ? 1 : 0;
			case TK_NONEQ:
				return (val1 != val2) ? 1 : 0;
			case TK_AND:
				return (val1 == 1 && val2 == 1) ? 1 : 0;

			default:
			    assert(0);
			}
		}
    }
}

uint32_t expr(char *e, bool * success)
{
//	printf("insert expr functions\n");
//	printf("the number of rules is %lu\n", NR_REGEX);
    if (!make_token(e)) {
		*success = false;
		return 0;
    }

	int i;
	for (i = 0; i < nr_token; i ++) {
	  if (tokens[i].type == TK_STAR && (i == 0 
		  || tokens[i - 1].type == TK_LP
		  || tokens[i - 1].type == TK_ADD
		  || tokens[i - 1].type == TK_MINUS
		  || tokens[i - 1].type == TK_STAR
		  || tokens[i - 1].type == TK_DIV
		  || tokens[i - 1].type == TK_EQ
		  || tokens[i - 1].type == TK_NONEQ
		  || tokens[i - 1].type == TK_AND
		  ) ) {
	    tokens[i].type = TK_DEREF;
	  }
	}

	for (i = 0; i < nr_token; i ++) {
	  if (tokens[i].type == TK_MINUS && (i == 0 
		  || tokens[i - 1].type == TK_LP
		  || tokens[i - 1].type == TK_ADD
		  || tokens[i - 1].type == TK_MINUS
		  || tokens[i - 1].type == TK_STAR
		  || tokens[i - 1].type == TK_DIV
		  || tokens[i - 1].type == TK_EQ
		  || tokens[i - 1].type == TK_NONEQ
		  || tokens[i - 1].type == TK_AND
		  ) ) {
	    tokens[i].type = TK_PREFIXMINUS;
	  }
	}
    /* TODO: Insert codes to evaluate the expression. */
    uint32_t val = eval(0, nr_token - 1, success);
    if (*success == true) {
		return val;
    }

    return 0;
}

//int main(void){
///* 例如在check_parentheses()函数中, (4 + 3)) * ((2 - 1)和(4 + 3) * (2 - 1)这两个表达式虽然都返回false, 因为前一种情况是表达式不合法, 是没有办法成功进行求值的; 而后一种情况是一个合法的表达式, 是可以成功求值的,*/ 
///*	"(2 - 1)"             // true
//	"(4 + 3 * (2 - 1))"   // true
//	"4 + 3 * (2 - 1)"     // false, the whole expression is not surrounded by a matched
//	                      // pair of parentheses
//	"(4 + 3)) * ((2 - 1)" // false, bad expression
//	"(4 + 3) * (2 - 1)"   // false, the leftmost '(' and the rightmost ')' are not matched */
//	tokens[0].type = TK_LP;
//	tokens[1].type = TK_INT;
//	tokens[2].type = TK_MINUS;
//	tokens[3].type = TK_INT;
//	tokens[4].type = TK_RP;
//	
//	strcat(tokens[1].str, "2");
//	strcat(tokens[3].str, "1");
//
//	bool success = true;
//	printf("%u", eval(0, 4, success));
//	return 0;
//}
