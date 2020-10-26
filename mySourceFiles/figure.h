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

#define CURSOR "_"          /*������*/
#define CURSOR_BLINK  1     /*�����˸��ʱ���¼���־��*/
#define TEXTLEN  100

typedef struct {/*��ͷ����*/
	double x1, y1;/*�������*/
    double x2, y2;/*�յ�����*/
    int penSize; /*�ʴ�ϸ*/
    string penType;/*�����ͣ�solid,dashed,dotted*/ 
    string penColor; /*����ɫ*/
    bool isSelected; /*ѡ��*/ 
} *myArrow;

typedef struct {/*��������*/
	double x1, y1;/*���Ͻ�����*/
    double x2, y2;/*���½�����*/
    int penSize; /*�ʴ�ϸ*/
    string penType;/*�����ͣ�solid,dashed,dotted*/ 
    string penColor; /*����ɫ*/
    string fillColor;/*�����ɫ*/ 
    bool isSelected; /*ѡ��*/ 
} *myRect;

typedef struct {/*Բ�Ǿ�������*/
	double x1, y1;/*���Ͻ�����*/
    double x2, y2;/*���½�����*/
    int penSize; /*�ʴ�ϸ*/
    string penType;/*�����ͣ�solid,dashed,dotted*/ 
    string penColor; /*����ɫ*/
    string fillColor;/*�����ɫ*/
    bool isSelected; /*ѡ��*/ 
} *myRndRect;

typedef struct {/*��������*/
	double x1, y1;/*���Ͻ�����*/
    double x2, y2;/*���½�����*/
    int penSize; /*�ʴ�ϸ*/
    string penType;/*�����ͣ�solid,dashed,dotted*/ 
    string penColor; /*����ɫ*/
    string fillColor;/*�����ɫ*/ 
    bool isSelected; /*ѡ��*/ 
} *myDiamond;

typedef struct {
	double x1, y1;/*���Ͻ�����*/
    double x2, y2;/*���½�����*/
    double angle;/*�Ƕ�*/ 
    int penSize; /*�ʴ�ϸ*/
    string penType;/*�����ͣ�solid,dashed,dotted*/ 
    string penColor; /*����ɫ*/
    bool direction; /*����˳ʱ�� 1����ʱ�� 0*/ 
    bool isSelected; /*ѡ��*/
} *myRtArrow;

typedef struct {/*�ı�����*/
	string text;/*�ı�ָ��*/
	double x, y;/*�ı���ʾ��ʼλ������*/
    int pointSize; /*���ִ�С*/
    string penColor; /*��ɫ*/
    bool isSelected; /*ѡ��*/
    int cursor; /*���λ��*/
    bool isBlink; /*����Ƿ�����˸״̬*/ 
} *myString;
 
extern linkedlistADT list[FLIST];/*����ͼ��Ԫ������ָ��*/
extern double minDistance[FLIST];
extern int curList; /*��ǰ�������*/
extern myArrow curArrow;/*��ͷ����ĵ�ǰ����ָ��*/
extern myRect curRect;/*��������ĵ�ǰ����ָ��*/
extern myRndRect curRndRect; /*Բ�Ǿ����ı�����ĵ�ǰ����ָ��*/
extern myDiamond curDiamond; /*��������ĵ�ǰ����ָ��*/
extern string selectedColor;

/*��ͷ������*/
void DrawArrow(void *arrow);
void MoveArrow(void *obj, double dx, double dy);
void ChangeArrow(void *obj, double dx, double dy);

/*���δ�����*/
void DrawRect(void *rect);
void MoveRect(void *rect, double dx, double dy);
void ChangeRect(void *rect, double dx, double dy);

/*Բ�Ǿ��δ�����*/
void DrawRndRect(void *rndRect);
void MoveRndRect(void *rndRect, double dx, double dy);
void ChangeRndRect(void *rndRect, double dx, double dy);

/*���δ�����*/
void DrawDiamond(void *diamond);
void MoveDiamond(void *diamond, double dx, double dy);
void ChangeDiamond(void *diamond, double dx, double dy);

/*ֱ�Ǽ�ͷ������*/
void DrawRtArrow(void *RtArrow);
void MoveRtArrow(void *obj, double dx, double dy);
void ChangeRtArrow(void *obj, double dx, double dy);

/*�ı�������*/
void DrawString(void *str); /*��ʾ�ı�*/
void MoveString(void *str, double dx, double dy);
void PointSizeUp(void *str);
void PointSizeDown(void *str);
void InsertCharToString(string str, int pos, char c);/*���ַ�c���뵽�ַ���str��posλ��*/
void DeleteCharFromString(string str, int pos);/*ɾ���ַ���str��posλ���ַ�*/
void DrawCursor(string str, int cursor, double startx, double starty);/*��ʾ�ı����*/

//�����ߺ���
//�÷��������� DrawLine() ������ͬ 
void DrawDashedLine(double dx, double dy);

#endif
