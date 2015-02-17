/*����/���ܷ���*/

#include "My_Encipher.h"

/*���ļ���ȡ���м���/����*/
void my_encipher(char option){
	FILE *fin = input_file();
	FILE *fout;
	int file_len, i, j;
	char *text;  //ÿ�ζ�8�ֽڽ��м��ܣ�ѭ���������ļ�β
	char key[9] = {'\0'};      //�ܳ�Ĭ���8�ֽ�
	char *res;
	char ser;

	fseek(fin, 0, SEEK_END);
	file_len = ftell(fin);
	text = (char *)malloc(sizeof(char)*(file_len / 8 + 1) * 8);   //���ڼ��ܲ���8����ʱ�����ַ�
	res = (char *)malloc(sizeof(char)*(file_len / 8 + 1) * 8 );    //��󳤶ȿ��ܳ���file_len
	ZeroMemory(text, (file_len / 8 + 1) * 8);
	ZeroMemory(res, (file_len / 8 + 1) * 8 );
	fseek(fin, 0, SEEK_SET);
	fread(text, file_len, 1, fin);


	if (option == ENCIPHER){  //������ʾ����Ӧ����
		printf(">�������Ը��ļ����м��ܲ���\n");
		printf("-----���������Ҫʹ�õ�����\n");
		printf("-----(����ʹ�������ַ�(*����)�����8λ)��\n>");
		for (i = 0; i < 8; i++) {  //�ܳ��8λ����������
			scanf("%c", &ser);
			if (ser == '\n'){  //�ܳײ����8λʱ��*�Ų���
				for (j = i; j < 8; j++) key[j] = '*';
				i = 8;
			}
			else
				key[i] = ser;
		}
	
		//res[0] = file_len % 8 + '0';
		printf("\n");		//����λ���ı�������8�����Ĳ�ֵ
		if (file_len % 8 == 0){
			RunDes(res, text, file_len, key, ENCIPHER);
			output_to_file(res, file_len, ENCIPHER);
		}
		else{
			RunDes(res, text, (file_len / 8 + 1) * 8, key, ENCIPHER);
			output_to_file(res, (file_len / 8 + 1) * 8, ENCIPHER);
		}
		printf("--------��������Ϊ�� ");
		for (i = 0; i < 8; i++) 
			if (key[i] != '*') printf("%c",key[i]);
		printf("\n--------���ס���������Ա�֤����ȷ���ܡ�\n");
	}

	else if(option == DECIPHER){   //������ʾ����Ӧ����
		printf(">�������Ը��ļ����н��ܲ���\n");
		printf("-----��������ļ��ļ������룺\n>");
		for (i = 0; i < 8; i++) {
			scanf("%c", &ser);
			if (ser == '\n'){
				for (j = i; j < 8; j++) key[j] = '*';
				i = 8;
			}
			else
				key[i] = ser;
		}
		printf("\n");
		RunDes(res, text, file_len, key, DECIPHER);
		output_to_file(res, file_len, DECIPHER);
	}
	
	//printf("file_len:%d\ntext:%s\nkey: %s\n", file_len, text, key);  //�����룬��ע��
	//printf("\nAfter DES:%s\n",res);   //�����룬��ע��
	free(text);
	free(res);
	fclose(fin);
	printf(">");
	system("pause");
	system("cls");
}


/*-------------------------------------------------------------*/
/*
 *DES��/�����㷨���Ĳ���
 */

const static char IP_Table[] = {       //IP_Table�û�     
	58, 50, 42, 34, 26, 18, 10,  2,     
	60, 52, 44, 36, 28, 20, 12,  4,     
	62, 54, 46, 38, 30, 22, 14,  6,     
	64, 56, 48, 40, 32, 24, 16,  8,     
	57, 49, 41, 33, 25, 17,  9,  1,     
	59, 51, 43, 35, 27, 19, 11,  3,     
	61, 53, 45, 37, 29, 21, 13,  5,     
	63, 55, 47, 39, 31, 23, 15,  7 
};            

const static char Final_Table[] = {     //�����û�     
		40,  8, 48, 16, 56, 24, 64, 32,     
		39,  7, 47, 15, 55, 23, 63, 31,     
		38,  6, 46, 14, 54, 22, 62, 30,     
		37,  5, 45, 13, 53, 21, 61, 29,     
		36,  4, 44, 12, 52, 20, 60, 28,     
		35,  3, 43, 11, 51, 19, 59, 27,     
		34,  2, 42, 10, 50, 18, 58, 26,     
		33,  1, 41,  9, 49, 17, 57, 25 
}; 

