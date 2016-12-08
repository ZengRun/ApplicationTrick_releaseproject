#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
//#include <windows.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>
//#include "Key.h"

#define FILE_SIZE 67512  	//without Pack

#define OFFSET_HMAC	0xFC98	//without Pack
#define OFFSET_IKEY	0xF970 	//without Pack
#define OFFSET_APPKEY	0xF9F0     //without Pack

#define LENGTH_HMAC	88	//without Pack
#define LENGTH_IKEY	120 	//without Pack
#define LENGTH_APPKEY	120	//without Pack


#define LENGTH_PASSWORD	32
#define LENGTH_PARAMETER 32


char szHMAC[LENGTH_HMAC+1] = {0};
char szIKEY[LENGTH_IKEY+1] = {0};
char szAPPKEY[LENGTH_APPKEY+1] = {0};

int main(int argc,char *argv[])  //name(argv[1]) ENCRYPT classes parameter(argv[2])  ENCRYPT wb_key parameter(argv[3])
{
	int fdin;
	char *str;
	int i;

//Param 2 is FileName,We need such as HMAC key file and so on later...
	struct stat statbuf;
	if(argc != 4 && argc != 3 )
	{
		printf("usage : %s <file>\n",argv[0]);
		return -1;
	}

	if((fdin = open(argv[1],O_RDWR)) == -1)
	{
		printf("can't open file %s\n",argv[1]);
		return -1;
	}

	printf("open file %s\n",argv[1]);
	printf("argv[2] is %s\n",argv[2]);
	printf("argv[2] is %s\n",argv[3]);
	//char szPASSWORD[LENGTH_PASSWORD+1] = {0};
	//char szPASSWORDOrigin[LENGTH_PASSWORD+1] = DEPASSWORD;
	char szPARAMETER[LENGTH_PARAMETER+1] = {0};
	char szPARAMETEROrigin[LENGTH_PARAMETER+1] = "destroyparamemterdestroyparamemt";
	
	char szPARAMETER_WB_kEY[LENGTH_PARAMETER+1] = {0};
	char szPARAMETEROrigin_WB_kEY[LENGTH_PARAMETER+1] = "paramemterdestroydestroyparamemt";
	
	
	char szHMAC1[LENGTH_HMAC+1] = {0};
	char szHMACOrigin1[LENGTH_HMAC+1] = "44546d714a64684f54346736326d4e452f576953534d36684f5479436338457665344e3461744c4757624d3d";
	
	char szHMAC2[LENGTH_HMAC+1] = {0};
	char szHMACOrigin2[LENGTH_HMAC+1] = "4e452f576953544546d714a64684f54346736326d34d36684f5479436338457665344e3461744c4757624d3d";
	
	char szHMAC3[LENGTH_HMAC+1] = {0};
	char szHMACOrigin3[LENGTH_HMAC+1] = "326d4e452f5769535344546d714a64684f543467364d36684f5479436338457665344e3461744c4757624d3d";
	
	char szIKEY_ENC_WB[LENGTH_IKEY+1] = {0};
	char szIKEYOrigin_ENC_WB[LENGTH_IKEY+1] = "CgkLDAgACA0IBwsMCA0HBALDgwHDwgPBw88IBAsFCAkIAgcPBwoIAQgHDg4KCAsCLBQcPCAcKDgoMCA0OBgsMCAsLCAILDAoMCA0OBAoAAgKCgsKDAsOCA==";
	
	char szIKEY[LENGTH_IKEY+1] = {0};
	char szIKEYOrigin[LENGTH_IKEY+1] = "CAILDAoMCA0OBAoLDgwHDwgPBw8LBQcPCAcKDgoMCA0OBgsMCAsLAAgKCgsKDAsOCA8IBAsFCAkIAgcPBwoIAQgHDg4KCAsCCgkLDAgACA0IBwsMCA0HBA==";
	
	
	char szIKEY_1st[LENGTH_IKEY+1] = {0};
	char szIKEYOrigin_1st[LENGTH_IKEY+1] = "8IBAsFCAkIAgcPBwoIAQgHDg4KCAsCCgkLDAgACA0IBwsMCA0HBALDgwHDwgPBw8LBQcPCAcKDgoMCA0OBgsMCAsLCAILDAoMCA0OBAoAAgKCgsKDAsOCA==";
	
	char szIKEY_2st[LENGTH_IKEY+1] = {0};
	char szIKEYOrigin_2st[LENGTH_IKEY+1] = "LDgwHDwgPBw8LBQcPCAcKDgoMCA0OBgsMCAsLCAILDAoMCA0OBAoAAgKCgsKDAsOCA8IBAsFCAkIAgcPBwoIAQgHDg4KCAsCCgkLDAgACA0IBwsMCA0HBA==";
	
	char szIKEY_3st[LENGTH_IKEY+1] = {0};
	char szIKEYOrigin_3st[LENGTH_IKEY+1] = "BAoLDgwHDwgPBw8LBQcPCAcKDgoMCA0OBgsMCAsLAAgKCgsKDAsOCA8IBCAILDAoMCA0OAsFCAkIAgcPBwoIAQgHDg4KCAsCCgkLDAgACA0IBwsMCA0HBA==";
//	char szAPPKEY[LENGTH_APPKEY+1] = {0};
//	char szAPPKEYOrigin[LENGTH_APPKEY+1] = "CgIKCg4MDg4KDw4KCggOCwoPBwUIAA4MCgcIDQsODgQKDgoKBwoKBQoKDggOCAoHCgoHAgoPCggKCAgBCA4KCQoLCggLBQgCCgsKCAgACgAKCAgBCgoHBA==";

	snprintf(szPARAMETER,LENGTH_PARAMETER+1,"%s",argv[2]);
	snprintf(szPARAMETER_WB_kEY,LENGTH_PARAMETER+1,"%s",argv[3]);
	printf("prameter is %s\n",szPARAMETER);
	printf("prameter_WB_kEY is %s\n",szPARAMETER_WB_kEY);
	
	
	int nread1 = 0;
	FILE *fp1 = fopen("hmac1.reinforce","rb");
	if(fp1 == NULL){
		printf("no hmac1.reinforce");
	}
	else{
		//return -1;
	nread1 = fread(szHMAC1,sizeof(char),LENGTH_HMAC,fp1);
	if(nread1 == 0 || nread1 < LENGTH_HMAC)
	//	return -1;
	fclose(fp1);
	}
	
	
	int nread2 = 0;
	FILE *fp2 = fopen("ikey_d.reinforce","rb");
	if(fp2 == NULL)
		{
		printf("no ikey_d.reinforce");
	}
	else{
		//return -1;
	nread2 = fread(szIKEY,sizeof(char),LENGTH_IKEY,fp2);
	if(nread2 == 0 || nread2 < LENGTH_IKEY)
		//return -1;
	fclose(fp2);
	}
	
	
	
	
	int nread3 = 0;
	FILE *fp3 = fopen("hmac2.reinforce","rb");
	if(fp3 == NULL)
		{
		printf("no hmac2.reinforce");
	}
	else{
	//	return -1;
	nread3 = fread(szHMAC2,sizeof(char),LENGTH_HMAC,fp3);
	if(nread3 == 0 || nread3 < LENGTH_HMAC)
	//	return -1;
	fclose(fp3);
	}
	
	
	
	
	int nread5 = 0;
	FILE *fp5 = fopen("ikey1.reinforce","rb");
	if(fp5 == NULL)
		{
		printf("no ikey1.reinforce");
	}
	else{
	//	return -1;
	nread5 = fread(szIKEY_1st,sizeof(char),LENGTH_IKEY,fp5);
	if(nread5 == 0 || nread5 < LENGTH_IKEY)
		return -1;
	fclose(fp5);
	}
	
	int nread6 = 0;
	FILE *fp6 = fopen("ikey2.reinforce","rb");
	if(fp6 == NULL)
		{
		printf("no ikey2.reinforce");
	}
	else{
		//return -1;
	nread6 = fread(szIKEY_2st,sizeof(char),LENGTH_IKEY,fp6);
	if(nread6 == 0 || nread6 < LENGTH_IKEY)
		return -1;
	fclose(fp6);
	}
	
	
	int nread7 = 0;
	FILE *fp7 = fopen("ikey_ENC_WB.reinforce","rb");
	if(fp7 == NULL)
		{
		printf("no ikey_ENC_WB.reinforce");
	}
	else{
		//return -1;
	nread7 = fread(szIKEY_ENC_WB,sizeof(char),LENGTH_IKEY,fp7);
	if(nread7 == 0 || nread7 < LENGTH_IKEY)
		return -1;
	fclose(fp7);
	}
	
	
	
	printf("szIKEY_1st is %s\n",szIKEY_1st);
	printf("szIKEY_2st is %s\n",szIKEY_2st);
	
	printf("szHMAC1 is %s\n",szHMAC1);
	printf("szHMAC2 is %s\n",szHMAC2);
	
	
	printf("szIKEY is %s\n",szIKEY);
	
	printf("ikey_ENC_WB is %s\n",szIKEY_ENC_WB);
	
	printf("szPARAMETER is %s\n",szPARAMETER);
	
	
//
//	int nread3 = 0;
//	FILE *fp3 = fopen("key.reinforce","rb");
//	if(fp3 == NULL)
//		return -1;
//	nread3 = fread(szAPPKEY,sizeof(char),LENGTH_APPKEY,fp3);
//	if(nread3 == 0 || nread3 < LENGTH_APPKEY)
//		return -1;
//	fclose(fp3);


//if the file length is 0,assert bug
    	assert(fstat(fdin,&statbuf) == 0);
    	printf("file size = %ld\n",statbuf.st_size);
//if the file length is not equal as FILE_SIZE,exit
/*
   	if( statbuf.st_size != FILE_SIZE )
	{
		printf("Need Update!...\n");
		exit(1);
	}
*/
//map the file into the memory
	str = (char*)mmap( 0, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fdin, 0);
	if( str == MAP_FAILED )
	{
		printf("mmap error for fdin.\n");
		return 1;
	}

	printf("replace the ikey_d\n");
	for (i = 0; i < statbuf.st_size - LENGTH_IKEY; i++ )
	{
		if ( memcmp(str + i , szIKEYOrigin , LENGTH_IKEY) == 0 )
		{
			printf("replace the pawword at : %d.\r\n\r\n",i);
			memcpy( str+i, szIKEY, LENGTH_IKEY);
			//break;
		}
	}
	
	
	printf("replace the ikey1\n");
	for (i = 0; i < statbuf.st_size - LENGTH_IKEY; i++ )
	{
		if ( memcmp(str + i , szIKEYOrigin_1st , LENGTH_IKEY) == 0 )
		{
			printf("replace the pawword at : %d.\r\n\r\n",i);
			memcpy( str+i, szIKEY_1st, LENGTH_IKEY);
			//break;
		}
	}
	
	printf("replace the ikey2\n");
	for (i = 0; i < statbuf.st_size - LENGTH_IKEY; i++ )
	{
		if ( memcmp(str + i , szIKEYOrigin_2st , LENGTH_IKEY) == 0 )
		{
			printf("replace the pawword at : %d.\r\n\r\n",i);
			memcpy( str+i, szIKEY_2st, LENGTH_IKEY);
			//break;
		}
	}
	
	printf("replace the ikey3\n");
	for (i = 0; i < statbuf.st_size - LENGTH_IKEY; i++ )
	{
		if ( memcmp(str + i , szIKEYOrigin_3st , LENGTH_IKEY) == 0 )
		{
			printf("replace the pawword at : %d.\r\n\r\n",i);
			memcpy( str+i, szIKEY_3st, LENGTH_IKEY);
			//break;
		}
	}

	
	printf("replace the szIKEY_ENC_WB\n");
	for (i = 0; i < statbuf.st_size - LENGTH_IKEY; i++ )
	{
		if ( memcmp(str + i , szIKEYOrigin_ENC_WB , LENGTH_IKEY) == 0 )
		{
			printf("replace the pawword at : %d.\r\n\r\n",i);
			memcpy( str+i, szIKEY_ENC_WB, LENGTH_IKEY);
			//break;
		}
	}
	
	printf("replace the szHMAC1\n");
		for (i = 0; i < statbuf.st_size - LENGTH_HMAC; i++ )
		{
			if ( memcmp(str + i , szHMACOrigin1 , LENGTH_HMAC) == 0 )
			{
				printf("replace the szHMAC1 at : %d.\r\n\r\n",i);
				memcpy( str+i, szHMAC1, LENGTH_HMAC);
//				break;
			}
		}
		
		
		
	printf("replace the szHMAC2\n");
	for (i = 0; i < statbuf.st_size - LENGTH_HMAC; i++ )
	{
		if ( memcmp(str + i , szHMACOrigin2 , LENGTH_HMAC) == 0 )
		{
			printf("replace the szHMAC1 at : %d.\r\n\r\n",i);
			memcpy( str+i, szHMAC2, LENGTH_HMAC);
//				break;
		}
	}
	
	
	printf("replace the szHMAC3\n");
	for (i = 0; i < statbuf.st_size - LENGTH_HMAC; i++ )
	{
		if ( memcmp(str + i , szHMACOrigin3 , LENGTH_HMAC) == 0 )
		{
			printf("replace the szHMAC3 at : %d.\r\n\r\n",i);
			memcpy( str+i, szHMAC3, LENGTH_HMAC);
//				break;
		}
	}
	
	
	printf("replace the PARAMETER\n");
	for (i = 0; i < statbuf.st_size - LENGTH_PARAMETER; i++ )
	{
		if ( memcmp(str + i , szPARAMETEROrigin , LENGTH_PARAMETER) == 0 )
		{
			printf("replace the PARAMETER at : %d.\r\n\r\n",i);
			memcpy( str+i, szPARAMETER, LENGTH_PARAMETER);
//				break;
		}
	}

	printf("replace the PARAMETER_WB_key\n");
	for (i = 0; i < statbuf.st_size - LENGTH_PARAMETER; i++ )
	{
		if ( memcmp(str + i , szPARAMETEROrigin_WB_kEY , LENGTH_PARAMETER) == 0 )
		{
			printf("replace the PARAMETER at : %d.\r\n\r\n",i);
			memcpy( str+i, szPARAMETER_WB_kEY, LENGTH_PARAMETER);
//				break;
		}
	}
	
	
	

//call off mmap
     munmap(str, statbuf.st_size);
     close(fdin);

    return 0;
}
