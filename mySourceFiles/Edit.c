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
#include <string.h>
#include "linkedlist.h"
#include "figure.h"
#include "Edit.h"

linkedlistADT CopyList[FLIST] = {NULL, NULL, NULL, NULL, NULL}; //���ڴ洢Ŀǰ�Ѹ��Ƶ�ͼ�� 
int pasteTime = 0; /*���ڼ�¼��ճ������*/ 

//ѡ���жϺ���
//x, yΪ�������
//���ɹ�ѡ�� 
bool Select(double x, double y)
{
	bool haveSelected;
	
	haveSelected = SelectArrow(x, y);
	if (haveSelected) return TRUE;
	haveSelected = SelectRtArrow(x, y);
	if (haveSelected) return TRUE;
	haveSelected = SelectRect(x, y);
	if (haveSelected) return TRUE;
	haveSelected = SelectRndRect(x, y);
	if (haveSelected) return TRUE;
	haveSelected = SelectDiamond(x, y);
	if (haveSelected) return TRUE;
	haveSelected = SelectText(x, y);
	if (haveSelected) return TRUE;
	
	return FALSE;
}

 
//�����������ж� cx �Ƿ��� x1 �� x2 ֮�� 
bool isBetween(double x1, double x2, double cx)
{
	if (x1 >= x2)
	{
		if (x1 >= cx && cx >= x2)
			return TRUE;
	}
	else
	{
		if (x1 <= cx && cx <= x2)
			return TRUE;
	}
	return FALSE;
}


//ѡ��ֱ�Ǽ�ͷ�жϺ���
//x, yλ������� 
//���ɹ�ѡ�У�����TRUE 
bool SelectRtArrow(double x, double y)
{
	linkedlistADT nodeptr = list[RTARROW];
	myRtArrow dataptr;
	double dx, dy;
	bool haveSelected = FALSE;  //�Ƿ��Ѿ�ѡ��һ��ͼ�� 
	bool isIn = FALSE;
	
	dataptr = (myRtArrow)nodeptr->dataptr;
	nodeptr = NextNode(list[RTARROW], nodeptr); //������һ���սڵ� 
	
	
	while (haveSelected == FALSE  && nodeptr != NULL)
	{
		dataptr = (myRtArrow)nodeptr->dataptr;
		dx = dataptr->x2 - dataptr->x1;
		dy = dataptr->y2 - dataptr->y1;
		if (dataptr->direction)  /*˳ʱ�����*/ 
		{
			if (dx * dy <= 0)  /*��������Ȼ����ٻ���*/ 
			{
				if (isBetween(MIN(dataptr->x1, dataptr->x2) - 0.2, MAX(dataptr->x1, dataptr->x2) + 0.2, x) &&
					isBetween(dataptr->y1 - 0.2, dataptr->y1 + 0.2, y))
						isIn = TRUE;
				else if (isBetween(dataptr->x2 - 0.2, dataptr->x2 + 0.2, x) &&
						 isBetween(MIN(dataptr->y1, dataptr->y2) - 0.2, MAX(dataptr->y1, dataptr->y2) + 0.2, y))
						isIn = TRUE; 
			}
			else /*��������Ȼ����ٻ���*/
			{
				if (isBetween(MIN(dataptr->x1, dataptr->x2) - 0.2, MAX(dataptr->x1, dataptr->x2) + 0.2, x) &&
					isBetween(dataptr->y2 - 0.2, dataptr->y2 + 0.2, y))
						isIn = TRUE;
				else if (isBetween(dataptr->x1 - 0.2, dataptr->x1 + 0.2, x) &&
						 isBetween(MIN(dataptr->y1, dataptr->y2) - 0.2, MAX(dataptr->y1, dataptr->y2) + 0.2, y))
						isIn = TRUE; 
			}
		}
		else /*��ʱ��*/ 
		{
			if (dx * dy <= 0)  /*��������Ȼ����ٻ���*/ 
			{
				if (isBetween(MIN(dataptr->x1, dataptr->x2) - 0.2, MAX(dataptr->x1, dataptr->x2) + 0.2, x) &&
					isBetween(dataptr->y2 - 0.2, dataptr->y2 + 0.2, y))
						isIn = TRUE;
				else if (isBetween(dataptr->x1 - 0.2, dataptr->x1 + 0.2, x) &&
						 isBetween(MIN(dataptr->y1, dataptr->y2) - 0.2, MAX(dataptr->y1, dataptr->y2) + 0.2, y))
						isIn = TRUE; 
			}
			else /*��������Ȼ����ٻ���*/
			{
				if (isBetween(MIN(dataptr->x1, dataptr->x2) - 0.2, MAX(dataptr->x1, dataptr->x2) + 0.2, x) &&
					isBetween(dataptr->y1 - 0.2, dataptr->y1 + 0.2, y))
						isIn = TRUE;
				else if (isBetween(dataptr->x2 - 0.2, dataptr->x2 + 0.2, x) &&
						 isBetween(MIN(dataptr->y1, dataptr->y2) - 0.2, MAX(dataptr->y1, dataptr->y2) + 0.2, y))
						isIn = TRUE;
			}
		}
		if (isIn)
		{
			dataptr->isSelected = TRUE;
			haveSelected = TRUE;
		}
		else
			nodeptr = NextNode(list[ARROW], nodeptr);
	}
	
	if (haveSelected)
		return TRUE;
	else
		return FALSE; 
}

