#include "appinit.h"
#include "quiwidget.h"
#include "dbhelper.h"
#include "dblocalthread.h"
#include "dbcleanthread.h"
#include "dbtcpclientthread.h"

QScopedPointer<AppInit> AppInit::self;
AppInit *AppInit::Instance()
{
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new AppInit);
        }
    }

    return self.data();
}

AppInit::AppInit(QObject *parent) : QObject(parent)
{
}

bool AppInit::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *w = (QWidget *)watched;
    if (!w->property("canMove").toBool()) {
        return QObject::eventFilter(watched, event);
    }

    static QPoint mousePoint;
    static bool mousePressed = false;

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (event->type() == QEvent::MouseButtonPress) {
        if (mouseEvent->button() == Qt::LeftButton) {
            mousePressed = true;
            mousePoint = mouseEvent->globalPos() - w->pos();
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        mousePressed = false;
    } else if (event->type() == QEvent::MouseMove) {
        if (mousePressed && (mouseEvent->buttons() && Qt::LeftButton)) {
            w->move(mouseEvent->globalPos() - mousePoint);
            return true;
        }
    }

    return QObject::eventFilter(watched, event);
}

void AppInit::start()
{
    qApp->installEventFilter(this);

    //设置全局字体
    QUIConfig::IconMain = 0xf108;
    QUIConfig::FontSize = 12;
    QFont font;
    font.setFamily(QUIConfig::FontName);
    font.setPixelSize(QUIConfig::FontSize);
    qApp->setFont(font);

    //设置编码
    QUIHelper::setCode();
    //设置翻译文件
    QUIHelper::setTranslator(":/image/qt_zh_CN.qm");
    QUIHelper::setTranslator(":/image/widgets.qm");
    QUIHelper::setTranslator(":/image/designer_zh_CN.qm");
    //初始化随机数种子
    QUIHelper::initRand();

    //新建不存在的目录
    QUIHelper::newDir("config");
    QUIHelper::newDir("log");
    QUIHelper::newDir("db");
    QUIHelper::newDir("map");

    //载入配置文件
    App::ConfigFile = QString("%1/config/%2.ini").arg(QUIHelper::appPath()).arg("sams");
    App::readConfig();

 #if 0
    //计算地图宽高
    App::MapWidth = QUIHelper::deskWidth() - App::LeftWidth - App::RightWidth - 10;
    App::MapHeight = QUIHelper::deskHeight() - App::TopHeight - App::BottomHeight - 8;

    //载入地图文件名称集合
    DBData::MapPath = QUIHelper::appPath() + "/map";
    QStringList filter;
    filter << "*.jpg" << "*.bmp" << "*.png";
    QDir mapPath(DBData::MapPath);
    DBData::MapNames.append(mapPath.entryList(filter));
    DeviceMap::Instance()->loadMap();
#endif

    //载入声音文件名称集合
    DBData::SoundPath = QUIHelper::appPath() + "/sound";
    QStringList filter;
    filter.clear();
    filter << "*.wav" << "*.mp3" << "*.mdi";
    QDir soundPath(DBData::SoundPath);
    DBData::SoundNames.append(soundPath.entryList(filter));

    //载入串口号
    for (int i = 1; i < 20; i++) {
        DBData::PortNames << QString("COM%1").arg(i);
    }

    DBData::PortNames << "ttyUSB0" << "ttyS0" << "ttyS1" << "ttyS2" << "ttyS3" << "ttyS4" << "ttyS5";

    //设置开机自动运行
    QString strPath = QApplication::applicationFilePath();
    strPath = QDir::toNativeSeparators(strPath);
    QUIHelper::runWithSystem(QUIHelper::appName(), strPath, App::AutoRun);
}

