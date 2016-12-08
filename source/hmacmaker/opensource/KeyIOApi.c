#include "../inc/KeyIOApi.h"

#include <stdio.h>
#include "../inc/alg_err.h"
#include <memory.h>

#ifdef unix
#define PidFileName			"e:\\test\\pid.key"
#define SeedFileName		"seed.key"
#define CounterFileName		"e:\\test\\counter.key"
#define FpFileName			"fp.key"
#define KIA_PID_LEN				32
#define KIA_PID_ENC_LEN			72
#define KIA_SEED_LEN			32
#define KIA_SEED_ENC_LEN		72
#define KIA_COUNTER_LEN			8
#define KIA_FP_LEN				20
#define KIA_DEFAULT_DEVICE_ID	"87654321"
#else
#define PidFileName			"e:\\test\\pid.key"
#define SeedFileName		"seed.key"
#define CounterFileName		"e:\\test\\counter.key"
#define FpFileName			"fp.key"
#define KIA_PID_LEN				32
#define KIA_PID_ENC_LEN			72
#define KIA_SEED_LEN			32
#define KIA_SEED_ENC_LEN		72
#define KIA_COUNTER_LEN			8
#define KIA_FP_LEN				20
#define KIA_DEFAULT_DEVICE_ID	"87654321"
#endif


int ompReadFile(const char *filepath, const int len, unsigned char *out);
int ompWriteFile(const char *filepath, const unsigned char *in, const int len);

/*
Name: getPid
Description:get Pid value from local file
Parameters:
*/ 

/*
int getLocalPid(char *pid)
{
	char encpid[KIA_PID_ENC_LEN+1] = "";
	char strpid[KIA_PID_ENC_LEN+1] = "";
	char pidex[KIA_PID_LEN+1] = "";
	char hash[41];
	int ret = 0;
	int len = 0;

	ret = ompReadFile(PidFileName, KIA_PID_ENC_LEN, encpid);
	if(ret)
	{
		return ret;
	}

	ret = desecb(KIA_DEFAULT_DEVICE_ID, encpid, KIA_PID_ENC_LEN, strpid, 1);
	if(ret != 0)
	{
		return ret;
	}

	memcpy(pidex, strpid, KIA_PID_LEN);
	ret = getHash(pidex, hash);
	if(ret != 0)
	{
		return ret;
	}

	if(memcmp(strpid+KIA_PID_LEN, hash, 40))
	{
		return KIA_DATA_CHANGED_ERR;
	}

	memcpy(pid,pidex, KIA_PID_LEN);
	pid[KIA_PID_LEN]= '\0';

	return 0;
}
*/
/*
Name: setPid
Description: set Pid to local file
*/
/*
int setLocalPid(const char *pid)
{
	char enlop[KIA_PID_ENC_LEN+1];
    char encpid[KIA_PID_ENC_LEN+10];
	char hashvalue[41];
	int ret;

	if(pid == NULL || strlen(pid) != KIA_PID_LEN) 
	{
		return 1;
	}

	ret = getHash(pid, hashvalue);
	if(ret != 0)
	{
		return ret;
	}

	memcpy(enlop, pid, strlen(pid));
	memcpy(enlop+KIA_PID_LEN,hashvalue, 40);//32+40

	ret = desecb(KIA_DEFAULT_DEVICE_ID,enlop,KIA_PID_ENC_LEN, encpid, 0);
	if(ret != 0)
	{
		return ret;
	}

	ret = ompWriteFile(PidFileName, encpid, KIA_PID_ENC_LEN);
	if(ret != 0)
	{
		return ret;
	}

	return 0;
}*/

/*
Name: getSeed
Description: get Seed from local file
Parameters:
*/

/*
int getLocalSeed(char *seed)
{
	char encseed[KIA_SEED_ENC_LEN+1] = "";
	char strseed[KIA_SEED_ENC_LEN+1] = "";
	char seedex[KIA_SEED_LEN+1] = "";
	char hash[41];
	int ret = 0;
	int len = 0;

	ret = ompReadFile(SeedFileName, KIA_SEED_ENC_LEN, encseed);
	if(ret)
	{
		return ret;
	}

	ret = desecb(KIA_DEFAULT_DEVICE_ID, encseed, KIA_SEED_ENC_LEN, strseed, 1);
	if(ret != 0)
	{
		return ret;
	}

	memcpy(seedex, strseed, KIA_SEED_LEN);
	ret = getHash(seedex, hash);
	if(ret != 0)
	{
		return ret;
	}

	if(memcmp(strseed+KIA_SEED_LEN, hash, 40))
	{
		return KIA_DATA_CHANGED_ERR;
	}

	memcpy(seed,seedex, KIA_SEED_LEN);
	seed[KIA_SEED_LEN]= '\0';

	return 0;
}*/

