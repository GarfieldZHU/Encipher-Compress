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

void ByteToBit(bool *Out, char *In, int bits);// �ֽ���ת����λ�� 
void BitToByte(char *Out, bool *In, int bits);// λ��ת�����ֽ��� 
void keyfc(char *In);   //��Կ���ɺ���
void DES(char Out[8], char In[8], char option); //���ܺ��ĳ���,option=ENCIPHERʱ���ܣ���֮���� 
bool RunDes(char *Out, char *In, int datalength, char *Key, char option);

#endif