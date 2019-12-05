#include "frmtool.h"
#include "frmmain.h"
#include "frmlogin.h"
#include "quiwidget.h"
#include "appinit.h"
#include "appkey.h"

int main(int argc, char *argv[])
{
#if (QT_VERSION > QT_VERSION_CHECK(5,6,0))
    //QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication a(argc, argv);

    AppInit::Instance()->start();
    AppInit::Instance()->initDb();
    AppInit::Instance()->initStyle();
    AppKey::Instance()->start();

    QWidget *w;

#ifdef tool
    w = new frmTool;
#else
    if (App::AutoLogin) {
        w = new frmMain;
    } else {
        w = new frmLogin;
    }
#endif

    w->show();
    return a.exec();
}
