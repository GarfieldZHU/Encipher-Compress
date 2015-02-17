/*加密/解密方法*/

#include "My_Encipher.h"

/*从文件读取进行加密/解密*/
void my_encipher(char option){
	FILE *fin = input_file();
	FILE *fout;
	int file_len, i, j;
	char *text;  //每次对8字节进行加密，循环进行至文件尾
	char key[9] = {'\0'};      //密匙默认最长8字节
	char *res;
	char ser;

	fseek(fin, 0, SEEK_END);
	file_len = ftell(fin);
	text = (char *)malloc(sizeof(char)*(file_len / 8 + 1) * 8);   //由于加密不满8倍数时补过字符
	res = (char *)malloc(sizeof(char)*(file_len / 8 + 1) * 8 );    //最大长度可能超出file_len
	ZeroMemory(text, (file_len / 8 + 1) * 8);
	ZeroMemory(res, (file_len / 8 + 1) * 8 );
	fseek(fin, 0, SEEK_SET);
	fread(text, file_len, 1, fin);


	if (option == ENCIPHER){  //加密提示及对应操作
		printf(">您即将对该文件进行加密操作\n");
		printf("-----请输入加密要使用的密码\n");
		printf("-----(可以使用任意字符(*除外)，最多8位)：\n>");
		for (i = 0; i < 8; i++) {  //密匙最长8位，超出忽略
			scanf("%c", &ser);
			if (ser == '\n'){  //密匙不足吧8位时用*号补齐
				for (j = i; j < 8; j++) key[j] = '*';
				i = 8;
			}
			else
				key[i] = ser;
		}
	
		//res[0] = file_len % 8 + '0';
		printf("\n");		//留首位存文本长度与8倍数的差值
		if (file_len % 8 == 0){
			RunDes(res, text, file_len, key, ENCIPHER);
			output_to_file(res, file_len, ENCIPHER);
		}
		else{
			RunDes(res, text, (file_len / 8 + 1) * 8, key, ENCIPHER);
			output_to_file(res, (file_len / 8 + 1) * 8, ENCIPHER);
		}
		printf("--------您的密码为： ");
		for (i = 0; i < 8; i++) 
			if (key[i] != '*') printf("%c",key[i]);
		printf("\n--------请记住您的密码以保证能正确解密。\n");
	}

	else if(option == DECIPHER){   //解密提示及对应操作
		printf(">您即将对该文件进行解密操作\n");
		printf("-----请输入该文件的加密密码：\n>");
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
	
	//printf("file_len:%d\ntext:%s\nkey: %s\n", file_len, text, key);  //测试码，请注释
	//printf("\nAfter DES:%s\n",res);   //测试码，请注释
	free(text);
	free(res);
	fclose(fin);
	printf(">");
	system("pause");
	system("cls");
}


/*-------------------------------------------------------------*/
/*
 *DES加/解密算法核心部分
 */

const static char IP_Table[] = {       //IP_Table置换     
	58, 50, 42, 34, 26, 18, 10,  2,     
	60, 52, 44, 36, 28, 20, 12,  4,     
	62, 54, 46, 38, 30, 22, 14,  6,     
	64, 56, 48, 40, 32, 24, 16,  8,     
	57, 49, 41, 33, 25, 17,  9,  1,     
	59, 51, 43, 35, 27, 19, 11,  3,     
	61, 53, 45, 37, 29, 21, 13,  5,     
	63, 55, 47, 39, 31, 23, 15,  7 
};            

const static char Final_Table[] = {     //最终置换     
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

const static char Rar_Table[] = {    //压缩置换     
	14, 17, 11, 24,  1,  5,   
	3, 28, 15,  6, 21, 10,     
	23, 19, 12,  4, 26,  8,     
	16,  7, 27, 20, 13,  2,     
	41, 52, 31, 37, 47, 55,     
	30, 40, 51, 45, 33, 48,     
	44, 49, 39, 56, 34, 53,     
	46, 42, 50, 36, 29, 32 
}; 

const static char Exp_Table[] = {    //扩展置换     
	32,  1,  2,  3,  4,  5,   
	4,  5,  6,  7,  8,  9, 
	8, 9, 10, 11, 12, 13, 
	12, 13, 14, 15, 16, 17, 
	16, 17, 18, 19, 20, 21, 
	20, 21, 22, 23, 24, 25, 
	24, 25, 26, 27, 28, 29, 
	28, 29, 30, 31, 32, 1 
};

const static char P_Table[] = {     //P置换 
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

//设置全局变量，16轮密钥 
bool key[16][48]={{0}};  

void ByteToBit(bool *Out, char *In, int bits){  //字节到位转换函数 
	int i;  
	for (i = 0; i<bits; i++)   
		Out[i] = (In[i / 8] >> (i % 8)) & 1;
}

void BitToByte(char *Out, bool *In, int bits){ //位到字节转换函数 
	int i;  
	for (i = 0; i<bits / 8; i++)  
		Out[i] = 0;  
	for (i = 0; i<bits; i++)  
		Out[i / 8] |= In[i] << (i % 8);
}
		
void Xor(bool *InA, const bool *InB, int length) {   //按位异或   
	for(int i = 0; i < length; i++)   
		InA[i] ^= InB[i]; 
} 
			
void keyfc(char *In){              //密钥生成函数
	int i, j=0, mov, k, m; 
	bool key0[56] = {0};
	bool keyin[64] = {0};
	bool temp;    
	ByteToBit(keyin, In, 64);      //字节到位的转换  
	for(i=0;i<56;i++)              //密钥压缩为56位   
		key0[i]=keyin[KeyRar_Table[i]-1];  
	for(i=0;i<16;i++) {            //16轮密钥产生  
		if(i==0||i==1||i==8||i==15)    
			mov=1;   
		else    
			mov=2;   
		for(k=0;k<mov;k++){       //分左右两块循环左移  
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
		for(j=0;j<48;j++)         //压缩置换并储存    
			key[i][j]=key0[Rar_Table[j]-1]; 
	}  
}

void DES(char Out[8], char In[8], char option){//加密核心程序,option=ENCIPHER时加密，反之解密 
	bool MW[64] = { 0 };
	bool tmp[32] = { 0 };
	bool PMW[64] = { 0 };
	bool  kzmw[48] = { 0 };
	bool keytem[48] = { 0 };
	bool ss[32] = { 0 };
	int hang, lie, i;

	ByteToBit(PMW, In, 64);
	for (int j = 0; j < 64; j++)  {
		MW[j] = PMW[IP_Table[j] - 1];          //初始置换  
	}
	bool *Li = &MW[0], *Ri = &MW[32];
	for (i = 0; i < 48; i++)       //右明文扩展置换   
		kzmw[i] = Ri[Exp_Table[i] - 1];
	if (option == ENCIPHER){         //DES加密过程  
		for (int lun = 0; lun < 16; lun++){
			for (i = 0; i < 32; i++)
				ss[i] = Ri[i];
			for (i = 0; i < 48; i++)       //右明文扩展置换    
				kzmw[i] = Ri[Exp_Table[i] - 1];
			for (i = 0; i < 48; i++)
				keytem[i] = key[lun][i];
			Xor(kzmw, keytem, 48);    /*S盒置换*/
			for (i = 0; i < 8; i++) {
				hang = kzmw[i * 6] * 2 + kzmw[i * 6 + 5];
				lie = kzmw[i * 6 + 1] * 8 + kzmw[i * 6 + 2] * 4 + kzmw[i * 6 + 3] * 2 + kzmw[i * 6 + 4];
				tmp[i * 4 + 3] = S_Box[i][(hang + 1) * 16 + lie] % 2;
				tmp[i * 4 + 2] = (S_Box[i][(hang + 1) * 16 + lie] / 2) % 2;
				tmp[i * 4 + 1] = (S_Box[i][(hang + 1) * 16 + lie] / 4) % 2;
				tmp[i * 4] = (S_Box[i][(hang + 1) * 16 + lie] / 8) % 2;
			}
			for (i = 0; i < 32; i++)  //P置换      
				Ri[i] = tmp[P_Table[i] - 1];
			Xor(Ri, Li, 32);    //异或    
			for (i = 0; i < 32; i++) { //交换左右明文      
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
		BitToByte(Out, PMW, 64);  //位到字节的转换   
	}
	else {         //DES解密过程  
		for (int lun = 15; lun >= 0; lun--) {
			for (i = 0; i < 32; i++)
				ss[i] = Ri[i];
			for (i = 0; i < 48; i++)       //右明文扩展置换   
				kzmw[i] = Ri[Exp_Table[i] - 1];
			for (i = 0; i < 48; i++)
				keytem[i] = key[lun][i];
			Xor(kzmw, keytem, 48);   /*S盒置换*/
			for (i = 0; i < 8; i++) {
				hang = kzmw[i * 6] * 2 + kzmw[i * 6 + 5];
				lie = kzmw[i * 6 + 1] * 8 + kzmw[i * 6 + 2] * 4 + kzmw[i * 6 + 3] * 2 + kzmw[i * 6 + 4];
				tmp[i * 4 + 3] = S_Box[i][(hang + 1) * 16 + lie] % 2;
				tmp[i * 4 + 2] = (S_Box[i][(hang + 1) * 16 + lie] / 2) % 2;
				tmp[i * 4 + 1] = (S_Box[i][(hang + 1) * 16 + lie] / 4) % 2;
				tmp[i * 4] = (S_Box[i][(hang + 1) * 16 + lie] / 8) % 2;
			}
			for (i = 0; i < 32; i++)  //P置换     
				Ri[i] = tmp[P_Table[i] - 1];
			Xor(Ri, Li, 32);    //异或   
			for (i = 0; i < 32; i++){  //交换左右明文  
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
		BitToByte(Out, PMW, 64);  //位到字节的转换  
	}
}

bool RunDes(char *Out, char *In, int datalength, char *Key, char option){ //加密运行函数，判断输入以及对输入文本8字节分割  
	//printf("file_len:%d\ntext:%s\nkey: %s\n", datalength, In, Key);  //测试码，请注释
	if( !( Out && In && Key && (datalength=(datalength+7)&0xfffffff8) ) )    
		return false; 
	keyfc(Key);  
	//printf("\nfile_len:%d\ntext:%s\nkey: %s\n", datalength, In, Key);  //测试码，请注释
	for (int i = 0, j = datalength / 8; i < j; i++, Out += 8, In += 8) {
		DES(Out, In, option);
	}
	return true; 
}  
