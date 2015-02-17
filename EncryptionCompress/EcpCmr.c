/*主函数
 *控制基本命令界面以及操作功能跳转
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include "My_Encipher.h"
#include "My_Compress.h"
#include "EcpCmr.h"
#include "tool.h"

int main(){
	FILE *fin;
	FILE *fout;

	char operate = 0;
	
	PrintTitle();
	//printf("%s",text);

	while (operate != QUIT){
		input_tip();
		do{
			scanf("%c", &operate);
		} while (operate == '\n' || operate == ' ');
		getchar();
		switch (operate){
		case ENCIPHER:
			system("cls");
			printf("【加密操作】:\n");

			my_encipher(ENCIPHER);
			break;
		case DECIPHER:
			system("cls");
			printf("【解密操作】:\n");
			my_encipher(DECIPHER);
			break;
		case COMPRESS:
			system("cls");
			printf("【压缩操作】:\n");
			my_compress(COMPRESS);
			break;
		case EXTRACT:
			system("cls");
			printf("【解压操作】:\n");
			my_compress(EXTRACT);
			break;
		case HELP:
			PrintGuide();
			break;
		case QUIT: 
			break;
		default:
			PrintWrong();
			break;
		}
	}
	PrintTail();
	system("pause");
	return 0;
}