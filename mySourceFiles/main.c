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
#include "imgui.h"
#include "linkedlist.h"
#include "figure.h"
#include "UI.h"

// 全局变量
double winwidth, winheight;   // 窗口尺寸
char *ToolName[] = {"箭头","矩形","圆角矩形","菱形","直角箭头","文字框"};
char currentToolName[20];
string currentPenColor;
string currentFillColor; 
int currentTool;
int currentPenSize; 
string currentPenType;
string selectedColor = "Magenta";
bool isDrawing;
bool isSelecting;
bool isChanging;
bool newFile = FALSE;
bool isSaved = FALSE;
int quit_confirm;
string selectedLabel = NULL;

const int timerseconds = 500; /*光标闪烁周期ms*/
bool isCursorBlink = FALSE;  /*光标是否闪烁*/ 
bool isEdited = FALSE; /*是否处于当前文本编辑状态*/
char textbuf[TEXTLEN+1];/*当前文本缓冲区*/

linkedlistADT list[FLIST]={NULL, NULL, NULL, NULL, NULL, NULL};/*六类图形元素链表指针*/

void KeyboardEventProcess(int key,int event);/*键盘消息回调函数*/
void CharEventProcess(char c);/*字符消息回调函数*/
void MouseEventProcess(int x, int y, int button, int event);/*鼠标消息回调函数*/
void TimerEventProcess(int timerID);/*定时器消息回调函数*/

void TraverseDrawAllList();/*刷新所有图形对象*/
void display();
void displayAllList();
void myTraverseLinkedList(linkedlistADT head, void (*traversefunptr)(void *obj, double dx, double dy), double dx, double dy);
void TraverseMoveAllList(double dx, double dy);
void TraverseChangeAllList(double dx, double dy);

static myRect rptr;
static myRndRect rndptr;
static myDiamond dptr;
static myArrow aptr;
static myRtArrow rtptr; 
myString sptr; /*当前编辑文本对象指针*/

void Main()
{
	// 初始化窗口和图形系统
	SetWindowTitle("DiagramDrawer");
	SetWindowSize(20, 10);  // 设置尺寸
	InitGraphics();
	
	// 获得窗口尺寸
	winwidth = GetWindowWidth();
	winheight = GetWindowHeight();
	
	registerKeyboardEvent(KeyboardEventProcess);/*注册键盘消息回调函数*/
	registerCharEvent(CharEventProcess);/*注册字符消息回调函数*/
	registerMouseEvent(MouseEventProcess);/*注册鼠标消息回调函数*/
	registerTimerEvent(TimerEventProcess);/*注册定时器消息回调函数*/
	int i; 
	for (i = 0; i < FLIST; i++) list[i] = NewLinkedList();
	
	isDrawing = FALSE;
	isSelecting = FALSE;
	isChanging = FALSE;
	currentTool = 0;
	currentPenSize = 1;
	currentPenType = "solid";
	currentPenColor = "Black";
	currentFillColor = "White";
	display();
}

double MAX(double a, double b)
{
	return a>b?a:b;
}

double MIN(double a, double b)
{
	return a<b?a:b;
}