const static char S_Box[8][64] = {    //s_box     
	/* S1 */     
	{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,   
	0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,   
	4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,     
	15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},     
	/* S2 */     
	{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,   
	3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,   
	0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,     
	13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},      
	/* S3 */     
	{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,     
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,     
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,  
	1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}, 
	/* S4 */
	{ 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 
	13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9, 
	10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, 
	3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 },      
	/* S5 */
	{ 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
	14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6, 
	4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, 
	11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 },      
	/* S6 */
	{ 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 
	10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8, 
	9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, 
	4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 },      
	/* S7 */
	{ 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
	13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6, 
	1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, 
	6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 },      
	/* S8 */
	{ 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 
	1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2, 
	7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8, 
	2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 } 
};

const static char Rar_Table[] = {    //ѹ���û�     
	14, 17, 11, 24,  1,  5,   
	3, 28, 15,  6, 21, 10,     
	23, 19, 12,  4, 26,  8,     
	16,  7, 27, 20, 13,  2,     
	41, 52, 31, 37, 47, 55,     
	30, 40, 51, 45, 33, 48,     
	44, 49, 39, 56, 34, 53,     
	46, 42, 50, 36, 29, 32 
}; 

const static char Exp_Table[] = {    //��չ�û�     
	32,  1,  2,  3,  4,  5,   
	4,  5,  6,  7,  8,  9, 
	8, 9, 10, 11, 12, 13, 
	12, 13, 14, 15, 16, 17, 
	16, 17, 18, 19, 20, 21, 
	20, 21, 22, 23, 24, 25, 
	24, 25, 26, 27, 28, 29, 
	28, 29, 30, 31, 32, 1 
};

const static char P_Table[] = {     //P�û� 
	16, 7, 20, 21, 
	29, 12, 28, 17,
	1, 15, 23, 26, 
	5, 18, 31, 10, 
	2, 8, 24, 14, 
	32, 27, 3, 9, 
	19, 13, 30, 6, 
	22, 11, 4, 25
};

const static char KeyRar_Table[] = { 
	57, 49, 41, 33, 25, 17, 9, 
	1, 58, 50, 42, 34, 26, 18, 
	10, 2, 59, 51, 43, 35, 27, 
	19, 11, 3, 60, 52, 44, 36, 
	63, 55, 47, 39, 31, 23, 15, 
	7, 62, 54, 46, 38, 30, 22, 
	14, 6, 61, 53, 45, 37, 29, 
	21, 13, 5, 28, 20, 12, 4 
};

//����ȫ�ֱ�����16����Կ 
bool key[16][48]={{0}};  

void ByteToBit(bool *Out, char *In, int bits){  //�ֽڵ�λת������ 
	int i;  
	for (i = 0; i<bits; i++)   
		Out[i] = (In[i / 8] >> (i % 8)) & 1;
}

void BitToByte(char *Out, bool *In, int bits){ //λ���ֽ�ת������ 
	int i;  
	for (i = 0; i<bits / 8; i++)  
		Out[i] = 0;  
	for (i = 0; i<bits; i++)  
		Out[i / 8] |= In[i] << (i % 8);
}
		
void Xor(bool *InA, const bool *InB, int length) {   //��λ���   
	for(int i = 0; i < length; i++)   
		InA[i] ^= InB[i]; 
} 
			
void keyfc(char *In){              //��Կ���ɺ���
	int i, j=0, mov, k, m; 
	bool key0[56] = {0};
	bool keyin[64] = {0};
	bool temp;    
	ByteToBit(keyin, In, 64);      //�ֽڵ�λ��ת��  
	for(i=0;i<56;i++)              //��Կѹ��Ϊ56λ   
		key0[i]=keyin[KeyRar_Table[i]-1];  
	for(i=0;i<16;i++) {            //16����Կ����  
		if(i==0||i==1||i==8||i==15)    
			mov=1;   
		else    
			mov=2;   
		for(k=0;k<mov;k++){       //����������ѭ������  
			for(m=0;m<8;m++){    
				temp=key0[m*7];     
				for(j=m*7;j<m*7+7;j++)      
					key0[j]=key0[j+1];     
				key0[m*7+6]=temp;    
			}    
			temp=key0[0];    
			for(m=0;m<27;m++)     
				key0[m]=key0[m+1];   
			key0[27]=temp;    
			temp=key0[28];    
			for(m=28;m<55;m++)     
				key0[m]=key0[m+1];    
			key0[55]=temp;   
		}    
		for(j=0;j<48;j++)         //ѹ���û�������    
			key[i][j]=key0[Rar_Table[j]-1]; 
	}  
}

