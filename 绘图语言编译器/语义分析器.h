#include<windows.h>
#include<wingdi.h>
#include"�﷨������.h"

#define blue RGB(0,0,255)//��ɫ
#define black RGB(0,0,0)//��ɫ
#define red RGB(255,0,0)//��ɫ
#define green RGB(0,255,0)//��ɫ

//�ⲿ��������
extern void DrawPixel(unsigned long x, unsigned long y);//����һ����
extern double GetExprValue(struct ExprNode * root);//��ñ��ʽ��ֵ
extern void DrawLoop(double Start,
	double End,
	double Step,
	struct ExprNode *HorPtr,
	struct ExprNode *VerPtr);//ͼ�λ���
extern void DelExprTree(struct ExprNode *root);//ɾ��һ����
#pragma once