//ѡ�м�ͷ�жϺ���
//x, yλ������� 
//���ɹ�ѡ�У�����TRUE 
bool SelectArrow(double x, double y)
{
	linkedlistADT nodeptr = list[ARROW];
	myArrow dataptr;
	bool haveSelected = FALSE;  //�Ƿ��Ѿ�ѡ��һ��ͼ�� 
	double angle; /*ֱ�ߵ���б��*/
	double dx, dy;
	
	dataptr = (myArrow)nodeptr->dataptr;
	nodeptr = NextNode(list[ARROW], nodeptr); //������һ���սڵ� 
	
	while (haveSelected == FALSE  && nodeptr != NULL)
	{
		dataptr = (myArrow)nodeptr->dataptr;
		dx = dataptr->x2 - dataptr->x1;
		dy = dataptr->y2 - dataptr->y1;
		/*Ϊangle��ֵ*/ 
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
		
		if (isBetween(dataptr->x1 - 0.15, dataptr->x2 + 0.15, x)) //���ж���Χ�� 
		{
			if (-90 <= angle && angle <= -85 || angle <= 90 && angle >= 85)
			{
				if (isBetween(dataptr->y1, dataptr->y2, y))
				{
					dataptr->isSelected = TRUE;  //��ͼ������Ϊ��ѡ�� 
					haveSelected = TRUE;
					return;
				}
			}
			else
			{
				if (isBetween(dataptr->y1 + (x - dataptr->x1) * tan(DegToReg(angle)) - 0.2, 
							  dataptr->y1 + (x - dataptr->x1) * tan(DegToReg(angle)) + 0.2, y))
				{
					dataptr->isSelected = TRUE;  //��ͼ������Ϊ��ѡ�� 
					haveSelected = TRUE;
					return;
				}
			}
			nodeptr = NextNode(list[ARROW], nodeptr);
		}
		else
		{
			nodeptr = NextNode(list[ARROW], nodeptr);
		}
	}
	
	if (haveSelected)
		return TRUE;
	else
		return FALSE; 
}
 
//ѡ��Բ�Ǿ����жϺ���
//x, yλ������� 
//���ɹ�ѡ�У�����TRUE 
bool SelectRndRect(double x, double y)
{
	linkedlistADT nodeptr = list[RNDRECT];
	myRndRect dataptr;
	bool haveSelected = FALSE;  //�Ƿ��Ѿ�ѡ��һ��ͼ�� 
	
	dataptr = (myRndRect)nodeptr->dataptr;
	nodeptr = NextNode(list[RNDRECT], nodeptr); //������һ���սڵ� 
	
	
	while (haveSelected == FALSE  && nodeptr != NULL)
	{
		dataptr = (myRndRect)nodeptr->dataptr;
		
		if (isBetween(dataptr->x1, dataptr->x2, x) && isBetween(dataptr->y1, dataptr->y2, y))   /*���ж���Χ��*/ 
		{
			dataptr->isSelected = TRUE;  /*��ͼ������Ϊ��ѡ�� */ 
			haveSelected = TRUE;
		}
		else
		{
			nodeptr = NextNode(list[RNDRECT], nodeptr);
		}
	}
	
	if (haveSelected)
		return TRUE;
	else
		return FALSE; 
}

