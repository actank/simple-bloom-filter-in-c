#include <stdio.h>
//develop

typedef unsigned int (*Func)(char*, unsigned int);

const int n = 10000;

int filter[n];

char* hashfuncindex[4] = {"RSHash", "JSHash", "PJWHash", "ELFHash"};

unsigned int RSHash(char* str, unsigned int len)  
{  
	unsigned int b    = 378551;  
	unsigned int a    = 63689;  
	unsigned int hash = 0;  
	unsigned int i    = 0;  
  
	for(i = 0; i < len; str++, i++)  
	{  
		hash = hash * a + (*str);  
		a    = a * b;  
	}  
  
	return hash;  
}  

unsigned int JSHash(char* str, unsigned int len)  
{  
	unsigned int hash = 1315423911;  
	unsigned int i    = 0;  
  
	for(i = 0; i < len; str++, i++)  
	{  
		hash ^= ((hash << 5) + (*str) + (hash >> 2));  
	}  
  
	return hash;  
}  

unsigned int PJWHash(char* str, unsigned int len)  
{  
	const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);  
	const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);  
	const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);  
	const unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);  
	unsigned int hash              = 0;  
	unsigned int test              = 0;  
	unsigned int i                 = 0;  
  
	for(i = 0; i < len; str++, i++)  
	{  
		hash = (hash << OneEighth) + (*str);  
  
		if((test = hash & HighBits)  != 0)  
		{  
			hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));  
		}  
	}  
  
	return hash;  
}  

unsigned int ELFHash(char* str, unsigned int len)  
{  
	unsigned int hash = 0;  
	unsigned int x    = 0;  
	unsigned int i    = 0;  
  
	for(i = 0; i < len; str++, i++)  
	{  
		hash = (hash << 4) + (*str);  
		if((x = hash & 0xF0000000L) != 0)  
		{  
			hash ^= (x >> 24);  
		}  
		hash &= ~x;  
	}  
  
	return hash;  
}  

char** hashindex(const char* str, unsigned int len, char *hash[3]) {
	int first = (int)(str[len - 1] - 'a');
	int second = (int)(str[len - 2] - 'a');
	int third = (int)(str[len - 3] - 'a');
	
	hash[0] = hashfuncindex[first];
	hash[1] = hashfuncindex[second];
	hash[2] = hashfuncindex[third];
	
	return hash;
	
}

Func getFunc(const char* str) {
	
	if (strcmp(str, hashfuncindex[0]) == 0) {
		return &RSHash;
	} else if (strcmp(str, hashfuncindex[1]) == 0 ) {
		return &JSHash;
	} else if (strcmp(str, hashfuncindex[2]) == 0 ) {
		return &PJWHash;
	} else if (strcmp(str, hashfuncindex[3]) == 0) {
		return &ELFHash;
	}
}

int inputStr(const char* str, unsigned int len) {
	char strnew[100] ;
	strcpy(strnew, str);
	char** hash;
	hash = (char**)malloc(3 * sizeof(char*));
	for (int i = 0; i < 3; i++) {
		hash[i] = (char*)malloc(10 * sizeof(char));
	}
	hash = hashindex(str, len, hash);
	
	
	Func func;
	for (int i = 0; i < 3; i++) {
		func = getFunc(hashfuncindex[i]);
		unsigned int hashval = (*func)(strnew, len);
		filter[hashval % n] = 1;
	}
	free(hash);
	
	return 0;
}

int findStr(const char* str, unsigned int len) {
	char strnew[100];
	strcpy(strnew, str);
	char** hash;
	hash = (char**)malloc(3 * sizeof(char*));
	for (int i = 0; i < 3; i++) {
		hash[i] = (char*)malloc(sizeof(int));
	}
	hash = hashindex(str, len, hash);
	
	Func func;
	for (int i = 0; i < 3; i++) {
		func = getFunc(hashfuncindex[i]);
		unsigned int hashval = (*func)(strnew, len);
		if (filter[hashval % n] == 0) {
			free(hash);
			return 0;
		}
	}
	free(hash);
	return 1; 
}

int main(int argc, char *argv[]) {
	memset(filter, 0, sizeof(filter));
	char* a = "hello";
	char* b = "tomhank";
	char* c = "whyyou";
	inputStr(a, strlen(a));
	inputStr(b, strlen(b));
	inputStr(c, strlen(c));
	
	char* d = "test";
	char* f = "meilishuo";
	
	printf("%d\n", findStr(a, strlen(a)));
	printf("%d\n", findStr(b, strlen(b)));
	printf("%d\n", findStr(c, strlen(c)));
	printf("%d\n", findStr(d, strlen(d)));
	printf("%d\n", findStr(f, strlen(f)));
}
