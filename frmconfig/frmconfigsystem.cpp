#include "frmconfigsystem.h"
#include "ui_frmconfigsystem.h"
#include "quiwidget.h"
#include "dbhelper.h"
#include "appinit.h"
#include "sendserver.h"
#include "soundapi.h"
#include "excelapi.h"
#include "api.h"
#include "savelog.h"

frmConfigSystem::frmConfigSystem(QWidget *parent) : QWidget(parent), ui(new Ui::frmConfigSystem)
{
    ui->setupUi(this);
    this->initForm();
    this->initConfig();
    this->initLog();
    this->initColor();
    this->initTime();
    this->initLocalDB();
}

frmConfigSystem::~frmConfigSystem()
{
    delete ui;
}

void frmConfigSystem::initForm()
{
    int fixWidth = 440;
    QList<QGroupBox *> gboxs = this->findChildren<QGroupBox *>();
    foreach (QGroupBox *gbox, gboxs) {
        gbox->setFixedWidth(fixWidth);
    }

    //禁用工作模式
    if (App::WorkMode > 0) {
        //ui->cboxWorkMode->setEnabled(false);
    }
}

void frmConfigSystem::initConfig()
{
    ui->txtLogoCn->setText(App::LogoCn);
    ui->txtLogoCn->setSelection(0, 0);
    connect(ui->txtLogoCn, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtLogoEn->setText(App::LogoEn);
    ui->txtLogoEn->setSelection(0, 0);
    connect(ui->txtLogoEn, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtAuthor->setText(App::Author);
    ui->txtAuthor->setSelection(0, 0);
    connect(ui->txtAuthor, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->cboxWorkMode->addItem("从设备采集");
    ui->cboxWorkMode->addItem("从数据库采集");
    ui->cboxWorkMode->addItem("从数据库读取");
    ui->cboxWorkMode->setCurrentIndex(App::WorkMode);
    connect(ui->cboxWorkMode, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));

    QStringList playCount;
    playCount << "1次" << "2次" << "3次" << "5次" << "10次" << "15次" << "20次" << "30次" << "40次" << "50次" << "80次" << "100次";
    ui->cboxPlayCount->addItems(playCount);
    QString strPlayCount = QString("%1次").arg(App::PlayCount);
    ui->cboxPlayCount->setCurrentIndex(ui->cboxPlayCount->findText(strPlayCount));
    connect(ui->cboxPlayCount, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));
    SoundAPI::Instance()->setPlayCount(App::PlayCount);

    ui->btnPlayAlarmSound->setChecked(App::PlayAlarmSound);
    connect(ui->btnPlayAlarmSound, SIGNAL(checkedChanged(bool)), this, SLOT(saveConfig()));
    SoundAPI::Instance()->setPlayAlarmSound(App::PlayAlarmSound);

    ui->btnSaveLog->setChecked(App::SaveLog);
    connect(ui->btnSaveLog, SIGNAL(checkedChanged(bool)), this, SLOT(saveConfig()));
    if (App::SaveLog) {
        SaveLog::Instance()->setPath(QUIHelper::appPath() + "/log");
        SaveLog::Instance()->setToNet(false);
        SaveLog::Instance()->start();
    }

    ui->btnAutoRun->setChecked(App::AutoRun);
    connect(ui->btnAutoRun, SIGNAL(checkedChanged(bool)), this, SLOT(saveConfig()));

    ui->btnAutoLogin->setChecked(App::AutoLogin);
    connect(ui->btnAutoLogin, SIGNAL(checkedChanged(bool)), this, SLOT(saveConfig()));

    ui->btnAutoPwd->setChecked(App::AutoPwd);
    connect(ui->btnAutoPwd, SIGNAL(checkedChanged(bool)), this, SLOT(saveConfig()));

    ui->btnAutoConfirm->setChecked(App::AutoConfirm);
    connect(ui->btnAutoConfirm, SIGNAL(checkedChanged(bool)), this, SLOT(saveConfig()));

    QStringList rowCount;
    for (int i = 12; i <= 40; i++) {
        rowCount << QString("%1行").arg(i);
    }
    rowCount << "45行" << "50行" << "55行" << "60行" << "70行" << "80行" << "90行" << "100行";

    ui->cboxMsgCount->addItems(rowCount);
    QString strMsgCount = QString("%1行").arg(App::MsgCount);
    ui->cboxMsgCount->setCurrentIndex(ui->cboxMsgCount->findText(strMsgCount));
    connect(ui->cboxMsgCount, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));

    ui->cboxPageCount->addItems(rowCount);
    QString strPageCount = QString("%1行").arg(App::PageCount);
    ui->cboxPageCount->setCurrentIndex(ui->cboxPageCount->findText(strPageCount));
    connect(ui->cboxPageCount, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));

    QStringList deviceColumn;
    for (int i = 2; i <= 20; i++) {
        deviceColumn << QString("%1列").arg(i);
    }

    ui->cboxDeviceColumn->addItems(deviceColumn);
    QString strDeviceColumn = QString("%1列").arg(App::DeviceColumn);
    ui->cboxDeviceColumn->setCurrentIndex(ui->cboxDeviceColumn->findText(strDeviceColumn));
    connect(ui->cboxDeviceColumn, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));

    QStringList styleDatas;
    styleDatas << "银色" << "蓝色" << "浅蓝色" << "深蓝色" << "灰色" << "浅灰色"
               << "深灰色" << "黑色" << "浅黑色" << "深黑色" << "PS黑色" << "黑色扁平"
               << "白色扁平" << "蓝色扁平" << "紫色" << "黑蓝色" << "视频黑";

    QStringList styleNames;
    styleNames << ":/qss/silvery.css" << ":/qss/blue.css" << ":/qss/lightblue.css" << ":/qss/darkblue.css" << ":/qss/gray.css" << ":/qss/lightgray.css"
               << ":/qss/darkgray.css" << ":/qss/black.css" << ":/qss/lightblack.css" << ":/qss/darkblack.css" << ":/qss/psblack.css" << ":/qss/flatblack.css"
               << ":/qss/flatwhite.css" << ":/qss/flatblue.css" << ":/qss/purple.css" << ":/qss/blackblue.css" << ":/qss/blackvideo.css";

    for (int i = 0; i < styleNames.count(); i++) {
        ui->cboxStyleName->addItem(styleDatas.at(i), styleNames.at(i));
    }

    ui->cboxStyleName->setCurrentIndex(ui->cboxStyleName->findData(App::StyleName));
    connect(ui->cboxStyleName, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));
}

