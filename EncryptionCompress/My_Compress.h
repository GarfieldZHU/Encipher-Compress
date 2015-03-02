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
typedef struct head{        //哈夫曼树结构定义
	unsigned char b;        //记录字符在数组中的位置
	long count;             //字符出现频率（权值） 
	long parent, lch, rch;  //定义哈夫曼树指针变量
	char bits[256];         //定义存储哈夫曼编码的数组
} T_HuffMan;
void Compress();
void Extract();
void my_compress(char option);
#endif