//ѡ�о����жϺ���
//x, yλ������� 
//���ɹ�ѡ�У�����TRUE 
bool SelectRect(double x, double y)
{
	linkedlistADT nodeptr = list[RECT];
	myRect dataptr;
	bool haveSelected = FALSE;  //�Ƿ��Ѿ�ѡ��һ��ͼ�� 
	
	dataptr = (myRect)nodeptr->dataptr;
	nodeptr = NextNode(list[RECT], nodeptr); //������һ���սڵ� 
	
	
	while (haveSelected == FALSE  && nodeptr != NULL)
	{
		dataptr = (myRect)nodeptr->dataptr;
		
		if (isBetween(dataptr->x1, dataptr->x2, x) && isBetween(dataptr->y1, dataptr->y2, y))   //���ж���Χ�� 
		{
			dataptr->isSelected = TRUE;  //��ͼ������Ϊ��ѡ�� 
			haveSelected = TRUE;
		}
		else
		{
			nodeptr = NextNode(list[RECT], nodeptr);
		}
	}
	
	if (haveSelected)
		return TRUE;
	else
		return FALSE; 
}

 //ѡ�������жϺ���
//x, yλ������� 
//���ɹ�ѡ�У�����TRUE 
bool SelectDiamond(double x, double y)
{
	linkedlistADT nodeptr = list[DIAMOND];
	myDiamond dataptr;
	bool haveSelected = FALSE;  //�Ƿ��Ѿ�ѡ��һ��ͼ�� 
	double angle = 0; 
	double cx, cy;
	
	dataptr = (myDiamond)nodeptr->dataptr;
	nodeptr = NextNode(list[DIAMOND], nodeptr); //������һ���սڵ� 
	
	
	while (haveSelected == FALSE  && nodeptr != NULL)
	{
		dataptr = (myDiamond)nodeptr->dataptr;
		cx = (dataptr->x1 + dataptr->x2) / 2;
		cy = (dataptr->y1 + dataptr->y2) / 2;
		if (dataptr->y2 - dataptr->y1 != 0)
		{
			angle = atan((dataptr->y2 - dataptr->y1) / (dataptr->x2 - dataptr->x1));
		}
		
		if (isBetween(dataptr->x1, cx, x) && 
			isBetween(cy - (x - dataptr->x1) * tan(angle), cy + (x - dataptr->x1) * tan(angle), y))   //���ж���Χ�� 
		{
			dataptr->isSelected = TRUE;  //��ͼ������Ϊ��ѡ�� 				
			haveSelected = TRUE;
		}
		else if (isBetween(cx, dataptr->x2, x) && 
				 isBetween(cy - (dataptr->x2 - x) * tan(angle), cy + (dataptr->x2 - x) * tan(angle), y))
		{
			dataptr->isSelected = TRUE;  //��ͼ������Ϊ��ѡ�� 				
			haveSelected = TRUE;
		}
		else
		{
			nodeptr = NextNode(list[DIAMOND], nodeptr);
		}
	}
	
	if (haveSelected)
		return TRUE;
	else
		return FALSE; 
}

//ѡ���ı����жϺ���
//x, yλ������� 
//���ɹ�ѡ�У�����TRUE 
bool SelectText(double x, double y)
{
	linkedlistADT nodeptr = list[STRING];
	myString dataptr;
	bool haveSelected = FALSE; /*�Ƿ��Ѿ�ѡ��һ��ͼ��*/
	int ori_pointSize = GetPointSize();
	dataptr = (myString)nodeptr->dataptr;
	nodeptr = NextNode(list[STRING], nodeptr); /*������һ���սڵ�*/
	
	while (haveSelected == FALSE && nodeptr != NULL)
	{
		dataptr = (myString)nodeptr->dataptr;
		SetPointSize(dataptr->pointSize);
		
		if (isBetween(dataptr->x, dataptr->x + TextStringWidth(dataptr->text), x) &&
			isBetween(dataptr->y, dataptr->y + GetFontHeight(), y))
		{
			dataptr->isSelected = TRUE; /*��ͼ������Ϊ��ѡ��*/
			haveSelected = TRUE; 
		}
		else
		{
			nodeptr = NextNode(list[STRING], nodeptr);
		}
		SetPointSize(ori_pointSize);
	}
	
	if (haveSelected)
		return TRUE;
	else
		return FALSE;
} 

