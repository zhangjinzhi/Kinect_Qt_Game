#include <QApplication>
#include "GUI_BubbleWidget.h"

/****************************************************************************/

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);			// 创建主进程
    //
    CBubbleWidget window;					// WIN32主界面
    QObject::connect(&window, SIGNAL(quit()), &app, SLOT(quit()));
    window.Loading();
    window.OnShow();
    //
    return app.exec();

}

/****************************************************************************/

// end of file
