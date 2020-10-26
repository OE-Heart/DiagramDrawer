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

void drawCanvas() //���ƻ���
{
	//���׻���
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
	double h = fH * 2;  // �ؼ��߶�
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 4.5; // �ؼ����
	double h_init = winheight / 5; // �ؼ����
	double w_init = winwidth / 50;
	int i = 0;
	int penSize = GetPenSize();
	string color = GetPenColor();
	static int cnt = 1;
	char fileName[30];
	
	SetPenColor("Black");
	drawLabel(winwidth / 55, winheight-3*h, "ͼ�깤����");
	usePredefinedColors(4);/*ʹ��simpleGUI��Ԥ����ɫ*/ 
	//ͼ�깤������ť
	if (button(GenUIID(0), w_init, h_init + 12.0*h, w / 4, h, "��ͷ"))
	{
		isDrawing = TRUE;
		currentTool = ARROW;
		strcpy(currentToolName, ToolName[0]);
	};


	if (button(GenUIID(0), w_init, h_init + 10.0*h, w / 4, h, "����"))
	{
		isDrawing = TRUE;
		currentTool = RECT;
		strcpy(currentToolName, ToolName[1]);
	};

	if (button(GenUIID(0), w_init, h_init + 8.0*h, w / 4, h, "Բ�Ǿ���"))
	{
		isDrawing = TRUE;
		currentTool = RNDRECT;
		strcpy(currentToolName, ToolName[2]);
	};

	if (button(GenUIID(0), w_init, h_init + 6.0*h, w / 4, h, "����"))
	{
		isDrawing = TRUE;
		currentTool = DIAMOND;
		strcpy(currentToolName, ToolName[3]);
	};

	if (button(GenUIID(0), w_init, h_init + 4.0*h, w / 4, h, "ֱ�Ǽ�ͷ"))
	{
		isDrawing = TRUE;
		currentTool = RTARROW;
		strcpy(currentToolName, ToolName[4]);
	};
	
	if (button(GenUIID(0), w_init, h_init + 2.0*h, w / 4, h, "�ı���"))
	{
		isDrawing = TRUE;
		currentTool = STRING;
		strcpy(currentToolName, ToolName[5]);
	};

	SetPenColor("Black");
	drawLabel(winwidth / 52 * 46, winheight / 50 * 46, "����ǰ�������ǣ�");
	//���Ͱ�ť 
	int isSolid;
	if (strcmp(currentPenType, "solid") == 0) isSolid = 1;
	else isSolid = 0;
	if (button(GenUIID(0), winwidth*0.897, h_init + 14.6 * h, w / 4, h, isSolid?"solid":"dashed"))
		if (isSolid) currentPenType = "dashed";
		else currentPenType = "solid";
	
	//������ť 
	if (button(GenUIID(0), w_init, h_init - 2.5*h, w / 4, h, "����"))
	{
		for (i = 0; i < FLIST; i++) FreeLinkedList(list[i]);
		for (i = 0; i < FLIST; i++) list[i] = NewLinkedList();
	}
	
	//������ť
	if (button(GenUIID(0), w_init, h_init - h, w / 4, h, "����"))
	{
		sprintf(fileName, "Diagram%d.bmp", cnt);
		saveDiagram(fileName);
		cnt++;
	}
	
	SetPenSize(penSize); /*�ָ���ϸ*/
	SetPenColor(color);/*�ָ���ɫ*/
}

void drawMenu()
{
	static char * menuListFile[] = { "�ļ�",
		"�½� (Ctrl+N)",
		"�� (Ctrl+O)",
		"���� (Ctrl+S)",
		"�ر� (Ctrl+W)",
		"�˳� (Esc)" };
	static char * menuListEdit[] = { "�༭",
		"ѡ�� (Ctrl+D)",
		"���� (Ctrl+C)",
		"ճ�� (Ctrl+V)",
		"ɾ�� (Back)",
		"�ı� (Ctrl+Z)" };
	static char * menuListTool[] = { "����",
		"��ͷ",
		"����",
		"Բ�Ǿ���",
		"����",
		"ֱ�Ǽ�ͷ",
		"�ı���" };
	static char * menuListHelp[] = { "����",
		"�û��ֲ�",
		"��������",
		"��������ON"};

	double fH = GetFontHeight();
	double x = 0; //fH/8;
	double y = winheight;
	double h = fH * 1.5; // �ؼ��߶�
	double w = TextStringWidth(menuListHelp[0]) * 2; // �ؼ����
	double wlist = TextStringWidth(menuListFile[1])*1.2;
	double xindent = winheight / 20; // ����
	int selection;
	int i; 
	int penSize = GetPenSize();
	string color = GetPenColor();
	
	usePredefinedColors(4);/*ʹ��simpleGUI��Ԥ����ɫ*/ 

	// menu bar
	drawMenuBar(0, y - h, winwidth, h);

	//File�˵�
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
			quit_confirm = MessageBox(NULL, TEXT("���Ľ�����δ���棬ȷ��Ҫ�ر���"), TEXT("��ܰ��ʾ"), MB_YESNOCANCEL | MB_ICONQUESTION);
			if (quit_confirm == IDNO)
				MessageBox(NULL, TEXT("�ǵñ���Ŷ"), TEXT("��ܰ��ʾ"), MB_OK);
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
			quit_confirm = MessageBox(NULL, TEXT("���Ľ�����δ���棬ȷ��Ҫ�ر���"), TEXT("��ܰ��ʾ"), MB_YESNOCANCEL | MB_ICONQUESTION);
			if (quit_confirm == IDNO) MessageBox(NULL, TEXT("�ǵñ���Ŷ"), TEXT("��ܰ��ʾ"), MB_OK);
			else if (quit_confirm == IDYES) exit(-1);
		}
	} 

