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
#define ReinforceIKeyFileName	"ikey.reinforce"


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

#ifdef  __cplusplus
}
#endif

#endif
