#include "�ʷ�������.h"
#define TOKEN_LEN 500//�Ǻ���󳤶�
#pragma warning(disable: 4996)

unsigned int LineNo;//Դ�ļ��к�
static FILE *InFile;//�����ļ���
static char TokenBuffer[TOKEN_LEN];//�Ǻ��ַ�����

								   //��ʼ���ʷ�������
extern int InitScanner(const char *FileName)
{
	LineNo = 1;//�к�
	InFile = fopen(FileName, "r");//ֻ����ʽ���ļ�
	if (InFile != NULL) return 1;//�򿪳ɹ�
	else return 0;//��ʧ��
}

//�رմʷ�������
extern void CloseScanner(void)
{
	if (InFile != NULL)
		fclose(InFile);//�ر��ļ�
}

//������Դ�����������ַ�
static char GetChar(void)
{
	char Char = getc(InFile);//����ַ���
	return toupper(Char);//ȫ��ת��Ϊ��д��Ȼ��return
}

//��Ԥ���ַ��˻�Դ��������
static void BackChar(char Char)
{
	if (Char != EOF)
		ungetc(Char, InFile);
}

//�����ַ����ǺŻ�����
static void AddCharTokenString(char Char)
{
	int TokenLength = strlen(TokenBuffer);
	if (TokenLength + 1 >= sizeof(TokenBuffer)) return;//����Խ��
	TokenBuffer[TokenLength] = Char;//д���ַ���������
	TokenBuffer[TokenLength + 1] = '\0';//�����ַ���������־��\0��
}

//��ռǺŻ�����
static void EmptyTokenString()
{
	memset(TokenBuffer, 0, TOKEN_LEN);
}

//�жϼǺ��Ƿ��ڼǺű���
static Token JudgeKeyToken(const char *IDString)
{
	int loop;
	//sizeof(TokenTab)L:��ʾ�������һ��ռ�˶����ֽ���;sizeof(TokenTab[0])����ʾһ��Ԫ����ռ���ֽ���,�������������������һ���ж��ٸ�Ԫ��
	for (loop = 0; loop < sizeof(TokenTab) / sizeof(TokenTab[0]); loop++)
	{	//����TokenTab��
		if (strcmp(TokenTab[loop].lexeme, IDString) == 0)
			return TokenTab[loop];//�ж����ַ����оͷ��ظüǺ�
	}
	Token errortoken;
	memset(&errortoken, 0, sizeof(Token));//�Ƚ�errortoken�ÿ�
	errortoken.type = ERRTOKEN;//Ȼ���������Ǻ�
	return errortoken;//���س���Ǻ�
}

//���һ���Ǻ�
extern Token GetToken(void)
{
	Token token;
	char Char;
	memset(&token, 0, sizeof(Token));//token�ÿ�
	EmptyTokenString();//��ռǺŻ�����
	token.lexeme = TokenBuffer;
	for (;;)//��ѭ���������˵�Դ�����еĿո�TAB���س��ȷָ������ļ��������ؿռǺ�
	{
		Char = GetChar();//��Դ������ַ�
		if (Char == EOF)//������
		{
			token.type = NONTOKEN;
			return token;
		}
		if (Char == '\n')
			LineNo++;//�к�+1
		if (!isspace(Char))//�����ո�üǺſ϶��Ѿ���ɣ��˳�ѭ��
			break;
	}
	AddCharTokenString(Char);//��������������Щ�ָ��������ȼ��뻺����
	if (isalpha(Char))//�����Ӣ����ĸ��һ���Ǻ������ؼ��֡�PI��E��
	{
		for (;;)
		{
			Char = GetChar();
			if (isalnum(Char))//�������ĸ������
				AddCharTokenString(Char);//���뻺����
			else break;
		}
		BackChar(Char);//�˻ػ�����
		token = JudgeKeyToken(TokenBuffer);//�ж��Ƿ��ڼǺű���
		token.lexeme = TokenBuffer;
		return token;
	}
	else if (isdigit(Char))//��������֣�һ���ǳ���
	{
		for (;;)
		{
			Char = GetChar();
			if (isdigit(Char))
				AddCharTokenString(Char);
			else break;
		}
		if (Char == '.')
		{
			AddCharTokenString(Char);
			for (;;)
			{
				Char = GetChar();
				if (isdigit(Char))
					AddCharTokenString(Char);
				else break;
			}
		}
		BackChar(Char);
		token.type = CONST_ID;
		token.value = atof(TokenBuffer);
		return token;
	}
	else//�������������
	{
		switch (Char)
		{
		case ';': token.type = SEMICO; break;
		case '(': token.type = L_BRACKET; break;
		case ')': token.type = R_BRACKET; break;
		case ',': token.type = COMMA; break;
		case '+': token.type = PLUS; break;
		case '-':
			Char = GetChar();
			if (Char == '-')
			{
				while (Char != '\n' && Char != EOF) Char = GetChar();
				BackChar(Char);
				return GetToken();
			}
			else
			{
				BackChar(Char);
				token.type = MINUS;
				break;
			}
		case '/':
			Char = GetChar();
			if (Char == '/')
			{
				while (Char != '\n' && Char != EOF) Char = GetChar();
				BackChar(Char);
				return GetToken();
			}
			else
			{
				BackChar(Char);
				token.type = DIV;
				break;
			}
		case '*':
			Char = GetChar();
			if (Char == '*')
			{
				token.type = POWER;
				break;
			}
			else
			{
				BackChar(Char);
				token.type = MUL;
				break;
			}
		default:token.type = ERRTOKEN; break;
		}
	}
	return token;
}