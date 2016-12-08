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

//App_id & AppKey saved in *.h file.
//pid, seed, counter, mac save in local file

/*
Name: getPid
Description:get Pid value from local file
Parameters:
*/ 
int getLocalPid(char *pid);

/*
Name: setPid
Description: set Pid to local file
*/
int setLocalPid(const char *pid);

/*
Name: getSeed
Description: get Seed from local file
Parameters:
*/
int getLocalSeed(char *seed);

/*
Name: setSeed
Description: set Seed to local file
*/
int setLocalSeed(const char *seed);

/*
Name: getCounter
Description: get counter from local file
*/
int getLocalCounter(char *counter);

/*
Name: increment Counter
Description: increment counter by step one.
*/
int incLocalCounter();

/*
Name: getFp
Description: get FingerPrint value from local file
*/
int getLocalFp(char *fp);

/*
Name: setFp
Descrition: set FingerPrint value to local file
*/
int setLocalFp(const char *fp);

//#ifdef  __cplusplus
//}
//#endif

//#endif