/*
Name: setSeed
Description: set Seed to local file
*/

/*
int setLocalSeed(const char *seed)
{
	char enlop[KIA_SEED_ENC_LEN+1];
    char encseed[KIA_SEED_ENC_LEN+10];
	char hashvalue[41];
	int ret;

	if(seed == NULL || strlen(seed) != KIA_SEED_LEN) 
	{
		return 1;
	}

	ret = getHash(seed, hashvalue);
	if(ret != 0)
	{
		return ret;
	}

	memcpy(enlop, seed, strlen(seed));
	memcpy(enlop+KIA_SEED_LEN,hashvalue, 40);//32+40

	ret = desecb(KIA_DEFAULT_DEVICE_ID,enlop,KIA_SEED_ENC_LEN, encseed, 0);
	if(ret != 0)
	{
		return ret;
	}

	ret = ompWriteFile(SeedFileName, encseed, KIA_SEED_ENC_LEN);
	if(ret != 0)
	{
		return ret;
	}

	return 0;
}
*/
/*
Name: getCounter
Description: get counter from local file
*/

/*
int getLocalCounter(char *counter)
{
	int ret;

	ret = ompReadFile(CounterFileName, 8, counter);
	if(ret != 0)
	{
		return ret;
	}

	counter[8] = '\0';

	return 0;
}
*/
/*
int incLocalCounter()
{
	int ret;
	int i = 0, j;
	unsigned char counter[32] = "";
	unsigned long ulcounter = 0;
	unsigned long decimals = 1;

	ret = ompReadFile(CounterFileName, 8, counter);
	if(ret != 0)
	{
		return ret;
	}

	for(i = 7;i>=0;i--)
	{
		if(counter[i]>=0x30 && counter[i]<=0x39)
		{
			decimals = 1;
			for(j=0;j<(7-i);j++)
			{
				decimals = decimals*10;
			}

			ulcounter = ulcounter + (counter[i]- 0x30)*decimals;
		}
		else
		{
			return KIA_COUNTER_DATA_ERR;
		}
	}

	ulcounter = (ulcounter+1)%100000000;

	sprintf(counter, "%08d", ulcounter);
	
	ret = ompWriteFile(CounterFileName, counter, 8);
	if(ret != 0)
	{
		return ret;
	}

	return ret;
}
*/
/*
Name: getFp
Description: get FingerPrint value from local file
*/
/*
int getLocalFp(char *fp)
{
	return 0;
}*/

/*
Name: setFp
Descrition: set FingerPrint value to local file
*/
/*
int setLocalFp(const char *fp)
{
	return 0;
}
*/
////////////////////////helper methods for FILE I/O operation////////////////////////
int ompReadFile(const char *filepath, const int len, unsigned char *out)
{
	FILE *fp = NULL;
	size_t retlen = 0;
	int ret = 0;
	
	if(filepath == NULL || len <1 || out == NULL)
	{
		ret = KIA_NULL_PTR_ERR;
		goto Ret;
	}

	fp = fopen(filepath, "rb");
	if(fp == NULL)
	{
		ret = KIA_OPEN_FILE_ERR;
		goto Ret;
	}
	
	retlen = fread(out, sizeof(unsigned char), len, fp);
	if(retlen == 0 || retlen < len)
	{
		ret = KIA_READ_LEN_ERR;
		goto Ret;
	}

Ret:
	if(fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}

	return ret;
}

int ompWriteFile(const char *filepath, const unsigned char *in, const int len)
{
	FILE *fp = NULL;
	size_t retlen = 0;
	int ret = 0;
	
	if(filepath == NULL || len <1 || in == NULL)
	{
		ret = KIA_NULL_PTR_ERR;
		goto Ret;
	}

	fp = fopen(filepath, "wb");
	if(fp == NULL)
	{
		ret = KIA_OPEN_FILE_ERR;
		goto Ret;
	}
	
	retlen = fwrite(in,sizeof(unsigned char), len, fp);
	if(retlen != len)
	{
		ret = KIA_READ_LEN_ERR;
		goto Ret;
	}

Ret:
	if( fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}

	return ret;
}
