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

	/*�������*/ 
	double a = r->x2-r->x1;/*���εĳ�*/ 
	double b = r->y1-r->y2;/*���εĿ�*/ 
	
	//���浱ǰ���ʴ�ϸ����ɫ
	int penSize = GetPenSize();
	string color = GetPenColor();
	
	SetPenSize(r->penSize);  //���û��ʴ�ϸ
	if (strcmp(r->penType, "solid") == 0)
	{
		SetPenColor(r->fillColor);/*������ɫ*/
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
		SetPenColor(r->fillColor);/*������ɫ*/
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
	
	SetPenSize(penSize); /*�ָ���ϸ*/
	SetPenColor(color);/*�ָ���ɫ*/
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
