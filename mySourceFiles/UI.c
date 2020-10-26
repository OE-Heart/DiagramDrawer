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
#pragma comment(lib,"winmm.lib")

void drawCanvas() //绘制画布
{
	//基底绘制
	SetPenColor("Gray");
	drawRectangle(0, 0, winwidth, winheight, 1);
	DefineColor("LightGray", 231.0 / 255, 231.0 / 255, 231.0 / 255);
	SetPenColor("LightGray");
	drawRectangle(0, 0, winwidth*0.10, winheight, 1);
	drawRectangle(winwidth*0.85, 0, winwidth*0.30, winheight, 1);
	if (newFile)
	{
		SetPenColor("White");
		drawRectangle(0.1*winwidth, 0.37, 0.75*winwidth, winheight-0.74, 1);
	}
}

void drawButtons()
{
	double fH = GetFontHeight();
	double h = fH * 2;  // 控件高度
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 4.5; // 控件宽度
	double h_init = winheight / 5; // 控件宽度
	double w_init = winwidth / 50;
	int i = 0;
	int penSize = GetPenSize();
	string color = GetPenColor();
	static int cnt = 1;
	char fileName[30];
	
	SetPenColor("Black");
	drawLabel(winwidth / 55, winheight-3*h, "图标工具栏");
	usePredefinedColors(4);/*使用simpleGUI库预设配色*/ 
	//图标工具栏按钮
	if (button(GenUIID(0), w_init, h_init + 12.0*h, w / 4, h, "箭头"))
	{
		isDrawing = TRUE;
		currentTool = ARROW;
		strcpy(currentToolName, ToolName[0]);
	};


	if (button(GenUIID(0), w_init, h_init + 10.0*h, w / 4, h, "矩形"))
	{
		isDrawing = TRUE;
		currentTool = RECT;
		strcpy(currentToolName, ToolName[1]);
	};

	if (button(GenUIID(0), w_init, h_init + 8.0*h, w / 4, h, "圆角矩形"))
	{
		isDrawing = TRUE;
		currentTool = RNDRECT;
		strcpy(currentToolName, ToolName[2]);
	};

	if (button(GenUIID(0), w_init, h_init + 6.0*h, w / 4, h, "菱形"))
	{
		isDrawing = TRUE;
		currentTool = DIAMOND;
		strcpy(currentToolName, ToolName[3]);
	};

	if (button(GenUIID(0), w_init, h_init + 4.0*h, w / 4, h, "直角箭头"))
	{
		isDrawing = TRUE;
		currentTool = RTARROW;
		strcpy(currentToolName, ToolName[4]);
	};
	
	if (button(GenUIID(0), w_init, h_init + 2.0*h, w / 4, h, "文本框"))
	{
		isDrawing = TRUE;
		currentTool = STRING;
		strcpy(currentToolName, ToolName[5]);
	};

	SetPenColor("Black");
	drawLabel(winwidth / 52 * 46, winheight / 50 * 46, "您当前的线型是：");
	//线型按钮 
	int isSolid;
	if (strcmp(currentPenType, "solid") == 0) isSolid = 1;
	else isSolid = 0;
	if (button(GenUIID(0), winwidth*0.897, h_init + 14.6 * h, w / 4, h, isSolid?"solid":"dashed"))
		if (isSolid) currentPenType = "dashed";
		else currentPenType = "solid";
	
	//清屏按钮 
	if (button(GenUIID(0), w_init, h_init - 2.5*h, w / 4, h, "清屏"))
	{
		for (i = 0; i < FLIST; i++) FreeLinkedList(list[i]);
		for (i = 0; i < FLIST; i++) list[i] = NewLinkedList();
	}
	
	//导出按钮
	if (button(GenUIID(0), w_init, h_init - h, w / 4, h, "导出"))
	{
		sprintf(fileName, "Diagram%d.bmp", cnt);
		saveDiagram(fileName);
		cnt++;
	}
	
	SetPenSize(penSize); /*恢复粗细*/
	SetPenColor(color);/*恢复颜色*/
}

