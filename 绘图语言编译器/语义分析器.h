#include<windows.h>
#include<wingdi.h>
#include"语法分析器.h"

#define blue RGB(0,0,255)//蓝色
#define black RGB(0,0,0)//黑色
#define red RGB(255,0,0)//红色
#define green RGB(0,255,0)//绿色

//外部函数声明
extern void DrawPixel(unsigned long x, unsigned long y);//绘制一个点
extern double GetExprValue(struct ExprNode * root);//获得表达式的值
extern void DrawLoop(double Start,
	double End,
	double Step,
	struct ExprNode *HorPtr,
	struct ExprNode *VerPtr);//图形绘制
extern void DelExprTree(struct ExprNode *root);//删除一棵树
#pragma once