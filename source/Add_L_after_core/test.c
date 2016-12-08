/*
 ============================================================================
 Name        : test.c
 Author      : sushuai
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <dlfcn.h>
#include "KeyIOApi.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

/**作用是把argv[1]文件的大小拼接到 argv[2]的后面
 * $OPAPKPATH/tmp/armeabi/libcmcc_noah.so $OPAPKPATH/tmp/armeabi/libcmcc_haze.so
 */
int main(int argc, char **argv) {
	int fd, length = 0, ret = 0;
	struct stat stat;
	char inpath[128] = "\0";
	char Addpath[128] = "\0";
	char lengthchar[4] = { 0 };
	int fnew, lengthnew;
	int newlength;

	struct stat ffstat;
	snprintf(inpath, 128, "%s", argv[1]);
	snprintf(Addpath, 128, "%s", argv[2]);
	printf("inpath is %s\n", inpath);
	printf("Addpath is %s\n", Addpath);
	if ((fd = open(inpath, O_RDONLY)) < 0) {
		printf("open %s error!\n", inpath);
		return -2;
	}
	if ((fstat(fd, &stat)) < 0)
		return -3;

	/**
	 * length = stat.st_size;
	 * memcpy(lengthchar, &length, sizeof(int));
	 * FILE* output = fopen(Addpath, "a+");
	 * fwrite(lengthchar, sizeof(int), 1, output);
	 * 核心代码如上, length 是 argv[1]文件的长度,把 int 型的长度转成 4 个字节的 char数组,
	 * 然后 fopen 打开 argv[2],打开的类型是 a+,就是拼接,然后 fwrite 把 4个字节的长度拼接到 argv[2]后面。
	 */
	length = stat.st_size;
	printf("length:%d\n", length);
	//memcpy函数的功能是从源src所指的内存地址的起始位置开始拷贝n个字节到目标dest所指的内存地址的起始位置中。
	memcpy(lengthchar, &length, sizeof(int));

	FILE* output = fopen(Addpath, "a+");
	if (output == NULL) {
		fputs("File write error\n", stderr);
		return 1;
	}

	if (fwrite(lengthchar, sizeof(int), 1, output) != 1) {
		fclose(output);
		fputs("File write error\n", stderr);
		return 1;
	}

	fclose(output);

	//下面只是为了验证是否拼接正确
	if ((fnew = open(Addpath, O_RDONLY)) < 0)
		return -2;
	if ((fstat(fnew, &ffstat)) < 0)
		return -3;
	lengthnew = ffstat.st_size;
	printf("lengthnew : %d\n", lengthnew);
	//在内存的动态存储区中分配n个长度为size的连续空间，函数返回一个指向分配起始地址的指针
	char *outdata = calloc(lengthnew, sizeof(char));
	ret = ompReadFile(Addpath, lengthnew, outdata);

	memcpy(&newlength, outdata + lengthnew - sizeof(int), sizeof(int));

	printf("newlength :%d\n", newlength);
	return 0;

}