//ȡ��ѡ�к���
//����ǰ��ѡ�е����к���ȡ��ѡ��
void CancelSelect()
{
	int i;
	void trans(void *obj)
	{
		myArrow Arrow;
		myRtArrow RtArrow;
		myRect Rect;
		myRndRect RndRect;
		myDiamond Diamond;
		myString String;
		switch(i)
		{
			case ARROW:
				Arrow = (myArrow)obj;
				Arrow->isSelected = FALSE;
				break;
			case RTARROW:
				RtArrow = (myRtArrow)obj;
				RtArrow->isSelected = FALSE;
				break;
			case RECT:
				Rect = (myRect)obj;
				Rect->isSelected = FALSE;
				break;
			case RNDRECT:
				RndRect = (myRndRect)obj;
				RndRect->isSelected = FALSE;
				break;
			case DIAMOND:
				Diamond = (myDiamond)obj;
				Diamond->isSelected = FALSE;
				break;
			case STRING:
				String = (myString)obj;
				String->isSelected = FALSE;
				break;
		}
	}
	for (i = 0; i < FLIST; i++)
	{
		TraverseLinkedList(list[i], trans);
	}
} 


//���ƺ���
//�������������һ�θ��Ƶ�����ͼ�� 
//�������������е�ǰ��ѡ�е�ͼ��
void Copy()
{
	int i;  //ѭ������
	linkedlistADT Readptr, Writeptr;
	//�����һ�ε����и��� 
	FreeLinkedList(CopyList[ARROW]);
	FreeLinkedList(CopyList[RTARROW]);
	FreeLinkedList(CopyList[RECT]);
	FreeLinkedList(CopyList[RNDRECT]);
	FreeLinkedList(CopyList[DIAMOND]);
	pasteTime = 0; 
	
	myArrow Arrow_add, Arrow;
	myRtArrow RtArrow_add, RtArrow;
	myRect Rect_add, Rect;
	myRndRect RndRect_add, RndRect;
	myDiamond Diamond_add, Diamond;
	myString String_add, String;
	
	for (i = 0; i < FLIST; i++)
	{
		Readptr = list[i];
		Readptr = NextNode(list[i], Readptr); //������һ���սڵ� 
		Writeptr = NewLinkedList(); 
		CopyList[i] = Writeptr;
		while (Readptr != NULL)
		{
			switch (i)
			{
				case ARROW:
					Arrow = (myArrow)Readptr->dataptr;
					if (Arrow->isSelected)
					{
						Arrow_add = (myArrow)GetBlock(sizeof(*Arrow_add));
						//Ϊ�����Ľṹ�帳ֵ 
						Arrow_add->x1 = Arrow->x1;
						Arrow_add->x2 = Arrow->x2;
						Arrow_add->y1 = Arrow->y1;
						Arrow_add->y2 = Arrow->y2;
						Arrow_add->isSelected = Arrow->isSelected;
						Arrow_add->penColor = Arrow->penColor;
						Arrow_add->penSize = Arrow->penSize;
						Arrow_add->penType = Arrow->penType;
						
						InsertNode(CopyList[i], NULL, Arrow_add);   //�������ṹ��������������� 
					}
					Readptr = NextNode(list[i], Readptr);
					break;
				case RTARROW:
					RtArrow = (myRtArrow)Readptr->dataptr;
					if (RtArrow->isSelected)
					{
						RtArrow_add = (myRtArrow)GetBlock(sizeof(*RtArrow_add));
						//Ϊ�����Ľṹ�帳ֵ 
						RtArrow_add->x1 = RtArrow->x1;
						RtArrow_add->x2 = RtArrow->x2;
						RtArrow_add->y1 = RtArrow->y1;
						RtArrow_add->y2 = RtArrow->y2;
						RtArrow_add->direction = RtArrow->direction;
						RtArrow_add->isSelected = RtArrow->isSelected;
						RtArrow_add->penColor = RtArrow->penColor;
						RtArrow_add->penSize = RtArrow->penSize;
						RtArrow_add->penType = RtArrow->penType;
					
						InsertNode(CopyList[i], NULL, RtArrow_add);   //�������ṹ��������������� 
					}
					Readptr = NextNode(list[i], Readptr);
					break;
				case RECT:
					Rect = (myRect)Readptr->dataptr;
					if (Rect->isSelected)
					{
						Rect_add = (myRect)GetBlock(sizeof(*Rect_add));
						//Ϊ�����Ľṹ�帳ֵ 
						Rect_add->x1 = Rect->x1;
						Rect_add->x2 = Rect->x2;
						Rect_add->y1 = Rect->y1;
						Rect_add->y2 = Rect->y2;						
						Rect_add->isSelected = Rect->isSelected;
						Rect_add->penColor = Rect->penColor;
						Rect_add->penSize = Rect->penSize;
						Rect_add->penType = Rect->penType;
						Rect_add->fillColor = Rect->fillColor;					
						
						InsertNode(CopyList[i], NULL, Rect_add);  //�������ṹ��������������� 
					}
					Readptr = NextNode(list[i], Readptr);
					break;
				case RNDRECT:
					RndRect = (myRndRect)Readptr->dataptr;
					if (RndRect->isSelected)
					{
						RndRect_add = (myRndRect)GetBlock(sizeof(*RndRect_add));
						//Ϊ�����Ľṹ�帳ֵ 
						RndRect_add->x1 = RndRect->x1;
						RndRect_add->x2 = RndRect->x2;
						RndRect_add->y1 = RndRect->y1;
						RndRect_add->y2 = RndRect->y2;
						RndRect_add->isSelected = RndRect->isSelected;
						RndRect_add->penColor = RndRect->penColor;
						RndRect_add->penSize = RndRect->penSize;
						RndRect_add->penType = RndRect->penType;
						RndRect_add->fillColor = RndRect->fillColor;
						
						InsertNode(CopyList[i], NULL, RndRect_add);   //�������ṹ��������������� 
					}
					Readptr = NextNode(list[i], Readptr);
					break;
				case DIAMOND:
					Diamond = (myDiamond)Readptr->dataptr;
					if (Diamond->isSelected)
					{
						Diamond_add = (myDiamond)GetBlock(sizeof(*Diamond_add));
						//Ϊ�����Ľṹ�帳ֵ 
						Diamond_add->x1 = Diamond->x1;
						Diamond_add->x2 = Diamond->x2;
						Diamond_add->y1 = Diamond->y1;
						Diamond_add->y2 = Diamond->y2;
						Diamond_add->isSelected = Diamond->isSelected;
						Diamond_add->penColor = Diamond->penColor;
						Diamond_add->penSize = Diamond->penSize;
						Diamond_add->penType = Diamond->penType;
						Diamond_add->fillColor = Diamond->fillColor;
						
						InsertNode(CopyList[i], NULL, Diamond_add);  //�������ṹ��������������� 
					}
					Readptr = NextNode(list[i], Readptr);
					break;	
				case STRING:
					String = (myString)Readptr->dataptr;
					if (String->isSelected)
					{
						String_add = (myString)GetBlock(sizeof(*String_add));
						/*Ϊ�����Ľṹ�帳ֵ*/
						String_add->cursor = String->cursor;
						String_add->isBlink = String->isBlink;
						String_add->isSelected = String->isSelected;
						String_add->penColor = String->penColor;
						String_add->pointSize = String->pointSize;
						String_add->text = String->text;
						String_add->x = String->x;
						String_add->y = String->y;
						
						InsertNode(CopyList[i], NULL, String_add); /*�������ṹ���������������*/ 
					}
					Readptr = NextNode(list[i], Readptr);
					break;	
			}
		}
	}
}

