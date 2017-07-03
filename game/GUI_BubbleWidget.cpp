/****************************************************************************
** CBubbleWidget Class Implementation                                       *
*****************************************************************************/
#include "ui_mainwindow.h"
#include "GUI_BubbleWidget.h"
#include <time.h>
#include <QApplication>
#include <QLabel>
#include <QMouseEvent>
#include <QToolTip>
#include <QPropertyAnimation>
#include <QPixmap>
#include <QLabel>

/****************************************************************************/

int xpos;
int ypos;

QPoint leftHand;
QPoint rightHand;
QBubbles::QBubbles()
{

}

void QBubbles::Init(int bubbleNum, int bubbleSize, double bubbleSpeed, QSize srnSize)
{
    mBubbleNum   = bubbleNum;
    mBubbleSize  = bubbleSize;
    mBubbleSpeed = bubbleSpeed;
    mSrnSize     = srnSize;
    // 随机位置
    int	gridY = (int)(sqrt(1.*bubbleNum));
    int gridX = bubbleNum/gridY;
    int stepY = srnSize.height()/gridY;
    int stepX = srnSize.width()/gridX;
    int bubbleHalf = bubbleSize/2;
    int* indics = new int[mBubbleNum];
    gRandomArray(indics, mBubbleNum);
    mBubblePos.resize(mBubbleNum);
    for( int yy=0; yy<gridY; ++yy )
    {
        for( int xx=0; xx<gridX; ++xx )
        {
            int minX = xx*stepX + bubbleHalf;
            int maxX = minX + stepX - 1 - bubbleHalf;
            int minY = yy*stepY + bubbleHalf;
            int maxY = yy*stepY + stepY -1 - bubbleHalf;
            int posX = gRandomGen(minX, maxX) - bubbleHalf;
            int posY = gRandomGen(minY, maxY) - bubbleHalf;
            mBubblePos[indics[yy*gridX+xx]] = QPoint(posX, posY);
        }
    }
    delete [] indics;
    // 随机方向
    mSpeedDirct.clear();
    for( int idx=0; idx<mBubbleNum; ++idx )
    {
        int direction = gRandomGen(0, 360);
        int dx = (int)(cos(direction*3.14159265358/180) *  mBubbleSpeed + 0.5);
        int dy = (int)(sin(direction*3.14159265358/180) *  mBubbleSpeed + 0.5);
        mSpeedDirct.push_back(QPoint(dx, dy));
    }
}

void QBubbles::Update()
{
    int bubbleHalf = mBubbleSize/2;
    for( int idx=0; idx<mBubbleNum; ++idx )
    {
        mBubblePos[idx].setX(mBubblePos[idx].x() + mSpeedDirct[idx].x());
        mBubblePos[idx].setY(mBubblePos[idx].y() + mSpeedDirct[idx].y());
        //与边缘的碰撞检测
        if( mBubblePos[idx].x() <  0 )
        {
            mBubblePos[idx].setX(0);
            mSpeedDirct[idx].setX(-mSpeedDirct[idx].x());
        }
        else if( mBubblePos[idx].x() >  mSrnSize.width() - mBubbleSize )
        {
            mBubblePos[idx].setX(mSrnSize.width() - mBubbleSize);
            mSpeedDirct[idx].setX(-mSpeedDirct[idx].x());
        }
        if( mBubblePos[idx].y() <  0 )
        {
            mBubblePos[idx].setY(0);
            mSpeedDirct[idx].setY(-mSpeedDirct[idx].y());
        }
        else if( mBubblePos[idx].y() >  mSrnSize.height() - mBubbleSize )
        {
            mBubblePos[idx].setY(mSrnSize.height() - mBubbleSize);
            mSpeedDirct[idx].setY(-mSpeedDirct[idx].y());
        }
    }
    // 泡泡之间的碰撞检测
    for( int ii=0; ii<mBubbleNum; ++ii )
    {
        for( int jj=0; jj<mBubbleNum; ++jj )
        {
            if( ii != jj)
            {
                double dis = gDistance(mBubblePos[ii], mBubblePos[jj]);
                if( dis < mBubbleSize )
                {
                    double direct = atan2( 1.*(mBubblePos[ii].y()-mBubblePos[jj].y()), 1.*(mBubblePos[ii].x()-mBubblePos[jj].x()) );
                    int dx = (int)(cos(direct) * mBubbleSpeed + 0.5);
                    int dy = (int)(sin(direct) * mBubbleSpeed + 0.5);
                    mSpeedDirct[ii].setX( dx);
                    mSpeedDirct[ii].setY( dy);
                }
            }
        }
    }
/************************************************************************************************************/

/*************************************************************/
}

/****************************************************************************/

/****************************************************************************/

