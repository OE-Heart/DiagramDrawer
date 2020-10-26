#ifndef _EDIT_H_
#define _EDIT_H_

extern linkedlistADT list[FLIST];

//ѡ���жϺ���
//x, yΪ�������
//���ɹ�ѡ�� 
bool Select(double x, double y);

//�����������ж� cx �Ƿ��� x1 �� x2 ֮�� 
bool isBetween(double x1, double x2, double cx);

//ѡ��ֱ�Ǽ�ͷ�жϺ���
//x, yλ������� 
//���ɹ�ѡ�У�����TRUE 
bool SelectRtArrow(double x, double y);

//ѡ�м�ͷ�жϺ���
//x, yλ������� 
//���ɹ�ѡ�У�����TRUE 
bool SelectArrow(double x, double y);

//ѡ��Բ�Ǿ����жϺ���
//x, yλ������� 
//���ɹ�ѡ�У�����TRUE 
bool SelectRndRect(double x, double y);

 //ѡ�о����жϺ���
//x, yλ������� 
//���ɹ�ѡ�У�����TRUE 
bool SelectRect(double x, double y);

 //ѡ�������жϺ���
//x, yλ������� 
//���ɹ�ѡ�У�����TRUE 
bool SelectDiamond(double x, double y);

 //ѡ���ı����жϺ���
//x, yλ������� 
//���ɹ�ѡ�У�����TRUE 
bool SelectText(double x, double y);

//ȡ��ѡ�к���
//����ǰ��ѡ�е����к���ȡ��ѡ��
void CancelSelect();

//���ƺ���
//�������������һ�θ��Ƶ�����ͼ�� 
//�������������е�ǰ��ѡ�е�ͼ��
void Copy();

//ճ������
//����ǰ��ճ���������ڵ�����ͼ�μ��뵽��������
void Paste();

extern double MAX(double a, double b);
extern double MIN(double a, double b);

#endif