void MouseEventProcess(int x, int y, int button, int event)
{
	static bool isDraw = FALSE; /*画笔标志*/
	static bool isChange = FALSE; 
	static bool isMove = FALSE; 
	static double bx, by;
	double lx, ly;
	
	uiGetMouse(x, y, button, event); //GUI获取鼠标
	if (!isDraw && !isChange && !isMove) display(); 
	else displayAllList();
	
	if (isDrawing)
	{
	switch (currentTool){
		case ARROW:
      		switch(event) {
        		case BUTTON_DOWN:
       				if (button == LEFT_BUTTON)
          			{
            			bx = ScaleXInches(x);
            			by = ScaleYInches(y);
            			if (bx <= 0.1*winwidth || bx >= 0.85*winwidth || by <= 0.37 || by >= winheight-0.37) break;
						aptr = (myArrow)GetBlock(sizeof(*aptr));
            			aptr->penSize = currentPenSize;
            			aptr->penType = currentPenType;
            			aptr->penColor = currentPenColor;
            			aptr->isSelected = FALSE;
            			aptr->x1 = 0;
						aptr->x2 = 0;
						aptr->y1 = 0;
						aptr->y2 = 0; 
            			isDraw = TRUE;
            			InsertNode(list[ARROW], NULL, aptr);
          			}
          			break;
        		case BUTTON_UP:
          			if (button == LEFT_BUTTON) 
          			{
            			isDraw = FALSE;
          			} 
          			break;
        			case BUTTON_DOUBLECLICK:
          			break;
        		case MOUSEMOVE:
          			if (isDraw)
          			{
            			lx = ScaleXInches(x);
            			ly = ScaleYInches(y);
            			aptr->x1 = bx;
            			aptr->y1 = by;
            			aptr->x2 = lx;
            			aptr->y2 = ly;
          			}
          			break;
      		}
      		break;
		case RECT:
			switch(event) {
				case BUTTON_DOWN:
				if (button == LEFT_BUTTON) 
					{
						bx = ScaleXInches(x);
            			by = ScaleYInches(y);
            			if (bx <= 0.1*winwidth || bx >= 0.85*winwidth || by <= 0.37 || by >= winheight-0.37) break;
						rptr = GetBlock(sizeof(*rptr));
						rptr->penSize = currentPenSize;
						rptr->fillColor = currentFillColor;
						rptr->penColor = currentPenColor;
						rptr->penType = currentPenType; 
						rptr->isSelected = FALSE;
						rptr->x1 = 0;
						rptr->x2 = 0;
						rptr->y1 = 0;
						rptr->y2 = 0; 
						isDraw = TRUE;
						InsertNode(list[RECT], NULL, rptr);
					} 
					break;
				case BUTTON_UP:
					if (button == LEFT_BUTTON) 
					{
						isDraw = FALSE;
					} 
					break;
				case BUTTON_DOUBLECLICK:
					break;
				case MOUSEMOVE:
					if (isDraw)
					{
						lx = ScaleXInches(x);
						ly = ScaleYInches(y);
						rptr->x1 = MIN(bx, lx);
						rptr->y1 = MAX(by, ly);
						rptr->x2 = MAX(bx, lx);
						rptr->y2 = MIN(by, ly);
						displayAllList();
					}
					break;
			}
			break;
		case RNDRECT:
			switch(event) {
				case BUTTON_DOWN:
					if (button == LEFT_BUTTON) 
					{
						bx = ScaleXInches(x);
            			by = ScaleYInches(y);
            			if (bx <= 0.1*winwidth || bx >= 0.85*winwidth || by <= 0.37 || by >= winheight-0.37) break;
						rndptr = GetBlock(sizeof(*rndptr));
						rndptr->penSize = currentPenSize;
						rndptr->fillColor = currentFillColor;
						rndptr->penColor = currentPenColor;
						rndptr->penType = currentPenType; 
						rndptr->isSelected = FALSE;
						rndptr->x1 = 0;
						rndptr->x2 = 0;
						rndptr->y1 = 0;
						rndptr->y2 = 0; 
						isDraw = TRUE;
						InsertNode(list[RNDRECT], NULL, rndptr);
					} 
					break;
				case BUTTON_UP:
					if (button == LEFT_BUTTON) 
					{
						isDraw = FALSE;
					} 
					break;
				case BUTTON_DOUBLECLICK:
					break;
				case MOUSEMOVE:
					if (isDraw)
					{
						lx = ScaleXInches(x);
						ly = ScaleYInches(y);
						rndptr->x1 = MIN(bx, lx);
						rndptr->y1 = MAX(by, ly);
						rndptr->x2 = MAX(bx, lx);
						rndptr->y2 = MIN(by, ly);
					}
					break;
			}
			break;
		case DIAMOND:
			switch(event) {
				case BUTTON_DOWN:
					if (button == LEFT_BUTTON) 
					{
						bx = ScaleXInches(x);
            			by = ScaleYInches(y);
            			if (bx <= 0.1*winwidth || bx >= 0.85*winwidth || by <= 0.37 || by >= winheight-0.37) break;
						dptr = GetBlock(sizeof(*dptr));
						dptr->penSize = currentPenSize;
						dptr->fillColor = currentFillColor;
						dptr->penColor = currentPenColor;
						dptr->penType = currentPenType; 
						dptr->isSelected = FALSE;
						dptr->x1 = 0;
						dptr->x2 = 0;
						dptr->y1 = 0;
						dptr->y2 = 0; 
						isDraw = TRUE;
						InsertNode(list[DIAMOND], NULL, dptr);
					} 
					break;
				case BUTTON_UP:
					if (button == LEFT_BUTTON) 
					{
						isDraw = FALSE;
					} 
					break;
				case BUTTON_DOUBLECLICK:
					break;
				case MOUSEMOVE:
					if (isDraw)
					{
						lx = ScaleXInches(x);
						ly = ScaleYInches(y);
						dptr->x1 = MIN(bx, lx);
						dptr->y1 = MAX(by, ly);
						dptr->x2 = MAX(bx, lx);
						dptr->y2 = MIN(by, ly);
					}
					break;
			}
			break;
		case RTARROW:
			switch(event) {
				case BUTTON_DOWN:
					if (button == LEFT_BUTTON) 
					{
						bx = ScaleXInches(x);
            			by = ScaleYInches(y);
            			if (bx <= 0.1*winwidth || bx >= 0.85*winwidth || by <= 0.37 || by >= winheight-0.37) break;
						rtptr = (myRtArrow)GetBlock(sizeof(*rtptr));
						rtptr->penSize = currentPenSize;
						rtptr->direction = TRUE;
						rtptr->penColor = currentPenColor;
						rtptr->penType = currentPenType; 
						rtptr->isSelected = FALSE;
						rtptr->x1 = 0;
						rtptr->x2 = 0;
						rtptr->y1 = 0;
						rtptr->y2 = 0; 
						isDraw = TRUE;
						InsertNode(list[RTARROW], NULL, rtptr);
					}
					if (button == RIGHT_BUTTON)
					{
						rtptr->direction = !rtptr->direction;
					}
					break;
				case BUTTON_UP:
					if (button == LEFT_BUTTON) 
					{
						isDraw = FALSE;
					} 
					break;
				case BUTTON_DOUBLECLICK:
					break;
				case MOUSEMOVE:
					if (isDraw)
					{
						lx = ScaleXInches(x);
						ly = ScaleYInches(y);
						rtptr->x1 = bx;
						rtptr->y1 = by;
						rtptr->x2 = lx;
						rtptr->y2 = ly;
					}
					break;
			}
			break;
		case STRING:
			switch (event){
				case BUTTON_DOWN:
					if (button == LEFT_BUTTON)
					{
						bx = ScaleXInches(x);
            			by = ScaleYInches(y);
            			if (bx <= 0.1*winwidth || bx >= 0.85*winwidth || by <= 0.37 || by >= winheight-0.37) break;
						if (isEdited) break;/*若已处于文本编辑状态，则不生成新的空文本*/
						isEdited = TRUE; /*进入新的文本行输入状态*/
						sptr = GetBlock(sizeof(*sptr));
						sptr->x = bx;
        				sptr->y = by;
						MovePen(sptr->x, sptr->y);/*文本显示起始位置*/
	 					DrawTextString(CURSOR);/*显示光标*/
        				textbuf[0] = '\0'; /*形成字符串*/
        				sptr->pointSize = 10;
        				sptr->penColor = currentPenColor;
        				sptr->isSelected = FALSE;
        				sptr->cursor = 0; /*设置当前光标位置*/
        				sptr->isBlink = TRUE; /*设置光标闪烁标志*/ 
        				startTimer(CURSOR_BLINK, timerseconds);/*光标闪烁定时器触发*/
					}
			}
			break;
	}
	}
	else if (isSelecting)
	{
		switch (event){
			case BUTTON_DOWN:
				if (button == LEFT_BUTTON) 
				{
					if (Select(ScaleXInches(x), ScaleYInches(y))) display();
				}
				break;
			}	
	}
	else if (isChanging)
	{
		switch (event){
			case BUTTON_DOWN:
				if (button == LEFT_BUTTON) 
				{
					isMove = TRUE;
					bx = ScaleXInches(x);
					by = ScaleYInches(y);
				}
				if (button == RIGHT_BUTTON) 
				{
					isChange = TRUE;
					bx = ScaleXInches(x);
					by = ScaleYInches(y);
				}
				break;
			case BUTTON_UP:
				if (button == LEFT_BUTTON) 
				{
					isMove = FALSE;
				}
				if (button == RIGHT_BUTTON)
				{
					isChange = FALSE;
				}
				break;
			case MOUSEMOVE:
				if (isMove) 
				{
					lx = ScaleXInches(x);
					ly = ScaleYInches(y);
					TraverseMoveAllList(lx-bx, ly-by);
					bx = lx;
					by = ly;
				}
				if (isChange) 
				{
					lx = ScaleXInches(x);
					ly = ScaleYInches(y);
					TraverseChangeAllList(lx-bx, ly-by);
					bx = lx;
					by = ly;
				}
				break;
			case ROLL_UP:
				TraverseLinkedList(list[STRING], PointSizeUp);
				break;
			case ROLL_DOWN:
				TraverseLinkedList(list[STRING], PointSizeDown);
				break; 
		}	
	}
}

