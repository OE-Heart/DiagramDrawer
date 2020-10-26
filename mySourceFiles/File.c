#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include "Edit.h"
#include "File.h"

int num[FLIST]; 

/*保存函数
  将main.c当中的 list[]所保存的图形全部写入一个文件*/
void SaveFile(linkedlistADT list[])
{
	FILE *fp;
	linkedlistADT nodeptr;
	int i; /*循环变量*/ 
	
	myArrow Arrow;
	myRtArrow RtArrow;
	myRect Rect;
	myRndRect RndRect;
	myDiamond Diamond;
	myString String; 
	 
	InitConsole();
	
	char FileName[50];
	printf("请输入您想保存的文件名:\n");
	scanf("%s", FileName);
	printf("保存成功！请关闭本控制台"); 
	FreeConsole();
		
	if ((fp = fopen(FileName, "w" )) == NULL)
	{
		return;
	}
	
	for (i = 0; i < FLIST; i++)   /*记录各种图形个数，以便下一次读取*/ 
	{
		nodeptr = list[i];
		nodeptr = NextNode(list[i], nodeptr); /*跳过第一个空节点*/ 
		while (nodeptr != NULL)
		{
			switch (i)
			{
				case ARROW:
					num[ARROW]++;
					nodeptr = NextNode(list[ARROW], nodeptr);
					break;
				case RTARROW:
					num[RTARROW]++;
					nodeptr = NextNode(list[RTARROW], nodeptr);
					break;
				case RECT:
					num[RECT]++;
					nodeptr = NextNode(list[RECT], nodeptr);
					break;
				case RNDRECT:
					num[RNDRECT]++;
					nodeptr = NextNode(list[RNDRECT], nodeptr);
					break;
				case DIAMOND:
					num[DIAMOND]++;
					nodeptr = NextNode(list[DIAMOND], nodeptr);
					break;
				case STRING:
					num[STRING]++;
					nodeptr = NextNode(list[STRING], nodeptr);
					break;
			}
		}
		fprintf(fp, "%d,", num[i]);
	}
	
	for (i = 0; i < FLIST; i++)
	{
		nodeptr = list[i];
		nodeptr = NextNode(list[i], nodeptr); /*跳过第一个空节点*/ 
		while (nodeptr != NULL)
		{
			switch (i)
			{
				case ARROW:
					Arrow = (myArrow)nodeptr->dataptr;
					fprintf(fp, "%lf %lf %lf %lf %d\n%s\n%s\n", 
								Arrow->x1, Arrow->y1, Arrow->x2, Arrow->y2, 
								Arrow->penSize, Arrow->penColor, Arrow->penType);
					nodeptr = NextNode(list[ARROW], nodeptr);
					break;
				case RTARROW:
					RtArrow = (myRtArrow)nodeptr->dataptr;
					fprintf(fp, "%lf %lf %lf %lf %d %d\n%s\n%s\n", 
								RtArrow->x1, RtArrow->y1, RtArrow->x2, RtArrow->y2,
								RtArrow->penSize, RtArrow->direction, RtArrow->penColor, RtArrow->penType);
					nodeptr = NextNode(list[RTARROW], nodeptr);
					break;
				case RECT:
					Rect = (myRect)nodeptr->dataptr;
					fprintf(fp, "%lf %lf %lf %lf %d\n%s\n%s\n%s\n", 
								Rect->x1, Rect->y1, Rect->x2, Rect->y2,
								Rect->penSize, Rect->penColor, Rect->penType, Rect->fillColor);
					nodeptr = NextNode(list[RECT], nodeptr);
					break;
				case RNDRECT:
					RndRect = (myRndRect)nodeptr->dataptr;
					fprintf(fp, "%lf %lf %lf %lf %d\n%s\n%s\n%s\n", 
								 RndRect->x1, RndRect->y1, RndRect->x2, RndRect->y2,
								 RndRect->penSize, RndRect->penColor, RndRect->penType, RndRect->fillColor);
					nodeptr = NextNode(list[RNDRECT], nodeptr);
					break;
				case DIAMOND:
					Diamond = (myDiamond)nodeptr->dataptr;
					fprintf(fp, "%lf %lf %lf %lf %d\n%s\n%s\n%s\n", 
								 Diamond->x1, Diamond->y1, Diamond->x2, Diamond->y2,
								 Diamond->penSize, Diamond->penColor, Diamond->penType, Diamond->fillColor);
					nodeptr = NextNode(list[DIAMOND], nodeptr);
					break;
				case STRING:
					String = (myString)nodeptr->dataptr;
					fprintf(fp, "%lf %lf %d %d %d\n%s\n%s\n", 
								 String->x, String->y, String->cursor, String->pointSize,
								 String->isBlink, String->penColor, String->text);
					nodeptr = NextNode(list[STRING], nodeptr);
					break;		
			}
		}
	}
	for (i = 0; i < FLIST; i++)
		num[i] = 0;
	fclose(fp);
}