void DES(char Out[8], char In[8], char option){//���ܺ��ĳ���,option=ENCIPHERʱ���ܣ���֮���� 
	bool MW[64] = { 0 };
	bool tmp[32] = { 0 };
	bool PMW[64] = { 0 };
	bool  kzmw[48] = { 0 };
	bool keytem[48] = { 0 };
	bool ss[32] = { 0 };
	int hang, lie, i;

	ByteToBit(PMW, In, 64);
	for (int j = 0; j < 64; j++)  {
		MW[j] = PMW[IP_Table[j] - 1];          //��ʼ�û�  
	}
	bool *Li = &MW[0], *Ri = &MW[32];
	for (i = 0; i < 48; i++)       //��������չ�û�   
		kzmw[i] = Ri[Exp_Table[i] - 1];
	if (option == ENCIPHER){         //DES���ܹ���  
		for (int lun = 0; lun < 16; lun++){
			for (i = 0; i < 32; i++)
				ss[i] = Ri[i];
			for (i = 0; i < 48; i++)       //��������չ�û�    
				kzmw[i] = Ri[Exp_Table[i] - 1];
			for (i = 0; i < 48; i++)
				keytem[i] = key[lun][i];
			Xor(kzmw, keytem, 48);    /*S���û�*/
			for (i = 0; i < 8; i++) {
				hang = kzmw[i * 6] * 2 + kzmw[i * 6 + 5];
				lie = kzmw[i * 6 + 1] * 8 + kzmw[i * 6 + 2] * 4 + kzmw[i * 6 + 3] * 2 + kzmw[i * 6 + 4];
				tmp[i * 4 + 3] = S_Box[i][(hang + 1) * 16 + lie] % 2;
				tmp[i * 4 + 2] = (S_Box[i][(hang + 1) * 16 + lie] / 2) % 2;
				tmp[i * 4 + 1] = (S_Box[i][(hang + 1) * 16 + lie] / 4) % 2;
				tmp[i * 4] = (S_Box[i][(hang + 1) * 16 + lie] / 8) % 2;
			}
			for (i = 0; i < 32; i++)  //P�û�      
				Ri[i] = tmp[P_Table[i] - 1];
			Xor(Ri, Li, 32);    //���    
			for (i = 0; i < 32; i++) { //������������      
				Li[i] = ss[i];
			}
		}
		for (i = 0; i < 32; i++){
			tmp[i] = Li[i];
			Li[i] = Ri[i];
			Ri[i] = tmp[i];
		}
		for (i = 0; i < 64; i++)
			PMW[i] = MW[Final_Table[i] - 1];
		BitToByte(Out, PMW, 64);  //λ���ֽڵ�ת��   
	}
	else {         //DES���ܹ���  
		for (int lun = 15; lun >= 0; lun--) {
			for (i = 0; i < 32; i++)
				ss[i] = Ri[i];
			for (i = 0; i < 48; i++)       //��������չ�û�   
				kzmw[i] = Ri[Exp_Table[i] - 1];
			for (i = 0; i < 48; i++)
				keytem[i] = key[lun][i];
			Xor(kzmw, keytem, 48);   /*S���û�*/
			for (i = 0; i < 8; i++) {
				hang = kzmw[i * 6] * 2 + kzmw[i * 6 + 5];
				lie = kzmw[i * 6 + 1] * 8 + kzmw[i * 6 + 2] * 4 + kzmw[i * 6 + 3] * 2 + kzmw[i * 6 + 4];
				tmp[i * 4 + 3] = S_Box[i][(hang + 1) * 16 + lie] % 2;
				tmp[i * 4 + 2] = (S_Box[i][(hang + 1) * 16 + lie] / 2) % 2;
				tmp[i * 4 + 1] = (S_Box[i][(hang + 1) * 16 + lie] / 4) % 2;
				tmp[i * 4] = (S_Box[i][(hang + 1) * 16 + lie] / 8) % 2;
			}
			for (i = 0; i < 32; i++)  //P�û�     
				Ri[i] = tmp[P_Table[i] - 1];
			Xor(Ri, Li, 32);    //���   
			for (i = 0; i < 32; i++){  //������������  
				Li[i] = ss[i];
			}
		}
		for (i = 0; i < 32; i++) {
			tmp[i] = Li[i];
			Li[i] = Ri[i];
			Ri[i] = tmp[i];
		}
		for (i = 0; i < 64; i++)
			PMW[i] = MW[Final_Table[i] - 1];
		BitToByte(Out, PMW, 64);  //λ���ֽڵ�ת��  
	}
}

bool RunDes(char *Out, char *In, int datalength, char *Key, char option){ //�������к������ж������Լ��������ı�8�ֽڷָ�  
	//printf("file_len:%d\ntext:%s\nkey: %s\n", datalength, In, Key);  //�����룬��ע��
	if( !( Out && In && Key && (datalength=(datalength+7)&0xfffffff8) ) )    
		return false; 
	keyfc(Key);  
	//printf("\nfile_len:%d\ntext:%s\nkey: %s\n", datalength, In, Key);  //�����룬��ע��
	for (int i = 0, j = datalength / 8; i < j; i++, Out += 8, In += 8) {
		DES(Out, In, option);
	}
	return true; 
}  