void frmConfigSystem::saveConfig()
{
    QString logoCn = ui->txtLogoCn->text();
    if (App::LogoCn != logoCn) {
        App::LogoCn = logoCn;
        AppEvent::Instance()->slot_changeText();
    }

    QString logoEn = ui->txtLogoEn->text();
    if (App::LogoEn != logoEn) {
        App::LogoEn = logoEn;
        AppEvent::Instance()->slot_changeText();
    }

    QString author = ui->txtAuthor->text();
    if (App::Author != author) {
        App::Author = author;
        AppEvent::Instance()->slot_changeText();
    }

    int workMode = ui->cboxWorkMode->currentIndex();
    if (App::WorkMode != workMode) {
        App::WorkMode = workMode;
        App::writeConfig();
        API::reboot();
    }

    bool playAlarmSound = ui->btnPlayAlarmSound->getChecked();
    if (App::PlayAlarmSound != playAlarmSound) {
        App::PlayAlarmSound = playAlarmSound;
        SoundAPI::Instance()->stop();
        SoundAPI::Instance()->setPlayAlarmSound(App::PlayAlarmSound);
    }

    QString strPlayCount = ui->cboxPlayCount->currentText();
    int playCount = strPlayCount.mid(0, strPlayCount.length() - 1).toInt();
    if (App::PlayCount != playCount) {
        App::PlayCount = playCount;
        SoundAPI::Instance()->setPlayCount(App::PlayCount);
    }

    bool saveLog = ui->btnSaveLog->getChecked();
    if (App::SaveLog != saveLog) {
        App::SaveLog = saveLog;
        App::SaveLog ? SaveLog::Instance()->start() : SaveLog::Instance()->stop();
    }

    App::AutoRun = ui->btnAutoRun->getChecked();
    App::AutoLogin = ui->btnAutoLogin->getChecked();
    App::AutoPwd = ui->btnAutoPwd->getChecked();
    App::AutoConfirm = ui->btnAutoConfirm->getChecked();

    QString strMsgCount = ui->cboxMsgCount->currentText();
    App::MsgCount = strMsgCount.mid(0, strMsgCount.length() - 1).toInt();

    QString strPageCount = ui->cboxPageCount->currentText();
    App::PageCount = strPageCount.mid(0, strPageCount.length() - 1).toInt();

    QString strDeviceColumn = ui->cboxDeviceColumn->currentText();
    App::DeviceColumn = strDeviceColumn.mid(0, strDeviceColumn.length() - 1).toInt();

    QString styleName = ui->cboxStyleName->itemData(ui->cboxStyleName->currentIndex()).toString();
    if (App::StyleName != styleName) {
        App::StyleName = styleName;
        AppInit::Instance()->initStyle();
        AppEvent::Instance()->slot_changeStyle();
    }

    App::writeConfig();
}