void drawMenu()
{
	static char * menuListFile[] = { "文件",
		"新建 (Ctrl+N)",
		"打开 (Ctrl+O)",
		"保存 (Ctrl+S)",
		"关闭 (Ctrl+W)",
		"退出 (Esc)" };
	static char * menuListEdit[] = { "编辑",
		"选择 (Ctrl+D)",
		"复制 (Ctrl+C)",
		"粘贴 (Ctrl+V)",
		"删除 (Back)",
		"改变 (Ctrl+Z)" };
	static char * menuListTool[] = { "工具",
		"箭头",
		"矩形",
		"圆角矩形",
		"菱形",
		"直角箭头",
		"文本框" };
	static char * menuListHelp[] = { "帮助",
		"用户手册",
		"关于我们",
		"背景音乐ON"};

	double fH = GetFontHeight();
	double x = 0; //fH/8;
	double y = winheight;
	double h = fH * 1.5; // 控件高度
	double w = TextStringWidth(menuListHelp[0]) * 2; // 控件宽度
	double wlist = TextStringWidth(menuListFile[1])*1.2;
	double xindent = winheight / 20; // 缩进
	int selection;
	int i; 
	int penSize = GetPenSize();
	string color = GetPenColor();
	
	usePredefinedColors(4);/*使用simpleGUI库预设配色*/ 

	// menu bar
	drawMenuBar(0, y - h, winwidth, h);

	//File菜单
	selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListFile, sizeof(menuListFile) / sizeof(menuListFile[0]));
	if (selection > 0) selectedLabel = menuListFile[selection];
	if (selection == 1)
	{
		newFile = TRUE;
		isSaved = FALSE;
	}
	if (selection == 2)
	{
		newFile = TRUE;
		LoadFile(list);
	}

	if (selection == 3)
	{
		SaveFile(list);
		isSaved = TRUE;
	}
	if (selection == 4)
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
				MessageBox(NULL, TEXT("记得保存哦"), TEXT("温馨提示"), MB_OK);
			else if (quit_confirm == IDYES)
			{
				newFile = FALSE;
				for (i = 0; i < FLIST; i++) FreeLinkedList(list[i]);
				for (i = 0; i < FLIST; i++) list[i] = NewLinkedList();
				isSaved = FALSE;
			}
		}
	}

	if (selection == 5)
	{ 
		if (isSaved) exit(-1);
		else
		{
			quit_confirm = MessageBox(NULL, TEXT("您的进度尚未保存，确定要关闭吗？"), TEXT("温馨提示"), MB_YESNOCANCEL | MB_ICONQUESTION);
			if (quit_confirm == IDNO) MessageBox(NULL, TEXT("记得保存哦"), TEXT("温馨提示"), MB_OK);
			else if (quit_confirm == IDYES) exit(-1);
		}
	} 

//	 Edit 菜单
	selection = menuList(GenUIID(0), x + w, y - h, w, wlist, h, menuListEdit, sizeof(menuListEdit) / sizeof(menuListEdit[0]));
	if (selection > 0) selectedLabel = menuListEdit[selection];
	if (selection == 1) 
	{ 
		isDrawing = FALSE;
		isSelecting = TRUE;
		isChanging = FALSE;
	}
	if (selection == 2) 
	{
		Copy();
	}
	if (selection == 3) 
	{
		Paste();
		display();
	}
	if (selection == 4) 
	{
		Delete();
		display();
	}
	if (selection == 5) 
	{ 
		isDrawing = FALSE;
		isSelecting = FALSE;
		isChanging = TRUE;
	}

