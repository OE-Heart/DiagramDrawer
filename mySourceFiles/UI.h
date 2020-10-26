#ifndef _UI_H_
#define _UI_H_

extern double winwidth, winheight;   // ���ڳߴ�
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

void drawPenColorPanel();//����ѡɫ�庯��

void drawFillColorPanel();//����ѡɫ�庯��

void customPenColor();//�û��Զ��廭����ɫ����

void customFillColor();//�û��Զ��������ɫ����

void customPenSize();//�û��Զ����ˢ��С����

void drawButtons();//����UI�еİ�ť

void drawMenu();//���в˵�����

void drawCanvas(); //���ƻ���

#endif