void frmConfigSystem::initLog()
{
    QStringList logMaxCount;
    logMaxCount << "1万条" << "5万条" << "10万条" << "50万条" << "100万条" << "500万条" << "1000万条";
    ui->cboxAlarmLogMaxCount->addItems(logMaxCount);
    ui->cboxNodeLogMaxCount->addItems(logMaxCount);
    ui->cboxUserLogMaxCount->addItems(logMaxCount);

    QString strAlarmLogMaxCount = QString("%1万条").arg(App::AlarmLogMaxCount);
    ui->cboxAlarmLogMaxCount->setCurrentIndex(ui->cboxAlarmLogMaxCount->findText(strAlarmLogMaxCount));
    connect(ui->cboxAlarmLogMaxCount, SIGNAL(currentIndexChanged(int)), this, SLOT(saveLog()));

    QString strNodeLogMaxCount = QString("%1万条").arg(App::NodeLogMaxCount);
    ui->cboxNodeLogMaxCount->setCurrentIndex(ui->cboxNodeLogMaxCount->findText(strNodeLogMaxCount));
    connect(ui->cboxNodeLogMaxCount, SIGNAL(currentIndexChanged(int)), this, SLOT(saveLog()));

    QString strUserLogMaxCount = QString("%1万条").arg(App::UserLogMaxCount);
    ui->cboxUserLogMaxCount->setCurrentIndex(ui->cboxUserLogMaxCount->findText(strUserLogMaxCount));
    connect(ui->cboxUserLogMaxCount, SIGNAL(currentIndexChanged(int)), this, SLOT(saveLog()));

    ui->cboxAlarmLogOrder->addItem("升序", "asc");
    ui->cboxAlarmLogOrder->addItem("降序", "desc");
    ui->cboxAlarmLogOrder->setCurrentIndex(App::AlarmLogOrder == "asc" ? 0 : 1);
    connect(ui->cboxAlarmLogOrder, SIGNAL(currentIndexChanged(int)), this, SLOT(saveLog()));

    ui->cboxNodeLogOrder->addItem("升序", "asc");
    ui->cboxNodeLogOrder->addItem("降序", "desc");
    ui->cboxNodeLogOrder->setCurrentIndex(App::NodeLogOrder == "asc" ? 0 : 1);
    connect(ui->cboxNodeLogOrder, SIGNAL(currentIndexChanged(int)), this, SLOT(saveLog()));

    ui->cboxUserLogOrder->addItem("升序", "asc");
    ui->cboxUserLogOrder->addItem("降序", "desc");
    ui->cboxUserLogOrder->setCurrentIndex(App::UserLogOrder == "asc" ? 0 : 1);
    connect(ui->cboxUserLogOrder, SIGNAL(currentIndexChanged(int)), this, SLOT(saveLog()));
}

void frmConfigSystem::saveLog()
{
    QString strAlarmLogMaxCount = ui->cboxAlarmLogMaxCount->currentText();
    int alarmLogMaxCount = strAlarmLogMaxCount.mid(0, strAlarmLogMaxCount.length() - 2).toInt();
    if (App::AlarmLogMaxCount != alarmLogMaxCount) {
        App::AlarmLogMaxCount = alarmLogMaxCount;
        AppInit::Instance()->changeDbMaxCount();
    }

    QString strNodeLogMaxCount = ui->cboxNodeLogMaxCount->currentText();
    int nodeLogMaxCount = strNodeLogMaxCount.mid(0, strNodeLogMaxCount.length() - 2).toInt();
    if (App::NodeLogMaxCount != nodeLogMaxCount) {
        App::NodeLogMaxCount = nodeLogMaxCount;
        AppInit::Instance()->changeDbMaxCount();
    }

    QString strUserLogMaxCount = ui->cboxUserLogMaxCount->currentText();
    int userLogMaxCount = strUserLogMaxCount.mid(0, strUserLogMaxCount.length() - 2).toInt();
    if (App::UserLogMaxCount != userLogMaxCount) {
        App::UserLogMaxCount = userLogMaxCount;
        AppInit::Instance()->changeDbMaxCount();
    }

    App::AlarmLogOrder = ui->cboxAlarmLogOrder->itemData(ui->cboxAlarmLogOrder->currentIndex()).toString();
    App::NodeLogOrder = ui->cboxNodeLogOrder->itemData(ui->cboxNodeLogOrder->currentIndex()).toString();
    App::UserLogOrder = ui->cboxUserLogOrder->itemData(ui->cboxUserLogOrder->currentIndex()).toString();

    App::writeConfig();
}

