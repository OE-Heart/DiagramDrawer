#ifndef _FILE_H_
#define _FILE_H_

extern double winwidth, winheight;   // ���ڳߴ�

void SaveFile(linkedlistADT list[]);/*���溯��:��main.c���е� list[]�������ͼ��ȫ��д��һ���ļ�*/

void LoadFile(linkedlistADT list[]);/*�ļ���ȡ����:��ȡ��ǰ�����һ���ļ���������list��������*/ 

void saveDiagram(char* fileName);/*������������ȡ��Ļ������ΪBMP�ļ�*/

#endif 