//	 Tool 菜单
	selection = menuList(GenUIID(0), x + 2 * w, y - h, w, wlist, h, menuListTool, sizeof(menuListTool) / sizeof(menuListTool[0]));
	if (selection > 0) selectedLabel = menuListTool[selection];
	switch (selection){
		case 1:
			isDrawing = TRUE;
			currentTool = ARROW;
			strcpy(currentToolName, ToolName[0]);
			break;
		case 2:
			isDrawing = TRUE;
			currentTool = RECT;
			strcpy(currentToolName, ToolName[1]);
			break;
		case 3:
			isDrawing = TRUE;
			currentTool = RNDRECT;
			strcpy(currentToolName, ToolName[2]);
			break;
		case 4:
			isDrawing = TRUE;
			currentTool = DIAMOND;
			strcpy(currentToolName, ToolName[3]);
			break;
		case 5:
			isDrawing = TRUE;
			currentTool = RTARROW;
			strcpy(currentToolName, ToolName[4]);
		case 6:
			isDrawing = TRUE;
			currentTool = STRING;
			strcpy(currentToolName, ToolName[5]);
			break;
	}

//	 Help 菜单
	selection = menuList(GenUIID(0), x + 3 * w, y - h, w, wlist, h, menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));
	if (selection > 0) selectedLabel = menuListHelp[selection];
	switch (selection){
		case 1:	
			isDrawing = FALSE; 
			MessageBox(NULL, TEXT("（ 注意：若您正在使用的是surface等配有高分辨率屏幕的计算机，请在桌面右键->显示设置中确保分辨率为1920 * 1080，更改文本、应用等大小为125%，否则将会出现用户界面错位等不可预期的错误！）\n\n初次使用： \n 点击文件->新建按钮可新建一张空白画布。\n\n在左边工具栏可选择绘图工具，并在空白画布上进行创作。（注意：选择文本框工具后需点击画布区域选择位置再进行输入）在创作时，我们为您默认的画笔颜色为黑色，填充颜色为白色，您可以根据自己的喜好和需求在右侧调色板中选择自己喜欢的颜色，我们为您预置了10种常用颜色，此外，您还可以根据自己的需求输入颜色的10进制RGB码，来选择自己心仪的颜色，当然，您也可以自定义线宽和线型。 \n\n 清屏：\n 对自己的创作不满意？点击清屏按钮，一切重新开始。\n\n 编辑功能:\n 点击编辑菜单，点击选择按钮，在点击图形后已选中图形会变为品红色，支持选中多个图形。点击编辑菜单，点击改变按钮进入编辑状态，利用左键进行拖移，右键进行缩放，鼠标滚轮可以调整文本框字体大小。\n\n 保存与打开文件： \n 在点击文件->保存后，本软件会将您的作品中的所有图元信息保存在根目录下的文本文件中，下次使用时您只需点击文件->打开，输入您想打开的文件名即可继续您的创作。\n\n 保存为图片格式：\n 如果您想永久保留您的作品，或者在社交网站上或向亲朋好友分享自己的作品，您可以点击保存画布为图片按钮将您的创作内容保存为BMP格式图片，这样您就可以向没有本软件的人分享你的创作。\n\n背景音乐：\n帮助菜单中可以打开背景音乐，听说音乐更能激发创作灵感哦。"),
			TEXT("用户手册"), MB_OK); 
			//return ;
			break;
		case 2:	
			isDrawing = FALSE;
			MessageBox(NULL, TEXT("感谢您使用DiagramDrawer！本软件是一个便捷高效、实用性强的小型算法流程图绘制工具。"),
			TEXT("关于我们"), MB_OK); 
			//return ;
			break;
		case 3:
			isDrawing = FALSE;
			menuListHelp[3] = isPlayingMusic ? "背景音乐ON" : "背景音乐OFF"; 
			isPlayingMusic = ! isPlayingMusic;
			if (isPlayingMusic == 1) PlaySound (TEXT("..\\城南花已开.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			else PlaySound (NULL, NULL, SND_FILENAME | SND_ASYNC);
			break; 
	}

	x = winwidth / 15;
	y = winheight / 8 * 7;
		
	//状态栏绘制
	drawMenuBar(0, 0, winwidth, h);
	SetPenColor("Black");
	drawLabel(x, winheight / 95, "最近选择的菜单项是:");
	drawLabel(winwidth / 5.6, winheight / 95, selectedLabel);
	drawLabel(winwidth / 5.6 + winwidth * 0.15, winheight / 95, "现在你正在使用的工具是：");
	drawLabel(winwidth *0.47, winheight / 95, currentToolName);
	drawLabel(winwidth / 1.6, winheight / 95, "你现在的状态是：");
	if (isDrawing) drawLabel(winwidth *0.47+5, winheight / 95, "绘图模式");
	else drawLabel(winwidth *0.47+5, winheight / 95, "编辑模式");
	
	SetPenSize(penSize); /*恢复粗细*/
	SetPenColor(color);/*恢复颜色*/
}

void customPenSize()
{
	double x = winwidth * 0.88, y = winheight * 0.45;
	double ColorPieceW = winwidth / 90, ColorPieceH = winwidth / 90;
	static char inputPenSize[10] = "1";
	int penSize = GetPenSize();
	string color = GetPenColor();

	setTextBoxColors("White", "Black", "LightGray", "Black", 1);
	textbox(GenUIID(0), x + ColorPieceW, 3.5 * ColorPieceW , 3 * ColorPieceW, 2 * ColorPieceH, inputPenSize, sizeof(inputPenSize));
	SetPenColor("Black");
	drawLabel(winwidth/52*45, winheight/50*7, "输入参数以自定义线宽：");

	if (button(GenUIID(0), x + 4.4 * ColorPieceW, 3.5*ColorPieceW, 3*ColorPieceW, 2*ColorPieceH*0.75, "确认"))
	{
		sscanf(inputPenSize, "%d", &currentPenSize);
		display(); 
	}
	
	SetPenSize(penSize); /*恢复粗细*/
	SetPenColor(color);/*恢复颜色*/
}

void customFillColor()
{
	double x = winwidth * 0.88, y = winheight * 0.45;
	double ColorPieceW = winwidth / 90, ColorPieceH = winwidth / 90;
	static char R_input[10] = "0", G_input[10] = "0", B_input[10] = "0",//用户自定义颜色RGB参数
		custom_color[20] = "customFillColor"; //自定义颜色的名称
	int RGB[3] = { 0,0,0 };
	
	SetPenColor("Black");
	drawLabel(winwidth / 50 * 43.6, winheight / 50 * 20, "您当前的填充颜色是：");
	drawLabel(winwidth / 50 * 43.3, winheight / 50 * 14.6, "输入参数以自定义颜色：");
	
	DefineColor("LightGray", 231.0 / 255, 231.0 / 255, 231.0 / 255);
	//创建自定义颜色输入框和按钮
	setTextBoxColors("White", "Red", "LightGray", "Red", 1);
	textbox(GenUIID(0), x-0.14, 10 * ColorPieceW, 3 * ColorPieceW, 2 * ColorPieceH, R_input, sizeof(R_input));

	setTextBoxColors("White", "Green", "LightGray", "Green", 1);
	textbox(GenUIID(0), x-0.10 + 3 * ColorPieceW, 10 * ColorPieceW, 3 * ColorPieceW, 2 * ColorPieceH, G_input, sizeof(G_input));

	setTextBoxColors("White", "Blue", "LightGray", "Blue", 1);
	textbox(GenUIID(0), x-0.05 + 6 * ColorPieceW, 10 * ColorPieceW, 3 * ColorPieceW, 2 * ColorPieceH, B_input, sizeof(B_input));

	usePredefinedColors(4);/*使用simpleGUI库预设配色*/ 
	if (button(GenUIID(403), x-0.09 + 3 * ColorPieceW, 8 * ColorPieceW, 3 * ColorPieceW, 2 * ColorPieceH *0.75, "确认"))
	{
		sscanf(R_input, "%d", &RGB[0]);
		sscanf(G_input, "%d", &RGB[1]);
		sscanf(B_input, "%d", &RGB[2]);

		DefineColor(custom_color, 1.0*RGB[0] / 255, 1.0*RGB[1] / 255, 1.0*RGB[2] / 255);
		currentFillColor = custom_color;
	}
	SetPenColor(currentFillColor);
	drawRectangle(x, 15 * ColorPieceW, 8 * ColorPieceW, 2 * ColorPieceH, 1);	
}

void customPenColor()
{
	double x = winwidth * 0.88, y = winheight * 0.45;
	double ColorPieceW = winwidth / 90, ColorPieceH = winwidth / 90;
	static char R_input[10] = "0", G_input[10] = "0", B_input[10] = "0",//用户自定义颜色RGB参数
		custom_color[20] = "customPenColor"; //自定义颜色的名称
	int RGB[3] = { 0,0,0 };
	
	SetPenColor("Black");
	drawLabel(winwidth / 50 * 43.6, winheight / 50 * 36.6, "您当前的画笔颜色是：");
	drawLabel(winwidth / 50 * 43.3, winheight / 50 * 31, "输入参数以自定义颜色：");
	
	DefineColor("LightGray", 231.0 / 255, 231.0 / 255, 231.0 / 255);
	//创建自定义颜色输入框和按钮
	setTextBoxColors("White", "Red", "LightGray", "Red", 1);
	textbox(GenUIID(0), x-0.14, 25 * ColorPieceW, 3 * ColorPieceW, 2 * ColorPieceH, R_input, sizeof(R_input));

	setTextBoxColors("White", "Green", "LightGray", "Green", 1);
	textbox(GenUIID(0), x-0.10 + 3 * ColorPieceW, 25 * ColorPieceW, 3 * ColorPieceW, 2 * ColorPieceH, G_input, sizeof(G_input));

	setTextBoxColors("White", "Blue", "LightGray", "Blue", 1);
	textbox(GenUIID(0), x-0.05 + 6 * ColorPieceW, 25 * ColorPieceW, 3 * ColorPieceW, 2 * ColorPieceH, B_input, sizeof(B_input));

	usePredefinedColors(4);/*使用simpleGUI库预设配色*/ 
	if (button(GenUIID(0), x-0.09 + 3 * ColorPieceW, 23* ColorPieceW, 3 * ColorPieceW, 2 * ColorPieceH *0.75, "确认"))
	{
		sscanf(R_input, "%d", &RGB[0]);
		sscanf(G_input, "%d", &RGB[1]);
		sscanf(B_input, "%d", &RGB[2]);

		DefineColor(custom_color, 1.0*RGB[0] / 255, 1.0*RGB[1] / 255, 1.0*RGB[2] / 255);
		currentPenColor = custom_color;
	}
	SetPenColor(currentPenColor);
	drawRectangle(x, 30 * ColorPieceW, 8 * ColorPieceW, 2 * ColorPieceH, 1);
}

void drawFillColorPanel()
{
	string color = GetPenColor();
	
	double ColorPieceW = winwidth / 70, ColorPieceH = winwidth / 70;
	double x = winwidth * 0.89, y = winheight /50 * 23.2;
	int i, j, cnt = 0;
	char *colorName[] = {"Black","White","Red","Blue","Green","Yellow", "Violet", "Orange", "Brown", "Cyan"};

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 2; j++)
		{
			setButtonColors(colorName[i+j*5], colorName[i+j*5], colorName[i+j*5], colorName[i+j*5], 1);
			if (button(GenUIID(cnt), x + i * ColorPieceW, y - j * ColorPieceW, ColorPieceW, ColorPieceH, " ?"))
			{
				currentFillColor = colorName[i+j*5];
			}
			cnt++;
		}
	}
	
	SetPenColor(color);/*恢复颜色*/
}

void drawPenColorPanel()
{
	string color = GetPenColor();
	
	double ColorPieceW = winwidth / 70, ColorPieceH = winwidth / 70;
	double x = winwidth * 0.89, y = winheight * 0.8;
	int i, j, cnt = 0;
	char *colorName[] = {"Black","White","Red","Blue","Green","Yellow", "Violet", "Orange", "Brown", "Cyan"};

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 2; j++)
		{
			setButtonColors(colorName[i+j*5], colorName[i+j*5], colorName[i+j*5], colorName[i+j*5], 1);
			if (button(GenUIID(cnt), x + i * ColorPieceW, y - j * ColorPieceW, ColorPieceW, ColorPieceH, " ?"))
			{
				currentPenColor = colorName[i+j*5];
			}
			cnt++;
		}
	}
	
	SetPenColor(color);/*恢复颜色*/
}
