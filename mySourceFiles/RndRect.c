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

void DrawRndRect(void *rndRect)
{
	if(rndRect == NULL) return;
	myRndRect r = (myRndRect)rndRect;	
	
	double a = r->x2-r->x1;/*¾ØÐÎµÄ³¤*/ 
	double b = r->y1-r->y2;/*¾ØÐÎµÄ¿í*/ 
	double cr = a>b?b/5:a/5;/*Ô²½Ç°ë¾¶*/
	
	//±£´æµ±Ç°»­±Ê´ÖÏ¸ºÍÑÕÉ«
	int penSize = GetPenSize();
	string color = GetPenColor();
	
	SetPenSize(r->penSize);  //ÉèÖÃ»­±Ê´ÖÏ¸
	if (strcmp(r->penType, "solid") == 0)
	{
		SetPenColor(r->fillColor);/*ÉèÖÃÑÕÉ«*/
		MovePen(r->x1+cr, r->y1);
		StartFilledRegion(1);
		DrawLine(a-2*cr, 0);
		DrawLine(cr, -cr);
		DrawLine(0, -(b-2*cr));
		DrawLine(-cr, -cr);
		DrawLine(-(a-2*cr), 0);
		DrawLine(-cr, cr);
		DrawLine(0, b-2*cr);
		DrawLine(cr, cr);
		EndFilledRegion();
		MovePen(r->x2-cr, r->y1);
		StartFilledRegion(1);
		DrawArc(cr, 90, -90);
		EndFilledRegion();
		MovePen(r->x2, r->y2+cr);
		StartFilledRegion(1);
		DrawArc(cr, 0, -90);
		EndFilledRegion();
		MovePen(r->x1+cr, r->y2);
		StartFilledRegion(1);
		DrawArc(cr, -90, -90);
		EndFilledRegion();
		MovePen(r->x1, r->y1-cr);
		StartFilledRegion(1);
		DrawArc(cr, -180, -90);
		EndFilledRegion();
		if (!r->isSelected) SetPenColor(r->penColor);
		else SetPenColor(selectedColor);
		DrawLine(a-2*cr, 0);
		DrawLine(cr, -cr);
		DrawLine(0, -(b-2*cr));
		DrawLine(-cr, -cr);
		DrawLine(-(a-2*cr), 0);
		DrawLine(-cr, cr);
		DrawLine(0, b-2*cr);
		DrawLine(cr, cr);
	}
	else
	{
		SetPenColor(r->fillColor);/*ÉèÖÃÑÕÉ«*/
		MovePen(r->x1+cr, r->y1);
		StartFilledRegion(1);
		DrawLine(a-2*cr, 0);
		DrawLine(cr, -cr);
		DrawLine(0, -(b-2*cr));
		DrawLine(-cr, -cr);
		DrawLine(-(a-2*cr), 0);
		DrawLine(-cr, cr);
		DrawLine(0, b-2*cr);
		DrawLine(cr, cr);
		EndFilledRegion();
		MovePen(r->x2-cr, r->y1);
		StartFilledRegion(1);
		DrawArc(cr, 90, -90);
		EndFilledRegion();
		MovePen(r->x2, r->y2+cr);
		StartFilledRegion(1);
		DrawArc(cr, 0, -90);
		EndFilledRegion();
		MovePen(r->x1+cr, r->y2);
		StartFilledRegion(1);
		DrawArc(cr, -90, -90);
		EndFilledRegion();
		MovePen(r->x1, r->y1-cr);
		StartFilledRegion(1);
		DrawArc(cr, -180, -90);
		EndFilledRegion();
		SetPenColor("White");
		DrawLine(a-2*cr, 0);
		DrawLine(cr, -cr);
		DrawLine(0, -(b-2*cr));
		DrawLine(-cr, -cr);
		DrawLine(-(a-2*cr), 0);
		DrawLine(-cr, cr);
		DrawLine(0, b-2*cr);
		DrawLine(cr, cr);
		if (!r->isSelected) SetPenColor(r->penColor);
		else SetPenColor(selectedColor);
		DrawDashedLine(a-2*cr, 0);
		DrawDashedLine(cr, -cr);
		DrawDashedLine(0, -(b-2*cr));
		DrawDashedLine(-cr, -cr);
		DrawDashedLine(-(a-2*cr), 0);
		DrawDashedLine(-cr, cr);
		DrawDashedLine(0, b-2*cr);
		DrawDashedLine(cr, cr);
	}
	SetEraseMode(TRUE);
	MovePen(r->x2-cr, r->y1);
	DrawArc(cr, 90, -90);
	MovePen(r->x2, r->y2+cr);
	DrawArc(cr, 0, -90);
	MovePen(r->x1+cr, r->y2);
	DrawArc(cr, -90, -90);
	MovePen(r->x1, r->y1-cr);
	DrawArc(cr, -180, -90);
	SetEraseMode(FALSE);
	if (!r->isSelected) SetPenColor(r->penColor);
	else SetPenColor(selectedColor);
	MovePen(r->x2-cr, r->y1);
	DrawArc(cr, 90, -90);
	MovePen(r->x2, r->y2+cr);
	DrawArc(cr, 0, -90);
	MovePen(r->x1+cr, r->y2);
	DrawArc(cr, -90, -90);
	MovePen(r->x1, r->y1-cr);
	DrawArc(cr, -180, -90);
	
	SetPenSize(penSize); /*»Ö¸´´ÖÏ¸*/
	SetPenColor(color);/*»Ö¸´ÑÕÉ«*/
} 

void MoveRndRect(void *rndrect, double dx, double dy)
{
	if(rndrect == NULL) return;
	myRndRect r = (myRndRect)rndrect;
	
	if (r->isSelected)
	{
		r->x1 += dx;
		r->x2 += dx;
		r->y1 += dy;
		r->y2 += dy;
	}
}

void ChangeRndRect(void *rndrect, double dx, double dy)
{
	if(rndrect == NULL) return;
	myRndRect r = (myRndRect)rndrect;
	
	if (r->isSelected)
	{
		r->x2 += dx;
		r->y2 += dy;
	}
}