CBubbleWidget::CBubbleWidget(QWidget* parent) : QWidget(parent)
{
    m_btn.clear();
    m_index.clear();
    m_group = new QButtonGroup(this);
      cursorgroup = new QButtonGroup(this);
    m_timer = new QTimer(this);
    m_size  = 0;
    //
    QDesktopWidget* pDesktop = QApplication::desktop();
    m_nWidth      = pDesktop->screenGeometry().width();	// 得到屏幕宽度
    m_nHeight     = pDesktop->screenGeometry().height();	// 得到屏幕高度
    //
    QObject::connect(m_timer, SIGNAL(timeout()),          this, SLOT(OnTimer()));
    QObject::connect(m_group, SIGNAL(buttonClicked(int)), this, SLOT(OnButtonClick(int)));
    //
    this->setFixedSize(m_nWidth, m_nHeight);
    this->move(0, 0);
}

CBubbleWidget::~CBubbleWidget()
{
    m_timer->stop();
    int i;
    for (i = m_index.size() - 1; i >= 0; i--)
    {
        m_group->removeButton(m_btn[i]);
        delete m_btn[i];
    }
    m_btn.clear();
}

/****************************************************************************/

void CBubbleWidget::OnTimer()
{
    m_bubbles.Update();
    int i;
    for (i = 0; i < m_index.size(); i++)
    {
        m_btn[i]->move(m_bubbles.mBubblePos[i]);
    }
    if(isMsg == true){
        OnSimulateMouseEvent();
        isMsg = false;
    }
}

void CBubbleWidget::mousePressEvent(QMouseEvent *event)
{
    QString coursePosition;
    QString temp_x;
    QString temp_y;
    QPoint coursePoint;

    if(event->button()==Qt::LeftButton)
    {
       coursePoint = QCursor::pos();//获取当前光标的位置
       temp_x.setNum(coursePoint.x());
       temp_y.setNum(coursePoint.y());
       coursePosition.append("Mouse Position\n");
       coursePosition.append(" X- ");
       coursePosition.append(temp_x);
       coursePosition.append(" Y- ");
       coursePosition.append(temp_y);
       QToolTip::showText(QCursor::pos(),coursePosition);//做一个小标签显示鼠标位置
       setCursor(Qt::CrossCursor);//设置鼠标为十字星
       if(10<coursePoint.x()&&coursePoint.x()<1900){
            xpos=coursePoint.x();
       }else{
            xpos = 1900;
       }
       if(10<coursePoint.y()&&coursePoint.y()<1900){
           ypos=coursePoint.y();
       }else{
           ypos = 1900;
       }
    }
}
/*****
void Thread::run(int idx)
{
    emit bubble.enterdetail(idx);
     int newx,newy,k,m;
    for(int i = 0; i < bubble.m_bubbles.mBubbleNum; i++)
{

   if (i==idx){
         srand((unsigned int)time(NULL));
        bubble.m_btn[i]->hide();
        // newx=rand() % 1600;
         //newy=rand( )% 800;
         while(1){
             //srand((unsigned int)time(NULL));
             newx=rand() % 1600;
             newy=rand( )% 800;
  if(
   sqrt(
       pow(newx-m_bubbles.mBubblePos[0].x(),2)+pow(newy-m_bubbles.mBubblePos[0].y(),2)
        )>m_bubbles.mBubbleSize

   &&
          sqrt(
              pow(newx-m_bubbles.mBubblePos[1].x(),2)+pow(newy-m_bubbles.mBubblePos[1].y(),2)
               )>m_bubbles.mBubbleSize
   &&
          sqrt(
              pow(newx-m_bubbles.mBubblePos[2].x(),2)+pow(newy-m_bubbles.mBubblePos[2].y(),2)
               )>m_bubbles.mBubbleSize
    &&
          sqrt(
              pow(newx-m_bubbles.mBubblePos[3].x(),2)+pow(newy-m_bubbles.mBubblePos[3].y(),2)
               )>m_bubbles.mBubbleSize
    &&
          sqrt(
              pow(newx-m_bubbles.mBubblePos[4].x(),2)+pow(newy-m_bubbles.mBubblePos[4].y(),2)
               )>m_bubbles.mBubbleSize

    )
break;
         }

         QPropertyAnimation *animation=new QPropertyAnimation( m_btn[i],"geometry");
         animation->setDuration(3000);
         animation->setStartValue(QRect(0,0,150,150));
         animation->setEndValue(QRect(500,500,0,0));
         animation->start();



         for(k=3000;k>0;k--)
         {
             m=k/10;
             if(k%10==0)
             {
                m_btn[i]->resize( QSize( m, m ));
                // m_bubbles.mSrnSize[i].setHeight(m);

             }
         }


         m_bubbles.mBubblePos[i].setX(newx);
         m_bubbles.mBubblePos[i].setY(newy);
         m_btn[i]->show();
         break;
   }
 }
}
******/

