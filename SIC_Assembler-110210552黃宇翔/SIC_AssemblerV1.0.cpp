#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

//----------------------------------------------指令結構 
struct instructionFormat{
	char mnemonic[7];
	int opcode;
};
struct instructionFormat instructions[26];
//----------------------------------------------Object Code結構
struct objectCodeFormat{
	int head;
	int tail;
	int objectCodeLength;
	int specialType;
	char spe1[20];
};
//----------------------------------------------程式結構 
struct sourceCodeFormat{
	int  LOCCTR;
	char label[9];
	char intstruction[7];
	char operand[19];
	char comment[32];
	objectCodeFormat objectCode;
};
struct sourceCodeFormat sourceCode[1000];
int sProgramLength;
//----------------------------------------------Symbol Table結構 
struct symbolTableFormat{
	int  LOCCTR;
	char label[9];
};
struct symbolTableFormat symbolTable[1000];
int symbolTableLength;
//----------------------------------------------OBTable結構 
struct OBTableFormat{
	char  LOCCTR[5];
	char  OBCode[20];
	int OBLength;
};
struct OBTableFormat OBTable[1000];
int OBTableLength;
//----------------------------------------------
void setUpInstruction();			//建立指令表  
void readProgeam();					//讀取程式  
void showSourceCode();				//印出程式 
int hexToDex(const char s[]);		//hexToDex
void setSymbolTable();				//建立 Symbol Table
void showSymbolTable();				//印出 Symbol Table
int strLength(const char str[]);    //算字串長度 
void setObjectCode();				//建立 Object Code
void outPutListFile();				//輸出 ListFile
void outPutObjectProgram();			//輸出 ObProgram
void setObjectTable();				//建立 ObjectTable
//----------------------------------------------
int main()
{
	setUpInstruction();
	readProgeam();
	setSymbolTable();
	setObjectCode();
	printf("\nLine LOCCTR  Label  Intstruction Operand               Comment                    Object Code\n");
	printf("---------------------------------------------------------------------------------------------\n");
	showSourceCode();
	printf("\n----Symbol Table-----------------------------------------------------------------------------\n\n");
	showSymbolTable();
	outPutListFile();
	setObjectTable();
	outPutObjectProgram();

	system("pause");
	return 0;
}


