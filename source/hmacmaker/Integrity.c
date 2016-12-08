/*
 * Integrity.c
 */

#include "Integrity.h"
#include "inc/hmac_sha256.h"
#include "Reinforce.h"
#include "Key.h"
#include "inc/zip.h"
#include "inc/unzip.h"
#include "inc/ioapi.h"
#include <unistd.h>

int ompReadFile(const char *filepath, const int len, unsigned char *out);
int ompWriteFile(const char *filepath, const unsigned char *in, const int len);
int generateRandomKey(char *key, int length);
int setIKey(char *ikey);
//int getIKey(char *ikey);
int getIKeyForGenerate(char *ikey);
void base64enc(char* dest, void* src, unsigned short length);
int encryptData(const char *Data, const char *Privatekey, char *reval);
int resDataLen = 0;
unsigned char writebuf[256] = "\0";
char *resFileData = NULL;
int resFileLen = 0;
char strResLen[20] = "\0";

/**
 * 根据最早生成的32位随机密钥和文件的内容通过hmac_sha256和base64enc得到44位的hmac值
 */
int calHmacForGenerate(char *filePath, char *result) {
	int fd, length, ret;
	int i;
	//读取打开文件信息
	struct stat stat;
	if ((fd = open(filePath, O_RDONLY)) < 0)
		return -2;
	if ((fstat(fd, &stat)) < 0)
		return -3;
	length = stat.st_size;

	char *out = malloc(sizeof(char) * length);
	//将文件内容读取到out
	ret = ompReadFile(filePath, length, out);
	if (ret != 0) {
		free(out);
		return -4;
	}

	char key[33];
	//key存放最早生成的32位ikey
	getIKeyForGenerate(key);

	printf("Ikey is ");
	for (i = 0; i < strlen(key); i++) {
		printf("%c", key[i]);
	}
	printf("\n");

	int klen = 32;
	char tmp[32];
	//然后hmac_sha256根据32位ikey和文件的内容计算hmac值tmp
	hmac_sha256(key, klen, out, length, tmp);
	//最后base64编码得到44位的hmac值result
	base64enc(result, tmp, 32);
	free(out);
	return 0;
}

int GetMarkNum(char *buf) {
	int i = 0;
	int mark = 0;
	for (i = 0; i < strlen(buf); i++) {
		if (buf[i] == ';') {
			mark++;
		}
	}
	return mark;
}

int GetstrToken(char *buf, char *str, int n) {
	int len = 0, i;
	char *pstart;
	char *pfind;
	char *startpr, *endpr;
	char tag[64];
	//char imsi[1024]={0};
	memset(tag, 0, sizeof(tag));
	strcpy(tag, ";");
	pstart = buf;
	for (i = 0; i < n; i++) {
		if ((pfind = (char *) strstr(pstart, tag)) != NULL) {
			if (i == n - 1) {
				len = pfind - pstart;
				if (len > 0) {
					memcpy(str, pstart, len);
				}
			} else {
				pfind++;
				pstart = pfind;
			}
		}
	}

	return 0;
}

/**
 * 首先获取要计算hmac的文件的大小length,动态分配length长度的堆内存空间out,ompReadFile读取要计算的文件到这个堆空间中。
 */
int calHmacForFile(char *filePath, char *result, char *apkpath) {
	unz_global_info64 gi;
	int err;
	unzFile uf = NULL;
	unsigned char *fileout = NULL;
	int lengthForout = 0;
	printf("calHmacForFile  filePath is%s\n", filePath);
	int ch = 0;
	char *seps = ";";
	char *token = NULL;
	int fd, length, ret;
	int i, x, y, z;
	int MarkNum = 0;
	int klen = 32;
	unsigned char tmp[32] = { 0 };
	unsigned char TemFile[33] = { 0 };
	char strfile[2056] = { 0 };
	char strstr[1024] = { 0 };
	int j = 0, mark = 0;
	struct stat stat;
	unsigned char cer1tmpmac[33] = { 0 };
	int filelength = 0;
	char key[33] = { 0 };
	getIKeyForGenerate(key);
	printf("Ikey is ");
	for (i = 0; i < strlen(key); i++) {
		printf("%c", key[i]);
	}
	printf("\n");
	//打开一个文件用于读或写
	if ((fd = open(filePath, O_RDONLY)) < 0)
		return -2;
	//获取打开文件信息
	if ((fstat(fd, &stat)) < 0)
		return -3;
	length = stat.st_size;

	char *out = malloc(sizeof(char) * length);
	ret = ompReadFile(filePath, length, out);
	if (ret != 0) {
		free(out);
		return -4;
	}
	printf("out  file number is %s\n", out);
	resFileLen = length;

	encryptData(out, key, writebuf);
	printf("writebuf is %s\n", writebuf);
	resFileData = writebuf;
	printf("resFileData is %s\n", resFileData);

	sprintf(strResLen, "%d", length);
	printf("strResLen is %s\n", strResLen);

	uf = unzOpen64(apkpath);
	if (uf == NULL) {
		printf("cannot open %s\n", apkpath);
		return -2;
	}
	err = unzGetGlobalInfo64(uf, &gi);
	if (err != UNZ_OK) {
		printf("err != UNZ_OK\n");
	}

	MarkNum = GetMarkNum(out);
	printf("MarkNum is %d\n", MarkNum);

	for (i = 0; i < gi.number_entry; i++) {
		unz_file_info64 file_infos;
		char filename_inzips[256];

		err = unzGetCurrentFileInfo64(uf, &file_infos, filename_inzips,
				sizeof(filename_inzips), NULL, 0, NULL, 0);
		if (err != UNZ_OK) {
			break;
		}

		for (mark = 1; mark <= MarkNum; mark++) {
			memset(strstr, 0, sizeof(strstr));
			GetstrToken(out, strstr, mark);
			printf("strstr is %s\n", strstr);
			printf("mark is %d\n", mark);
			if (strcmp(filename_inzips, strstr) == 0) {
				fileout = (unsigned char *) extractFile_inmem(apkpath,
						filename_inzips, &filelength);
				printf("\nin calculate filename_inzips is %s \n",
						filename_inzips);
				printf("\nin calculate fileout is %s \n", fileout);
				hmac_sha256(key, klen, fileout, filelength, tmp);
				if (fileout != NULL) {
					free(fileout);
					fileout = NULL;
				}

				printf("after tmp is ");
				for (x = 0; x < 32; x++) {
					printf("%x ", tmp[x]);
				}

				for (y = 0; y < 32; y++) {
					TemFile[y] ^= tmp[y];
				}
				TemFile[32] = '\0';
				printf("TemFile is ");
				for (z = 0; z < 32; z++) {
					printf("%x  ", TemFile[z]);
					//printf(" ");
				}
				printf("\n");
			}

		}

		if ((i + 1) < gi.number_entry) {
			err = unzGoToNextFile(uf);
			if (err != UNZ_OK) {
				break;
			}
		}

	}
	if (out != NULL) {
		free(out);
		out = NULL;
	}
	base64enc(result, TemFile, 32);
	printf("result is %s\n", result);
	unzClose(uf);
	return 0;
}