static bool isCtrl = FALSE;/*Ctrl标记*/ 

void KeyboardEventProcess(int key,int event)
{
	uiGetKeyboard(key, event); 
	display();
	int i;
	switch (event){
		case KEY_DOWN:
			switch (key){
				case VK_CONTROL:
					isCtrl = TRUE;
					break;
				case VK_RETURN:
					CancelSelect();
					displayAllList();
					break;
				case 0x43:/*ctrl+C复制*/
					if (isCtrl)
					{
						Copy();
						selectedLabel = "复制 (Ctrl+C)";
					} 
					break;
				case 0x44:/*ctrl+D选择*/
					if (isCtrl)
					{
						isDrawing = FALSE;
						isSelecting = TRUE;
						isChanging = FALSE;
						selectedLabel = "选择 (Ctrl+D)";
					}
					break;
				case 0x4E:/*ctrl+N新建*/
					if (isCtrl)
					{
						newFile = TRUE;
						isSaved = FALSE;
						selectedLabel = "新建 (Ctrl+N)";
					}
					break;
				case 0x4F:/*ctrl+O打开*/
					if (isCtrl)
					{
						newFile = TRUE;
						LoadFile(list);
						selectedLabel = "新建 (Ctrl+O)";
					}
					break;
				case 0x53:/*ctrl+S保存*/
					if (isCtrl)
					{
						SaveFile(list);
						isSaved = TRUE;
						selectedLabel = "保存 (Ctrl+S)";
					}
					break;
				case VK_BACK:
					Delete();
					displayAllList();
					selectedLabel = "删除 (Back)";
					break; 
				case 0x56:/*ctrl+V粘贴*/
					if (isCtrl)
					{
						Paste();
						displayAllList();
						selectedLabel = "粘贴 (Ctrl+V)";
					}
					break;
				case 0x57:/*ctrl+W关闭*/
					if (isCtrl)
					{	
						if (isSaved)
						{
							newFile = FALSE;
							for (i = 0; i < FLIST; i++) FreeLinkedList(list[i]);
							for (i = 0; i < FLIST; i++) list[i] = NewLinkedList();
							isSaved = FALSE;
						}
					else
					{
						quit_confirm = MessageBox(NULL, TEXT("您的进度尚未保存，确定要关闭吗？"), TEXT("温馨提示"), MB_YESNOCANCEL | MB_ICONQUESTION);
						if (quit_confirm == IDNO)
						MessageBox(NULL, TEXT("下次记得保存哦"), TEXT("温馨提示"), MB_OK);
						else if (quit_confirm == IDYES)
						{
							newFile = FALSE;
							for (i = 0; i < FLIST; i++) FreeLinkedList(list[i]);
							for (i = 0; i < FLIST; i++) list[i] = NewLinkedList();
							isSaved = FALSE;
						}
					}
					selectedLabel = "关闭 (Ctrl+W)";
					}
					break;
				case 0x5A:/*ctrl+Z改变*/
					if (isCtrl)
					{
						isDrawing = FALSE;
						isSelecting = FALSE;
						isChanging = TRUE;
						selectedLabel = "改变 (Ctrl+Z)";
					}
					break;
				case VK_ESCAPE:
					if (isSaved) exit(-1);
					else
					{
						quit_confirm = MessageBox(NULL, TEXT("您的进度尚未保存，确定要关闭吗？"), TEXT("温馨提示"), MB_YESNOCANCEL | MB_ICONQUESTION);
						if (quit_confirm == IDNO) MessageBox(NULL, TEXT("记得保存哦"), TEXT("温馨提示"), MB_OK);
						else if (quit_confirm == IDYES)	exit(-1);
					}
					break;
				case VK_LEFT: /*左移键*/
                 	if (!isEdited) break;/*不处于文本编辑状态*/
                    SetEraseMode(TRUE);
	 	   			MovePen(sptr->x, sptr->y);
	 	   			DrawTextString(textbuf);/*擦除当前文本*/
					DrawCursor(textbuf, sptr->cursor, sptr->x, sptr->y); /*擦除光标*/
                    if (sptr->cursor>0) sptr->cursor--;/*左移光标一个字符位置*/
                    SetEraseMode(FALSE);
                    SetPenColor(currentPenColor); 
	 	   			MovePen(sptr->x, sptr->y);
	 	   			DrawTextString(textbuf);/*重新显示当前文本*/
					DrawCursor(textbuf, sptr->cursor, sptr->x, sptr->y); /*显示光标*/
					break;                    
                 case VK_RIGHT: /*右移键*/
                 	if (!isEdited) break;/*不处于文本编辑状态*/
                    SetEraseMode(TRUE);
	 	   			MovePen(sptr->x, sptr->y);
	 	   			DrawTextString(textbuf);/*擦除当前文本*/
					DrawCursor(textbuf, sptr->cursor, sptr->x, sptr->y); /*擦除光标*/
                    if (sptr->cursor<strlen(textbuf)) sptr->cursor++;/*右移光标一个字符位置*/
                    SetEraseMode(FALSE);
	 	   			SetPenColor(currentPenColor); 
					MovePen(sptr->x, sptr->y);
	 	   			DrawTextString(textbuf);/*重新显示当前文本*/
					DrawCursor(textbuf, sptr->cursor, sptr->x, sptr->y); /*显示光标*/
				    break;       
			}
			break;
		case KEY_UP:
			switch (key){
				case VK_CONTROL:
					isCtrl = FALSE;
					break;
			}
			break;
		default:
			break;
	}
}

