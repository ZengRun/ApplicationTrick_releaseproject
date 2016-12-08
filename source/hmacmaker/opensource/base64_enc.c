/* base64_enc.c */
/*
 *   This file is part of the AVR-Crypto-Lib.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
 * base64 encoder (RFC3548)
 * Author: Daniel Otte
 * License: GPLv3
 * 
 * 
 */

#include "../inc/base64_enc.h"

//#if 1
//#include <avr/pgmspace.h>

char base64_alphabet[64] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
	'w', 'x', 'y', 'z', '0', '1', '2', '3', 
	'4', '5', '6', '7', '8', '9', '+', '/' }; 

static 
char bit6toAscii(unsigned char a){
	a &= (unsigned char)0x3F;
	return base64_alphabet[a];//pgm_read_byte
}

void base64enc(char* dest, void* src, unsigned short length){
	unsigned short i,j;
	unsigned char a[4];
	for(i=0; i<length/3; ++i){
		a[0]= (((unsigned char*)src)[i*3+0])>>2;
		a[1]= (((((unsigned char*)src)[i*3+0])<<4) | ((((unsigned char*)src)[i*3+1])>>4)) & 0x3F;
		a[2]= (((((unsigned char*)src)[i*3+1])<<2) | ((((unsigned char*)src)[i*3+2])>>6)) & 0x3F;
		a[3]= (((unsigned char*)src)[i*3+2]) & 0x3F;
		for(j=0; j<4; ++j){
			*dest++=bit6toAscii(a[j]);
		}
	}
	/* now we do the rest */
	switch(length%3){
		case 0: 
			break;
		case 1:
			a[0]=(((unsigned char*)src)[i*3+0])>>2;
			a[1]=((((unsigned char*)src)[i*3+0])<<4)&0x3F;
			*dest++ = bit6toAscii(a[0]);
			*dest++ = bit6toAscii(a[1]);
			*dest++ = '=';
			*dest++ = '=';
			break;
		case 2:		
			a[0]= (((unsigned char*)src)[i*3+0])>>2;
			a[1]= (((((unsigned char*)src)[i*3+0])<<4) | ((((unsigned char*)src)[i*3+1])>>4)) & 0x3F;
			a[2]= ((((unsigned char*)src)[i*3+1])<<2) & 0x3F;
			*dest++ = bit6toAscii(a[0]);
			*dest++ = bit6toAscii(a[1]);
			*dest++ = bit6toAscii(a[2]);
			*dest++ = '=';
			break;
		default: /* this will not happen! */
			break;	
	}
/*  finalize: */
  	*dest='\0';
}