void frmConfigSystem::initColor()
{
    ui->cboxColorZero->setColorName(App::ColorZero);
    connect(ui->cboxColorZero, SIGNAL(colorChanged(QString)), this, SLOT(saveColor()));

    ui->cboxColorUpper->setColorName(App::ColorUpper);
    connect(ui->cboxColorUpper, SIGNAL(colorChanged(QString)), this, SLOT(saveColor()));

    ui->cboxColorLimit->setColorName(App::ColorLimit);
    connect(ui->cboxColorLimit, SIGNAL(colorChanged(QString)), this, SLOT(saveColor()));

    ui->cboxColorNormal->setColorName(App::ColorNormal);
    connect(ui->cboxColorNormal, SIGNAL(colorChanged(QString)), this, SLOT(saveColor()));

    ui->cboxColorPlotBg->setColorName(App::ColorPlotBg);
    connect(ui->cboxColorPlotBg, SIGNAL(colorChanged(QString)), this, SLOT(saveColor()));

    ui->cboxColorPlotText->setColorName(App::ColorPlotText);
    connect(ui->cboxColorPlotText, SIGNAL(colorChanged(QString)), this, SLOT(saveColor()));

    ui->cboxColorPlotLine->setColorName(App::ColorPlotLine);
    connect(ui->cboxColorPlotLine, SIGNAL(colorChanged(QString)), this, SLOT(saveColor()));

    ui->cboxColorOther->setColorName(App::ColorOther);
    connect(ui->cboxColorOther, SIGNAL(colorChanged(QString)), this, SLOT(saveColor()));
}

void frmConfigSystem::saveColor()
{
    App::ColorZero = ui->cboxColorZero->getColorName();
    App::ColorUpper = ui->cboxColorUpper->getColorName();
    App::ColorLimit = ui->cboxColorLimit->getColorName();
    App::ColorNormal = ui->cboxColorNormal->getColorName();
    App::ColorPlotBg = ui->cboxColorPlotBg->getColorName();
    App::ColorPlotText = ui->cboxColorPlotText->getColorName();
    App::ColorPlotLine = ui->cboxColorPlotLine->getColorName();
    App::ColorOther = ui->cboxColorOther->getColorName();
    App::writeConfig();
}

void frmConfigSystem::initTime()
{
    for (int i = 2018; i <= 2030; i++) {
        ui->cboxYear->addItem(QString::number(i));
    }

    for (int i = 1; i <= 12; i++) {
        ui->cboxMonth->addItem(QString::number(i));
    }

    for (int i = 1; i <= 31; i++) {
        ui->cboxDay->addItem(QString::number(i));
    }

    for (int i = 0; i < 24; i++) {
        ui->cboxHour->addItem(QString::number(i));
    }

    for (int i = 0; i < 60; i++) {
        ui->cboxMin->addItem(QString::number(i));
        ui->cboxSec->addItem(QString::number(i));
    }

    QString now = QDateTime::currentDateTime().toString("yyyy-M-d-H-m-s");
    QStringList str = now.split("-");

    int indexYear = ui->cboxYear->findText(str.at(0));
    ui->cboxYear->setCurrentIndex(indexYear < 0 ? 0 : indexYear);
    ui->cboxMonth->setCurrentIndex(ui->cboxMonth->findText(str.at(1)));
    ui->cboxDay->setCurrentIndex(ui->cboxDay->findText(str.at(2)));
    ui->cboxHour->setCurrentIndex(ui->cboxHour->findText(str.at(3)));
    ui->cboxMin->setCurrentIndex(ui->cboxMin->findText(str.at(4)));
    ui->cboxSec->setCurrentIndex(ui->cboxSec->findText(str.at(5)));

    connect(ui->btnSetTime, SIGNAL(clicked(bool)), this, SLOT(saveTime()));
}

