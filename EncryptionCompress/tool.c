/*���ߺ���
 *���ڿ������������������ʾ������
 */
#include "tool.h"

/*��ӡ��ͷ����*/
void PrintTitle(){
	printf("\t\t+--+---------------------------------------+\n");
	printf("\t\t|  |  ��ӭʹ���ļ����ܼ�����ѹ��ϵͳ��  |  |\n");
	printf("\t\t+--+---------------------------------------+\n");
}

/*��ʾ������Ҫ���������ѡ��*/
void input_tip(){  
	printf("\n>�밴����ʾѡ�����Ĳ�����\n");
	printf("*** 1. ����\n");
	printf("*** 2. ����\n");
	printf("*** 3. ѹ��\n");
	printf("*** 4. ��ѹ\n");
	printf("*** 5. ����\n");
	printf("*** q. �˳�\n");
	printf(">");
}

/*��ʾ����ѡ��*/
void PrintWrong(){
	printf("--------------------------------\n");
	printf("���벻��ȷ����������ȷ�Ĳ���ѡ��\n");
	printf("--------------------------------\n");
}

/*��ʾ�������*/
void PrintTail(){
	printf("\n\t\t+--+-----------------------------------+\n");
	printf("\t\t|  |  лл����֧�֣���ӭ�ٴ�ʹ�ã�  |  |\n");
	printf("\t\t+--+-----------------------------------+\n");
}

/*��ӡ����˵��*/
void PrintGuide(){
	system("cls");
	printf("\t\t\t\t��ʹ���ֲ᡿\n\n");
	printf(">��л��ʹ�ñ������ (�R���Q)/\n");
	printf(">������ṩ�򵥵��ı��ļ����������ܡ�\n");
	printf(">�����ļ��ļ��ܡ����ܡ�ѹ������ѹ��:\n");
	printf(" #����: ʹ��DES�㷨���ı��ļ�ʵ�ּ��ܲ������û��ڲ�������ѡ��1\n");
	printf("        ������Ҫ���ܵ��ļ���Ȼ����������,�����������ļ�����\n");
	printf("        �������������ס������Ϊ������8λ�Ĳ�Ϊ��*���������ַ�\n");
	printf("        (���μ�ʹ�õ����룬��������ļ��޷����ʣ�\n");
	printf(" #����: ���ܵķ����̣��ڲ�������ѡ��2������������ͬ��\n");
	printf("        ��ע��ʹ�ü���ʱʹ�õ���ȷ�����룬�������벻����ʾ�����ǽ���Ϊ�����ļ�\n");
	printf(" #ѹ��: ʹ�ù�����������ı��ļ����벢����ѹ�����ڲ�������ѡ��3\n");
	printf("        ������Ҫѹ�����ļ���������������ļ�������,\n");
	printf("        ���ѹ�����ṩ�ڣ�ѹ���ٷֱȼ���\n");
	printf("        (ע��ѹ������ѹ��ѹ�����ļ���ʽΪ.zy�������ļ���ʱֻ�����֣������׺)\n");
	printf(" #��ѹ: ѹ���ķ����̣��ڲ�������ѡ��Ϊ4\n");
	printf("        ���������׺���ļ���������������ļ�������\n");
	printf(">Ŀǰ��������ı��ļ�(.txt)������֧�֣����������ļ�ʱ�뼰ʱ�����������ݶ�ʧ��\n");
	printf(">����/����/ѹ��/��ѹ�㷨���Ժ���Ҳ�пɿ���֧�֣������ʹ��\n");
	printf(">PS: �ı��ļ������ʽΪ��������Ŀ¼�� + ���ļ����� + ����׺��\n");
	printf("     ���磺  F��\\File\\input.txt \n");
	printf("     ѹ�����ļ�Ϊ������Ŀ¼�� + ���ļ����� \n\n\n");

	system("pause");
	system("cls");
	PrintTitle();

}

/*�����ļ�����*/
FILE *input_file(){ 
	FILE *fin;
	char fileName[40];
	printf("\n>��������Ҫ���в������ļ�����ʽ����f:\\input.txt��:\n>");
	scanf("%s", fileName);
	fin = fopen(fileName, "rb+");

	while (!fin){
		printf(">�����ʽ����ȷ���ļ������� @_@ ,���������룺\n>");
		scanf("%s", fileName);
		fin = fopen(fileName, "rb+");
	}
	getchar();
	printf("\n");
	return fin;
}

/*д���ļ�����*/
void output_to_file(char *res, int len, char option){
	//printf("\nfile_len:%d\nres:%s\n", len, res);  //�����룬��ע��
	FILE *fout;
	char fileName[40];
	switch(option){
		case ENCIPHER:
			printf("\n>���������  O(��_��)O\n");   break;
		case DECIPHER:
			printf("\n>���������  O(��_��)O\n");   break;
	}
	printf(">----������Ҫ���������ļ�Ŀ¼(�����ڽ��Զ�������)��\n>");
	scanf("%s", fileName);
	fout = fopen(fileName, "wb+");

	while (!fout){
		printf(">�����ʽ����ȷ��Ŀ¼������ @_@ �����������룺\n>");
		scanf("%s", fileName);
		fout = fopen(fileName, "wb+");
	}
	getchar();
	fwrite(res, len, 1, fout);
	printf("\n>д�����!\n");
	printf(">�뵽Ŀ¼��%s���²鿴���ܺ���ļ���\n",fileName);
	fclose(fout);
}