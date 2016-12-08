/*
 * Integrity.h
 */

#ifndef INTEGRITY_H_
#define INTEGRITY_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define HMACFILE "hmac.reinforce"
int generateHmac(char *filePath);
int verifyHmac(char *filePath);
int checkIntegrity(char *packageName);
int apiProtect(char *pathName);

#ifdef  __cplusplus
}
#endif

#endif /* INTEGRITY_H_ */