void frmConfigSystem::saveTime()
{
    QString year = QString("%1").arg(ui->cboxYear->currentText());
    QString month = QString("%1").arg(ui->cboxMonth->currentText().toInt(), 2, 10, QChar('0'));
    QString day = QString("%1").arg(ui->cboxDay->currentText().toInt(), 2, 10, QChar('0'));
    QString hour = QString("%1").arg(ui->cboxHour->currentText().toInt(), 2, 10, QChar('0'));
    QString min = QString("%1").arg(ui->cboxMin->currentText().toInt(), 2, 10, QChar('0'));
    QString sec = QString("%1").arg(ui->cboxSec->currentText().toInt(), 2, 10, QChar('0'));
    QUIHelper::setSystemDateTime(year, month, day, hour, min, sec);
}

void frmConfigSystem::initLocalDB()
{
    ui->cboxLocalDBType->addItem("Sqlite");
    ui->cboxLocalDBType->addItem("MySql");
    ui->cboxLocalDBType->addItem("PostgreSQL");
    ui->cboxLocalDBType->addItem("SqlServer");
    ui->cboxLocalDBType->addItem("Oracle");
    ui->cboxLocalDBType->setCurrentIndex(ui->cboxLocalDBType->findText(App::LocalDBType));
    connect(ui->cboxLocalDBType, SIGNAL(currentIndexChanged(int)), this, SLOT(saveLocalDB()));

    ui->txtLocalDBIP->setText(App::LocalDBIP);
    connect(ui->txtLocalDBIP, SIGNAL(textChanged(QString)), this, SLOT(saveLocalDB()));

    ui->txtLocalDBPort->setText(QString::number(App::LocalDBPort));
    connect(ui->txtLocalDBPort, SIGNAL(textChanged(QString)), this, SLOT(saveLocalDB()));

    ui->txtLocalDBName->setText(App::LocalDBName);
    connect(ui->txtLocalDBName, SIGNAL(textChanged(QString)), this, SLOT(saveLocalDB()));

    ui->txtLocalUserName->setText(App::LocalUserName);
    connect(ui->txtLocalUserName, SIGNAL(textChanged(QString)), this, SLOT(saveLocalDB()));

    ui->txtLocalUserPwd->setText(App::LocalUserPwd);
    connect(ui->txtLocalUserPwd, SIGNAL(textChanged(QString)), this, SLOT(saveLocalDB()));
}

void frmConfigSystem::saveLocalDB()
{
    App::LocalDBType = ui->cboxLocalDBType->currentText();
    App::LocalDBIP = ui->txtLocalDBIP->text();
    App::LocalDBPort = ui->txtLocalDBPort->text().toInt();
    App::LocalDBName = ui->txtLocalDBName->text();
    App::LocalUserName = ui->txtLocalUserName->text();
    App::LocalUserPwd = ui->txtLocalUserPwd->text();
    App::writeConfig();
}

QString frmConfigSystem::saveExcel()
{
    QString whereSql = whereSql = "where Content like '%报警'";
    QString name = "报警记录";
    QString name2 = "alarmlog";

    QList<QString> columnNames;
    QList<int> columnWidths;
    columnNames << "位号" << "控制器名称" << "探测器名称" << "报警值" << "单位" << "报警类型" << "开始时间" << "结束时间";
    columnWidths << 130 << 150 << 150 << 70 << 70 << 120 << 150 << 150;

    QString fileName = QString("%1/db/%2_%3.xls").arg(QUIHelper::appPath()).arg(name2).arg(STRDATETIME);
    QString columns = "PositionID,DeviceName,NodeName,NodeValue,NodeSign,Content,StartTime,EndTime";
    QString where = whereSql + " order by LogID desc limit 10000";
    QStringList content = DBHelper::getContent("AlarmLog", columns, where, "", ";");

    if (content.count() == 0) {
        QUIHelper::showMessageBoxError("没有一条对应的报警记录,不用处理", 5);
        return "";
    }

    ExcelAPI::Instance()->saveExcel(fileName, name, name, "", columnNames, columnWidths, content);
    QString msg = QString("发送%1").arg(name);
    DBHelper::addUserLog(msg);
    return fileName;
}

