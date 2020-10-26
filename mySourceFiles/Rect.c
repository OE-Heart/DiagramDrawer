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

void DrawRect(void *rect)
{
	if(rect == NULL) return;
	myRect r = (myRect)rect;
	
	double a = r->x2-r->x1;/*¾ØÐÎµÄ³¤*/ 
	double b = r->y1-r->y2;/*¾ØÐÎµÄ¿í*/ 
	
	//±£´æµ±Ç°»­±Ê´ÖÏ¸ºÍÑÕÉ«
	int penSize = GetPenSize();
	string color = GetPenColor();
	
	SetPenSize(r->penSize);  //ÉèÖÃ»­±Ê´ÖÏ¸
	if (strcmp(r->penType, "solid") == 0)
	{
		SetPenColor(r->fillColor);/*ÉèÖÃÑÕÉ«*/
		MovePen(r->x1, r->y1);
		StartFilledRegion(1);
		DrawLine(a, 0);
		DrawLine(0, -b);
		DrawLine(-a, 0);
		DrawLine(0, b);
		EndFilledRegion();
		SetEraseMode(TRUE);
		DrawLine(a, 0);
		DrawLine(0, -b);
		DrawLine(-a, 0);
		DrawLine(0, b);
		SetEraseMode(FALSE);
		if (!r->isSelected) SetPenColor(r->penColor);
		else SetPenColor(selectedColor);
		DrawLine(a, 0);
		DrawLine(0, -b);
		DrawLine(-a, 0);
		DrawLine(0, b);
	}
	else
	{
		SetPenColor(r->fillColor);/*ÉèÖÃÑÕÉ«*/
		MovePen(r->x1, r->y1);
		StartFilledRegion(1);
		DrawLine(a, 0);
		DrawLine(0, -b);
		DrawLine(-a, 0);
		DrawLine(0, b);
		EndFilledRegion();
		SetEraseMode(TRUE);
		DrawLine(a, 0);
		DrawLine(0, -b);
		DrawLine(-a, 0);
		DrawLine(0, b);
		SetEraseMode(FALSE);
		if (!r->isSelected) SetPenColor(r->penColor);
		else SetPenColor(selectedColor);
		DrawDashedLine(a, 0);
		DrawDashedLine(0, -b);
		DrawDashedLine(-a, 0);
		DrawDashedLine(0, b);
	}
	
	SetPenSize(penSize); /*»Ö¸´´ÖÏ¸*/
	SetPenColor(color);/*»Ö¸´ÑÕÉ«*/
} 

void MoveRect(void *rect, double dx, double dy)
{
	if(rect == NULL) return;
	myRect r = (myRect)rect;
	
	if (r->isSelected)
	{
		r->x1 += dx;
		r->x2 += dx;
		r->y1 += dy;
		r->y2 += dy;
	}
}

void ChangeRect(void *rect, double dx, double dy)
{
	if(rect == NULL) return;
	myRect r = (myRect)rect;
	
	if (r->isSelected)
	{
		r->x2 += dx;
		r->y2 += dy;
	}
}
