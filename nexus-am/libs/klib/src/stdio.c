#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

/* _putc */

int printf(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	char msg[512];
	int pos = vsprintf(msg, fmt, ap);

	void _putc(char ch);
	for(int  i = 0; i < 512 && msg[i] != '\0'; i ++){
		_putc(msg[i]);
	}

	va_end(ap);
	return pos;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
	int d;
	char c, *s;

	int pos = 0;   // position in out
	int flag = 0;
	int flag_transfer = 0; // in sorts of \n
	while(*fmt){
		switch(*fmt){
			case '\\' :
				flag_transfer = 1;
				break;

			case '%' :
				flag = 1;
				break;

			case 's' :{
				if(flag == 0){
					out[pos++] = 's';
					break;
				}
				s = va_arg(ap, char *);
				strncpy(out + pos, s, strlen(s));
				pos += strlen(s);
				flag = 0;
				break;
					  }

			case 'd' : {
				if(flag == 0){
					out[pos++] = 'd';
					break;
				}
				d = va_arg(ap, int);
				char x[35];
				memset(x, 0, 35);
				int i = 0;
				if(d < 0){
					out[pos++] = '-';
					d = -d;
				}
				int rem = 0;
				while(d){
					rem = d % 10;
					x[i++] = rem + '0';
					d /= 10;
				}
				char temp[35];
				memset(temp, 0, 35);
				int t = 0;
				for(; t < i; t++){
					temp[t] = x[i-1-t];
				}
				for(t = 0; t < i; t++){
					x[t] = temp[t];
				}
				x[i] = '\0';
				strncpy(out + pos, x, i);
				pos += i;
				flag = 0;
				break;
					   }

			case 'c' :
				if(flag == 0){
					out[pos++] = 'c';
					break;
				}
				c = (char)va_arg(ap, int);
				out[pos++] = c;
				flag = 0;
				break;

			case 'n' :
				if(flag_transfer == 0){
					out[pos++] = 'n';
					break;
				}
				out[pos++] = '\n';
				flag_transfer = 0;
				break;

			default :
				out[pos++] = *fmt;

		}
		fmt++;
	}
	out[pos] = '\0';

	return pos;
}

int sprintf(char *out, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	int pos = vsprintf(out, fmt, ap);
	va_end(ap);
	return pos;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	int d;
	char c, *s;

	int pos = 0;   // position in out
	int flag = 0;
	int flag_transfer = 0; // in sorts of \n
	while(*fmt){
		switch(*fmt){
			case '\\' :
				flag_transfer = 1;
				break;

			case '%' :
				flag = 1;
				break;

			case 's' :{
				if(flag == 0){
					out[pos++] = 's';
					break;
				}
				s = va_arg(ap, char *);
				int copy_size = n - pos < strlen(s) ? n - pos : strlen(s);
				strncpy(out + pos, s, copy_size);
				pos += copy_size;
				flag = 0;
				break;
					  }

			case 'd' : {
				if(flag == 0){
					out[pos++] = 'd';
					break;
				}
				d = va_arg(ap, int);
				char x[35];
				memset(x, 0, 35);
				int i = 0;
				if(d < 0){
					out[pos++] = '-';
					d = -d;
				}
				int rem = 0;
				while(d){
					rem = d % 10;
					x[i++] = rem + '0';
					d /= 10;
				}
				char temp[35];
				memset(temp, 0, 35);
				int t = 0;
				for(; t < i; t++){
					temp[t] = x[i-1-t];
				}
				for(t = 0; t < i; t++){
					x[t] = temp[t];
				}
				x[i] = '\0';
				int copy_size = n - pos < i ? n - pos : i;
				strncpy(out + pos, x, copy_size);
				pos += copy_size;
				flag = 0;
				break;
					   }

			case 'c' :
				if(flag == 0){
					out[pos++] = 'c';
					break;
				}
				c = (char)va_arg(ap, int);
				out[pos++] = c;
				flag = 0;
				break;

			case 'n' :
				if(flag_transfer == 0){
					out[pos++] = 'n';
					break;
				}
				out[pos++] = '\n';
				flag_transfer = 0;
				break;

			default :
				out[pos++] = *fmt;

		}
		fmt++;
		if(pos == n){
			va_end(ap);
			return pos;
		}
	}
	va_end(ap);
	out[pos] = '\0';

	return pos;
}

#endif