void CharEventProcess(char c)
{
	uiGetChar(c);
	display();
	int len;

	if (!isEdited) return;
    switch (c) {
    	case '\r':
			isEdited = FALSE;/*退出当前文本输入*/
           	
			SetEraseMode(TRUE);
	 	   	MovePen(sptr->x, sptr->y);
	 	   	DrawTextString(textbuf);
			DrawCursor(textbuf, sptr->cursor, sptr->x, sptr->y);/*擦除当前光标*/
			SetEraseMode(FALSE);
			
			sptr->isBlink = FALSE;
			sptr->text = CopyString(textbuf);/*申请字符串空间*/
           	InsertNode(list[STRING], NULL, sptr);
			textbuf[0] = '\0';
			cancelTimer(CURSOR_BLINK);/*注销光标闪烁定时器*/
			isDrawing = FALSE; 
			break;
 	    case '\b':/*BACKSPACE*/
 	    	if ((len = strlen(textbuf)) == 0) break;
			
			SetEraseMode(TRUE);
			MovePen(sptr->x, sptr->y);
			DrawTextString(textbuf);
			DrawCursor(textbuf, sptr->cursor, sptr->x, sptr->y);/*擦除当前光标*/
			DeleteCharFromString(textbuf, sptr->cursor-1);
	 	   	SetEraseMode(FALSE);
	 	   	
			SetPenColor(currentPenColor); 
			MovePen(sptr->x, sptr->y);
	 	   	DrawTextString(textbuf);
	 	   	if (sptr->cursor > 0) sptr->cursor--;
	 	   	DrawCursor(textbuf, sptr->cursor, sptr->x, sptr->y);/*显示当前光标*/
 	    	break;
    	default:
    		if ((len = strlen(textbuf)) >= TEXTLEN) break; 
			
			SetEraseMode(TRUE);
			MovePen(sptr->x, sptr->y);
			DrawTextString(textbuf);
			DrawCursor(textbuf, sptr->cursor, sptr->x, sptr->y);/*擦除当前光标*/
			InsertCharToString(textbuf, sptr->cursor, c);/*将当前字符插入到光标位置*/
	 	   	SetEraseMode(FALSE);
	 	   	
			SetPenColor(currentPenColor); 
			MovePen(sptr->x, sptr->y);
	 	   	DrawTextString(textbuf);
	 	   	sptr->cursor++; 
	 	   	DrawCursor(textbuf, sptr->cursor, sptr->x, sptr->y);/*显示当前光标*/
			break;
    }
}

