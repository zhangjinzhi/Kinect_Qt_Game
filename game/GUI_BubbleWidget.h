#ifndef GUI_BUBBLEWIDGET_H
#define GUI_BUBBLEWIDGET_H
#include "GlobalFunction.h"
#include "thread.h"
#include "my_socket.h"
#include <QMainWindow>
#include <QLabel>
#include <QStatusBar>
#include <QMouseEvent>
/****************************************************************************
 CBubbleWidget Class Definition
****************************************************************************/


class QBubbles
{
public:
    QBubbles();
    void Init(int bubbleNum, int bubbleSize, double bubbleSpeed, QSize srnSize);
    void Update();
public:
    int             mBubbleNum;
    int             mBubbleSize;
    double          mBubbleSpeed;
    QSize           mSrnSize;
    QVector<QPoint> mBubblePos;
    QVector<QPoint> mSpeedDirct;
};

/****************************************************************************/

class CBubbleWidget : public QWidget	// 气泡选择界面，六大类模块
{
    Q_OBJECT

public:
    CBubbleWidget(QWidget* parent = 0);
    ~CBubbleWidget();

public:
    bool Loading();							// 载入
    void OnShow();							// 显示

signals:
    void enterdetail(int idx);


private slots:
    void OnTimer();
    void OnButtonClick(int idx);

public:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void collisionDetection(QPoint hand);

public slots:
    void OnSimulateMouseEvent();
/*
protected:
       void mouseMoveEvent(QMouseEvent *e);
private:
    QLabel *MousePosLabel;

*/
private:
    QVector<QPushButton*> m_btn;			// 气泡按钮
    QVector<QPushButton*> cursorbtn;
    QButtonGroup*         m_group;			// 按钮组
    QButtonGroup*         cursorgroup;     //鼠标手套
    QTimer*               m_timer;			// 计时器
    QVector<int>          m_index;			// 气泡序号
    int                   m_size;			// 气泡大小
    int     m_nWidth;												// 屏幕分辨率，宽度
    int     m_nHeight;// 屏幕分辨率，高度
    int idbubble;
    QBubbles  m_bubbles; // 气泡
    Thread socket;
};

/****************************************************************************/

#endif //

// end of file
