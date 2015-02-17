#ifndef _MY_ENCIPHER_H_
#define _MY_ENCIPHER_H_
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "EcpCmr.h"
#include "My_Encipher.h"
#include "tool.h"
void my_encipher(char option);

void ByteToBit(bool *Out, char *In, int bits);// 字节组转换成位组 
void BitToByte(char *Out, bool *In, int bits);// 位组转换成字节组 
void keyfc(char *In);   //密钥生成函数
void DES(char Out[8], char In[8], char option); //加密核心程序,option=ENCIPHER时加密，反之解密 
bool RunDes(char *Out, char *In, int datalength, char *Key, char option);

#endif