/*
 * Integrity.h
 */

#ifndef INTEGRITY_H_
#define INTEGRITY_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "inc/des.h"
#include "inc/md5.h"
#include "inc/sha1.h"
#include "inc/sha256.h"
#include "inc/aes_cbc.h"
#include "inc/hmac_md5.h"
#include "inc/hmac_sha1.h"
#include "inc/base64_enc.h"
#include "inc/base64_dec.h"
#include "inc/hmac_sha256.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>



#define HMACFILE "hmac.reinforce"
#define RESDATAFILE  "res.reinforce"
#define FLENFILE "flen.reinforce"

int generateHmac(char *resPath,char *filePath,char *apkpath);
int verifyHmac(char *filePath);
int checkIntegrity(char *packageName);
int apiProtect(char *pathName);

#ifdef  __cplusplus
}
#endif

#endif /* INTEGRITY_H_ */
