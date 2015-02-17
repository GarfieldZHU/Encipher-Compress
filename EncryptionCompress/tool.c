/*工具函数
 *用于控制命令行输入输出提示及操作
 */
#include "tool.h"

/*打印题头界面*/
void PrintTitle(){
	printf("\t\t+--+---------------------------------------+\n");
	printf("\t\t|  |  欢迎使用文件加密及数据压缩系统！  |  |\n");
	printf("\t\t+--+---------------------------------------+\n");
}

/*提示函数，要求输入操作选项*/
void input_tip(){  
	printf("\n>请按照提示选择您的操作：\n");
	printf("*** 1. 加密\n");
	printf("*** 2. 解密\n");
	printf("*** 3. 压缩\n");
	printf("*** 4. 解压\n");
	printf("*** 5. 帮助\n");
	printf("*** q. 退出\n");
	printf(">");
}

/*提示错误选项*/
void PrintWrong(){
	printf("--------------------------------\n");
	printf("输入不正确，请输入正确的操作选项\n");
	printf("--------------------------------\n");
}

/*提示程序结束*/
void PrintTail(){
	printf("\n\t\t+--+-----------------------------------+\n");
	printf("\t\t|  |  谢谢您的支持，欢迎再次使用！  |  |\n");
	printf("\t\t+--+-----------------------------------+\n");
}

/*打印功能说明*/
void PrintGuide(){
	system("cls");
	printf("\t\t\t\t【使用手册】\n\n");
	printf(">感谢您使用本软件！ (≧▽≦)/\n");
	printf(">本软件提供简单的文本文件操作处理功能。\n");
	printf(">包括文件的加密、解密、压缩、解压缩:\n");
	printf(" #加密: 使用DES算法对文本文件实现加密操作，用户在操作界面选择1\n");
	printf("        输入需要加密的文件名然后输入密码,最后输入输出文件即可\n");
	printf("        设置密码是请记住，密码为不长于8位的不为‘*’的任意字符\n");
	printf("        (请牢记使用的密码，避免加密文件无法访问）\n");
	printf(" #解密: 加密的反过程，在操作界面选择2，后续操作相同。\n");
	printf("        请注意使用加密时使用的正确的密码，错误密码不会提示，而是解密为错误文件\n");
	printf(" #压缩: 使用哈夫曼编码对文本文件编码并进行压缩，在操作界面选择3\n");
	printf("        输入需要压缩的文件名，再输入输出文件名即可,\n");
	printf("        完成压缩后提供在，压缩百分比计算\n");
	printf("        (注意压缩及解压中压缩后文件格式为.zy，输入文件名时只需名字，无需后缀)\n");
	printf(" #解压: 压缩的反过程，在操作界面选项为4\n");
	printf("        输入无需后缀的文件名，再输入输出文件名即可\n");
	printf(">目前程序仅对文本文件(.txt)有良好支持，操作其他文件时请及时备份以免数据丢失。\n");
	printf(">加密/解密/压缩/解压算法均对汉字也有可靠的支持，请放心使用\n");
	printf(">PS: 文本文件输入格式为：“完整目录” + “文件名” + “后缀”\n");
	printf("     例如：  F：\\File\\input.txt \n");
	printf("     压缩后文件为“完整目录” + “文件名” \n\n\n");

	system("pause");
	system("cls");
	PrintTitle();

}

/*输入文件函数*/
FILE *input_file(){ 
	FILE *fin;
	char fileName[40];
	printf("\n>请输入您要进行操作的文件（格式例：f:\\input.txt）:\n>");
	scanf("%s", fileName);
	fin = fopen(fileName, "rb+");

	while (!fin){
		printf(">输入格式不正确或文件不存在 @_@ ,请重新输入：\n>");
		scanf("%s", fileName);
		fin = fopen(fileName, "rb+");
	}
	getchar();
	printf("\n");
	return fin;
}

/*写入文件函数*/
void output_to_file(char *res, int len, char option){
	//printf("\nfile_len:%d\nres:%s\n", len, res);  //测试码，请注释
	FILE *fout;
	char fileName[40];
	switch(option){
		case ENCIPHER:
			printf("\n>加密已完成  O(∩_∩)O\n");   break;
		case DECIPHER:
			printf("\n>解密已完成  O(∩_∩)O\n");   break;
	}
	printf(">----请输入要保存至的文件目录(不存在将自动被创建)：\n>");
	scanf("%s", fileName);
	fout = fopen(fileName, "wb+");

	while (!fout){
		printf(">输入格式不正确或目录不存在 @_@ ，请重新输入：\n>");
		scanf("%s", fileName);
		fout = fopen(fileName, "wb+");
	}
	getchar();
	fwrite(res, len, 1, fout);
	printf("\n>写入完成!\n");
	printf(">请到目录“%s”下查看解密后的文件。\n",fileName);
	fclose(fout);
}