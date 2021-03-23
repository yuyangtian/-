#ifndef PARSER_H
#define PARSER_H
#include"�ʷ�������.h"
typedef double(*FuncPtr)(double);
struct ExprNode                                             //�﷨���ڵ�����
{
	enum Token_Type OpCode;                                 //PLUS,MINUS,MUL,DIV,POWER,FUNC,CONST_ID��T
	union
	{
		struct { ExprNode *Left, *Right; }CaseOperater;     //��Ԫ���㣺ֻ�����Һ��ӵ��ڲ��ڵ�
		struct { ExprNode *Child; FuncPtr MathFuncPtr; }CaseFunc;//�������ã�ֻ��һ�����ӵ��ڲ��ڵ㣬����һ��ָ���Ӧ��������ָ�� MathFuncPtr
		double CaseConst;                                   //������Ҷ�ӽڵ�  ��ֵ
		double *CaseParmPtr;                                //����T   ��ֵ�����T��ֵ�õ�ַ
	}Content;
};

extern void Parser(char *SrcFilePtr);                       //�﷨����������ӿ�
#endif #pragma once
#pragma once
