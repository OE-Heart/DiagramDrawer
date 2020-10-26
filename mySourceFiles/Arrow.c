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
#include <string.h>

//画箭头函数 
void DrawArrow(void *arrow)
{
	if (arrow == NULL) return;
	
	myArrow ptr = (myArrow)arrow;
	double angle, rangle;   //直线与x轴夹角，用于绘制箭头实心三角
	double dx = ptr->x2 - ptr->x1, dy = ptr->y2 - ptr->y1;
	if (dy == 0 && dx == 0) return;
	
	//保存当前画笔粗细和颜色
	int penSize = GetPenSize();
	string color = GetPenColor();
	
	SetPenSize(ptr->penSize);  //设置画笔粗细
	SetPenColor(ptr->penColor);  //设置画笔颜色
	
	if (ptr->isSelected) SetPenColor(selectedColor); 
	
	//画出主体直线
	if (!strcmp(ptr->penType, "solid"))
	{
		MovePen(ptr->x1, ptr->y1);
		DrawLine(dx, dy);
	}
	else if (!strcmp(ptr->penType, "dashed"))
	{
		MovePen(ptr->x1, ptr->y1);
		DrawDashedLine(dx, dy);
	}
	
	//画出实心箭头三角,首先为angle赋值 
	if (dx == 0)   //斜率不存在 
	{
		if (dy > 0)
			angle = 90;
		else if (dy < 0)
			angle = -90;
	}
	else if (dx < 0 && dy >= 0)    //第二象限
	    angle = RegToDeg(atan(dy / dx)) + 180.0;
	else if (dx < 0 && dy < 0)    //第三象限
		angle = RegToDeg(atan(dy / dx)) - 180.0;
	else	//第一四象限
		angle = RegToDeg(atan(dy / dx));
	
	StartFilledRegion(1);
		angle += 150;
		DrawLine(0.12 * cos(DegToReg(angle)), 0.12 * sin(DegToReg(angle)));
		angle += 120;
		DrawLine(0.12 * cos(DegToReg(angle)), 0.12 * sin(DegToReg(angle)));
		angle += 120;
		DrawLine(0.12 * cos(DegToReg(angle)), 0.12 * sin(DegToReg(angle)));
	EndFilledRegion();
	
	SetPenSize(penSize); /*恢复粗细*/
	SetPenColor(color);/*恢复颜色*/
}


//画直角箭头函数
void DrawRtArrow(void *RtArrow)
{
	if (RtArrow == NULL) return;
	
	myRtArrow ptr = (myRtArrow)RtArrow;
	double angle = ptr->angle; //画图用角度
	void (*DrawSelectLine)(double, double); //用于选择画什么线 
	double dx, dy;
	dx = ptr->x2 - ptr->x1;
	dy = ptr->y2 - ptr->y1;
	
	if (dx == 0 && dy == 0) return;
	
	//保存当前画笔粗细和颜色
	int penSize = GetPenSize();
	string color = GetPenColor();
	
	SetPenSize(ptr->penSize);  //设置画笔粗细
	SetPenColor(ptr->penColor);  //设置画笔颜色
	
	if (ptr->isSelected) SetPenColor(selectedColor); 
	
	MovePen(ptr->x1, ptr->y1);
	
	if (!strcmp(ptr->penType, "solid"))
	{
		DrawSelectLine = DrawLine;
	}
	else if (!strcmp(ptr->penType, "dashed"))
	{
		DrawSelectLine = DrawDashedLine;
	}
	
	//绘制直角箭头主体部分 
	if (ptr->direction == TRUE) //顺时针
	{
		if (dx * dy <= 0)
		{
			DrawSelectLine(dx, 0);
			DrawSelectLine(0, dy);
			if (dy == 0)
			{
				if (dx > 0)
					angle = 0;
				else
					angle = 180;
			}
			else angle = dy * 90 / fabs(dy);
		}
		else
		{
			DrawSelectLine(0, dy);
			DrawSelectLine(dx, 0);
			if (dx > 0)
				angle = 0;
			else
				angle = 180;
		}
	}
	else  //逆时针 
	{
		if (dx * dy <= 0)
		{
			DrawSelectLine(0, dy);
			DrawSelectLine(dx, 0);
			if (dx > 0)
				angle = 0;
			else
				angle = 180;
		}
		else
		{
			DrawSelectLine(dx, 0);
			DrawSelectLine(0, dy);
			if (dy == 0)
			{
				if (dx > 0)
					angle = 0;
				else
					angle = 180;
			}
			else angle = dy * 90 / fabs(dy);
		}
	}
	
	//绘制箭头
	StartFilledRegion(1);
		angle += 150;
		DrawLine(0.12 * cos(DegToReg(angle)), 0.12 * sin(DegToReg(angle)));
		angle += 120;
		DrawLine(0.12 * cos(DegToReg(angle)), 0.12 * sin(DegToReg(angle)));
		angle += 120;
		DrawLine(0.12 * cos(DegToReg(angle)), 0.12 * sin(DegToReg(angle)));
	EndFilledRegion();
	
	SetPenSize(penSize); /*恢复粗细*/
	SetPenColor(color);/*恢复颜色*/
}

