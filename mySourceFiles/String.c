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

/*for text*/
void DrawString(void *str)
{
	myString s = (myString)str;
	string color = GetPenColor();
	int pointsize = GetPointSize();

	MovePen(s->x, s->y);
	if (!s->isSelected) SetPenColor(s->penColor);
	else SetPenColor(selectedColor);
	SetPointSize(s->pointSize);
    DrawTextString(s->text);

    SetPenColor(color);
    SetPointSize(pointsize);
}

void MoveString(void *str, double dx, double dy)
{
	if(str == NULL) return;
	myString r = (myString)str;
	
	if (r->isSelected)
	{
		r->x += dx;
		r->y += dy;
	}
}

void PointSizeUp(void *str)
{
	if(str == NULL) return;
	myString r = (myString)str;
	
	if (r->isSelected)
	{
		if (r->pointSize < 50) r->pointSize += 5;
	}
}

void PointSizeDown(void *str)
{
	if(str == NULL) return;
	myString r = (myString)str;
	
	if (r->isSelected)
	{
		if (r->pointSize > 10) r->pointSize -= 5;
	}
}
/*画光标*/
void DrawCursor(string str, int cursor, double startx, double starty)
{
	if (cursor < 0 || cursor > strlen(str)) return;
	MovePen(startx+TextStringWidth(SubString(str, 0, cursor-1)), starty);
	DrawTextString(CURSOR);
	return;
}

/*Insert char c to the string str at the position pos*/
void InsertCharToString(string str, int pos, char c)
{
	int len;
	
	if (pos < 0 || pos >= TEXTLEN) return;
	len = strlen(str);
	*(str+len+1) = '\0';/*插入后形成字符串*/
	while (len > pos) {
		*(str+len) = *(str+len-1);
		len--;
	}	
	*(str+len) = c;
	return;
}

/*Delete the character at the pos position of string str*/
void DeleteCharFromString(string str, int pos)
{
	int len;
	
	len = strlen(str);
	if (pos < 0 || pos >= len) return;
	
	while (*(str+pos) != '\0') {
		*(str+pos) = *(str+pos+1);
		pos++; 
	}
	return;
}
