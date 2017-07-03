#ifndef GLOBALFUNCTION_H
#define GLOBALFUNCTION_H

#include <QtGlobal>							// Qt全局宏定义
#include <QtCore>							// Qt核心
#include <QtGui>							// Qt界面

/****************************************************************************/
// 全局函数
/****************************************************************************/

// GUI_BubbleWidget调用以下三个函数
int gRandomGen(int min, int max);
int gRandomArray(int* pData, int num);
double gDistance(const QPoint& p1, const QPoint& p2);
/****************************************************************************/

#endif // GLOBALFUNCTION_H

// end of file