void AppInit::initDb()
{
    //根据不同的数据库类型打开数据库
    QString dbName = QString("%1/db/%2.db").arg(QUIHelper::appPath()).arg("sams");
    if (App::LocalDBType.toUpper() == "SQLITE") {
        //判断数据库文件是否存在,不存在则从资源文件复制出来
        QUIHelper::initDb(dbName);
        DbLocalThread::Instance()->setConnInfo(DbLocalThread::DbType_Sqlite, "qt_sql_default_connection", App::LocalDBIP, App::LocalDBPort, dbName, App::LocalUserName, App::LocalUserPwd);
    } else if (App::LocalDBType.toUpper() == "MYSQL") {
        //先判断数据库服务器IP地址是否存在,否则会卡很久
        if (!QUIHelper::ipLive(App::LocalDBIP, App::LocalDBPort)) {
            QUIHelper::showMessageBoxError("本地数据库服务器不存在!", 5);
            App::DbError = true;
        } else {
            DbLocalThread::Instance()->setConnInfo(DbLocalThread::DbType_MySql, "qt_sql_default_connection", App::LocalDBIP, App::LocalDBPort, App::LocalDBName, App::LocalUserName, App::LocalUserPwd);
        }
    }

    //数据库错误不用再继续
    if (App::DbError) {
        return;
    }

    //程序启动后打开数据库连接,在任何窗体需要的地方调用数据库
    if (!DbLocalThread::Instance()->openDb()) {
        QUIHelper::showMessageBoxError("数据库打开出错,请检查设置!", 5);
        App::DbError = true;
    } else {
        //启动线程实现本地数据库自动重连
        //2019-4-7 去掉线程执行,已改成设置自动重连参数
        //DbLocalThread::Instance()->start();
    }

    //提前载入数据库数据
    DBHelper::loadUserInfo();
    DBHelper::loadPortInfo();
#if 0
    DBHelper::loadDeviceInfo();
    DBHelper::loadNodeInfo();
    DBHelper::loadAlarmLink();
    DBHelper::loadTypeInfo();
#endif

    //启动线程自动清除早期数据
#if 0
    cleanAlarmLog = new DbCleanThread(this);
    cleanAlarmLog->setObjectName("cleanAlarmLog");
    cleanAlarmLog->setPar("AlarmLog", "LogID", "LogID", "LogID asc", App::AlarmLogMaxCount * 10000, 1 * 60);

    cleanNodeLog = new DbCleanThread(this);
    cleanNodeLog->setObjectName("cleanNodeLog");
    cleanNodeLog->setPar("NodeLog", "LogID", "LogID", "LogID asc", App::NodeLogMaxCount * 10000, 2 * 60);
#endif

    cleanUserLog = new DbCleanThread(this);
    cleanUserLog->setObjectName("cleanUserLog");
    cleanUserLog->setPar("UserLog", "LogID", "LogID", "LogID asc", App::UserLogMaxCount * 10000, 3 * 60);

    if (App::LocalDBType.toUpper() == "SQLITE") {
        //cleanAlarmLog->setConnInfo(DbCleanThread::DbType_Sqlite, "cleanAlarmLog", App::LocalDBIP, App::LocalDBPort, dbName, App::LocalUserName, App::LocalUserPwd);
        //cleanNodeLog->setConnInfo(DbCleanThread::DbType_Sqlite, "cleanNodeLog", App::LocalDBIP, App::LocalDBPort, dbName, App::LocalUserName, App::LocalUserPwd);
        cleanUserLog->setConnInfo(DbCleanThread::DbType_Sqlite, "cleanUserLog", App::LocalDBIP, App::LocalDBPort, dbName, App::LocalUserName, App::LocalUserPwd);
    } else if (App::LocalDBType.toUpper() == "MYSQL") {
        //cleanAlarmLog->setConnInfo(DbCleanThread::DbType_MySql, "cleanAlarmLog", App::LocalDBIP, App::LocalDBPort, App::LocalDBName, App::LocalUserName, App::LocalUserPwd);
        //cleanNodeLog->setConnInfo(DbCleanThread::DbType_MySql, "cleanNodeLog", App::LocalDBIP, App::LocalDBPort, App::LocalDBName, App::LocalUserName, App::LocalUserPwd);
        cleanUserLog->setConnInfo(DbCleanThread::DbType_MySql, "cleanUserLog", App::LocalDBIP, App::LocalDBPort, App::LocalDBName, App::LocalUserName, App::LocalUserPwd);
    }

    //统一启动线程
    //cleanAlarmLog->start();
    //cleanNodeLog->start();
    cleanUserLog->start();

    //如果启用了云端远程数据库同步,目前就mysql
    if (App::UseNetDB) {
        //先判断数据库服务器IP地址是否存在,否则会卡很久
        if (!QUIHelper::ipLive(App::NetDBIP, App::NetDBPort)) {
            QUIHelper::showMessageBoxError("云端数据库服务器不存在!", 5);
            App::DbError = true;
        } else {
            DbTcpClientThread::Instance()->setConnInfo(DbTcpClientThread::DbType_MySql, "qt_sql_tcp_connection", App::NetDBIP, App::NetDBPort, App::NetDBName, App::NetUserName, App::NetUserPwd);
        }

        if (!DbTcpClientThread::Instance()->openDb()) {
            QUIHelper::showMessageBoxError("数据库打开出错,请检查设置!", 5);
            App::DbError = true;
        } else {
            //根据本地数据库的信息生成初始化远程数据库临时表的数据
            //填充默认设备的初始数据
            QStringList initSql;
            //首先插入删除表语句
            initSql << "delete from NodeData";
            //再插入新建语句
            for (int i = 0; i < DBData::NodeInfo_Count; i++) {
                QString positionID = DBData::NodeInfo_PositionID.at(i);
                QString sql = QString("insert into NodeData(PositionID,NodeValue,NodeStatus,SaveTime) values('%1','0',0,'%2')").arg(positionID).arg(DATETIME);
                initSql << sql;
            }

            DbTcpClientThread::Instance()->setInitSql(initSql);
            DbTcpClientThread::Instance()->initDb();
            DbTcpClientThread::Instance()->start();
        }
    }
}

