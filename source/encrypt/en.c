#include <stdio.h>
#include "inc/KeyIOApi.h"
#include "inc/rijndael.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Reinforce.h"
#include <stdlib.h>
#include <string.h>
#define KEYBITS 256

int encryptFile(char* data, int len, char* pwd, char* outFile);
int destroyFile(char* input, int n, char* output);

int main(int argc, char **argv) //input n output
{
//	printf("encrypt %s begin\n", argv[1]);
	int fd, length, ret;
	struct stat stat;
	char enpath[128] = "\0";
	char enckey[33] = "\0";
	int i,n;
	snprintf(enpath, 128, "%s", argv[1]);
	if ((fd = open(enpath, O_RDONLY)) < 0)
		return -2;
	if ((fstat(fd, &stat)) < 0)
		return -3;
	length = stat.st_size;
	char *endata = (char*) calloc(length, sizeof(char));
	ret = ompReadFile(enpath, length, endata);
	if (ret != 0) {
		free(endata);
		return -4;
	}
	char* tempFile = "tempEncrypted";

	generateRandomKey(enckey, 32);

	printf("generateRandomKey is");
	for (i = 0; i < strlen(enckey); i++) {
		printf("%c", enckey[i]);
	}
	printf("\n");

	setIKey(enckey);

	encryptFile(endata, length, enckey, tempFile);
	n = atoi(argv[2]);
	destroyFile(tempFile, n, argv[3]);
	free(endata);
	remove(tempFile);
//	printf("encrypt %s over!!\n", argv[1]);
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
int destroyFile(char* input, int n, char* output) {
	int fd, length;
	char **ndata = NULL;
	int i;
	int npr;
	int nlast;
	int k;
	char *last;
	int start;
	FILE* outFile;
	int j;
	if ((fd = open(input, O_RDONLY)) < 0) {
		return -1;
	}
	struct stat filestat;
	if (fstat(fd, &filestat) < 0) {
		return -2;
	}
	length = filestat.st_size;
	char *data = calloc(length, sizeof(char));
	ompReadFile(input, length, data);
	printf("length : %d\n", length);
	npr = length / n; //chars per row
	nlast = length % n;
	ndata = (char**) calloc(n, sizeof(char*));
	for (i = 0; i < n; i++) {
		ndata[i] = (char*) calloc(npr, sizeof(char));
	}

	for (i = 0; i < length - nlast; i++) {
		ndata[i % n][i / n] = data[i];
	}

	outFile = fopen(output, "wb");
	if (outFile == NULL) {
		fputs("file error", stderr);
		return -1;
	}
	for (j = 0; j < n; j++) {
		printf("size: %d\n", npr);
		if (fwrite(ndata[j], npr, 1, outFile) != 1) {
			{
				fclose(outFile);
				fputs("File write error", stderr);
				return -1;
			}
		}
	}
	if (nlast > 0) {
		printf("nlast is %d", nlast);
		last = calloc(nlast, sizeof(char));
		start = length - nlast;
		for (k = 0; k < nlast; k++) {
			last[k] = data[start + k];
		}
		if (fwrite(last, nlast, 1, outFile) != 1) {

			fclose(outFile);
			fputs("File write error", stderr);
			return -1;

		}
		free(last);
	}
	fclose(outFile);
	for (i = 0; i < n; i++) {
		free(ndata[i]);
	}
	free(ndata);
	return 0;
}
