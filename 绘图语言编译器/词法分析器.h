#ifndef SCANNER_H
#define SCANNER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //ctype.h��C��׼�������е�ͷ�ļ���������һ��C�����ַ����ຯ�������ڲ����ַ��Ƿ������ض����ַ����,����ĸ�ַ��������ַ��ȵȡ�
#include <stdarg.h>//�ú������Խ��տɱ����
#include <math.h>

enum Token_Type//�Ǻ����ö��
{
	ORIGIN,SCALE,ROT,IS,TO,STEP,DRAW,FOR,FROM,T,
	SEMICO, L_BRACKET, R_BRACKET, COMMA,
	PLUS, MINUS, MUL, DIV, POWER,FUNC,
	CONST_ID,NONTOKEN,ERRTOKEN,
};

typedef double(*MathFuncPtr)(double);//MathFuncPtr����һ��ָ�򷵻�doubleֵ������һ��double�βεĺ�����ָ�������

struct Token
{
	Token_Type type;//�Ǻ����
	char * lexeme;//���ԣ��ַ�����ָ��char���͵�ָ��
	double value;//���ԣ�������ֵ��double��
	double(*FuncPtr)(double);//���ԣ�����ָ�룬����һ��ָ�򷵻�doubleֵ������һ��double�βεĺ�����ָ�������
};

static Token TokenTab[] = //���ű�����
{
	{ CONST_ID,	(char*)"PI", 3.141592653,	NULL },
{ CONST_ID,	(char*)"E",	2.71828,	NULL },
{ T,		(char*)"T",		0.0,	NULL },
{ FUNC,		(char*)"SIN",		0.0,	sin },
{ FUNC,		(char*)"COS",		0.0,	cos },
{ FUNC,		(char*)"TAN",		0.0,	tan },
{ FUNC,		(char*)"LN",		0.0,	log },
{ FUNC,		(char*)"EXP",		0.0,	exp },
{ FUNC,		(char*)"SQRT",		0.0,	sqrt },
{ ORIGIN,	(char*)"ORIGIN",	0.0,	NULL },
{ SCALE,	(char*)"SCALE",	0.0,	NULL },
{ ROT,		(char*)"ROT",		0.0,	NULL },
{ IS,		(char*)"IS",		0.0,	NULL },
{ FOR,		(char*)"FOR",		0.0,	NULL },
{ FROM,		(char*)"FROM",		0.0,	NULL },
{ TO,		(char*)"TO",		0.0,	NULL },
{ STEP,		(char*)"STEP",		0.0,	NULL },
{ DRAW,		(char*)"DRAW",		0.0,	NULL },
};

extern unsigned int LineNo;//���ټǺ�����Դ�ļ��к�
extern int InitScanner(const char *);//��ʼ���ʷ�������
extern Token GetToken(void);//��ȡ�Ǻź���
extern void CloseScanner(void);//�رմʷ�������

#endif
#pragma once