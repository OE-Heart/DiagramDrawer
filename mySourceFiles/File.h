#ifndef _FILE_H_
#define _FILE_H_

extern double winwidth, winheight;   // 窗口尺寸

void SaveFile(linkedlistADT list[]);/*保存函数:将main.c当中的 list[]所保存的图形全部写入一个文件*/

void LoadFile(linkedlistADT list[]);/*文件读取函数:读取先前保存的一个文件，载入至list链表数组*/ 

void saveDiagram(char* fileName);/*导出函数：截取屏幕并保存为BMP文件*/

#endif 
