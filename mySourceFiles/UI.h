#ifndef _UI_H_
#define _UI_H_

extern double winwidth, winheight;   // 窗口尺寸
extern char  *ToolName[];
extern char currentToolName[20];
extern string currentPenColor;
extern string currentFillColor; 
extern int currentTool;
extern int currentPenSize; 
extern string currentPenType;
extern bool isDrawing;
extern bool isSelecting;
extern bool isChanging;
extern bool newFile;
extern bool isSaved;
extern int quit_confirm;
extern string selectedLabel;
static int isPlayingMusic = 0;

void drawPenColorPanel();//绘制选色板函数

void drawFillColorPanel();//绘制选色板函数

void customPenColor();//用户自定义画笔颜色函数

void customFillColor();//用户自定义填充颜色函数

void customPenSize();//用户自定义笔刷大小函数

void drawButtons();//绘制UI中的按钮

void drawMenu();//进行菜单绘制

void drawCanvas(); //绘制画布

#endif