int generateHmac(char *resPath, char *filePath, char *apkpath) {
	int ret = 0, i, fret = -1;
	char ikey[33];
	char output[44];
	//生成一个32位长度随机的ikey(算上‘\0’33位)
	generateRandomKey(ikey, 32);

	int retResData = 0;
	int retResLen = 0;
	printf("\ngenerateRandomKey is");
	for (i = 0; i < strlen(ikey); i++) {
		printf("%c", ikey[i]);
	}

	//把最后生成的120位ikey写入ikey.reinforce文件中
	setIKey(ikey);

	if (strcmp(resPath, "-f") == 0) {
		printf("apkpath is %s\n", apkpath);
		//先创建内存空间再把文件内容读取到内存中（output存放Hmac值）
		ret = calHmacForFile(filePath, output, apkpath);
		printf("resFildData is >>>>>>>>>>%s\n", resFileData);
		fret = 1;
	} else {
		//output是生成的44位Hmac值
		ret = calHmacForGenerate(resPath, output);
		fret = 0;
	}
	if (ret != 0)
		return -1;
	//最后再把output这44位的hmac值变成88位
	char final[88];
	char *tmp = final;
	for (i = 0; i < 44; i++) {
		sprintf(tmp, "%2x", output[i]);
		tmp += 2;
	}

	printf("Hmac is ");
	for (i = 0; i < strlen(final); i++) {
		printf("%c", final[i]);
	}
	printf("\n");
	if (1 == fret) {
		retResData = ompWriteFile(RESDATAFILE, resFileData,strlen(resFileData));
		printf("RESDATAFILE wite>>>>is>>>%d", retResData);
		retResLen = ompWriteFile(FLENFILE, strResLen, resFileLen);
		printf("strResLen is %s\n", strResLen);
		printf("retResLen is >>>%d\n", retResLen);
		printf("resFileLen is >>>%d\n", resFileLen);
	}

	//最后将88位的Hmac值写入hmac.reinforce文件中
	ret = ompWriteFile(HMACFILE, final, 88);

	return ret;
}

int encryptData(const char *Data, const char *Privatekey, char *reval) {
	unsigned char plaintext[1024] = { 0 };
	unsigned char iv[16] = { 1 };
	int pading = 0;
	char data[256];
	memset(data, 0, sizeof(data));
	unsigned char ucResult[256] = { 0x00 };
	AES_KEY key;
	int ret = 0;
	unsigned char pucKey[16] = { 0x00 };
	memcpy(pucKey, Privatekey + 16, 16);
	ret = AES_set_encrypt_key((const unsigned char*) pucKey, 128, &key);
	resDataLen = strlen(Data);
	printf("使用前为: %s\n", Data);
	if (resDataLen % 16 != 0) {
		pading = ((int) resDataLen / 16 + 1) * 16;
		memcpy(plaintext, Data, resDataLen);
		memset(data + resDataLen, pading - resDataLen, pading - resDataLen);
		resDataLen = pading;
	}
	printf("使用后为: %s\n", Data);
	memcpy(iv, Privatekey, 16);
	AES_cbc_encrypt((const unsigned char*) plaintext, ucResult, resDataLen,
			&key, iv, AES_ENCRYPT);
	base64enc(reval, ucResult, (unsigned short) resDataLen);
	return 0;
}

