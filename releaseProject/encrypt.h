#include <stdio.h>
#include "inc/KeyIOApi.h"
#include "inc/rijndael.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "logcat.h"
#define KEYBITS 256
#define LOG_TAG "encrypt_oat"

int encrypt_oat(char* oatpath){
	int fd, length, ret;
	struct stat stat;
	char enckey[33] = "\0";
	int i,n;
	if ((fd = open(oatpath, O_RDONLY)) < 0)
		return -2;
	if ((fstat(fd, &stat)) < 0)
		return -3;
	length = stat.st_size;
	close(fd);
	char *endata = (char*) calloc(length, sizeof(char));
	ret = ompReadFile(oatpath, length, endata);
	if (ret != 0) {
		free(endata);
		return -4;
	}
	remove(oatpath);//删除softsec_march
	generateRandomKey(enckey, 32);
	LOGI("generateRandomKey is %s",enckey);
	encryptFile(endata, length, enckey, oatpath);//密文文件命名为softsec_march
	free(endata);
	addKeyAfterCipher(enckey,oatpath);
}

int generateRandomKey(char *key, int length) {
	int i;
	char tmp;
	srand((unsigned) time(NULL));
	sleep(1);
	char symbols[36] = "0123456789abcdefghijklmnopqrstuvwxyz";
	for (i = 0; i < length; i++) {
		tmp = symbols[rand() % 36];
		*key++ = tmp;
	}
	*key++ = '\0';
	return 0;
}

int encryptFile(char* data, int len, char* pwd, char* outFile) {
	unsigned long rk[RKLENGTH(KEYBITS)];
	unsigned char key[KEYLENGTH(KEYBITS)];
	int i;
	int nrounds;
	char *password;
	FILE *output;
	char* type = "wb+";
	password = pwd;
	for (i = 0; i < sizeof(key); i++)
		key[i] = *password != 0 ? *password++ : 0;

	output = fopen(outFile, type);
	if (output == NULL) {
		fputs("File write error\n", stderr);
		return 1;
	}
	nrounds = rijndaelSetupEncrypt(rk, key, 256);
	int ngroups = len / 16;
	int lastgroup = len % 16;
	int k;
	for (k = 0; k < ngroups; k++) {
		unsigned char plaintext[16];
		unsigned char ciphertext[16];
		int j;
		for (j = 0; j < sizeof(plaintext); j++) {
			plaintext[j] = data[k * 16 + j];
		}
		if (j == 0)
			break;
		rijndaelEncrypt(rk, nrounds, plaintext, ciphertext);
		if (fwrite(ciphertext, sizeof(ciphertext), 1, output) != 1) {
			fclose(output);
			fputs("File write error\n", stderr);
			return 1;
		}

	}
	if (lastgroup > 0) {
		unsigned char last[lastgroup];
		int start = 16 * ngroups;
		int l;
		for (l = 0; l < lastgroup; l++) {

			last[l] = data[start + l];
		}
		if (fwrite(last, sizeof(last), 1, output) != 1) {
			fclose(output);
			fputs("File write error", stderr);
			return 1;
		}
	}
	fclose(output);
	return 1;
}

int addKeyAfterCipher(char* key,char* cipherPath){
	FILE* output = fopen(cipherPath, "a+");
	if (output == NULL)
	{
		fputs("File write error\n", stderr);
		return 1;
	}
	if (fwrite(key, strlen(key), 1, output) != 1)
	{
		 fclose(output);
		 fputs("File write error\n", stderr);
		 return 1;
	}
	fclose(output);
	return 0;
}