void LoadFile(linkedlistADT list[])
{
	int i; /*循环变量*/
	FILE *fp;
	myArrow Arrow;
	myRtArrow RtArrow;
	myRect Rect;
	myRndRect RndRect;
	myDiamond Diamond;
	myString String;
	
	for (i = 0; i < FLIST; i++)
	{
		FreeLinkedList(list[i]);  /*清空所有链表*/ 
	}
	
	for (i = 0; i < FLIST; i++) list[i] = NewLinkedList();
	
	InitConsole();
	
	char FileName[50];
	printf("请输入您想打开的文件名:\n");
	scanf("%s", FileName);
	if ((fp = fopen(FileName, "r")) == NULL)
	{
		printf("该文件名无效!请关闭本控制台");
		return;
	}
	
	FreeConsole();
	
	for (i = 0; i < FLIST; i++)
	{
		fscanf(fp, "%d,", &num[i]);
	}
	
	for (i = 0; i < num[ARROW]; i++)
	{
		Arrow = (myArrow)GetBlock(sizeof(*Arrow));
		Arrow->penColor = (string)malloc(10 * sizeof(char));
		Arrow->penType = (string)malloc(10 * sizeof(char));
		fscanf(fp, "%lf%lf%lf%lf%d",&Arrow->x1, &Arrow->y1, &Arrow->x2, &Arrow->y2, &Arrow->penSize);
		fscanf(fp, "%s%s", Arrow->penColor, Arrow->penType);

		Arrow->isSelected = FALSE;
		InsertNode(list[ARROW], NULL, Arrow);
	}
	
	for (i = 0; i < num[RECT]; i++)
	{
		Rect = (myRect)GetBlock(sizeof(*Rect));
		Rect->penColor = (string)malloc(10 * sizeof(char));
		Rect->penType = (string)malloc(10 * sizeof(char));
		Rect->fillColor = (string)malloc(10 * sizeof(char));
		fscanf(fp, "%lf%lf%lf%lf%d",&Rect->x1, &Rect->y1, &Rect->x2, &Rect->y2, &Rect->penSize);
		fscanf(fp, "%s\n%s\n%s", Rect->penColor, Rect->penType, Rect->fillColor);

		Rect->isSelected = FALSE;
		InsertNode(list[RECT], NULL, Rect);
	}
	
	for (i = 0; i < num[RNDRECT]; i++)
	{
		RndRect = (myRndRect)GetBlock(sizeof(*RndRect));
		RndRect->penColor = (string)malloc(10 * sizeof(char));
		RndRect->penType = (string)malloc(10 * sizeof(char));
		RndRect->fillColor = (string)malloc(10 * sizeof(char));
		fscanf(fp, "%lf%lf%lf%lf%d",&RndRect->x1, &RndRect->y1, &RndRect->x2, &RndRect->y2, &RndRect->penSize);
		fscanf(fp, "%s%s%s", RndRect->penColor, RndRect->penType, RndRect->fillColor);

		RndRect->isSelected = FALSE;
		InsertNode(list[RNDRECT], NULL, RndRect);
	}
	
	for (i = 0; i < num[DIAMOND]; i++)
	{
		Diamond = (myDiamond)GetBlock(sizeof(*Diamond));
		Diamond->penColor = (string)malloc(10 * sizeof(char));
		Diamond->penType = (string)malloc(10 * sizeof(char));
		Diamond->fillColor = (string)malloc(10 * sizeof(char));
		fscanf(fp, "%lf%lf%lf%lf%d", &Diamond->x1, &Diamond->y1, &Diamond->x2, &Diamond->y2, &Diamond->penSize);
		fscanf(fp, "%s%s%s", Diamond->penColor, Diamond->penType, Diamond->fillColor);

		Diamond->isSelected = FALSE;
		InsertNode(list[DIAMOND], NULL, Diamond);
	}
	
	for (i = 0; i < num[RTARROW]; i++)
	{
		int direction;
		RtArrow = (myRtArrow)GetBlock(sizeof(*RtArrow));
		RtArrow->penColor = (string)malloc(10 * sizeof(char));
		RtArrow->penType = (string)malloc(10 * sizeof(char));
		fscanf(fp, "%lf%lf%lf%lf%d%d", &RtArrow->x1, &RtArrow->y1, &RtArrow->x2, &RtArrow->y2, &RtArrow->penSize, &direction);
		fscanf(fp, "%s%s", RtArrow->penColor, RtArrow->penType);
		
		RtArrow->direction = (bool)direction;
		RtArrow->isSelected = FALSE;
		InsertNode(list[RTARROW], NULL, RtArrow);
	}
	
	for (i = 0; i < num[STRING]; i++)
	{
		int isBlink;
		String = (myString)GetBlock(sizeof(*String));
		String->penColor = (string)malloc(10 * sizeof(char));
		String->text = (string)malloc(100 * sizeof(char));
		fscanf(fp, "%lf%lf%d%d%d", &String->x, &String->y, &String->cursor, &String->pointSize, &isBlink);
		fscanf(fp, "%s%s", String->penColor, String->text);
		
		String->isBlink = (bool)isBlink;
		String->isSelected = FALSE;
		InsertNode(list[STRING], NULL, String);
	}
	fclose(fp);
}