void CBubbleWidget::OnButtonClick(int idx)
//void Thread::run()
{
    emit enterdetail(idx);
     int newx,newy;
    for(int i = 0; i < m_bubbles.mBubbleNum; i++)
{

   if (i==idx){
         srand((unsigned int)time(NULL));
           m_bubbles.mSpeedDirct[i].setX(0);
           m_bubbles.mSpeedDirct[i].setY(0);

         //m_bubbles.mBubbleSpeed=0;
         // m_btn[i]->hide();
         // newx=rand() % 1600;
         //newy=rand( )% 800;
         while(1){
             //srand((unsigned int)time(NULL));
             newx=rand() % 1600;
             newy=rand( )% 800;
  if(
   sqrt(
       pow(newx-m_bubbles.mBubblePos[0].x(),2)+pow(newy-m_bubbles.mBubblePos[0].y(),2)
        )>m_bubbles.mBubbleSize

   &&
          sqrt(
              pow(newx-m_bubbles.mBubblePos[1].x(),2)+pow(newy-m_bubbles.mBubblePos[1].y(),2)
               )>m_bubbles.mBubbleSize
   &&
          sqrt(
              pow(newx-m_bubbles.mBubblePos[2].x(),2)+pow(newy-m_bubbles.mBubblePos[2].y(),2)
               )>m_bubbles.mBubbleSize
    &&
          sqrt(
              pow(newx-m_bubbles.mBubblePos[3].x(),2)+pow(newy-m_bubbles.mBubblePos[3].y(),2)
               )>m_bubbles.mBubbleSize
    &&
          sqrt(
              pow(newx-m_bubbles.mBubblePos[4].x(),2)+pow(newy-m_bubbles.mBubblePos[4].y(),2)
               )>m_bubbles.mBubbleSize

    )
break;
         }

         QPropertyAnimation *animation1=new QPropertyAnimation( m_btn[i],"geometry");
         animation1->setDuration(700);
        // animation1->setStartValue(QRect(m_bubbles.mBubblePos[i].x(),m_bubbles.mBubblePos[i].y(),150,150));
        // animation1->setEndValue(QRect(m_bubbles.mBubblePos[i].x(),m_bubbles.mBubblePos[i].y(),0,0));

         animation1->setKeyValueAt(0,QRect(m_bubbles.mBubblePos[i].x(),m_bubbles.mBubblePos[i].y(),150,150));
         animation1->setKeyValueAt(0.4,QRect(m_bubbles.mBubblePos[i].x(),m_bubbles.mBubblePos[i].y(),0,0));
         animation1->setKeyValueAt(0.8,QRect(m_bubbles.mBubblePos[i].x(),m_bubbles.mBubblePos[i].y(),0,0));
         m_bubbles.mBubblePos[i].setX(newx);
         m_bubbles.mBubblePos[i].setY(newy);
         animation1->setKeyValueAt(0.8,QRect(m_bubbles.mBubblePos[i].x(),m_bubbles.mBubblePos[i].y(),0,0));
         animation1->setKeyValueAt(1, QRect(m_bubbles.mBubblePos[i].x(), m_bubbles.mBubblePos[i].y(), 150, 150));
         animation1->setEndValue(QRect(m_bubbles.mBubblePos[i].x(),m_bubbles.mBubblePos[i].y(),150,150));

         animation1->start();






         int direction = gRandomGen(0, 360);
         int dx = (int)(cos(direction*3.14159265358/180) *  4 + 0.5);
         int dy = (int)(sin(direction*3.14159265358/180) *  4 + 0.5);
          m_bubbles.mSpeedDirct[i].setX(dx);
          m_bubbles.mSpeedDirct[i].setY(dy);
         // m_bubbles.mBubbleSpeed=4;
         break;

   }
 }
}


/****************************************************************************/

