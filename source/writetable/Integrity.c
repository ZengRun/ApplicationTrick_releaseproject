/*
 * Integrity.c
 */

#include "Integrity.h"
#include "inc/hmac_sha256.h"
#include "Reinforce.h"
#include "Key.h"
#include <unistd.h>

int ompReadFile(const char *filepath, const int len, unsigned char *out);
int ompWriteFile(const char *filepath, const unsigned char *in, const int len);
int generateRandomKey(char *key, int length);
int setIKey(char *ikey);
int getIKey(char *ikey);
int getIKeyForGenerate(char *ikey);
void base64enc(char* dest, void* src, unsigned short length);

int calHmac(char *filePath,char *result){
	int fd,length,ret;
	struct stat stat;
	if((fd = open(filePath, O_RDONLY)) < 0)
		return -2;
	if((fstat(fd,&stat)) < 0)
		return -3;
	length = stat.st_size;

	char *out = malloc(sizeof(char) * length);
	ret = ompReadFile(filePath, length, out);
	if(ret != 0){
		free(out);
		return -4;
	}

	char key[33];
	getIKey(key);
	int klen = 32;
	char tmp[32];
	hmac_sha256(key,klen,out,length,tmp);
	base64enc(result,tmp,32);
	free(out);
	return 0;
}

int calHmacForGenerate(char *filePath,char *result){
	int fd,length,ret;
	int i;
	struct stat stat;
	if((fd = open(filePath, O_RDONLY)) < 0)
		return -2;
	if((fstat(fd,&stat)) < 0)
		return -3;
	length = stat.st_size;

	char *out = malloc(sizeof(char) * length);
	ret = ompReadFile(filePath, length, out);
	if(ret != 0){
		free(out);
		return -4;
	}

	char key[33];
	getIKeyForGenerate(key);
	
	printf("Ikey is ");
	for(i=0;i<strlen(key);i++){
	printf("%c",key[i]);
	//printf(" ");
	}
	printf("\n");
	
	int klen = 32;
	char tmp[32];
	hmac_sha256(key,klen,out,length,tmp);
	base64enc(result,tmp,32);
	free(out);
	return 0;
}

int generateHmac(char *filePath){
	int ret = 0,i;
	char ikey[33];
	char output[44];
	generateRandomKey(ikey,32);
	
	printf("\ngenerateRandomKey is");
		for(i=0;i<strlen(ikey);i++){
		printf("%c",ikey[i]);
		}
		
	
		
	setIKey(ikey);
	ret = calHmacForGenerate(filePath,output);
	if(ret != 0)
		return -1;
	char final[88];
	char *tmp = final;
	for(i = 0; i < 44; i++){
		sprintf(tmp,"%2x",output[i]);
		tmp += 2;
	}
	
	printf("Hmac is ");
	for(i=0;i<strlen(final);i++){
	printf("%c",final[i]);
	//printf(" ");
	}
	printf("\n");
	
	ret = ompWriteFile(HMACFILE, final, 88);
	return ret;
}

int verifyHmac(char *filePath){
	int ret = 0,i;
	char oldHmac[89] = HMACVALUE;
	char newHmac[89];
	char tmpHmac[45];
	ret = calHmac(filePath,tmpHmac);
	if(ret != 0)
		return ret;
	char *tmp = newHmac;
	for(i = 0; i < 44; i++){
		sprintf(tmp,"%2x",tmpHmac[i]);
		tmp += 2;
	}
	for(i = 0; i < 88; i++){
		if(oldHmac[i] != newHmac[i]){
			return -1;
		}
	}
	return 0;
}

int apiProtect(char *pathName){
	char *p;
	pid_t pid = getpid();
	char command[20];
	snprintf(command,20,"ps -p %d",pid);
	FILE *pp;
	char buffer[512];
	pp = popen(command,"r");
	fgets(buffer,sizeof(buffer),pp);
	fgets(buffer,sizeof(buffer),pp);
	char *tmp = strtok(buffer," ");
	char *prevs = NULL;
	while(tmp != NULL){
		prevs = tmp;
		tmp = strtok(NULL," ");
	}
	if(prevs == NULL)
		return -2;
	unsigned int n = strlen(prevs);
	char result[128];
	strncpy(result,prevs,n);
	result[n-1] = '\0';
	pclose(pp);
	p = strstr(pathName,result);
	if(p == NULL)
		return -3;
	return 0;
}

int checkIntegrity(char *packageName){
	int ret = -5;
	/* get the dex file path: /data/data/apkname/classes.dex, then verify hmac */
	char dexpath[128];
	snprintf(dexpath,128,"/data/data/%s/classes.dex",packageName);
	ret = verifyHmac(dexpath);
	return ret;
}
