#ifndef _MY_COMPRESS_H_
#define _MY_COMPRESS_H_

#define _CRT_SECURE_NO_WARNINGS
#include "My_compress.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include "EcpCmr.h"
#include "tool.h"
typedef struct head{        //���������ṹ����
	unsigned char b;        //��¼�ַ��������е�λ��
	long count;             //�ַ�����Ƶ�ʣ�Ȩֵ�� 
	long parent, lch, rch;  //�����������ָ�����
	char bits[256];         //����洢���������������
} T_HuffMan;
void Compress();
void Extract();
void my_compress(char option);
#endif