//ճ������
//����ǰ��ճ���������ڵ�����ͼ�μ��뵽��������
void Paste()
{
	linkedlistADT Readptr;
	int i; //ѭ������
	myArrow Arrow, Arrow_add;
	myRtArrow RtArrow, RtArrow_add;
	myRect Rect, Rect_add;
	myRndRect RndRect, RndRect_add;
	myDiamond Diamond, Diamond_add;
	myString String, String_add;
	
	pasteTime++;
	
	for (i = 0; i < FLIST; i++)
	{
		Readptr = CopyList[i];
		Readptr = NextNode(CopyList[i], Readptr); //������һ���սڵ� 
	
		
		while (Readptr != NULL)
		{
			switch (i)
			{
				case ARROW:
					Arrow = (myArrow)Readptr->dataptr;
					Arrow_add = (myArrow)GetBlock(sizeof(*Arrow_add));
						//Ϊ�����Ľṹ�帳ֵ 
					Arrow_add->x1 = Arrow->x1 + 0.5 * pasteTime;
					Arrow_add->x2 = Arrow->x2 + 0.5 * pasteTime;
					Arrow_add->y1 = Arrow->y1 - 0.3 * pasteTime;
					Arrow_add->y2 = Arrow->y2 - 0.3 * pasteTime;
					Arrow_add->isSelected = Arrow->isSelected;
					Arrow_add->penColor = Arrow->penColor;
					Arrow_add->penSize = Arrow->penSize;
					Arrow_add->penType = Arrow->penType;
					
					InsertNode(list[i], NULL, Arrow_add);
					break;
				case RTARROW:
					RtArrow = (myRtArrow)Readptr->dataptr;
					RtArrow_add = (myRtArrow)GetBlock(sizeof(*RtArrow_add));
						//Ϊ�����Ľṹ�帳ֵ 
					RtArrow_add->x1 = RtArrow->x1 + 0.5 * pasteTime;
					RtArrow_add->x2 = RtArrow->x2 + 0.5 * pasteTime;
					RtArrow_add->y1 = RtArrow->y1 - 0.3 * pasteTime;
					RtArrow_add->y2 = RtArrow->y2 - 0.3 * pasteTime;
					RtArrow_add->direction = RtArrow->direction;
					RtArrow_add->isSelected = RtArrow->isSelected;					
					RtArrow_add->penColor = RtArrow->penColor;
					RtArrow_add->penSize = RtArrow->penSize;
					RtArrow_add->penType = RtArrow->penType;
					
					InsertNode(list[i], NULL, RtArrow_add);
					break;
				case RECT:
					Rect = (myRect)Readptr->dataptr;
					Rect_add = (myRect)GetBlock(sizeof(*Rect_add));
						//Ϊ�����Ľṹ�帳ֵ 
					Rect_add->x1 = Rect->x1 + 0.5 * pasteTime;
					Rect_add->x2 = Rect->x2 + 0.5 * pasteTime;
					Rect_add->y1 = Rect->y1 - 0.3 * pasteTime;
					Rect_add->y2 = Rect->y2 - 0.3 * pasteTime;
					Rect_add->isSelected = Rect->isSelected;					
					Rect_add->penColor = Rect->penColor;
					Rect_add->penSize = Rect->penSize;
					Rect_add->penType = Rect->penType;
					Rect_add->fillColor = Rect->fillColor;
					
					InsertNode(list[i], NULL, Rect_add);
					break;
				case RNDRECT:
					RndRect = (myRndRect)Readptr->dataptr;
					RndRect_add = (myRndRect)GetBlock(sizeof(*RndRect_add));
						//Ϊ�����Ľṹ�帳ֵ 
					RndRect_add->x1 = RndRect->x1 + 0.5 * pasteTime;
					RndRect_add->x2 = RndRect->x2 + 0.5 * pasteTime;
					RndRect_add->y1 = RndRect->y1 - 0.3 * pasteTime;
					RndRect_add->y2 = RndRect->y2 - 0.3 * pasteTime;
					RndRect_add->isSelected = RndRect->isSelected;
					RndRect_add->penColor = RndRect->penColor;
					RndRect_add->penSize = RndRect->penSize;
					RndRect_add->penType = RndRect->penType;
					RndRect_add->fillColor = RndRect->fillColor;
					
					InsertNode(list[i], NULL, RndRect_add);
					break;
				case DIAMOND:
					Diamond = (myDiamond)Readptr->dataptr;
					Diamond_add = (myDiamond)GetBlock(sizeof(*Diamond_add));
						//Ϊ�����Ľṹ�帳ֵ 
					Diamond_add->x1 = Diamond->x1 + 0.5 * pasteTime;
					Diamond_add->x2 = Diamond->x2 + 0.5 * pasteTime;
					Diamond_add->y1 = Diamond->y1 - 0.3 * pasteTime;
					Diamond_add->y2 = Diamond->y2 - 0.3 * pasteTime;
					Diamond_add->isSelected = Diamond->isSelected;	
					Diamond_add->penColor = Diamond->penColor;	
					Diamond_add->penSize = Diamond->penSize;				
					Diamond_add->penType = Diamond->penType;
					Diamond_add->fillColor = Diamond->fillColor;
					
					InsertNode(list[i], NULL, Diamond_add);
					break;
				case STRING:
					String = (myString)Readptr->dataptr;
					String_add = (myString)GetBlock(sizeof(*String_add));
					   /*Ϊ�����Ľṹ�帳ֵ*/
					String_add->x = String->x + 0.5 * pasteTime;
					String_add->y = String->y - 0.3 * pasteTime;
					String_add->cursor = String->cursor;
					String_add->isBlink = String->isBlink;
					String_add->isSelected = String->isSelected;
					String_add->penColor = String->penColor;
					String_add->pointSize = String->pointSize;
					String_add->text = String->text;
					
					InsertNode(list[i], NULL, String_add);
					break;
			}
			Readptr = NextNode(CopyList[i], Readptr);
		}
	}
}