bool CBubbleWidget::Loading()
{
    int i;
    for (i = 0; i < 5; i++)
    {
        m_index.append(i);
    }
    //

    socket.serverStart();
    QString str;
    str = QString(":/resource/gui/framewater.jpg");
    QPixmap imgback = QPixmap(str);			// 直接载入图片
    QPixmap image;
    image = imgback.scaled(m_nWidth, m_nHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette pal;
    pal.setBrush(QPalette::Window, QBrush(image));
    this->setPalette(pal);					// 用处理后的图片为背景
    //
    QPushButton* btn;
     QPushButton* curbtn;
    for (i = 0; i < m_index.size(); i++)
    {
        btn = new QPushButton(this);
        str = QString(":/resource/gui/button/bubble1%2.png").arg(m_index[i] + 1);
        //
        QPixmap img = QPixmap(str).scaled(150,150);			// 直接载入图片

        btn->setIcon(img);						// 设置按钮图标
        btn->setIconSize(img.size());
       // btn->setFixedSize(img.size());
        btn->setStyleSheet("QPushButton{border:0px;background-color:rgba(0,0,0,0);}");
        //
        m_btn.push_back(btn);
        m_group->addButton(m_btn[i], i);
        m_size = (btn->width() + btn->height()) / 2;
    }
    for(int i = 0 ; i < 2 ; i++)
      {
        QString string;
        curbtn = new QPushButton(this);
        if(i == 0)
        string = QString(":/resource/gui/cursorLeft.png");
        if(i == 1)
        string = QString(":/resource/gui/cursorRight.png");
        //
        QPixmap img = QPixmap(string).scaled(100,100);	// 直接载入图片
        curbtn->setIcon(img);						// 设置按钮图标
        curbtn->setIconSize(img.size());
        curbtn->setFixedSize(img.size());
        curbtn->setStyleSheet("QPushButton{border:0px;background-color:rgba(0,0,0,0);}");
        cursorbtn.push_back(curbtn);
        cursorgroup->addButton(cursorbtn[i], i);
    }
    m_bubbles.Init(m_index.size(), 150, 4, QSize(m_nWidth, m_nHeight));

     cursorbtn[0]->move(m_nWidth/2-300,m_nHeight/2-100);
     cursorbtn[1]->move(m_nWidth/2+300,m_nHeight/2-100);   //由于设定的泡泡大小是150，所以直接将150作为m_size传入
    for (i = 0; i < m_index.size(); i++)
    {
        m_btn[i]->move(m_bubbles.mBubblePos[i]);
    }

    return true;
}

void CBubbleWidget::OnShow()
{
    this->show();
    m_timer->setInterval(40);				// 每秒25帧
    m_timer->start();
}
void CBubbleWidget::OnSimulateMouseEvent(){
    bool ok;
    QStringList strlist = string.split(",");
    if(strlist.at(0).toInt(&ok,10) == 1){
        leftHand.setX(strlist.at(1).toInt(&ok,10));
        leftHand.setY(strlist.at(2).toInt(&ok,10));
       // cursorbtn[0]->move(strlist.at(1).toInt(&ok,10),strlist.at(2).toInt(&ok,10));
         cursorbtn[0]->move(strlist.at(1).toInt(&ok,10),strlist.at(2).toInt(&ok,10));
    }
    else if(strlist.at(0).toInt(&ok,10) == 2){
        rightHand.setX(strlist.at(3).toInt(&ok,10));
        rightHand.setY(strlist.at(4).toInt(&ok,10));
         cursorbtn[1]->move(strlist.at(3).toInt(&ok,10),strlist.at(4).toInt(&ok,10));
    }
    else if(strlist.at(0).toInt(&ok,10) == 3){
        leftHand.setX(strlist.at(1).toInt(&ok,10));
        leftHand.setY(strlist.at(2).toInt(&ok,10));
        rightHand.setX(strlist.at(3).toInt(&ok,10));
        rightHand.setY(strlist.at(4).toInt(&ok,10));
        cursorbtn[0]->move(strlist.at(1).toInt(&ok,10),strlist.at(2).toInt(&ok,10));
        cursorbtn[1]->move(strlist.at(3).toInt(&ok,10),strlist.at(4).toInt(&ok,10));
    }
    //QCursor::setPos(hand);
//    xpos = hand.x();
//    ypos = hand.y();
//    QMouseEvent *event;
//    event = new QMouseEvent(QEvent::MouseButtonPress, hand, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
//    QApplication::sendEvent(this, event);
//    QMouseEvent *mEvnRelease;
//    mEvnRelease = new QMouseEvent(QEvent::MouseButtonRelease, hand, Qt::LeftButton,Qt::LeftButton, Qt::NoModifier);
//    QApplication::sendEvent(this,mEvnRelease);

    collisionDetection(leftHand);
    collisionDetection(rightHand);
    this->repaint();
}

void CBubbleWidget::paintEvent(QPaintEvent *)
{
  /*
  QPainter painter(this);
    QPen pen;                                 //创建一个画笔
    pen.setColor(Qt::darkCyan);
    pen.setWidth(5);
    painter.setPen(pen);
    painter.drawEllipse(leftHand.x() - 25,leftHand.y() - 25,50,50);
    pen.setColor(Qt::yellow);
    painter.drawEllipse(rightHand.x() - 25,rightHand.y() - 25,50,50);
  */
}

void CBubbleWidget::collisionDetection(QPoint hand){
    for(int i = 0; i < m_bubbles.mBubbleNum; i++){
        if(gDistance(hand,m_bubbles.mBubblePos[i])<m_bubbles.mBubbleSize){
            OnButtonClick(i);
        }
    }
}
/****************************************************************************/

// end of file