void TimerEventProcess(int timerID)
{
	switch (timerID) {
		case CURSOR_BLINK: /*100ms光标闪烁定时器*/
			SetEraseMode(!sptr->isBlink);
			DrawCursor(textbuf, sptr->cursor, sptr->x, sptr->y);
			SetEraseMode(FALSE);
			SetPenColor(currentPenColor); 
			MovePen(sptr->x, sptr->y);
			DrawTextString(textbuf);
			sptr->isBlink=!sptr->isBlink;
			break;
	    default:
			break;
	}
}

void TraverseDrawAllList()
{
    TraverseLinkedList(list[ARROW], DrawArrow);
    TraverseLinkedList(list[RECT], DrawRect);
    TraverseLinkedList(list[RNDRECT], DrawRndRect);
    TraverseLinkedList(list[DIAMOND], DrawDiamond);
    TraverseLinkedList(list[RTARROW], DrawRtArrow);
    TraverseLinkedList(list[STRING], DrawString);
}

void myTraverseLinkedList(linkedlistADT head, void (*traversefunptr)(void *obj, double dx, double dy), double dx, double dy)
{
	linkedlistADT nodeptr;

	if (traversefunptr == NULL) return;
	nodeptr = head->next;
	while (nodeptr != NULL) {
		(*traversefunptr)(nodeptr->dataptr, dx, dy);
		nodeptr = nodeptr->next;
	}
}