void AppInit::initStyle()
{
    QFile file(App::StyleName);
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        file.close();

        QUIHelper::getQssColor(qss, QUIConfig::TextColor, QUIConfig::PanelColor, QUIConfig::BorderColor, QUIConfig::NormalColorStart,
                               QUIConfig::NormalColorEnd, QUIConfig::DarkColorStart, QUIConfig::DarkColorEnd, QUIConfig::HighColor);

        //追加样式
        QStringList list;

        //主背景
        list.append(QString("QWidget#widgetMain{background:%1;}").arg(QUIConfig::BorderColor));

        //中英文标题字体大小
        list.append(QString("#labLogoCn{font-size:%1px;}#labLogoEn{font-size:%2px;}")
                    .arg(QUIConfig::FontSize + 13).arg(QUIConfig::FontSize + 4));

        //右上角菜单
        list.append(QString("QWidget#widgetMenu>QPushButton{border-radius:0px;padding:0px;margin:1px 1px 2px 1px;}"));

        //视频画面
        bool black = (App::StyleName.contains("black") || App::StyleName.contains("darkblue") || App::StyleName.contains("purple"));
        App::LogoBg = QString("config/logo_%1.png").arg(black ? "white" : "black");

        list.append(QString("QWidget#widgetVideo>QLabel{font-size:%1px;border:2px solid %2;background:%3;}")
                    .arg(QUIConfig::FontSize + 10).arg(QUIConfig::BorderColor).arg(QUIConfig::NormalColorStart));
        list.append(QString("QWidget#widgetVideo>QLabel:focus{border:2px solid %1;background:%2;}")
                    .arg(QUIConfig::HighColor).arg(black ? QUIConfig::NormalColorEnd : QUIConfig::DarkColorStart));

        //画面分割栏
        list.append(QString("QWidget#widgetSplite>QPushButton{background:transparent;border:none;}"));

        //顶部导航按钮,可以自行修改圆角角度
        int topBtnRadius = 20;
        list.append(QString("QWidget#widgetBtn>QPushButton{font-size:%1px;background:transparent;border:none;border-radius:%2px;}")
                    .arg(QUIConfig::FontSize + 3).arg(topBtnRadius));
        list.append(QString("QWidget#widgetBtn>QPushButton:checked,QWidget#widgetBtn>QPushButton:hover{background:%1;border:2px solid %2;}")
                    .arg(QUIConfig::DarkColorEnd).arg(QUIConfig::BorderColor));

        //左侧导航按钮,可以自行修改圆角角度
        int leftBtnRadius = 5;
        list.append(QString("QWidget[flag=\"btn\"]>QPushButton{min-height:%1px;max-height:%1px;}").arg(30));
        list.append(QString("QWidget[flag=\"btn\"]>QPushButton{font-size:%1px;background:transparent;border:none;border-radius:%2px;}")
                    .arg(QUIConfig::FontSize + 3).arg(leftBtnRadius));
        list.append(QString("QWidget[flag=\"btn\"]>QPushButton:checked,QWidget[flag=\"btn\"]>QPushButton:hover{background:%1;border:2px solid %2;}")
                    .arg(QUIConfig::DarkColorEnd).arg(QUIConfig::BorderColor));

        //面板框
        list.append(QString("QFrame[flag=\"btn\"]{background:%1;}").arg(QUIConfig::BorderColor));
        list.append(QString("QFrame[flag=\"btn\"]>QPushButton{font-size:%1px;font-weight:bold;}").arg(QUIConfig::FontSize + 3));

        //翻页导航标签
        list.append(QString("QFrame[flag=\"navlab\"] QLabel{min-height:%1px;max-height:%1px;font:%2px;}").arg(30).arg(QUIConfig::FontSize + 3));

        //字体放大
        list.append(QString("#widgetControl>QLabel{font-size:%1px;}").arg(QUIConfig::FontSize + 12));
        list.append(QString("#widgetControl>QCheckBox::indicator{width:%1px;height:%1px;}").arg(30));

        //左侧导航栏及按钮图标大小
        list.append(QString("#labTip{background-color:%1;}").arg(QUIConfig::NormalColorEnd));
        list.append(QString("QLineEdit#txtCard,QLabel#labTip,QLabel#labCard,QLabel#labTime{font-weight:bold;font-size:%1px;min-height:%2px;qproperty-alignment:AlignCenter;}").arg(QUIConfig::FontSize + 2).arg(28));
        list.append(QString("QWidget[flag=\"navbtn\"] QPushButton,QFrame[flag=\"navbtn\"] QPushButton{qproperty-iconSize:%1px;min-width:%2px;}").arg(App::BtnIconSize).arg(App::BtnMinWidth));
        //list.append(QString("QLCDNumber{qproperty-segmentStyle:Flat;background-color:rgb(0,0,0,0);color:%1;}").arg(QUIConfig::TextColor));

        //选项卡
        list.append(QString("QTabWidget::pane{border:0px;}QTabBar::tab{min-width:%1px;min-height:%2px;}").arg(52).arg(20));

        //控件无边框
        list.append(QString("#widgetRight>QListView,#widgetRight>QTreeView,#widgetLeft>QTableWidget{border-width:0px;}"));
        list.append(QString("#treeWidget{padding:5px 0px 0px 0px;}"));
        list.append(QString("#frameXml{border-radius:0px;}"));

        //表格选中背景
        list.append(QString("QTableView::item:hover,QTableView::item:selected,QHeaderView::section,QTableCornerButton:section{background:%1;}").arg(QUIConfig::NormalColorStart));
        //表格奇数偶数行不同颜色
        list.append(QString("QTableView,QTreeView{qproperty-alternatingRowColors:false;}"));

        //元素间隔
        //list.append(QString("QTableWidget[flag=\"table\"]::item{margin:2px;color:%1;background:%2;}").arg(QUIConfig::TextColor).arg(QUIConfig::DarkColorStart));

        //文本框控件
        //list.append(QString("QTextEdit,QTextEdit:hover,QTextEdit:focus{border-width:0px;border-radius:0px;}"));

        //自定义面板标题控件
        list.append(QString("NavTitle{qproperty-bgColor:%1;qproperty-textColor:%2;qproperty-borderColor:%3;"
                            "qproperty-iconNormalColor:%4;qproperty-iconHoverColor:%5;qproperty-iconPressColor:%6;min-height:%7px;}")
                    .arg(QUIConfig::NormalColorStart).arg(QUIConfig::TextColor).arg(QUIConfig::BorderColor)
                    .arg(QUIConfig::TextColor).arg(QUIConfig::HighColor).arg(QUIConfig::BorderColor).arg(30));

        //开关按钮
        //bool black = (App::StyleName.contains("black") || App::StyleName.contains("darkblue") || App::StyleName.contains("purple"));
        if (App::StyleName.contains("darkblue") || App::StyleName.contains("darkblack") || App::StyleName.contains("purple") || App::StyleName.contains("blackblue") || App::StyleName.contains("blackvideo")) {
            list.append(QString("SwitchButton{min-width:%1px;max-width:%1px;min-height:%2px;max-height:%2px;"
                                "qproperty-bgColorOn:%3;qproperty-bgColorOff:%4;"
                                "qproperty-textColorOn:%5;qproperty-textColorOff:%5;"
                                "qproperty-sliderColorOn:%6;qproperty-sliderColorOff:%6;}")
                        .arg(App::SwitchBtnWidth).arg(App::SwitchBtnHeight)
                        .arg(black ? QUIConfig::DarkColorEnd : QUIConfig::NormalColorEnd)
                        .arg(black ? QUIConfig::NormalColorEnd : QUIConfig::DarkColorEnd)
                        .arg(QUIConfig::TextColor).arg(QUIConfig::PanelColor));
        } else {
            list.append(QString("SwitchButton{min-width:%1px;max-width:%1px;min-height:%2px;max-height:%2px;"
                                "qproperty-bgColorOn:%3;qproperty-bgColorOff:%4;"
                                "qproperty-textColorOn:%5;qproperty-textColorOff:%5;"
                                "qproperty-sliderColorOn:%6;qproperty-sliderColorOff:%6;}")
                        .arg(App::SwitchBtnWidth).arg(App::SwitchBtnHeight)
                        .arg(QColor(21, 156, 119).name()).arg(QColor(111, 122, 126).name())
                        .arg(QColor(250, 250, 250).name()).arg(QColor(255, 255, 255).name()));
        }

        //仪表盘
        list.append(QString("GaugeSpeed{qproperty-textColor:%1;}").arg(QUIConfig::TextColor));
        list.append(QString("#labNodeInfo{background-color:%1;}").arg(QUIConfig::DarkColorEnd));

        //将新增的样式加到统一样式表中
        qss += list.join("");

        qApp->setPalette(QPalette(QUIConfig::PanelColor));
        qApp->setStyleSheet(qss);
    }
}

void AppInit::changeDbMaxCount()
{
    cleanAlarmLog->setMaxCount(App::AlarmLogMaxCount * 10000);
    cleanNodeLog->setMaxCount(App::NodeLogMaxCount * 10000);
    cleanUserLog->setMaxCount(App::UserLogMaxCount * 10000);
}
