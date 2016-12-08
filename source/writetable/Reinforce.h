/*
 * Reinforce.h
 */

#ifndef REINFORCE_H
#define REINFORCE_H

#ifdef  __cplusplus
extern "C" {
#endif

#define ReinforceKeyFileName	"key.reinforce"
#define ReinforceSeedFileName 	"seed.reinforce"
#define ReinforceWBKeyFileName	"ikey_wb16.reinforce"
#define ReinforceIKeyFileName	"ikey_d.reinforce"

#define ReinforceHmac1FileName	"ikey1.reinforce"
#define ReinforceHmac2FileName	"ikey2.reinforce"
#define ReinforceHmac3FileName	"ikey3.reinforce"


/*
Name: getAppKey
Description: Get app key.
Parameter:
	appkey:	32 chars*/
int getAppKey(char *appKey, const char *path);

/*
Name: getSeed
Description: Get seed.
Parameter:
	seed: 44(base64 code)*/
int getSeed(char *seed, const char *path);

/*
Name: setSeed
Description: set seed.
Parameter:
	seed: 44(base64 code)*/
int setSeed(const char *seed, const char *path);



int generateRandomKey(char *key,int length);


int setIKey(char *iKey);

#ifdef  __cplusplus
}
#endif

#endif