void TraverseMoveAllList(double dx, double dy)
{
	myTraverseLinkedList(list[ARROW], MoveArrow, dx, dy);
    myTraverseLinkedList(list[RECT], MoveRect, dx, dy);
    myTraverseLinkedList(list[RNDRECT], MoveRndRect, dx, dy);
    myTraverseLinkedList(list[DIAMOND], MoveDiamond, dx, dy);
    myTraverseLinkedList(list[RTARROW], MoveRtArrow, dx, dy);
    myTraverseLinkedList(list[STRING], MoveString, dx, dy);
}

void TraverseChangeAllList(double dx, double dy)
{
	myTraverseLinkedList(list[ARROW], ChangeArrow, dx, dy);
    myTraverseLinkedList(list[RECT], ChangeRect, dx, dy);
    myTraverseLinkedList(list[RNDRECT], ChangeRndRect, dx, dy);
    myTraverseLinkedList(list[DIAMOND], ChangeDiamond, dx, dy);
    myTraverseLinkedList(list[RTARROW], ChangeRtArrow, dx, dy);
}

void display()
{
	DisplayClear();//清屏 

	drawCanvas();

	drawButtons();

	drawMenu();//制和处理菜单
	
	drawPenColorPanel();
	
	drawFillColorPanel();
	
	customPenColor();
	
	customFillColor();
	
	customPenSize();
	
	TraverseDrawAllList(); 
}

void displayAllList()
{
	SetPenColor("White");
	drawRectangle(0.1*winwidth, 0.37, 0.75*winwidth, winheight-0.74, 1);
	TraverseDrawAllList(); 
}
