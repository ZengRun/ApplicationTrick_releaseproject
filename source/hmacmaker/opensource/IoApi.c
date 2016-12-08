#include "../inc/KeyIOApi.h"
#include <stdio.h>
#include <memory.h>

int ompReadFile(const char *filepath, const int len, unsigned char *out)
{
	FILE *fp = NULL;
	size_t retlen = 0;
	int ret = 0;

	if(filepath == NULL || len <1 || out == NULL)
	{
		ret = KIA_NULL_PTR_ERR;
		return ret;
	}

	fp = fopen(filepath, "rb");
	if(fp == NULL)
	{
		ret = KIA_OPEN_FILE_ERR;
		return ret;
	}

	retlen = fread(out, sizeof(unsigned char), len, fp);
	if(retlen == 0 || retlen < len)
	{
		ret = KIA_READ_LEN_ERR;
	}

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

	//写内容到流中
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