void frmConfigSystem::on_btnLocalDBConnect_clicked()
{
    {
        bool error = false;
        QSqlDatabase dbConn;
        if (App::LocalDBType.toUpper() == "SQLITE") {
            QString dbName = QString("%1/db/%2.db").arg(QUIHelper::appPath()).arg(QUIHelper::appName());
            dbConn = QSqlDatabase::addDatabase("QSQLITE", "test");
            dbConn.setDatabaseName(dbName);
        } else if (App::LocalDBType.toUpper() == "MYSQL") {
            if (!QUIHelper::ipLive(App::LocalDBIP, App::LocalDBPort)) {
                error = true;
            } else {
                dbConn = QSqlDatabase::addDatabase("QMYSQL", "test");
                dbConn.setHostName(App::LocalDBIP);
                dbConn.setPort(App::LocalDBPort);
                dbConn.setDatabaseName("mysql");
                dbConn.setUserName(App::LocalUserName);
                dbConn.setPassword(App::LocalUserPwd);
            }
        }

        if (!error && dbConn.open()) {
            QUIHelper::showMessageBoxInfo("连接数据库成功!");
        } else {
            QUIHelper::showMessageBoxError("连接数据库失败!");
        }

        dbConn.close();
    }

    QSqlDatabase::removeDatabase("test");
}

void frmConfigSystem::on_btnLocalDBInit_clicked()
{
    if (App::LocalDBType.toUpper() == "MYSQL") {
        QString sqlName = QString("%1/db/%2.sql").arg(QUIHelper::appPath()).arg(QUIHelper::appName());
        QFile file(sqlName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QUIHelper::showMessageBoxError("数据库脚本文件打开失败!");
            return;
        }

        QDateTime dtStart = QDateTime::currentDateTime();

        {
            if (!QUIHelper::ipLive(App::LocalDBIP, App::LocalDBPort)) {
                QUIHelper::showMessageBoxError("连接数据库失败!");
                return;
            }

            QSqlDatabase dbConn = QSqlDatabase::addDatabase("QMYSQL", "mysqldb");
            dbConn.setHostName(App::LocalDBIP);
            dbConn.setPort(App::LocalDBPort);
            dbConn.setDatabaseName("mysql");
            dbConn.setUserName(App::LocalUserName);
            dbConn.setPassword(App::LocalUserPwd);

            if (!dbConn.open()) {
                QUIHelper::showMessageBoxError("连接数据库失败!");
                return;
            }

            QSqlQuery query(QSqlDatabase::database("mysqldb"));

            //第一步,删除原有数据库
            QString sql = QString("DROP DATABASE IF EXISTS %1;").arg(App::LocalDBName);
            qDebug() << TIMEMS << "sql:" << sql << "result:" << query.exec(sql);

            //第二步,新建数据库
            sql = QString("CREATE DATABASE %1;").arg(App::LocalDBName);
            qDebug() << TIMEMS << "sql:" << sql << "result:" << query.exec(sql);

            dbConn.close();

            //第三步,切换到新建的数据库库并执行建表语句
            dbConn.setHostName(App::LocalDBIP);
            dbConn.setPort(App::LocalDBPort);
            dbConn.setDatabaseName(App::LocalDBName);
            dbConn.setUserName(App::LocalUserName);
            dbConn.setPassword(App::LocalUserPwd);

            if (!dbConn.open()) {
                QUIHelper::showMessageBoxError("连接数据库失败!");
                return;
            }

            QSqlQuery query2(QSqlDatabase::database("mysqldb"));

            sql = "BEGIN;";
            qDebug() << TIMEMS << "sql:" << sql << "result:" << query2.exec(sql);

            while (!file.atEnd()) {
                sql = QString::fromUtf8(file.readLine());
                sql = sql.replace("\n", "");
                if (sql.startsWith("--") || sql.length() < 5 || sql.toUpper().startsWith("DROP INDEX IF EXISTS")) {
                    continue;
                }

                int index = sql.indexOf("INTEGER PRIMARY KEY AUTOINCREMENT");
                if (index > 0) {
                    sql = sql.replace("INTEGER PRIMARY KEY AUTOINCREMENT", "INTEGER (11) NOT NULL PRIMARY KEY AUTO_INCREMENT");
                }

                qDebug() << TIMEMS << "sql:" << sql << "result:" << query2.exec(sql);
            }

            sql = "COMMIT;";
            qDebug() << TIMEMS << "sql:" << sql << "result:" << query2.exec(sql);

            dbConn.close();
        }

        QSqlDatabase::removeDatabase("mysqldb");
        QDateTime dtEnd = QDateTime::currentDateTime();
        double ms = dtStart.msecsTo(dtEnd);

        QUIHelper::showMessageBoxInfo(QString("数据库脚本执行成功,总共用时 %1 秒!").arg(QString::number(ms / 1000, 'f', 1)));
    }
}
