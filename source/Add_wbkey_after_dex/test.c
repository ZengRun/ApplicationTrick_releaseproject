#include <stdio.h>
#include <sys/ptrace.h>
#include <dlfcn.h>
#include "KeyIOApi.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
int main()
{
	int fd,length,ret;
	struct stat stat;
	char enpath[128]="\0";
	//将可变个参数(...)按照format格式化成字符串，然后将其复制到enpath中
	//WBkey.reinforce是白盒查找表
	snprintf(enpath,128,"%s","WBkey.reinforce");
	if((fd = open(enpath, O_RDONLY)) < 0)
	{
		printf("open %s error!\n",enpath);
		return -2;
	}
	if((fstat(fd,&stat)) < 0){
		return -3;
	}

	length = stat.st_size;
	printf("%s length:%d\n",enpath,length);
	char *endata = calloc(length,sizeof(char));
	//endata是WBkey.reinforce的内容
	ret = ompReadFile(enpath, length, endata);

	//a+：以附加方式打开可读写的文件。如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。
	FILE* output = fopen("cmcc_march", "a+");
	if (output == NULL)
	{
	  fputs("File write error\n", stderr);
	  return 1;
	}
	if (fwrite(endata, length, 1, output) != 1)
	{
	  fclose(output);
	  fputs("File write error\n", stderr);
	  return 1;
	}
	fclose(output);
	return 0;

}

