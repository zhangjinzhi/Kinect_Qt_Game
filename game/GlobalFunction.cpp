/****************************************************************************
** Global Function Implementation                                           *
*****************************************************************************/

#include "GlobalFunction.h"
#include <time.h>
#include <QLabel>
#include <QToolTip>
#include <QtGui>

/****************************************************************************/

int gRandomGen(int min, int max)
{
    if (min < max)
    {
        return rand() % (max - min) + min;
    }
    else if (min > max)
    {
        return rand() % (min - max) + max;
    }
    else
    {
        return min;
    }
}

int gRandomArray(int* pData, int num)
{
    if (pData == NULL)
    {
        return 0;
    }
    int i, j, temp;
    for (i = 0; i < num; i++)
    {
        pData[i] = i;
    }
    srand((unsigned int)time(NULL));
    for (i = 0; i < num; i++)
    {
        j = rand() % (num - i) + i;
        if (i != j)
        {
            temp     = pData[i];
            pData[i] = pData[j];
            pData[j] = temp;
        }
    }
    return 1;
}

double gDistance(const QPoint& p1, const QPoint& p2)
{
    return sqrt( 1. * (p1.x() - p2.x()) * (p1.x() - p2.x()) + (p1.y() - p2.y()) * (p1.y() - p2.y()) );
}

/****************************************************************************/

// end of file
