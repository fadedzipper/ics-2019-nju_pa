#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
	int i;
	for(i = 0; s[i] != '\0'; i++);
	return i;
}

char *strcpy(char* dst,const char* src) {
	int i;
	for(i = 0; src[i] != '\0'; i++){
		dst[i] = src[i];
	}
	dst[i] = '\0';
	return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
	int i;
	for(i = 0; i < n && src[i] != '\0'; i++){
		dst[i] = src[i];
	}
	while(i < n){
		dst[i] = '\0';
	}
	return dst;
}

char* strcat(char* dst, const char* src) {
	int n = strlen(dst);
	int i;
	for(i = 0; src[i] != '\0'; i++){
		dst[n+i] = src[i];
	}
	dst[n+i] = '\0';
	return dst;
}

int strcmp(const char* s1, const char* s2) {
	int n1 = strlen(s1);
	int n2 = strlen(s2);
	int i;
	for(i = 0; i < n1 && i < n2; i++){
		if(s1[i] < s2[i]){
			return -1;
		}
		else if(s1[i] > s2[i]){
			return 1;
		}
	}
	if(i < n1){
		return 1;
	}
	else if(i == n1 && i == n2){
		return 0;
	}
	return -1;
}

int strncmp(const char* s1, const char* s2, size_t n) {
	int n1 = strlen(s1);
	int n2 = strlen(s2);
	int i;
	for(i = 0; i < n1 && i < n2 && i < n; i++){
		if(s1[i] < s2[i]){
			return -1;
		}
		else if(s1[i] > s2[i]){
			return 1;
		}
	}
	if(i == n){
		return 0;
	}
	
	if(i < n1){
		return 1;
	}
	else if(i == n1 && i == n2){
		return 0;
	}
	return -1;
}

void* memset(void* v,int c,size_t n) {
	unsigned char ch = (unsigned char)c;
	unsigned char *p = (unsigned char *)v;
	int i;
	for(i = 0; i < n; i++){
		p[i] = ch;
	}
	return v;
}

void* memcpy(void* out, const void* in, size_t n) {
	char *a = (char *)out;
	char *b = (char *)in;
	int i;
	for(i = 0; i < n; i ++){
		a[i] = b[i];
	}
	return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
	int i;
	unsigned char *p = (unsigned char *)s1, *q = (unsigned char *)s2;
	for(i = 0; i < n; i++){
		if(p[i] < q[i]){
			return -1;
		}
		else if(p[i] > q[i]){
			return 1;
		}
	}
	return 0;
}

#endif
