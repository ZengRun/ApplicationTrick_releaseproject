/*
 * main.c
 *
 *  Created on: 2010-9-25
 *      Author: Allen Yang
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {  //file
	printf("argv[1] is  %s\n", argv[1]);
	printf("argv[2] is  %s\n", argv[2]);
	printf("argv[3] is  %s\n", argv[3]);
	//strcmp:compare two strings the same return 0
	if (argv[2] != NULL && strcmp(argv[1], "-f") == 0) {
		printf("run argv generateHmac(argv[2],argv[1]); \n");
		generateHmac(argv[1], argv[2], argv[3]);
	} else {
		printf("run argv generateHmac(argv[1]); \n");
		generateHmac(argv[1]);
	}

	return 0;
}
