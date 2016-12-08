/*
 * Reinforce.c
 */
#include "Reinforce.h"
#include "inc/KeyIOApi.h"
#include "Key.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "inc/base64_dec.h"
#include "inc/base64_enc.h"

static int func0() {
	//return 0;
	int result = 0, i;
	result >>= 4;
	i = rand() % 5;
	switch (i) {
	case 0:
		i &= 0xffff;
		if (i >= 0)
			return 0;
		break;
	case 1:
		return i - 1;
		break;
	case 2:
		return i - 2;
		break;
	case 3:
		return i - 3;
		break;
	case 4:
		return 0;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}
static int func1() {
	//return 1;
	int a = 32;
	a <<= 1;
	if (a > 20)
		return a - 63;
	else
		return 23;
}
static int func2() {
	//return 2;
	int i, j = 0;
	for (i = 0; i < 5; i++) {
		j++;
	}
	return j - 3;
}
static int func3() {
	//return 3;
	int i = rand() % 3;
	switch (i) {
	case 0:
		i += 2;
		break;
	case 1:
		i++;
		break;
	default:
		break;
	}
	return i + 1;
}
static int func4() {
	//return 4;
	int b = 1;
	b ^= 0x0f;
	return b - 10;
}
static int func5() {
	//return 5;
	int i = 0;
	while (i < 10) {
		i++;
		i <<= 1;
	}
	return i - 9;
}
static int func6() {
	//return 6;
	int i, j, k;
	j = func3();
	k = func4();
	i = j + k + 5;
	return (i / 2);
}
static int func7() {
	//return 7;
	int i, j, k;
	j = func2();
	k = func5();
	i = j + k;
	return i;
}
static int func8() {
	//return 8;
	int i, j, k;
	j = func1();
	k = func7();
	i = j + k;
	return i;
}
static int func9() {
	//return 9;
	int i, j, k;
	j = func2();
	k = func6();
	i = j + k + 1;
	return i;
}
static int func10() {
	//return 10;
	int i, j, k;
	j = func1();
	k = func9();
	i = j + k;
	return i;
}
static int func11() {
	//return 11;
	int i, j, k, l;
	j = func2();
	k = func3();
	l = func4();
	i = j + 2 + k + l;
	return i;
}
static int func12() {
	//return 12;
	int i, j, k;
	j = func2();
	k = func10();
	i = j + k;
	return i;
}
static int func13() {
	//return 13;
	int i, j, k;
	j = func1();
	k = func12();
	i = j + k;
	return i;
}
static int func14() {
	//return 14;
	int i, j, k;
	j = func2();
	k = func7();
	i = j + k + 5;
	return i;
}
static int func15() {
	//return 15;
	int i, j, k, l;
	j = func4();
	k = func5();
	l = func6();
	i = j + k + l;
	return i;
}

static int confuseFunc(int input) {
	int result;
	switch (input) {
	case 0:
		result = func13();
		break;
	case 1:
		result = func11();
		break;
	case 2:
		result = func3();
		break;
	case 3:
		result = func5();
		break;
	case 4:
		result = func7();
		break;
	case 5:
		result = func9();
		break;
	case 6:
		result = func1();
		break;
	case 7:
		result = func2();
		break;
	case 8:
		result = func15();
		break;
	case 9:
		result = func12();
		break;
	case 10:
		result = func4();
		break;
	case 11:
		result = func8();
		break;
	case 12:
		result = func10();
		break;
	case 13:
		result = func6();
		break;
	case 14:
		result = func0();
		break;
	case 15:
		result = func14();
		break;
	default:
		result = 255;
		break;
	}
	return result;
}

static int reconfuseFunc(int input1) {
	int result1;
	switch (input1) {
	case 0:
		result1 = func14();
		break;
	case 1:
		result1 = func6();
		break;
	case 2:
		result1 = func7();
		break;
	case 3:
		result1 = func2();
		break;
	case 4:
		result1 = func10();
		break;
	case 5:
		result1 = func3();
		break;
	case 6:
		result1 = func13();
		break;
	case 7:
		result1 = func4();
		break;
	case 8:
		result1 = func11();
		break;
	case 9:
		result1 = func5();
		break;
	case 10:
		result1 = func12();
		break;
	case 11:
		result1 = func1();
		break;
	case 12:
		result1 = func9();
		break;
	case 13:
		result1 = func0();
		break;
	case 14:
		result1 = func15();
		break;
	case 15:
		result1 = func8();
		break;
	default:
		result1 = 255;
		break;
	}
	return result1;
}
static char *obfuscateIKey(char const *src, int length) {
	if (src == NULL)
		return NULL;
	int i, number, temp;
	if (length <= 0)
		return NULL;

	char *a = calloc(2 * length, sizeof(char));
	int mappingArray[] =
			{ 5, 2, 15, 7, 14, 10, 8, 11, 0, 9, 12, 13, 1, 4, 6, 3 };
	for (i = 0; i < length * 2; i++) {
		if (i % 2 == 0)
			number = src[i / 2] >> 4;
		else
			number = src[i / 2] & 0xf;
		number = mappingArray[number];
		temp = confuseFunc(number);
		*(a + i) = (char) temp;
	}
	return a;
}
int setIKey(char *iKey) {
	char encKey[44];
	int i;
	base64enc(encKey, (void *) iKey, strlen(iKey));
	char *a = obfuscateIKey(encKey, 44);
	int ret = 0;
	if (a) {
		char tmp[120];
		base64enc(tmp, (void *) a, 2 * 44 * sizeof(char));

		printf("obfuscateIKey is");
		for (i = 0; i < strlen(tmp); i++) {
			printf("%c", tmp[i]);
		}
		printf("\n");

		ret = ompWriteFile(ReinforceIKeyFileName, tmp, 120);
		free(a);
		return ret;
	}
	return -1;
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
