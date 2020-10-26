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

//����ͷ���� 
void DrawArrow(void *arrow)
{
	if (arrow == NULL) return;
	
	myArrow ptr = (myArrow)arrow;
	double angle, rangle;   //ֱ����x��нǣ����ڻ��Ƽ�ͷʵ������
	double dx = ptr->x2 - ptr->x1, dy = ptr->y2 - ptr->y1;
	if (dy == 0 && dx == 0) return;
	
	//���浱ǰ���ʴ�ϸ����ɫ
	int penSize = GetPenSize();
	string color = GetPenColor();
	
	SetPenSize(ptr->penSize);  //���û��ʴ�ϸ
	SetPenColor(ptr->penColor);  //���û�����ɫ
	
	if (ptr->isSelected) SetPenColor(selectedColor); 
	
	//��������ֱ��
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
	
	//����ʵ�ļ�ͷ����,����Ϊangle��ֵ 
	if (dx == 0)   //б�ʲ����� 
	{
		if (dy > 0)
			angle = 90;
		else if (dy < 0)
			angle = -90;
	}
	else if (dx < 0 && dy >= 0)    //�ڶ�����
	    angle = RegToDeg(atan(dy / dx)) + 180.0;
	else if (dx < 0 && dy < 0)    //��������
		angle = RegToDeg(atan(dy / dx)) - 180.0;
	else	//��һ������
		angle = RegToDeg(atan(dy / dx));
	
	StartFilledRegion(1);
		angle += 150;
		DrawLine(0.12 * cos(DegToReg(angle)), 0.12 * sin(DegToReg(angle)));
		angle += 120;
		DrawLine(0.12 * cos(DegToReg(angle)), 0.12 * sin(DegToReg(angle)));
		angle += 120;
		DrawLine(0.12 * cos(DegToReg(angle)), 0.12 * sin(DegToReg(angle)));
	EndFilledRegion();
	
	SetPenSize(penSize); /*�ָ���ϸ*/
	SetPenColor(color);/*�ָ���ɫ*/
}


//��ֱ�Ǽ�ͷ����
void DrawRtArrow(void *RtArrow)
{
	if (RtArrow == NULL) return;
	
	myRtArrow ptr = (myRtArrow)RtArrow;
	double angle = ptr->angle; //��ͼ�ýǶ�
	void (*DrawSelectLine)(double, double); //����ѡ��ʲô�� 
	double dx, dy;
	dx = ptr->x2 - ptr->x1;
	dy = ptr->y2 - ptr->y1;
	
	if (dx == 0 && dy == 0) return;
	
	//���浱ǰ���ʴ�ϸ����ɫ
	int penSize = GetPenSize();
	string color = GetPenColor();
	
	SetPenSize(ptr->penSize);  //���û��ʴ�ϸ
	SetPenColor(ptr->penColor);  //���û�����ɫ
	
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
	
	//����ֱ�Ǽ�ͷ���岿�� 
	if (ptr->direction == TRUE) //˳ʱ��
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
	else  //��ʱ�� 
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
	
	//���Ƽ�ͷ
	StartFilledRegion(1);
		angle += 150;
		DrawLine(0.12 * cos(DegToReg(angle)), 0.12 * sin(DegToReg(angle)));
		angle += 120;
		DrawLine(0.12 * cos(DegToReg(angle)), 0.12 * sin(DegToReg(angle)));
		angle += 120;
		DrawLine(0.12 * cos(DegToReg(angle)), 0.12 * sin(DegToReg(angle)));
	EndFilledRegion();
	
	SetPenSize(penSize); /*�ָ���ϸ*/
	SetPenColor(color);/*�ָ���ɫ*/
}

/*��ͷ���ƺ���
 dx��dyΪ�ƶ�����*/ 
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

/*��ͷ���ź���
 dx��dyΪ��ͷĩ���ƶ�����*/ 
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

/*ֱ�Ǽ�ͷ���ƺ���
 dx��dyΪ�ƶ�����*/ 
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

/*ֱ�Ǽ�ͷ���ź���
 dx��dyΪ��ͷĩ���ƶ�����*/ 
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

//�����ߺ���
//�÷��������� DrawLine() ������ͬ 
void DrawDashedLine(double dx, double dy)
{
	double angle; //ֱ�߽Ƕ�
	double Length, currentLen = 0; //�ߵ��ܳ��͵�ǰ����
	double DashLen = 0.07;  //�������߳��Ⱥͼ�϶���� 
	int i;   //ѭ������ 
	double cx, cy, InitX, InitY;
	
	InitX = GetCurrentX();
	InitY = GetCurrentY();
	 
	//Ϊangle��ֵ 
	if (dx == 0)   //б�ʲ����� 
	{
		if (dy > 0)
			angle = 90;
		else if (dy < 0)
			angle = -90;
	}
	else if (dx < 0 && dy >= 0)    //�ڶ�����
	    angle = RegToDeg(atan(dy / dx)) + 180.0;
	else if (dx < 0 && dy < 0)    //��������
		angle = RegToDeg(atan(dy / dx)) - 180.0;
	else	//��һ������
		angle = RegToDeg(atan(dy / dx));
	
	Length = sqrt(dx * dx + dy * dy);
	
	for (i = 0; i < (int)(Length / (2 * DashLen)); i++)
						/*�ܳ��ȳ���Ԫ���ȵ�ѭ������*/
	{
		DrawLine(DashLen * cos(DegToReg(angle)), DashLen * sin(DegToReg(angle)));   //��һС��ֱ�� 
		cx = GetCurrentX();
		cy = GetCurrentY();
		MovePen(cx + DashLen * cos(DegToReg(angle)), cy + DashLen * sin(DegToReg(angle)));   //��ǰ�ƶ�����
		currentLen += 2 * DashLen;
	}
	
	//����ĩ��ʣ�೤�� 
	if (Length - currentLen <= DashLen)  //ʣ�೤��С�ڵ��ڵ�������
	{
		DrawLine((Length - currentLen) * cos(DegToReg(angle)), (Length - currentLen) * sin(DegToReg(angle)));
	}
	else
	{
		DrawLine(DashLen * cos(DegToReg(angle)), DashLen * sin(DegToReg(angle)));   //��һС��ֱ��		
	}
	
	MovePen(InitX + dx, InitY + dy);
}
