//#ifndef KEYIOAPI_H
//#define KEYIOAPI_H

//#ifdef  __cplusplus
//extern "C" {
//#endif

#ifdef unix
#define KIA_NULL_PTR_ERR		0x100100
#define KIA_OPEN_FILE_ERR		0x100101
#define KIA_READ_LEN_ERR		0x100102
#define KIA_DATA_CHANGED_ERR	0x100103
#define KIA_COUNTER_DATA_ERR	0x100104
#else
#define KIA_NULL_PTR_ERR		0x100100
#define KIA_OPEN_FILE_ERR		0x100101
#define KIA_READ_LEN_ERR		0x100102
#define KIA_DATA_CHANGED_ERR	0x100103
#define KIA_COUNTER_DATA_ERR	0x100104
#endif

int ompReadFile(const char *filepath, const int len, unsigned char *out);
int ompWriteFile(const char *filepath, const unsigned char *in, const int len);