/*箭头拖移函数
 dx和dy为移动距离*/ 
void MoveArrow(void *obj, double dx, double dy)
{
	if (obj == NULL) return;
	myArrow arrow = (myArrow)obj;
	
	if (arrow->isSelected)
	{
		arrow->x1 += dx;
		arrow->x2 += dx;
		arrow->y1 += dy;
		arrow->y2 += dy;
	}
}

/*箭头缩放函数
 dx和dy为箭头末端移动距离*/ 
void ChangeArrow(void *obj, double dx, double dy)
{
	if (obj == NULL) return;
	myArrow arrow = (myArrow)obj;
	
	if (arrow->isSelected)
	{
		arrow->x2 += dx;
		arrow->y2 += dy;
	}
}

/*直角箭头拖移函数
 dx和dy为移动距离*/ 
void MoveRtArrow(void *obj, double dx, double dy)
{
	if (obj == NULL) return;
	myRtArrow rtarrow = (myRtArrow)obj;
	
	if (rtarrow->isSelected)
	{
		rtarrow->x1 += dx;
		rtarrow->x2 += dx;
		rtarrow->y1 += dy;
		rtarrow->y2 += dy;
	}
}

/*直角箭头缩放函数
 dx和dy为箭头末端移动距离*/ 
void ChangeRtArrow(void *obj, double dx, double dy)
{
	if (obj == NULL) return;
	myRtArrow rtarrow = (myRtArrow)obj;
	
	if (rtarrow->isSelected)
	{
		rtarrow->x2 += dx;
		rtarrow->y2 += dy;
	}
}

//画虚线函数
//用法和特性与 DrawLine() 函数相同 
void DrawDashedLine(double dx, double dy)
{
	double angle; //直线角度
	double Length, currentLen = 0; //线的总长和当前长度
	double DashLen = 0.07;  //单个虚线长度和间隙长度 
	int i;   //循环变量 
	double cx, cy, InitX, InitY;
	
	InitX = GetCurrentX();
	InitY = GetCurrentY();
	 
	//为angle赋值 
	if (dx == 0)   //斜率不存在 
	{
		if (dy > 0)
			angle = 90;
		else if (dy < 0)
			angle = -90;
	}
	else if (dx < 0 && dy >= 0)    //第二象限
	    angle = RegToDeg(atan(dy / dx)) + 180.0;
	else if (dx < 0 && dy < 0)    //第三象限
		angle = RegToDeg(atan(dy / dx)) - 180.0;
	else	//第一四象限
		angle = RegToDeg(atan(dy / dx));
	
	Length = sqrt(dx * dx + dy * dy);
	
	for (i = 0; i < (int)(Length / (2 * DashLen)); i++)
						/*总长度除单元长度得循环次数*/
	{
		DrawLine(DashLen * cos(DegToReg(angle)), DashLen * sin(DegToReg(angle)));   //画一小段直线 
		cx = GetCurrentX();
		cy = GetCurrentY();
		MovePen(cx + DashLen * cos(DegToReg(angle)), cy + DashLen * sin(DegToReg(angle)));   //向前移动画笔
		currentLen += 2 * DashLen;
	}
	
	//处理末端剩余长度 
	if (Length - currentLen <= DashLen)  //剩余长度小于等于单条短线
	{
		DrawLine((Length - currentLen) * cos(DegToReg(angle)), (Length - currentLen) * sin(DegToReg(angle)));
	}
	else
	{
		DrawLine(DashLen * cos(DegToReg(angle)), DashLen * sin(DegToReg(angle)));   //画一小段直线		
	}
	
	MovePen(InitX + dx, InitY + dy);
}
