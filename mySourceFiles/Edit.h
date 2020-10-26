#ifndef _EDIT_H_
#define _EDIT_H_

extern linkedlistADT list[FLIST];

//选中判断函数
//x, y为鼠标坐标
//若成功选中 
bool Select(double x, double y);

//函数：用于判断 cx 是否处于 x1 和 x2 之间 
bool isBetween(double x1, double x2, double cx);

//选中直角箭头判断函数
//x, y位鼠标坐标 
//若成功选中，返回TRUE 
bool SelectRtArrow(double x, double y);

//选中箭头判断函数
//x, y位鼠标坐标 
//若成功选中，返回TRUE 
bool SelectArrow(double x, double y);

//选中圆角矩形判断函数
//x, y位鼠标坐标 
//若成功选中，返回TRUE 
bool SelectRndRect(double x, double y);

 //选中矩形判断函数
//x, y位鼠标坐标 
//若成功选中，返回TRUE 
bool SelectRect(double x, double y);

 //选中菱形判断函数
//x, y位鼠标坐标 
//若成功选中，返回TRUE 
bool SelectDiamond(double x, double y);

 //选中文本框判断函数
//x, y位鼠标坐标 
//若成功选中，返回TRUE 
bool SelectText(double x, double y);

//取消选中函数
//将当前已选中的所有函数取消选中
void CancelSelect();

//复制函数
//先清除链表中上一次复制的所有图形 
//复制链表中所有当前已选中的图形
void Copy();

//粘贴函数
//将当前在粘贴板链表内的所有图形加入到总链表当中
void Paste();

extern double MAX(double a, double b);
extern double MIN(double a, double b);

#endif