/*ɾ������
  ɾ�����б�ѡ�е�ͼ�Σ����ͷ����ڴ�*/ 
void Delete()
{
	linkedlistADT nodeptr, preNodeptr;
	int i; /*ѭ������*/
	myArrow arrow;
	myRtArrow RtArrow;
	myRect Rect;
	myRndRect RndRect;
	myDiamond Diamond;
	myString String;
	for (i = 0; i < FLIST; i++)
	{
		nodeptr = list[i];
		nodeptr = NextNode(list[i], nodeptr);  /*������һ���սڵ�*/
		preNodeptr = list[i];
		while (nodeptr != NULL)
		{
			switch (i)
			{
				case ARROW:
					arrow = nodeptr->dataptr;
					if (arrow->isSelected)
					{
						preNodeptr->next = nodeptr->next;
						free(nodeptr);
						nodeptr = preNodeptr->next;
					}
					else
					{
						preNodeptr = NextNode(list[i], preNodeptr);
						nodeptr = NextNode(list[i], nodeptr);
					}
					break;
				case RTARROW:
					RtArrow = nodeptr->dataptr;
					if (RtArrow->isSelected)
					{
						preNodeptr->next = nodeptr->next;
						free(nodeptr);
						nodeptr = preNodeptr->next;
					}
					else
					{
						preNodeptr = NextNode(list[i], preNodeptr);
						nodeptr = NextNode(list[i], nodeptr);
					}
					break;
				case RECT:
					Rect = nodeptr->dataptr;
					if (Rect->isSelected)
					{
						preNodeptr->next = nodeptr->next;
						free(nodeptr);
						nodeptr = preNodeptr->next;
					}
					else
					{
						preNodeptr = NextNode(list[i], preNodeptr);
						nodeptr = NextNode(list[i], nodeptr);
					}
					break;
				case RNDRECT:
					RndRect = nodeptr->dataptr;
					if (RndRect->isSelected)
					{
						preNodeptr->next = nodeptr->next;
						free(nodeptr);
						nodeptr = preNodeptr->next;
					}
					else
					{
						preNodeptr = NextNode(list[i], preNodeptr);
						nodeptr = NextNode(list[i], nodeptr);
					}
					break;
				case DIAMOND:
					Diamond = nodeptr->dataptr;
					if (Diamond->isSelected)
					{
						preNodeptr->next = nodeptr->next;
						free(nodeptr);
						nodeptr = preNodeptr->next;
					}
					else
					{
						preNodeptr = NextNode(list[i], preNodeptr);
						nodeptr = NextNode(list[i], nodeptr);
					}
					break;
				case STRING:
					String = nodeptr->dataptr;
					if (String->isSelected)
					{
						preNodeptr->next = nodeptr->next;
						free(nodeptr);
						nodeptr = preNodeptr->next;
					}
					else
					{
						preNodeptr = NextNode(list[i], preNodeptr);
						nodeptr = NextNode(list[i], nodeptr);
					}
					break;
					
			}
		} 
	} 
} 