//--------------------------------------------------------------讀取程式 
void readProgeam(){
	char aLine[67];
	char start[6]="START";
	sProgramLength=0;
	FILE *fp = fopen("SRCFILE","r");
	if(fp==NULL){
		printf("程式讀取失敗，請檢查程式文件是否在此目錄!\n");
		system("pause");
		exit(1);
	}
	else{
		while(fgets(aLine,70,fp) != NULL){
			for(int i=0;i<strlen(aLine);i++){
				if(aLine[i]!='\n'){
					if(i<8)
						sourceCode[sProgramLength].label[i] = aLine[i];
					else if(i>8&&i<15)
						sourceCode[sProgramLength].intstruction[i-9] = aLine[i];
					else if(i>16&&i<35)
						sourceCode[sProgramLength].operand[i-17] = aLine[i];
					else if(i>35)
						sourceCode[sProgramLength].comment[i-36] = aLine[i];
				}
			}
			if(sourceCode[sProgramLength].label[0]=='.'){
				sourceCode[sProgramLength+1].LOCCTR = sourceCode[sProgramLength].LOCCTR;
				sProgramLength++;
				continue;
			}		
			if(!strncmp(sourceCode[sProgramLength].intstruction,"START",strLength(sourceCode[sProgramLength].intstruction))&&strLength(sourceCode[sProgramLength].intstruction)==strLength("START")){
				sourceCode[sProgramLength].LOCCTR = hexToDex(sourceCode[sProgramLength].operand);
				sourceCode[sProgramLength+1].LOCCTR = sourceCode[sProgramLength].LOCCTR;
			}
			else if(!strncmp(sourceCode[sProgramLength].intstruction,"WORD",strLength(sourceCode[sProgramLength].intstruction))&&strLength(sourceCode[sProgramLength].intstruction)==strLength("WORD")){
				sourceCode[sProgramLength+1].LOCCTR = sourceCode[sProgramLength].LOCCTR+3;
			}
			else if(!strncmp(sourceCode[sProgramLength].intstruction,"BYTE",strLength(sourceCode[sProgramLength].intstruction))&&strLength(sourceCode[sProgramLength].intstruction)==strLength("BYTE")){
				if(sourceCode[sProgramLength].operand[0]=='X'||sourceCode[sProgramLength].operand[0]=='x')
					sourceCode[sProgramLength+1].LOCCTR = sourceCode[sProgramLength].LOCCTR+(strLength(sourceCode[sProgramLength].operand)-3)/2;
				else if(sourceCode[sProgramLength].operand[0]=='C'||sourceCode[sProgramLength].operand[0]=='c')
					sourceCode[sProgramLength+1].LOCCTR = sourceCode[sProgramLength].LOCCTR+(strLength(sourceCode[sProgramLength].operand)-3);
			}
			else if(!strncmp(sourceCode[sProgramLength].intstruction,"RESW",strLength(sourceCode[sProgramLength].intstruction))&&strLength(sourceCode[sProgramLength].intstruction)==strLength("RESW")){
				sourceCode[sProgramLength+1].LOCCTR = sourceCode[sProgramLength].LOCCTR+3*atoi(sourceCode[sProgramLength].operand);
			}
			else if(!strncmp(sourceCode[sProgramLength].intstruction,"RESB",strLength(sourceCode[sProgramLength].intstruction))&&strLength(sourceCode[sProgramLength].intstruction)==strLength("RESB")){
				sourceCode[sProgramLength+1].LOCCTR = sourceCode[sProgramLength].LOCCTR+atoi(sourceCode[sProgramLength].operand);
			}
			else if(!strncmp(sourceCode[sProgramLength].intstruction,"END",strLength(sourceCode[sProgramLength].intstruction))&&strLength(sourceCode[sProgramLength].intstruction)==strLength("END")){
			}
			else{
				sourceCode[sProgramLength+1].LOCCTR = sourceCode[sProgramLength].LOCCTR+3;
			}
			sProgramLength++;
		}
	}
	fclose(fp);
}
//--------------------------------------------------------------印出程式 
void showSourceCode(){
	for(int i=0;i<sProgramLength;i++){
		if(sourceCode[i].objectCode.specialType == 0){
			if(sourceCode[i].label[0]=='.'){
				printf("%03d          .\n",i);
				continue;
			}
			if(sourceCode[i].objectCode.head==-1&&sourceCode[i].objectCode.tail==-1)
				printf("%03d   %04X   %-8s %-6s     %-18s %-31s\n",i,sourceCode[i].LOCCTR,sourceCode[i].label,sourceCode[i].intstruction,sourceCode[i].operand,sourceCode[i].comment);
			else
				printf("%03d   %04X   %-8s %-6s     %-18s %-31s %02X%04X\n",i,sourceCode[i].LOCCTR,sourceCode[i].label,sourceCode[i].intstruction,sourceCode[i].operand,sourceCode[i].comment,sourceCode[i].objectCode.head,sourceCode[i].objectCode.tail);
		}
		else if(sourceCode[i].objectCode.specialType == 1){
			printf("%03d   %04X    %-8s %-6s     %-18s %-31s ",i,sourceCode[i].LOCCTR,sourceCode[i].label,sourceCode[i].intstruction,sourceCode[i].operand,sourceCode[i].comment);
			for(int k=0;k<sourceCode[i].objectCode.objectCodeLength;k++){
				printf("%d",sourceCode[i].objectCode.spe1[k]);
			}
			printf("\n");
		}
		else if(sourceCode[i].objectCode.specialType == 2){
			printf("%03d   %04X   %-8s %-6s     %-18s %-31s ",i,sourceCode[i].LOCCTR,sourceCode[i].label,sourceCode[i].intstruction,sourceCode[i].operand,sourceCode[i].comment);
			for(int k=0;k<sourceCode[i].objectCode.objectCodeLength*2;k++){
				printf("%c",sourceCode[i].objectCode.spe1[k]);
			}
			printf("\n");
		}
	}
} 
//--------------------------------------------------------------hexToDex
int hexToDex(const char s[]){
	int num=atoi(s),result=0,j=strLength(s)-1;
	for(int i=0;i<strLength(s);i++){
		if(s[i]=='A'||s[i]=='a')
			num=10;
		else if(s[i]=='B'||s[i]=='b')
			num=11;
		else if(s[i]=='C'||s[i]=='c')
			num=12;
		else if(s[i]=='D'||s[i]=='d')
			num=13;
		else if(s[i]=='E'||s[i]=='e')
			num=14;
		else if(s[i]=='F'||s[i]=='f')
			num=15;
		else
			num=s[i]-'0';
		result+=num*pow(16,j);
		j--;
	}
	return result;
}
//--------------------------------------------------------------建立Symbol Table
void setSymbolTable(){
	symbolTableLength = 0;
	int exist;
	for(int i=0;i<sProgramLength;i++){
		if(!strcmp(sourceCode[i].label,"        ")||sourceCode[i].label[0]=='.'){
			continue;
		}
		else{
			exist=0;
			for(int j=0;j<symbolTableLength;j++){
				if(!strcmp(sourceCode[i].label,symbolTable[j].label)){
					exist=1;
					//這裡要顯示錯誤 
				}
			}
			if(exist==0){
				strcpy(symbolTable[symbolTableLength].label,sourceCode[i].label);
				symbolTable[symbolTableLength].LOCCTR = sourceCode[i].LOCCTR;
				symbolTableLength++;
			}
		}
	}
} 
//--------------------------------------------------------------印出Symbol Table
void showSymbolTable(){
	for(int i=0;i<symbolTableLength;i++){
		printf("%04X %-8s\n",symbolTable[i].LOCCTR,symbolTable[i].label);
	}
} 
//--------------------------------------------------------------算字串長度 
int strLength(const char str[]){
	int length=0;
	for(int i=0;i<strlen(str);i++){
		if(str[i]!=' ')
			length++;
	}
	return length;
} 
//--------------------------------------------------------------建立 Object Code
void setObjectCode(){
	int exist=0,useX=0,b;
	for(int i=0;i<sProgramLength;i++){
		exist=0;
		if(sourceCode[i].label[0]=='.'){
				continue;
		}
		if(!strncmp(sourceCode[i].intstruction,"START",strLength(sourceCode[i].intstruction))&&strLength(sourceCode[i].intstruction)==strLength("START")){
			sourceCode[i].objectCode.objectCodeLength = 0;
			sourceCode[i].objectCode.specialType =  0;
			sourceCode[i].objectCode.head = -1;
			sourceCode[i].objectCode.tail = -1;
		}
		else if(!strncmp(sourceCode[i].intstruction,"WORD",strLength(sourceCode[i].intstruction))&&strLength(sourceCode[i].intstruction)==strLength("WORD")){
			sourceCode[i].objectCode.objectCodeLength = 3;
			sourceCode[i].objectCode.specialType =  0;
			if(atoi(sourceCode[i].operand)>65535){
				sourceCode[i].objectCode.head = atoi(sourceCode[i].operand)/65535;
				sourceCode[i].objectCode.tail = atoi(sourceCode[i].operand)%65535;
			}
			else{
				sourceCode[i].objectCode.head = 0;
				sourceCode[i].objectCode.tail = atoi(sourceCode[i].operand);
			}
		}
		else if(!strncmp(sourceCode[i].intstruction,"BYTE",strLength(sourceCode[i].intstruction))&&strLength(sourceCode[i].intstruction)==strLength("BYTE")){
			if(sourceCode[i].operand[0]=='X'||sourceCode[i].operand[0]=='x'){
				sourceCode[i].objectCode.objectCodeLength = (strLength(sourceCode[i].operand)-3)/2;
				for(int k=0;k<strLength(sourceCode[i].operand)-3;k++){
					sourceCode[i].objectCode.spe1[k] = sourceCode[i].operand[k+2];
				}
				sourceCode[i].objectCode.specialType =  2;
			}
			else if(sourceCode[i].operand[0]=='C'||sourceCode[i].operand[0]=='c'){
				sourceCode[i].objectCode.objectCodeLength = (strLength(sourceCode[i].operand)-3);
				for(int k=0;k<(strLength(sourceCode[i].operand)-3);k++){
					sourceCode[i].objectCode.spe1[k] = sourceCode[i].operand[k+2];
				}
				sourceCode[i].objectCode.specialType =  1;
			}
		}
		else if(!strncmp(sourceCode[i].intstruction,"RESW",strLength(sourceCode[i].intstruction))&&strLength(sourceCode[i].intstruction)==strLength("RESW")){
			sourceCode[i].objectCode.objectCodeLength = 3;
			sourceCode[i].objectCode.specialType =  0;
			sourceCode[i].objectCode.head = -1;
			sourceCode[i].objectCode.tail = -1;
		}
		else if(!strncmp(sourceCode[i].intstruction,"RESB",strLength(sourceCode[i].intstruction))&&strLength(sourceCode[i].intstruction)==strLength("RESB")){
			sourceCode[i].objectCode.objectCodeLength = 3;
			sourceCode[i].objectCode.specialType =  0;
			sourceCode[i].objectCode.head = -1;
			sourceCode[i].objectCode.tail = -1;
		}
		else if(!strncmp(sourceCode[i].intstruction,"END",strLength(sourceCode[i].intstruction))&&strLength(sourceCode[i].intstruction)==strLength("END")){
			sourceCode[i].objectCode.objectCodeLength = 3;
			sourceCode[i].objectCode.specialType =  0;
			for(int j=0;j<symbolTableLength;j++){
				if(!strncmp(symbolTable[j].label,sourceCode[i].operand,strLength(sourceCode[i].operand))&&strLength(sourceCode[i].operand)==strLength(symbolTable[j].label)){
					sourceCode[i].objectCode.head = 0;
					sourceCode[i].objectCode.tail = symbolTable[j].LOCCTR;
					exist=1;
					break;
				}
			}
			if(exist==0){
				//這裡要顯示錯誤訊息
			}
		}
		else{
			sourceCode[i].objectCode.objectCodeLength = 3;
			sourceCode[i].objectCode.specialType =  0;
			for(int j=0;j<26;j++){
				if(!strncmp(instructions[j].mnemonic,sourceCode[i].intstruction,strLength(sourceCode[i].intstruction))&&strLength(sourceCode[i].intstruction)==strLength(instructions[j].mnemonic)){
					sourceCode[i].objectCode.head = instructions[j].opcode;
					exist=1;
					break;
				}
			}
			if(exist==0){
				//這裡要顯示錯誤訊息
			}
			else{
				exist=0;
				useX=0;
				for(b=0;b<strLength(sourceCode[i].operand);b++){
					if(sourceCode[i].operand[b] == ','){
						if((sourceCode[i].operand[b+1] == 'X' || sourceCode[i].operand[b+1] == 'x' ) && sourceCode[i].operand[b+2]== ' '){
							useX=1;
							sourceCode[i].operand[b]=' ';
							sourceCode[i].operand[b+1]=' ';
							break;
						}
						else{
							//這裡要顯示錯誤訊息
						}
					}
				}
				for(int j=0;j<symbolTableLength;j++){
					if(!strncmp(symbolTable[j].label,sourceCode[i].operand,strLength(symbolTable[j].label))&&strLength(symbolTable[j].label)==strLength(sourceCode[i].operand)){
						if(useX==1){
							sourceCode[i].objectCode.tail = symbolTable[j].LOCCTR+32768;
							sourceCode[i].operand[b]=',';
							sourceCode[i].operand[b+1]='X';
						}
						else{
							sourceCode[i].objectCode.tail = symbolTable[j].LOCCTR;
						}
						exist=1;
						break;
					}
				}
				if(exist==0){
					//這裡要顯示錯誤訊息
				}
			}
			
		}
	}
	
}
//--------------------------------------------------------------輸出 ListFile
void outPutListFile(){
	FILE *fp = fopen("LISTFILE","w");
	if(fp==NULL){
		printf("檔案建立失敗，請檢查記憶體空間。\n");
		system("pause");
		exit(1);
	}
	else{
		fprintf(fp,"\nLine LOCCTR  Label  Intstruction Operand               Comment                    Object Code\n");
		fprintf(fp,"---------------------------------------------------------------------------------------------\n");
		for(int i=0;i<sProgramLength;i++){
		if(sourceCode[i].objectCode.specialType == 0){
			if(sourceCode[i].label[0]=='.'){
				fprintf(fp,"%03d          .\n",i);
				continue;
			}
			if(sourceCode[i].objectCode.head==-1&&sourceCode[i].objectCode.tail==-1)
				fprintf(fp,"%03d   %04X   %-8s %-6s     %-18s %-31s\n",i,sourceCode[i].LOCCTR,sourceCode[i].label,sourceCode[i].intstruction,sourceCode[i].operand,sourceCode[i].comment);
			else
				fprintf(fp,"%03d   %04X   %-8s %-6s     %-18s %-31s %02X%04X\n",i,sourceCode[i].LOCCTR,sourceCode[i].label,sourceCode[i].intstruction,sourceCode[i].operand,sourceCode[i].comment,sourceCode[i].objectCode.head,sourceCode[i].objectCode.tail);
		}
		else if(sourceCode[i].objectCode.specialType == 1){
			fprintf(fp,"%03d   %04X    %-8s %-6s     %-18s %-31s ",i,sourceCode[i].LOCCTR,sourceCode[i].label,sourceCode[i].intstruction,sourceCode[i].operand,sourceCode[i].comment);
			for(int k=0;k<sourceCode[i].objectCode.objectCodeLength;k++){
				fprintf(fp,"%d",sourceCode[i].objectCode.spe1[k]);
			}
			fprintf(fp,"\n");
		}
		else if(sourceCode[i].objectCode.specialType == 2){
			fprintf(fp,"%03d   %04X   %-8s %-6s     %-18s %-31s ",i,sourceCode[i].LOCCTR,sourceCode[i].label,sourceCode[i].intstruction,sourceCode[i].operand,sourceCode[i].comment);
			for(int k=0;k<(sourceCode[i].objectCode.objectCodeLength)*2;k++){
				fprintf(fp,"%c",sourceCode[i].objectCode.spe1[k]);
			}
			fprintf(fp,"\n");
		}
	}
	fprintf(fp,"\n----Symbol Table-----------------------------------------------------------------------------\n\n");
	for(int i=0;i<symbolTableLength;i++){
		fprintf(fp,"%04X %-8s\n",symbolTable[i].LOCCTR,symbolTable[i].label);
	}
	}
	fclose(fp);
		
} 
//--------------------------------------------------------------輸出 ObProgram
void outPutObjectProgram(){
	int i=0,j,k,lineLength=3;
	FILE *fp = fopen("ObProgram","w");
	if(fp==NULL){
		printf("檔案建立失敗，請檢查記憶體空間。\n");
		system("pause");
		exit(1);
	}
	else{
		fprintf(fp,"H%s%06X%06X\n",sourceCode[0].label,hexToDex(sourceCode[0].operand),sourceCode[sProgramLength-1].LOCCTR-sourceCode[0].LOCCTR);
		i=0;
		j=0;
		while(1){
			if(j==0){
				for(k=1;k<20;k++){
					if(hexToDex(OBTable[i+k].LOCCTR)==hexToDex(OBTable[OBTableLength-2].LOCCTR)||hexToDex(OBTable[i+k+1].LOCCTR)-hexToDex(OBTable[i+k].LOCCTR)!=OBTable[i+k].OBLength){
						k-=2;
						break;
					}
					lineLength += OBTable[i+k].OBLength;
					if(lineLength>27){
						k--;
						break;
					}
				}
				if(i!=0)
					i++;
				fprintf(fp,"T%06X%02X%s",hexToDex(OBTable[i].LOCCTR),lineLength,OBTable[i].OBCode);
			}
			for(int m=0;m<=k;m++){
				i++;
				fprintf(fp,"%s",OBTable[i].OBCode);
			}
			fprintf(fp,"\n");
			if(i>=OBTableLength-2)
				break;
			else{
				lineLength=3;
				j=0;
			}
		}
		fprintf(fp,"E%06X",hexToDex(OBTable[OBTableLength-1].OBCode));
	}
	
	fclose(fp);
}
//--------------------------------------------------------------建立 ObjectTable
void setObjectTable(){
	int i;
	FILE *fp = fopen("OBTable","w");
	if(fp==NULL){
		printf("檔案建立失敗，請檢查記憶體空間。\n");
		system("pause");
		exit(1);
	}
	else{
		for(int i=0;i<sProgramLength;i++){
			if(sourceCode[i].objectCode.specialType == 0){
				if(sourceCode[i].label[0]=='.'){
					continue;
				}
				if(sourceCode[i].objectCode.head==-1&&sourceCode[i].objectCode.tail==-1)
					continue;
				else
					fprintf(fp,"%04X %02X%04X %d\n",sourceCode[i].LOCCTR,sourceCode[i].objectCode.head,sourceCode[i].objectCode.tail,sourceCode[i].objectCode.objectCodeLength);
			}
			else if(sourceCode[i].objectCode.specialType == 1){
				fprintf(fp,"%04X ",sourceCode[i].LOCCTR);
				for(int k=0;k<sourceCode[i].objectCode.objectCodeLength;k++){
					fprintf(fp,"%d",sourceCode[i].objectCode.spe1[k]);
				}
				fprintf(fp," %d",sourceCode[i].objectCode.objectCodeLength);
				fprintf(fp,"\n");
			}
			else if(sourceCode[i].objectCode.specialType == 2){
				fprintf(fp,"%04X ",sourceCode[i].LOCCTR);
				for(int k=0;k<sourceCode[i].objectCode.objectCodeLength*2;k++){
					fprintf(fp,"%c",sourceCode[i].objectCode.spe1[k]);
				}
				fprintf(fp," %d",sourceCode[i].objectCode.objectCodeLength);
				fprintf(fp,"\n");
			}
		}
	}
	fclose(fp);
	fp = fopen("OBTable","r");
	if(fp==NULL){
		printf("檔案建立失敗，請檢查記憶體空間。\n");
		system("pause");
		exit(1);
	}
	else{
		i=0;
		while(fscanf(fp,"%s %s %d",OBTable[i].LOCCTR,OBTable[i].OBCode,&OBTable[i].OBLength)!=EOF){
			OBTableLength++;
			i++;
		}
	}
	fclose(fp);
	remove("OBTable");
}				
//--------------------------------------------------------------建立指令表 
void setUpInstruction(){
	
	strcpy(instructions[0].mnemonic,"ADD");   instructions[0].opcode = 24;
	strcpy(instructions[1].mnemonic,"AND");   instructions[1].opcode = 64;
	strcpy(instructions[2].mnemonic,"COMP");  instructions[2].opcode = 40;
	strcpy(instructions[3].mnemonic,"DIV");   instructions[3].opcode = 36;
	strcpy(instructions[4].mnemonic,"J");     instructions[4].opcode = 60;
	strcpy(instructions[5].mnemonic,"JEQ");   instructions[5].opcode = 48;
	strcpy(instructions[6].mnemonic,"JGT");   instructions[6].opcode = 52;
	strcpy(instructions[7].mnemonic,"JLT");   instructions[7].opcode = 56;
	strcpy(instructions[8].mnemonic,"JSUB");  instructions[8].opcode = 72;
	strcpy(instructions[9].mnemonic,"LDA");   instructions[9].opcode = 0;
	strcpy(instructions[10].mnemonic,"LDCH"); instructions[10].opcode = 80;
	strcpy(instructions[11].mnemonic,"LDL");  instructions[11].opcode = 8;
	strcpy(instructions[12].mnemonic,"LDX");  instructions[12].opcode = 4;
	strcpy(instructions[13].mnemonic,"MUL");  instructions[13].opcode = 32;
	strcpy(instructions[14].mnemonic,"STA");  instructions[14].opcode = 12;
	strcpy(instructions[15].mnemonic,"STCH"); instructions[15].opcode = 84;
	strcpy(instructions[16].mnemonic,"STL");  instructions[16].opcode = 20;
	strcpy(instructions[17].mnemonic,"STSW"); instructions[17].opcode = 232;
	strcpy(instructions[18].mnemonic,"STX");  instructions[18].opcode = 16;
	strcpy(instructions[19].mnemonic,"SUB");  instructions[19].opcode = 28;
	strcpy(instructions[20].mnemonic,"OR");   instructions[20].opcode = 68;
	strcpy(instructions[21].mnemonic,"RD");   instructions[21].opcode = 216;
	strcpy(instructions[22].mnemonic,"RSUB"); instructions[22].opcode = 76;
	strcpy(instructions[23].mnemonic,"TD");   instructions[23].opcode = 224;
	strcpy(instructions[24].mnemonic,"TIX");  instructions[24].opcode = 44;
	strcpy(instructions[25].mnemonic,"WD");   instructions[25].opcode = 220;
}
















