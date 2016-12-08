/*
 ============================================================================
 Name        : writetable.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


//#include <stdio.h>
//#include <sys/ptrace.h>
//#include <dlfcn.h>
#include "inc/KeyIOApi.h"
#include "inc/rijndael.h"
#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define Nb 4			//明文行数目或者扩展密钥的行数目

#define Nk 4			//密钥列数目
#define Nr 10			//加密轮数
//#define ReinforceIKeyFileName "ikey.reinforce"
#define KEYBITS_128 128





//S-box
unsigned char Sbox[256] = {
0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

// inverse s-box
unsigned char InvSbox[256] = {
0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};


unsigned char XTIME(unsigned char x){
	return((x<<1)^((x&0x80)?0x1b:0x00));
}
unsigned char multiply(unsigned char a,unsigned char b){
	unsigned char temp[8]={a};
	unsigned char tempmultiply=0x00;
	int i=0;
	for(i=1;i<8;i++){
		temp[i]=XTIME(temp[i-1]);
	}
	tempmultiply=(b&0x01)*a;
	for(i=1;i<=7;i++){
		tempmultiply^=(((b>>i)&0x01)*temp[i]);
	}
	return tempmultiply;
}


//void ShiftRows (unsigned char *State)
//{
//unsigned char tmp;
//
//	//第一行保持不变
////	State[0] = State[0];
////	State[4] = State[4];
////	State[8] = State[8];
////	State[12] = State[12];
//
//	//第二行行移位
//	tmp = State[1], State[1] = State[5];
//	State[5] = State[9], State[9] = State[13], State[13] = tmp;
//
//	//第三行行移位
//	tmp = State[2], State[2] = State[10], State[10] = tmp;
//	tmp = State[6], State[6] = State[14], State[14] = tmp;
//
//	//第三行行移位
//	tmp = State[15], State[15] = State[11];
//	State[11] = State[7], State[7] = State[3], State[3] = tmp;
//
//}

//s盒替换
//void InvSubBytes (unsigned char *State){
//	State[0]=InvSbox[State[0]];
//	State[1]=InvSbox[State[1]];
//	State[2]=InvSbox[State[2]];
//	State[3]=InvSbox[State[3]];
//	State[4]=InvSbox[State[4]];
//	State[5]=InvSbox[State[5]];
//	State[6]=InvSbox[State[6]];
//	State[7]=InvSbox[State[7]];
//	State[8]=InvSbox[State[8]];
//	State[9]=InvSbox[State[9]];
//	State[10]=InvSbox[State[10]];
//	State[11]=InvSbox[State[11]];
//	State[12]=InvSbox[State[12]];
//	State[13]=InvSbox[State[13]];
//	State[14]=InvSbox[State[14]];
//	State[15]=InvSbox[State[15]];
//}
//
//void AddRoundKey (unsigned char *State, unsigned char *Key)
//{
//
//int i;
//	for( i = 0; i < 16; i++ )
//	{
//		State[i] ^= Key[i];
//	}
//}


//s盒替换
//void SubBytes (unsigned char *State){
//	State[0]=Sbox[State[0]];
//	State[1]=Sbox[State[1]];
//	State[2]=Sbox[State[2]];
//	State[3]=Sbox[State[3]];
//	State[4]=Sbox[State[4]];
//	State[5]=Sbox[State[5]];
//	State[6]=Sbox[State[6]];
//	State[7]=Sbox[State[7]];
//	State[8]=Sbox[State[8]];
//	State[9]=Sbox[State[9]];
//	State[10]=Sbox[State[10]];
//	State[11]=Sbox[State[11]];
//	State[12]=Sbox[State[12]];
//	State[13]=Sbox[State[13]];
//	State[14]=Sbox[State[14]];
//	State[15]=Sbox[State[15]];
//}
//
//
//void InvShiftRows (unsigned char *State)
//{
//unsigned char tmp;
//
//	//第一行不变
//	//第二行行逆移位
//	tmp = State[13];
//	State[13] = State[9];
//	State[9] = State[5];
//	State[5] = State[1];
//	State[1] = tmp;
//
//	//第三行行逆移位
//	tmp = State[2];
//	State[2] = State[10];
//	State[10] = tmp;
//	tmp = State[6];
//	State[6] = State[14];
//	State[14] = tmp;
//
//	//第四行行逆移位
//	tmp = State[3];
//	State[3] = State[7];
//	State[7] = State[11];
//	State[11] = State[15];
//	State[15] = tmp;
//}

//列混淆
//void MixColums (unsigned char *State){
//
//	unsigned char NextState[4 * Nb];
//	unsigned char j;
//		// 混淆第0列
//		NextState[0] = multiply(State[0],0x02) ^ multiply(State[1],0x03) ^ State[2] ^ State[3];
//		NextState[1] = State[0] ^ multiply(State[1],0x02) ^ multiply(State[2],0x03) ^ State[3];
//		NextState[2] = State[0] ^ State[1] ^ multiply(State[2],0x02) ^ multiply(State[3],0x03);
//		NextState[3] = multiply(State[0],0x03) ^ State[1] ^ State[2] ^ multiply(State[3],0x02);
//
//
//
//		// 混淆第1列
//		NextState[4] = multiply(State[4],0x02) ^ multiply(State[5],0x03) ^ State[6] ^ State[7];
//		NextState[5] = State[4] ^ multiply(State[5],0x02) ^ multiply(State[6],0x03) ^ State[7];
//		NextState[6] = State[4] ^ State[5] ^ multiply(State[6],0x02) ^ multiply(State[7],0x03);
//		NextState[7] = multiply(State[4],0x03) ^ State[5] ^ State[6] ^ multiply(State[7],0x02);
//
//
//		// 混淆第2列
//		NextState[8] = multiply(State[8],0x02) ^ multiply(State[9],0x03) ^ State[10] ^ State[11];
//		NextState[9] = State[8] ^ multiply(State[9],0x02) ^ multiply(State[10],0x03) ^ State[11];
//		NextState[10]  = State[8] ^ State[9] ^ multiply(State[10],0x02) ^ multiply(State[11],0x03);
//		NextState[11]  = multiply(State[8],0x03) ^ State[9] ^ State[10] ^ multiply(State[11],0x02);
//
//
//
//		//混淆第3列
//		NextState[12] = multiply(State[12],0x02) ^ multiply(State[13],0x03)^ State[14] ^ State[15];
//		NextState[13] = State[12] ^ multiply(State[13],0x02) ^ multiply(State[14],0x03)^ State[15];
//		NextState[14] = State[12] ^ State[13] ^ multiply(State[14],0x02) ^ multiply(State[15],0x03);
//		NextState[15] = multiply(State[12],0x03) ^ State[13] ^ State[14] ^ multiply(State[15],0x02);
//
//		for(j=0;j<sizeof(NextState);j++){
//			*(State+j)=*(NextState+j);
//		}
//}

//逆列混淆
//void InvMixSubColumns (unsigned char *State)
//{
//unsigned char NextState[4 * Nb];
//int j;
//
//	//逆列混淆第1列
//	NextState[0] = multiply(State[0],0x0e) ^ multiply(State[1],0x0b) ^ multiply(State[2],0x0d) ^ multiply(State[3],0x09);
//	NextState[1] = multiply(State[0],0x09) ^ multiply(State[1],0x0e) ^ multiply(State[2],0x0b) ^ multiply(State[3],0x0d);
//	NextState[2] = multiply(State[0],0x0d) ^ multiply(State[1],0x09) ^ multiply(State[2],0x0e) ^ multiply(State[3],0x0b);
//	NextState[3] = multiply(State[0],0x0b) ^ multiply(State[1],0x0d) ^ multiply(State[2],0x09) ^ multiply(State[3],0x0e);
//
//	//逆列混淆第2列
//	NextState[4] = multiply(State[4],0x0e) ^ multiply(State[5],0x0b) ^ multiply(State[6],0x0d) ^ multiply(State[7],0x09);
//	NextState[5] = multiply(State[4],0x09) ^ multiply(State[5],0x0e) ^ multiply(State[6],0x0b) ^ multiply(State[7],0x0d);
//	NextState[6] = multiply(State[4],0x0d) ^ multiply(State[5],0x09) ^ multiply(State[6],0x0e) ^ multiply(State[7],0x0b);
//	NextState[7] = multiply(State[4],0x0b) ^ multiply(State[5],0x0d) ^ multiply(State[6],0x09) ^multiply(State[7],0x0e);
//
//	//逆列混淆第3列
//	NextState[8] = multiply(State[8],0x0e) ^ multiply(State[9],0x0b) ^ multiply(State[10],0x0d) ^ multiply(State[11],0x09);
//	NextState[9] = multiply(State[8],0x09) ^ multiply(State[9],0x0e) ^ multiply(State[10],0x0b) ^ multiply(State[11],0x0d);
//	NextState[10]  = multiply(State[8],0x0d) ^ multiply(State[9],0x09) ^ multiply(State[10],0x0e) ^ multiply(State[11],0x0b);
//	NextState[11]  = multiply(State[8],0x0b) ^ multiply(State[9],0x0d) ^ multiply(State[10],0x09) ^ multiply(State[11],0x0e);
//
//	//逆列混淆第4列
//	NextState[12] = multiply(State[12],0x0e) ^ multiply(State[13],0x0b)^ multiply(State[14],0x0d) ^ multiply(State[15],0x09);
//	NextState[13] = multiply(State[12],0x09) ^ multiply(State[13],0x0e) ^ multiply(State[14],0x0b) ^ multiply(State[15],0x0d);
//	NextState[14] = multiply(State[12],0x0d) ^ multiply(State[13],0x09) ^ multiply(State[14],0x0e) ^ multiply(State[15],0x0b);
//	NextState[15] = multiply(State[12],0x0b) ^ multiply(State[13],0x0d)^ multiply(State[14],0x09) ^ multiply(State[15],0x0e);
//
//	for(j=0;j<sizeof(NextState);j++){
//		*(State+j)=*(NextState+j);
//	}
//}


//写入文件的一个封装
int WriteFile(const char * filepath, unsigned char *input,const int len){
		 FILE * fp=NULL;
		 size_t retlen=0;
		 int ret =0;
		 if(filepath==NULL||len<1||input ==NULL){
		 printf("filepath==NULL||len<1||input ==NULL\n");
		 ret = 0x1001101;
		 fclose(fp);
		 fp=NULL;
		 }
		 //fp=fopen(filepath,"ab");
		 fp=fopen(filepath,"w+");

		 if(fp==NULL)
		 {
			 printf("fp==NULL\n");
			 fclose(fp);
			 fp=NULL;
			 ret = 0x1001101;
		 }

		 retlen = fwrite(input,sizeof(unsigned char),len,fp);
		// printf("\nlen is %d ",len);
		// printf("\n%#x ",input[len-1]);

		 if(retlen!=len){
			 ret = 0x1001101;
			 printf("retlen!=len\n");
			 fclose(fp);
			 fp=NULL;
		 }
		 return ret;
	 }





unsigned char Rcon[11] = {
0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

//产生轮密钥
void ExpandKey (unsigned char *key, unsigned char *ExpKey)
{
unsigned char tmp0, tmp1, tmp2, tmp3, tmp4;
unsigned idx;


	for( idx = 0; idx < Nk; idx++ ) {
		ExpKey[4*idx+0] = key[4 * idx + 0];
		ExpKey[4*idx+1] = key[4 * idx + 1];
		ExpKey[4*idx+2] = key[4 * idx + 2];
		ExpKey[4*idx+3] = key[4 * idx + 3];
	}

	for( idx = Nk; idx < Nb * (Nr + 1); idx++ ) {
		tmp0 = ExpKey[4*idx - 4];
		tmp1 = ExpKey[4*idx - 3];
		tmp2 = ExpKey[4*idx - 2];
		tmp3 = ExpKey[4*idx - 1];
		if( !(idx % Nk) ) {
			tmp4 = tmp3;
			tmp3 = Sbox[tmp0];
			tmp0 = Sbox[tmp1] ^ Rcon[idx/Nk];
			tmp1 = Sbox[tmp2];
			tmp2 = Sbox[tmp4];
		}
		ExpKey[4*idx+0] = ExpKey[4*idx - 4*Nk + 0] ^ tmp0;
		ExpKey[4*idx+1] = ExpKey[4*idx - 4*Nk + 1] ^ tmp1;
		ExpKey[4*idx+2] = ExpKey[4*idx - 4*Nk + 2] ^ tmp2;
		ExpKey[4*idx+3] = ExpKey[4*idx - 4*Nk + 3] ^ tmp3;
	}

	//第11轮轮密钥反行移位
	tmp0=ExpKey[161];
	ExpKey[161]=ExpKey[173];
	ExpKey[173]=ExpKey[169];
	ExpKey[169]=ExpKey[165];
	ExpKey[165]=tmp0;

	tmp0=ExpKey[162];
	ExpKey[162]=ExpKey[170];
	ExpKey[170]=tmp0;
	tmp0=ExpKey[166];
	ExpKey[166]=ExpKey[174];
	ExpKey[174]=tmp0;

	tmp0=ExpKey[163];
	ExpKey[163]=ExpKey[167];
	ExpKey[167]=ExpKey[171];
	ExpKey[171]=ExpKey[175];
	ExpKey[175]=tmp0;
	//第11轮轮密钥反行移位--完
}


//void Encrypt (unsigned char *InText, unsigned char *ExpKey, unsigned char *OutText)
//{
//unsigned idx;
//unsigned char State[Nb * 4];
//int j;
//
//	for( idx = 0; idx < Nb; idx++ ) {
//		State[4*idx+0] = *InText++;
//		State[4*idx+1] = *InText++;
//		State[4*idx+2] = *InText++;
//		State[4*idx+3] = *InText++;
//	}
//    for(j=0;j<9;j++){
//    	ShiftRows(State);
//    	AddRoundKey((unsigned char *)State,(unsigned char *)ExpKey+16*j);
//    	SubBytes(State);
//    	MixColums(State);
//    }
//    ShiftRows(State);
//    AddRoundKey((unsigned char *)State,(unsigned char *)ExpKey+16*9);
//    SubBytes(State);
//    AddRoundKey((unsigned char *)State,(unsigned char *)ExpKey+16*10);
//	for( idx = 0; idx < Nb; idx++ ) {
//		*OutText++ = State[4*idx+0];
//		*OutText++ = State[4*idx+1];
//		*OutText++ = State[4*idx+2];
//		*OutText++ = State[4*idx+3];
//	}
//}

void WriteDecryptTable(unsigned char *ExpKey)
{
unsigned i,j,m;
//unsigned char State[16];


unsigned char temp[160][256];
unsigned char InvMC[16]={0x0e,0x09,0x0d,0x0b,0x0b,0x0e,0x09,0x0d,0x0d,0x0b,0x0e,0x09,0x09,0x0d,0x0b,0x0e};
unsigned char DecTable[592][256];
unsigned char DecTable1[592*256]="";
char path[35];
strcpy(path,"WBkey.reinforce");




//产生混淆项
//unsigned char ConfusNumber[64*9+16*9];
unsigned char ConfusNumber[720];


srand((unsigned)time(NULL));//初始化随机数
	for(i=0;i<576;i++){
		ConfusNumber[i]=rand()%255+1;
	}
	for(j=0;j<144;j++){
	ConfusNumber[576+j]=ConfusNumber[j*4+0]^ConfusNumber[j*4+1]^ConfusNumber[j*4+2]^ConfusNumber[j*4+3];
	}
	//产生混淆项 --完



//遍历10轮所有输入
	for(m=0;m<160;m++){
		temp[m][0]=0x00;
	}
	for(i=0;i<160;i++){
		for(j=1;j<256;j++){
			temp[i][j]=temp[i][j-1]+0x01;
		}
	}



//在下一轮输入中加入混淆项
	for(i=0;i<9;i++){
		for(j=0;j<256;j++){
			temp[16+16*i+0][j]^=ConfusNumber[576+i*16+0];
			temp[16+16*i+1][j]^=ConfusNumber[576+i*16+13];
			temp[16+16*i+2][j]^=ConfusNumber[576+i*16+10];
			temp[16+16*i+3][j]^=ConfusNumber[576+i*16+7];

			temp[16+16*i+4][j]^=ConfusNumber[576+i*16+4];
			temp[16+16*i+5][j]^=ConfusNumber[576+i*16+1];
			temp[16+16*i+6][j]^=ConfusNumber[576+i*16+14];
			temp[16+16*i+7][j]^=ConfusNumber[576+i*16+11];

			temp[16+16*i+8][j]^=ConfusNumber[576+i*16+8];
			temp[16+16*i+9][j]^=ConfusNumber[576+i*16+5];
			temp[16+16*i+10][j]^=ConfusNumber[576+i*16+2];
			temp[16+16*i+11][j]^=ConfusNumber[576+i*16+15];

			temp[16+16*i+12][j]^=ConfusNumber[576+i*16+12];
			temp[16+16*i+13][j]^=ConfusNumber[576+i*16+9];
			temp[16+16*i+14][j]^=ConfusNumber[576+i*16+6];
			temp[16+16*i+15][j]^=ConfusNumber[576+i*16+3];
		}
	}











	//第一轮查找表
	for(i=0;i<16;i++){
		for(m=0;m<256;m++){
			temp[i][m]^=ExpKey[i+160];
		}
	}

	for(i=0;i<16;i++){
		for(m=0;m<256;m++){
			temp[i][m]=InvSbox[temp[i][m]];
		}
	}

	for(i=0;i<16;i++){
		for(m=0;m<256;m++){
			temp[i][m]^=ExpKey[i+144];
		}
	}

	for(j=0;j<4;j++){
				for(m=0;m<256;m++){
					DecTable[16*j+0][m]=multiply(InvMC[0],temp[4*j][m]);
					DecTable[16*j+1][m]=multiply(InvMC[1],temp[4*j][m]);
					DecTable[16*j+2][m]=multiply(InvMC[2],temp[4*j][m]);
					DecTable[16*j+3][m]=multiply(InvMC[3],temp[4*j][m]);

					DecTable[16*j+4][m]=multiply(InvMC[4],temp[4*j+1][m]);
					DecTable[16*j+5][m]=multiply(InvMC[5],temp[4*j+1][m]);
					DecTable[16*j+6][m]=multiply(InvMC[6],temp[4*j+1][m]);
					DecTable[16*j+7][m]=multiply(InvMC[7],temp[4*j+1][m]);

					DecTable[16*j+8][m]=multiply(InvMC[8],temp[4*j+2][m]);
					DecTable[16*j+9][m]=multiply(InvMC[9],temp[4*j+2][m]);
					DecTable[16*j+10][m]=multiply(InvMC[10],temp[4*j+2][m]);
					DecTable[16*j+11][m]=multiply(InvMC[11],temp[4*j+2][m]);

					DecTable[16*j+12][m]=multiply(InvMC[12],temp[4*j+3][m]);
					DecTable[16*j+13][m]=multiply(InvMC[13],temp[4*j+3][m]);
					DecTable[16*j+14][m]=multiply(InvMC[14],temp[4*j+3][m]);
					DecTable[16*j+15][m]=multiply(InvMC[15],temp[4*j+3][m]);

					//加入置乱
					DecTable[16*j+0][m]^=ConfusNumber[16*j+0];
					DecTable[16*j+1][m]^=ConfusNumber[16*j+4];
					DecTable[16*j+2][m]^=ConfusNumber[16*j+8];
					DecTable[16*j+3][m]^=ConfusNumber[16*j+12];

					DecTable[16*j+4][m]^=ConfusNumber[16*j+1];
					DecTable[16*j+5][m]^=ConfusNumber[16*j+5];
					DecTable[16*j+6][m]^=ConfusNumber[16*j+9];
					DecTable[16*j+7][m]^=ConfusNumber[16*j+13];

					DecTable[16*j+8][m]^=ConfusNumber[16*j+2];
					DecTable[16*j+9][m]^=ConfusNumber[16*j+6];
					DecTable[16*j+10][m]^=ConfusNumber[16*j+10];
					DecTable[16*j+11][m]^=ConfusNumber[16*j+14];

					DecTable[16*j+12][m]^=ConfusNumber[16*j+3];
					DecTable[16*j+13][m]^=ConfusNumber[16*j+7];
					DecTable[16*j+14][m]^=ConfusNumber[16*j+11];
					DecTable[16*j+15][m]^=ConfusNumber[16*j+15];
				}
			}
	//第一轮查找表  完


	//第2-9轮查找表
	for(i=0;i<128;i++){
		for(m=0;m<256;m++){
			temp[i+16][m]=InvSbox[temp[i+16][m]];
		}
	}

	for(i=1;i<9;i++){
		for(j=0;j<16;j++){
			for(m=0;m<256;m++){
				temp[i*16+j][m]^=ExpKey[144-16*(i)+j];
			}
		}
	}
	//逆mixcolumcs运算
	for(i=1;i<9;i++){
		for(j=0;j<4;j++){
			for(m=0;m<256;m++){
				DecTable[64*i+16*j+0][m]=multiply(InvMC[0],temp[16*i+4*j][m]);
				DecTable[64*i+16*j+1][m]=multiply(InvMC[1],temp[16*i+4*j][m]);
				DecTable[64*i+16*j+2][m]=multiply(InvMC[2],temp[16*i+4*j][m]);
				DecTable[64*i+16*j+3][m]=multiply(InvMC[3],temp[16*i+4*j][m]);

				DecTable[64*i+16*j+4][m]=multiply(InvMC[4],temp[16*i+4*j+1][m]);
				DecTable[64*i+16*j+5][m]=multiply(InvMC[5],temp[16*i+4*j+1][m]);
				DecTable[64*i+16*j+6][m]=multiply(InvMC[6],temp[16*i+4*j+1][m]);
				DecTable[64*i+16*j+7][m]=multiply(InvMC[7],temp[16*i+4*j+1][m]);

				DecTable[64*i+16*j+8][m]=multiply(InvMC[8],temp[16*i+4*j+2][m]);
				DecTable[64*i+16*j+9][m]=multiply(InvMC[9],temp[16*i+4*j+2][m]);
				DecTable[64*i+16*j+10][m]=multiply(InvMC[10],temp[16*i+4*j+2][m]);
				DecTable[64*i+16*j+11][m]=multiply(InvMC[11],temp[16*i+4*j+2][m]);

				DecTable[64*i+16*j+12][m]=multiply(InvMC[12],temp[16*i+4*j+3][m]);
				DecTable[64*i+16*j+13][m]=multiply(InvMC[13],temp[16*i+4*j+3][m]);
				DecTable[64*i+16*j+14][m]=multiply(InvMC[14],temp[16*i+4*j+3][m]);
				DecTable[64*i+16*j+15][m]=multiply(InvMC[15],temp[16*i+4*j+3][m]);


				//加入置乱
				DecTable[64*i+16*j+0][m]^=ConfusNumber[64*i+16*j+0];
				DecTable[64*i+16*j+1][m]^=ConfusNumber[64*i+16*j+4];
				DecTable[64*i+16*j+2][m]^=ConfusNumber[64*i+16*j+8];
				DecTable[64*i+16*j+3][m]^=ConfusNumber[64*i+16*j+12];

				DecTable[64*i+16*j+4][m]^=ConfusNumber[64*i+16*j+1];
				DecTable[64*i+16*j+5][m]^=ConfusNumber[64*i+16*j+5];
				DecTable[64*i+16*j+6][m]^=ConfusNumber[64*i+16*j+9];
				DecTable[64*i+16*j+7][m]^=ConfusNumber[64*i+16*j+13];

				DecTable[64*i+16*j+8][m]^=ConfusNumber[64*i+16*j+2];
				DecTable[64*i+16*j+9][m]^=ConfusNumber[64*i+16*j+6];
				DecTable[64*i+16*j+10][m]^=ConfusNumber[64*i+16*j+10];
				DecTable[64*i+16*j+11][m]^=ConfusNumber[64*i+16*j+14];

				DecTable[64*i+16*j+12][m]^=ConfusNumber[64*i+16*j+3];
				DecTable[64*i+16*j+13][m]^=ConfusNumber[64*i+16*j+7];
				DecTable[64*i+16*j+14][m]^=ConfusNumber[64*i+16*j+11];
				DecTable[64*i+16*j+15][m]^=ConfusNumber[64*i+16*j+15];





			}
		}
	}

	//第10轮
	for(i=0;i<16;i++){
		for(m=0;m<256;m++){
			temp[144+i][m]=InvSbox[temp[144+i][m]];
		}
	}
	for(i=0;i<16;i++){
		for(m=0;m<256;m++){
			DecTable[576+i][m]=temp[144+i][m]^ExpKey[i];
		}
	}

	//printf("{");
	//for(i=0;i<592;i++){
		//("{");
		//for(j=0;j<256;j++){

		//if(j==255){
		//	printf("%#x",DecTable[i][j]);
		//}
		//else{
		//	printf("%#x,",DecTable[i][j]);
		//}
		//}
		//printf("},\n");
	//}
	//printf("}");

//	printf("\n\n\n\n\n\n\n\n\n\n");

	for(i=0;i<592;i++){
		for(j=0;j<256;j++){
			DecTable1[256*i+j]=DecTable[i][j];
		}
	}

//	for(i=0;i<592*256;i++){
//		if(i%256==0&&i!=0){
//			printf("%#x\n",DecTable1[i]);
//		}
//		else{
//			printf("%#x,",DecTable1[i]);
//		}
//	}


	WriteFile(path,DecTable1,592*256*sizeof(unsigned char));




//
//	for( idx = 0; idx < Nb; idx++ ) {
//		State[4*idx+0] = *InText++;
//		State[4*idx+1] = *InText++;
//		State[4*idx+2] = *InText++;
//		State[4*idx+3] = *InText++;
//	}
//
//	InvShiftRows(State);
//	AddRoundKey ((unsigned char *)State, (unsigned char *)ExpKey + 16 * 10);
//	InvSubBytes(State);
//	AddRoundKey((unsigned char *)State, (unsigned char *)ExpKey + 16 * 9);
//	InvMixSubColumns(State);
//
//	for(i=1;i<9;i++){
//		InvShiftRows(State);
//		InvSubBytes(State);
//		AddRoundKey((unsigned char *)State, (unsigned char *)ExpKey + 16 * (9-i));
//		InvMixSubColumns(State);
//	}
//	InvShiftRows(State);
//	InvSubBytes(State);
//	AddRoundKey((unsigned char *)State, (unsigned char *)ExpKey + 16 * (9-9));
//
//
//
//	for( idx = 0; idx < Nb; idx++ ) {
//		*OutText++ = State[4*idx+0];
//		*OutText++ = State[4*idx+1];
//		*OutText++ = State[4*idx+2];
//		*OutText++ = State[4*idx+3];
//	}
}



//ss 2012/7/31
//功能：把一串字符串转换成16进制数组，每2个字符合并成为1个16进制数组元素，
//把长度是32的字符串转换成长度是16的16进制数组，作为初始密钥。4*4矩阵的密钥。
int Char2Hex(unsigned char *in,unsigned char *Hexout){

	unsigned char temp[32];
	//unsigned char Hexout[16];
	int i,j;
	for(i=0;i<32;i++){
		if(in[i]>=48&&in[i]<=57){
		temp[i]=in[i]-'0';
		}
		else if(in[i]>=65&&in[i]<=70)
		{
		temp[i]=in[i]-'7';
		}

		else{
		return 0;
		}
	}
	for(i=0,j=0;j<16;j++){
		Hexout[j]=(temp[i]<<4)+temp[i+1];
		i=i+2;
	}
return 1;
}



int StrToHex(const char *pbStr,	unsigned char *pbHex)
{
	int i;
	int len = strlen(pbStr);
	for(i=0;i<len*2;i++){
		if(i%2==0){
			if(pbStr[i]>='0' && pbStr[i]<='9')
				pbHex[i/2]=(pbStr[i]-0x30)<<4;
			else if(pbStr[i]>='a' && pbStr[i]<='f')
				pbHex[i/2]=(pbStr[i]-0x57)<<4;
			else if(pbStr[i]>='A' && pbStr[i]<='F')
				pbHex[i/2]=(pbStr[i]-0x37)<<4;
			else
				return 1;
		}
		else{
			if(pbStr[i]>='0' && pbStr[i]<='9')
				pbHex[i/2]|=pbStr[i]-0x30;
			else if(pbStr[i]>='a' && pbStr[i]<='f')
				pbHex[i/2]|=pbStr[i]-0x57;
			else if(pbStr[i]>='A' && pbStr[i]<='F')
				pbHex[i/2]|=pbStr[i]-0x37;
			else
				return 1;
		}
	}
	return 0;
}


int encryptFile_128(char* data,int len,char* pwd,char* outdata)
{
	 unsigned long rk[RKLENGTH(KEYBITS_128)];
	  unsigned char key[KEYLENGTH(KEYBITS_128)];
	  int i;
	  int nrounds;
	  //char* type = "wb+";
	  //char *password;
	 // FILE *output;

	  //password =pwd;

	  for (i = 0; i < sizeof(key); i++){
	    key[i] = pwd[i];
	  }
	//    key[i] = *password != 0 ? *password++ : 0;
	 // printf("key is %s\n",key);


	  //fputs(type,stdout);
	 // printf("\n");

	 // output = fopen(outFile, type);
	  //if (output == NULL)
	  //{
	  //  fputs("File write error\n", stderr);
	  //  return 1;
	  //}
	  nrounds = rijndaelSetupEncrypt(rk, key, KEYBITS_128);

	  int ngroups = len/16;
	  int k;
	  for(k = 0 ;k < ngroups ; k ++)
	  {
		  unsigned char plaintext[16];
		  unsigned char ciphertext[16];
		  int j;
		  for (j = 0; j < sizeof(plaintext); j++)
		  {
			  plaintext[j] = data[k*16 + j];
		  }
		  if (j == 0)
		 	      break;
		  rijndaelEncrypt(rk, nrounds, plaintext, ciphertext);

		  for (j = 0; j < sizeof(ciphertext); j++)
		  {
			  outdata[k*16+j] = ciphertext[j];
		  }


	  }
	  return 1;
}








//int main(int argc, char ** argv)
int main()
{
unsigned char key1[33]="";
unsigned char keyhmac1[33]="";
unsigned char keyhmac2[33]="";
unsigned char keyhmac3[33]="";
unsigned char key3[33]="";
unsigned char encrypted_data[33]="";
unsigned char keyhmac1_data[33]="";
unsigned char keyhmac2_data[33]="";
unsigned char keyhmac3_data[33]="";
unsigned char key2[17]="";
//unsigned char in[16];
//unsigned char OutText[16]={0x75,0x7c,0xcd,0xc,0xdc,0x5c,0x90,0xea,0xdb,0xee,0xec,0xf6,0x38,0xdd,0,0};
unsigned char ExpKey[4*4*11];
//unsigned char i;
int ret;
int i;
//getIKeyForGenerate(key);
//ret = getwbKeyForGenerate(key1);


ret = gethmacForGenerate(keyhmac1,1);
ret = gethmacForGenerate(keyhmac2,2);
//ret = gethmacForGenerate(keyhmac3,3);

ret = getIKeyForGenerate(key3);

//printf("key1 is %s\n",key1);

//for(i=0;i<16;i++){
//  key2[i]=key1[i];
//}

generateRandomKey(key2,16);

printf("key2 is %s\n",key2);

printf("key3 is %s\n",key3);


encryptFile_128(key3,32,key2,encrypted_data);

//printf("encrypted_data is %s\n",encrypted_data);

//for(i=0;i<sizeof(encrypted_data);i++){
//	printf("%#x ",encrypted_data[i]);
//}
//printf("\n");

	setIKey(encrypted_data);

	encryptFile_128(keyhmac1,32,key2,keyhmac1_data);
	setIKey_hmac(keyhmac1_data,1);
	encryptFile_128(keyhmac2,32,key2,keyhmac2_data);
	setIKey_hmac(keyhmac2_data,2);
	//encryptFile_128(keyhmac3,32,key2,keyhmac3_data);
	//setIKey_hmac(keyhmac3_data,3);

  ExpandKey(key2,ExpKey);
  WriteDecryptTable(ExpKey);
//  printf("\nwritetable over!\n");
//	printf("解密前：");
//	for(i=0;i<16;i++){
//		printf("%#x",OutText[i]);
//		printf(" ");
//	}
//	printf("\n解密后：");
//	for(i=0;i<16;i++){
//		printf("%#x",in[i]);
//		printf(" ");
//	}
return 0;
}


