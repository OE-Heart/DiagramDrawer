#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include "linkedlist.h"
#include "figure.h"

void DrawDiamond(void *diamond)
{
	if(diamond == NULL) return; 
	myDiamond r = (myDiamond)diamond;

	/*矩形外框*/ 
	double a = r->x2-r->x1;/*矩形的长*/ 
	double b = r->y1-r->y2;/*矩形的宽*/ 
	
	//保存当前画笔粗细和颜色
	int penSize = GetPenSize();
	string color = GetPenColor();
	
	SetPenSize(r->penSize);  //设置画笔粗细
	if (strcmp(r->penType, "solid") == 0)
	{
		SetPenColor(r->fillColor);/*设置颜色*/
		MovePen(r->x1, r->y1-b/2);
		StartFilledRegion(1);
		DrawLine(a/2, b/2);
		DrawLine(a/2, -b/2);
		DrawLine(-a/2, -b/2);
		DrawLine(-a/2, b/2);
		EndFilledRegion();
		SetEraseMode(TRUE);
		DrawLine(a/2, b/2);
		DrawLine(a/2, -b/2);
		DrawLine(-a/2, -b/2);
		DrawLine(-a/2, b/2);
		SetEraseMode(FALSE);
		if (!r->isSelected) SetPenColor(r->penColor);
		else SetPenColor(selectedColor);
		DrawLine(a/2, b/2);
		DrawLine(a/2, -b/2);
		DrawLine(-a/2, -b/2);
		DrawLine(-a/2, b/2);
	}
	else
	{
		SetPenColor(r->fillColor);/*设置颜色*/
		MovePen(r->x1, r->y1-b/2);
		StartFilledRegion(1);
		DrawLine(a/2, b/2);
		DrawLine(a/2, -b/2);
		DrawLine(-a/2, -b/2);
		DrawLine(-a/2, b/2);
		EndFilledRegion();
		SetEraseMode(TRUE);
		DrawLine(a/2, b/2);
		DrawLine(a/2, -b/2);
		DrawLine(-a/2, -b/2);
		DrawLine(-a/2, b/2);
		SetEraseMode(FALSE);
		if (!r->isSelected) SetPenColor(r->penColor);
		else SetPenColor(selectedColor);
		DrawDashedLine(a/2, b/2);
		DrawDashedLine(a/2, -b/2);
		DrawDashedLine(-a/2, -b/2);
		DrawDashedLine(-a/2, b/2);
	}
	
	SetPenSize(penSize); /*恢复粗细*/
	SetPenColor(color);/*恢复颜色*/
} 

void MoveDiamond(void *diamond, double dx, double dy)
{
	if(diamond == NULL) return;
	myDiamond r = (myDiamond)diamond;
	
	if (r->isSelected)
	{
		r->x1 += dx;
		r->x2 += dx;
		r->y1 += dy;
		r->y2 += dy;
	}
}

void ChangeDiamond(void *diamond, double dx, double dy)
{
	if(diamond == NULL) return;
	myDiamond r = (myDiamond)diamond;
	
	if (r->isSelected)
	{
		r->x2 += dx;
		r->y2 += dy;
	}
}
