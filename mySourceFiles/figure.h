#ifndef _FIGURE_H_
#define _FIGURE_H_

#define FLIST     6 
#define ARROW     0
#define RECT      1
#define RNDRECT   2 
#define DIAMOND   3 
#define RTARROW   4
#define STRING    5

#define PI acos(-1) 
#define DegToReg(x) (x) * PI / 180.0
#define RegToDeg(x) (x) * 180.0 / PI

#define CURSOR "_"          /*光标符号*/
#define CURSOR_BLINK  1     /*光标闪烁定时器事件标志号*/
#define TEXTLEN  100

typedef struct {/*箭头类型*/
	double x1, y1;/*起点坐标*/
    double x2, y2;/*终点坐标*/
    int penSize; /*笔粗细*/
    string penType;/*笔类型：solid,dashed,dotted*/ 
    string penColor; /*笔颜色*/
    bool isSelected; /*选中*/ 
} *myArrow;

typedef struct {/*矩形类型*/
	double x1, y1;/*左上角坐标*/
    double x2, y2;/*右下角坐标*/
    int penSize; /*笔粗细*/
    string penType;/*笔类型：solid,dashed,dotted*/ 
    string penColor; /*笔颜色*/
    string fillColor;/*填充颜色*/ 
    bool isSelected; /*选中*/ 
} *myRect;

typedef struct {/*圆角矩形类型*/
	double x1, y1;/*左上角坐标*/
    double x2, y2;/*右下角坐标*/
    int penSize; /*笔粗细*/
    string penType;/*笔类型：solid,dashed,dotted*/ 
    string penColor; /*笔颜色*/
    string fillColor;/*填充颜色*/
    bool isSelected; /*选中*/ 
} *myRndRect;

typedef struct {/*菱形类型*/
	double x1, y1;/*左上角坐标*/
    double x2, y2;/*右下角坐标*/
    int penSize; /*笔粗细*/
    string penType;/*笔类型：solid,dashed,dotted*/ 
    string penColor; /*笔颜色*/
    string fillColor;/*填充颜色*/ 
    bool isSelected; /*选中*/ 
} *myDiamond;

typedef struct {
	double x1, y1;/*左上角坐标*/
    double x2, y2;/*右下角坐标*/
    double angle;/*角度*/ 
    int penSize; /*笔粗细*/
    string penType;/*笔类型：solid,dashed,dotted*/ 
    string penColor; /*笔颜色*/
    bool direction; /*方向：顺时针 1，逆时针 0*/ 
    bool isSelected; /*选中*/
} *myRtArrow;

typedef struct {/*文本类型*/
	string text;/*文本指针*/
	double x, y;/*文本显示起始位置坐标*/
    int pointSize; /*文字大小*/
    string penColor; /*颜色*/
    bool isSelected; /*选中*/
    int cursor; /*光标位置*/
    bool isBlink; /*光标是否处于闪烁状态*/ 
} *myString;
 
extern linkedlistADT list[FLIST];/*五类图形元素链表指针*/
extern double minDistance[FLIST];
extern int curList; /*当前链表序号*/
extern myArrow curArrow;/*箭头链表的当前对象指针*/
extern myRect curRect;/*矩形链表的当前对象指针*/
extern myRndRect curRndRect; /*圆角矩形文本链表的当前对象指针*/
extern myDiamond curDiamond; /*菱形链表的当前对象指针*/
extern string selectedColor;

/*箭头处理函数*/
void DrawArrow(void *arrow);
void MoveArrow(void *obj, double dx, double dy);
void ChangeArrow(void *obj, double dx, double dy);

/*矩形处理函数*/
void DrawRect(void *rect);
void MoveRect(void *rect, double dx, double dy);
void ChangeRect(void *rect, double dx, double dy);

/*圆角矩形处理函数*/
void DrawRndRect(void *rndRect);
void MoveRndRect(void *rndRect, double dx, double dy);
void ChangeRndRect(void *rndRect, double dx, double dy);

/*菱形处理函数*/
void DrawDiamond(void *diamond);
void MoveDiamond(void *diamond, double dx, double dy);
void ChangeDiamond(void *diamond, double dx, double dy);

/*直角箭头处理函数*/
void DrawRtArrow(void *RtArrow);
void MoveRtArrow(void *obj, double dx, double dy);
void ChangeRtArrow(void *obj, double dx, double dy);

/*文本处理函数*/
void DrawString(void *str); /*显示文本*/
void MoveString(void *str, double dx, double dy);
void PointSizeUp(void *str);
void PointSizeDown(void *str);
void InsertCharToString(string str, int pos, char c);/*将字符c插入到字符串str的pos位置*/
void DeleteCharFromString(string str, int pos);/*删除字符串str的pos位置字符*/
void DrawCursor(string str, int cursor, double startx, double starty);/*显示文本光标*/

//画虚线函数
//用法和特性与 DrawLine() 函数相同 
void DrawDashedLine(double dx, double dy);

#endif