//	 Edit �˵�
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

//	 Tool �˵�
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

//	 Help �˵�
	selection = menuList(GenUIID(0), x + 3 * w, y - h, w, wlist, h, menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));
	if (selection > 0) selectedLabel = menuListHelp[selection];
	switch (selection){
		case 1:	
			isDrawing = FALSE; 
			MessageBox(NULL, TEXT("�� ע�⣺��������ʹ�õ���surface�����и߷ֱ�����Ļ�ļ���������������Ҽ�->��ʾ������ȷ���ֱ���Ϊ1920 * 1080�������ı���Ӧ�õȴ�СΪ125%�����򽫻�����û������λ�Ȳ���Ԥ�ڵĴ��󣡣�\n\n����ʹ�ã� \n ����ļ�->�½���ť���½�һ�ſհ׻�����\n\n����߹�������ѡ���ͼ���ߣ����ڿհ׻����Ͻ��д�������ע�⣺ѡ���ı��򹤾ߺ�������������ѡ��λ���ٽ������룩�ڴ���ʱ������Ϊ��Ĭ�ϵĻ�����ɫΪ��ɫ�������ɫΪ��ɫ�������Ը����Լ���ϲ�ú��������Ҳ��ɫ����ѡ���Լ�ϲ������ɫ������Ϊ��Ԥ����10�ֳ�����ɫ�����⣬�������Ը����Լ�������������ɫ��10����RGB�룬��ѡ���Լ����ǵ���ɫ����Ȼ����Ҳ�����Զ����߿�����͡� \n\n ������\n ���Լ��Ĵ��������⣿���������ť��һ�����¿�ʼ��\n\n �༭����:\n ����༭�˵������ѡ��ť���ڵ��ͼ�κ���ѡ��ͼ�λ��ΪƷ��ɫ��֧��ѡ�ж��ͼ�Ρ�����༭�˵�������ı䰴ť����༭״̬����������������ƣ��Ҽ��������ţ������ֿ��Ե����ı��������С��\n\n ��������ļ��� \n �ڵ���ļ�->����󣬱�����Ὣ������Ʒ�е�����ͼԪ��Ϣ�����ڸ�Ŀ¼�µ��ı��ļ��У��´�ʹ��ʱ��ֻ�����ļ�->�򿪣���������򿪵��ļ������ɼ������Ĵ�����\n\n ����ΪͼƬ��ʽ��\n ����������ñ���������Ʒ���������罻��վ�ϻ���������ѷ����Լ�����Ʒ�������Ե�����滭��ΪͼƬ��ť�����Ĵ������ݱ���ΪBMP��ʽͼƬ���������Ϳ�����û�б�������˷�����Ĵ�����\n\n�������֣�\n�����˵��п��Դ򿪱������֣���˵���ָ��ܼ����������Ŷ��"),
			TEXT("�û��ֲ�"), MB_OK); 
			//return ;
			break;
		case 2:	
			isDrawing = FALSE;
			MessageBox(NULL, TEXT("��л��ʹ��DiagramDrawer���������һ����ݸ�Ч��ʵ����ǿ��С���㷨����ͼ���ƹ��ߡ�"),
			TEXT("��������"), MB_OK); 
			//return ;
			break;
		case 3:
			isDrawing = FALSE;
			menuListHelp[3] = isPlayingMusic ? "��������ON" : "��������OFF"; 
			isPlayingMusic = ! isPlayingMusic;
			if (isPlayingMusic == 1) PlaySound (TEXT("..\\���ϻ��ѿ�.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			else PlaySound (NULL, NULL, SND_FILENAME | SND_ASYNC);
			break; 
	}

	x = winwidth / 15;
	y = winheight / 8 * 7;
		
	//״̬������
	drawMenuBar(0, 0, winwidth, h);
	SetPenColor("Black");
	drawLabel(x, winheight / 95, "���ѡ��Ĳ˵�����:");
	drawLabel(winwidth / 5.6, winheight / 95, selectedLabel);
	drawLabel(winwidth / 5.6 + winwidth * 0.15, winheight / 95, "����������ʹ�õĹ����ǣ�");
	drawLabel(winwidth *0.47, winheight / 95, currentToolName);
	drawLabel(winwidth / 1.6, winheight / 95, "�����ڵ�״̬�ǣ�");
	if (isDrawing) drawLabel(winwidth *0.47+5, winheight / 95, "��ͼģʽ");
	else drawLabel(winwidth *0.47+5, winheight / 95, "�༭ģʽ");
	
	SetPenSize(penSize); /*�ָ���ϸ*/
	SetPenColor(color);/*�ָ���ɫ*/
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
	drawLabel(winwidth/52*45, winheight/50*7, "����������Զ����߿�");

	if (button(GenUIID(0), x + 4.4 * ColorPieceW, 3.5*ColorPieceW, 3*ColorPieceW, 2*ColorPieceH*0.75, "ȷ��"))
	{
		sscanf(inputPenSize, "%d", &currentPenSize);
		display(); 
	}
	
	SetPenSize(penSize); /*�ָ���ϸ*/
	SetPenColor(color);/*�ָ���ɫ*/
}

void customFillColor()
{
	double x = winwidth * 0.88, y = winheight * 0.45;
	double ColorPieceW = winwidth / 90, ColorPieceH = winwidth / 90;
	static char R_input[10] = "0", G_input[10] = "0", B_input[10] = "0",//�û��Զ�����ɫRGB����
		custom_color[20] = "customFillColor"; //�Զ�����ɫ������
	int RGB[3] = { 0,0,0 };
	
	SetPenColor("Black");
	drawLabel(winwidth / 50 * 43.6, winheight / 50 * 20, "����ǰ�������ɫ�ǣ�");
	drawLabel(winwidth / 50 * 43.3, winheight / 50 * 14.6, "����������Զ�����ɫ��");
	
	DefineColor("LightGray", 231.0 / 255, 231.0 / 255, 231.0 / 255);
	//�����Զ�����ɫ�����Ͱ�ť
	setTextBoxColors("White", "Red", "LightGray", "Red", 1);
	textbox(GenUIID(0), x-0.14, 10 * ColorPieceW, 3 * ColorPieceW, 2 * ColorPieceH, R_input, sizeof(R_input));

	setTextBoxColors("White", "Green", "LightGray", "Green", 1);
	textbox(GenUIID(0), x-0.10 + 3 * ColorPieceW, 10 * ColorPieceW, 3 * ColorPieceW, 2 * ColorPieceH, G_input, sizeof(G_input));

	setTextBoxColors("White", "Blue", "LightGray", "Blue", 1);
	textbox(GenUIID(0), x-0.05 + 6 * ColorPieceW, 10 * ColorPieceW, 3 * ColorPieceW, 2 * ColorPieceH, B_input, sizeof(B_input));

	usePredefinedColors(4);/*ʹ��simpleGUI��Ԥ����ɫ*/ 
	if (button(GenUIID(403), x-0.09 + 3 * ColorPieceW, 8 * ColorPieceW, 3 * ColorPieceW, 2 * ColorPieceH *0.75, "ȷ��"))
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
	static char R_input[10] = "0", G_input[10] = "0", B_input[10] = "0",//�û��Զ�����ɫRGB����
		custom_color[20] = "customPenColor"; //�Զ�����ɫ������
	int RGB[3] = { 0,0,0 };
	
	SetPenColor("Black");
	drawLabel(winwidth / 50 * 43.6, winheight / 50 * 36.6, "����ǰ�Ļ�����ɫ�ǣ�");
	drawLabel(winwidth / 50 * 43.3, winheight / 50 * 31, "����������Զ�����ɫ��");
	
	DefineColor("LightGray", 231.0 / 255, 231.0 / 255, 231.0 / 255);
	//�����Զ�����ɫ�����Ͱ�ť
	setTextBoxColors("White", "Red", "LightGray", "Red", 1);
	textbox(GenUIID(0), x-0.14, 25 * ColorPieceW, 3 * ColorPieceW, 2 * ColorPieceH, R_input, sizeof(R_input));

	setTextBoxColors("White", "Green", "LightGray", "Green", 1);
	textbox(GenUIID(0), x-0.10 + 3 * ColorPieceW, 25 * ColorPieceW, 3 * ColorPieceW, 2 * ColorPieceH, G_input, sizeof(G_input));

	setTextBoxColors("White", "Blue", "LightGray", "Blue", 1);
	textbox(GenUIID(0), x-0.05 + 6 * ColorPieceW, 25 * ColorPieceW, 3 * ColorPieceW, 2 * ColorPieceH, B_input, sizeof(B_input));

	usePredefinedColors(4);/*ʹ��simpleGUI��Ԥ����ɫ*/ 
	if (button(GenUIID(0), x-0.09 + 3 * ColorPieceW, 23* ColorPieceW, 3 * ColorPieceW, 2 * ColorPieceH *0.75, "ȷ��"))
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
	
	SetPenColor(color);/*�ָ���ɫ*/
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
	
	SetPenColor(color);/*�ָ���ɫ*/
}