void saveDiagram(char* fileName)
{
    HWND window = GetDesktopWindow();
    HDC _dc = GetWindowDC(window);
    HDC dc = CreateCompatibleDC(0);
    
    struct tagRECT re;
    GetWindowRect(window, &re);
    int w = re.right*1.25,
		h = re.bottom*1.25;
    //void* buf = new char[w*h*4];
    void* buf = (char *)malloc((w*h * 4) * sizeof(char));
    
    HBITMAP bm = CreateCompatibleBitmap(_dc, w, h);
    SelectObject(dc, bm);
    
    StretchBlt(dc, 0, 0, w, h, _dc, 0, 0, w, h, SRCCOPY);
    
    void* f =CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, 0);
    
    GetObject(bm, 84, buf);
    
    BITMAPINFO bi;
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = w;
    bi.bmiHeader.biHeight = h;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = 0;
    bi.bmiHeader.biSizeImage = 0;
    
    CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &buf, 0, 0);
    GetDIBits(dc, bm, 0, h, buf, &bi, DIB_RGB_COLORS);
        
    BITMAPFILEHEADER bif;
    bif.bfType = MAKEWORD('B','M');
    bif.bfSize = w*h*4+54;
    bif.bfOffBits = 54;

    BITMAPINFOHEADER bii;
    bii.biSize = 40;
    bii.biWidth = w;
    bii.biHeight = h;
    bii.biPlanes = 1;
    bii.biBitCount = 32;
    bii.biCompression = 0;
    bii.biSizeImage = w*h*4;
    
    DWORD r;
    WriteFile(f, &bif, sizeof(bif), &r, NULL);
    WriteFile(f, &bii, sizeof(bii), &r, NULL);
    WriteFile(f, buf, w*h*4, &r, NULL);

    CloseHandle(f);
    DeleteDC(_dc);
    DeleteDC(dc);
}
