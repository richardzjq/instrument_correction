#include "app.h"
#include "quiwidget.h"

QString App::ConfigFile = "config.ini";

int App::MapWidth = 800;
int App::MapHeight = 600;
bool App::IsMove = false;
bool App::DbError = false;
QString App::CurrentImage = "bg_alarm.jpg";
QString App::CurrentUserName = "admin";
QString App::CurrentUserPwd = "admin";
QString App::CurrentUserType = QString::fromUtf8("管理员");
QString App::FileFilter = QString::fromUtf8("保存文件(*.csv)");
QString App::FileExtension = ".sams";
QString App::FileSpliter = ",";

int App::RowHeight = 25;
int App::LeftWidth = 200;
int App::RightWidth = 210;
int App::TopHeight = 80;
int App::BottomHeight = 25;
int App::DeviceWidth = 75;
int App::DeviceHeight = 35;

int App::BtnMinWidth = 70;
int App::BtnMinHeight = 55;
int App::BtnIconSize = 18;
int App::BorderWidth = 4;
int App::IconSize = 20;
int App::IconWidth = 40;
int App::IconHeight = 30;
int App::SwitchBtnWidth = 65;
int App::SwitchBtnHeight = 26;

bool App::AutoRun = false;
bool App::AutoPwd = false;
bool App::AutoLogin = true;
bool App::AutoConfirm = true;
bool App::SaveLog = false;
bool App::ShowTip = true;
int App::TipInterval = 0;
int App::Precision = 3;
QString App::LogoCn = QString::fromUtf8("数字多用表检定系统");
QString App::LogoEn = "Digital multimeter inspect system";
QString App::LogoBg = "config/logo_black.png";
QString App::Version = "V20191101";
QString App::Author = QString::fromUtf8("常州市远屏电子有限公司");
QString App::StyleName = ":/qss/lightblue.css";
QString App::LastLoginer = "admin";
QString App::LastFormMain = "btnViewData";
QString App::LastFormData = "btnDataNode";
QString App::LastFormConfig = "btnConfigSystem";

int App::WorkMode = 0;
int App::PanelMode = 0;
bool App::PlayAlarmSound = true;
int App::PlayCount = 1;
int App::MsgCount = 28;
int App::DeviceColumn = 6;
int App::PageCount = 28;

int App::AlarmLogMaxCount = 10;
int App::NodeLogMaxCount = 10;
int App::UserLogMaxCount = 10;
QString App::AlarmLogOrder = "desc";
QString App::NodeLogOrder = "desc";
QString App::UserLogOrder = "desc";

QString App::ColorZero = "blueviolet";
QString App::ColorUpper = "crimson";
QString App::ColorLimit = "darkorange";
QString App::ColorNormal = "lightseagreen";
QString App::ColorPlotBg = "darkcyan";
QString App::ColorPlotText = "white";
QString App::ColorPlotLine = "lightskyblue";
QString App::ColorOther = "firebrick";

bool App::UseNetSend = false;
QString App::NetSendInfo = "39.107.90.218:6060";
bool App::UseNetReceive = false;
int App::NetReceivePort = 6060;

QString App::MsgPortName = "COM8";
int App::MsgBaudRate = 9600;
int App::MsgInterval = 10000;
QString App::MsgTel = "13888888888";

QString App::SendEmailAddr = "feiyangqingyun@126.com";
QString App::SendEmailPwd = "19880210liu";
int App::EmailInterval = 10000;
QString App::ReceiveEmailAddr = "feiyangqingyun@163.com;517216493@qq.com";

QString App::LocalDBType = "Sqlite";
QString App::LocalDBIP = "127.0.0.1";
int App::LocalDBPort = 3306;
QString App::LocalDBName = "sams";
QString App::LocalUserName = "root";
QString App::LocalUserPwd = "root";

bool App::UseNetDB = false;
QString App::NetDBType = "MySql";
QString App::NetDBIP = "127.0.0.1";
int App::NetDBPort = 3306;
QString App::NetDBName = "sams";
QString App::NetUserName = "root";
QString App::NetUserPwd = "root";

int App::PortNameIndex = 0;
int App::DeviceNameIndex = 0;
QString App::KeyValue1 = "|";
QString App::KeyColor1 = "darkred";
QString App::KeyValue2 = "|";
QString App::KeyColor2 = "darkgreen";
QString App::SelectDirName = ".";

QString App::DeviceType = "FC-1003-8";
int App::DeviceAddr = 1;
QString App::PortName = "COM2";
int App::BaudRate = 9600;
int App::ListenPort = 502;
QString App::NodeValues = "20|785|60|88|75|62|32|99|20|785|60|88|75|62|32|99";
bool App::AutoData = false;
int App::TabIndex = 0;

void App::readConfig()
{
    if (!checkConfig()) {
        return;
    }

    QSettings set(App::ConfigFile, QSettings::IniFormat);

    set.beginGroup("AppConfig");
    App::AutoRun = set.value("AutoRun", App::AutoRun).toBool();
    App::AutoPwd = set.value("AutoPwd", App::AutoPwd).toBool();
    App::AutoLogin = set.value("AutoLogin", App::AutoLogin).toBool();
    App::AutoConfirm = set.value("AutoConfirm", App::AutoConfirm).toBool();
    App::SaveLog = set.value("SaveLog", App::SaveLog).toBool();
    App::ShowTip = set.value("ShowTip", App::ShowTip).toBool();
    App::TipInterval = set.value("TipInterval", App::TipInterval).toInt();
    App::Precision = set.value("Precision", App::Precision).toInt();
    App::LogoCn = set.value("LogoCn", App::LogoCn).toString();
    App::LogoEn = set.value("LogoEn", App::LogoEn).toString();
    App::LogoBg = set.value("LogoBg", App::LogoBg).toString();
    App::Author = set.value("Author", App::Author).toString();
    App::StyleName = set.value("StyleName", App::StyleName).toString();
    App::LastLoginer = set.value("LastLoginer", App::LastLoginer).toString();
    App::LastFormMain = set.value("LastFormMain", App::LastFormMain).toString();
    App::LastFormData = set.value("LastFormData", App::LastFormData).toString();
    App::LastFormConfig = set.value("LastFormConfig", App::LastFormConfig).toString();
    set.endGroup();

    set.beginGroup("BaseConfig");
    App::WorkMode = set.value("WorkMode", App::WorkMode).toInt();
    App::PanelMode = set.value("PanelMode", App::PanelMode).toInt();
    App::PlayAlarmSound = set.value("PlayAlarmSound", App::PlayAlarmSound).toBool();
    App::PlayCount = set.value("PlayCount", App::PlayCount).toInt();
    App::MsgCount = set.value("MsgCount", App::MsgCount).toInt();
    App::DeviceColumn = set.value("DeviceColumn", App::DeviceColumn).toInt();
    App::PageCount = set.value("PageCount", App::PageCount).toInt();
    set.endGroup();

    set.beginGroup("LogConfig");
    App::AlarmLogMaxCount = set.value("AlarmLogMaxCount", App::AlarmLogMaxCount).toInt();
    App::NodeLogMaxCount = set.value("NodeLogMaxCount", App::NodeLogMaxCount).toInt();
    App::UserLogMaxCount = set.value("UserLogMaxCount", App::UserLogMaxCount).toInt();
    App::AlarmLogOrder = set.value("AlarmLogOrder", App::AlarmLogOrder).toString();
    App::NodeLogOrder = set.value("NodeLogOrder", App::NodeLogOrder).toString();
    App::UserLogOrder = set.value("UserLogOrder", App::UserLogOrder).toString();
    set.endGroup();

    set.beginGroup("ColorConfig");
    App::ColorZero = set.value("ColorZero", App::ColorZero).toString();
    App::ColorUpper = set.value("ColorUpper", App::ColorUpper).toString();
    App::ColorLimit = set.value("ColorLimit", App::ColorLimit).toString();
    App::ColorNormal = set.value("ColorNormal", App::ColorNormal).toString();
    App::ColorPlotBg = set.value("ColorPlotBg", App::ColorPlotBg).toString();
    App::ColorPlotText = set.value("ColorPlotText", App::ColorPlotText).toString();
    App::ColorPlotLine = set.value("ColorPlotLine", App::ColorPlotLine).toString();
    App::ColorOther = set.value("ColorOther", App::ColorOther).toString();
    set.endGroup();

    set.beginGroup("NetLink");
    App::UseNetSend = set.value("UseNetSend", App::UseNetSend).toBool();
    App::NetSendInfo = set.value("NetSendInfo", App::NetSendInfo).toString();
    App::UseNetReceive = set.value("UseNetReceive", App::UseNetReceive).toBool();
    App::NetReceivePort = set.value("NetReceivePort", App::NetReceivePort).toInt();
    set.endGroup();

    set.beginGroup("MsgLink");
    App::MsgPortName = set.value("MsgPortName", App::MsgPortName).toString();
    App::MsgBaudRate = set.value("MsgBaudRate", App::MsgBaudRate).toInt();
    App::MsgInterval = set.value("MsgInterval", App::MsgInterval).toInt();
    App::MsgTel = set.value("MsgTel", App::MsgTel).toString();
    set.endGroup();

    set.beginGroup("EmailLink");
    App::SendEmailAddr = set.value("SendEmailAddr", App::SendEmailAddr).toString();
    App::SendEmailPwd = set.value("SendEmailPwd", App::SendEmailPwd).toString();
    App::EmailInterval = set.value("EmailInterval", App::EmailInterval).toInt();
    App::ReceiveEmailAddr = set.value("ReceiveEmailAddr", App::ReceiveEmailAddr).toString();
    App::SendEmailPwd = QUIHelper::getXorEncryptDecrypt(App::SendEmailPwd, 255);
    set.endGroup();

    set.beginGroup("LocalDBConfig");
    App::LocalDBType = set.value("LocalDBType").toString();
    App::LocalDBIP = set.value("LocalDBIP").toString();
    App::LocalDBPort = set.value("LocalDBPort").toInt();
    App::LocalDBName = set.value("LocalDBName").toString();
    App::LocalUserName = set.value("LocalUserName").toString();
    App::LocalUserPwd = set.value("LocalUserPwd").toString();
    App::LocalUserPwd = QUIHelper::getXorEncryptDecrypt(App::LocalUserPwd, 255);
    set.endGroup();

    set.beginGroup("NetDBConfig");
    App::UseNetDB = set.value("UseNetDB").toBool();
    App::NetDBType = set.value("NetDBType").toString();
    App::NetDBIP = set.value("NetDBIP").toString();
    App::NetDBPort = set.value("NetDBPort").toInt();
    App::NetDBName = set.value("NetDBName").toString();
    App::NetUserName = set.value("NetUserName").toString();
    App::NetUserPwd = set.value("NetUserPwd").toString();
    App::NetUserPwd = QUIHelper::getXorEncryptDecrypt(App::NetUserPwd, 255);
    set.endGroup();

    set.beginGroup("DebugConfig");
    App::PortNameIndex = set.value("PortNameIndex", App::PortNameIndex).toInt();
    App::DeviceNameIndex = set.value("DeviceNameIndex", App::DeviceNameIndex).toInt();
    App::KeyValue1 = set.value("KeyValue1", App::KeyValue1).toString();
    App::KeyColor1 = set.value("KeyColor1", App::KeyColor1).toString();
    App::KeyValue2 = set.value("KeyValue2", App::KeyValue2).toString();
    App::KeyColor2 = set.value("KeyColor2", App::KeyColor2).toString();
    App::SelectDirName = set.value("SelectDirName", App::SelectDirName).toString();
    set.endGroup();

    set.beginGroup("ToolConfig");
    App::DeviceType = set.value("DeviceType", App::DeviceType).toString();
    App::DeviceAddr = set.value("DeviceAddr", App::DeviceAddr).toInt();
    App::PortName = set.value("PortName", App::PortName).toString();
    App::BaudRate = set.value("BaudRate", App::BaudRate).toInt();
    App::ListenPort = set.value("ListenPort", App::ListenPort).toInt();
    App::NodeValues = set.value("NodeValues", App::NodeValues).toString();
    App::AutoData = set.value("AutoData", App::AutoData).toBool();
    App::TabIndex = set.value("TabIndex", App::TabIndex).toInt();
    set.endGroup();

    //过滤不符合要求的配置
    App::AlarmLogMaxCount = App::AlarmLogMaxCount > 1000 ? 1000 : App::AlarmLogMaxCount;
    App::NodeLogMaxCount = App::NodeLogMaxCount > 1000 ? 1000 : App::NodeLogMaxCount;
    App::UserLogMaxCount = App::UserLogMaxCount > 1000 ? 1000 : App::UserLogMaxCount;
}

void App::writeConfig()
{
    QSettings set(App::ConfigFile, QSettings::IniFormat);

    set.beginGroup("AppConfig");
    set.setValue("AutoRun", App::AutoRun);
    set.setValue("AutoPwd", App::AutoPwd);
    set.setValue("AutoLogin", App::AutoLogin);
    set.setValue("AutoConfirm", App::AutoConfirm);
    set.setValue("SaveLog", App::SaveLog);
    set.setValue("ShowTip", App::ShowTip);
    set.setValue("TipInterval", App::TipInterval);
    set.setValue("Precision", App::Precision);
    set.setValue("LogoCn", App::LogoCn);
    set.setValue("LogoEn", App::LogoEn);
    set.setValue("LogoBg", App::LogoBg);
    set.setValue("Author", App::Author);
    set.setValue("StyleName", App::StyleName);
    set.setValue("LastLoginer", App::LastLoginer);
    set.setValue("LastFormMain", App::LastFormMain);
    set.setValue("LastFormData", App::LastFormData);
    set.setValue("LastFormConfig", App::LastFormConfig);
    set.endGroup();

    set.beginGroup("BaseConfig");
    set.setValue("WorkMode", App::WorkMode);
    set.setValue("PanelMode", App::PanelMode);
    set.setValue("PlayAlarmSound", App::PlayAlarmSound);
    set.setValue("PlayCount", App::PlayCount);
    set.setValue("MsgCount", App::MsgCount);
    set.setValue("DeviceColumn", App::DeviceColumn);
    set.setValue("PageCount", App::PageCount);
    set.endGroup();

    set.beginGroup("LogConfig");
    set.setValue("AlarmLogMaxCount", App::AlarmLogMaxCount);
    set.setValue("NodeLogMaxCount", App::NodeLogMaxCount);
    set.setValue("UserLogMaxCount", App::UserLogMaxCount);
    set.setValue("AlarmLogOrder", App::AlarmLogOrder);
    set.setValue("NodeLogOrder", App::NodeLogOrder);
    set.setValue("UserLogOrder", App::UserLogOrder);
    set.endGroup();

    set.beginGroup("ColorConfig");
    set.setValue("ColorZero", App::ColorZero);
    set.setValue("ColorUpper", App::ColorUpper);
    set.setValue("ColorLimit", App::ColorLimit);
    set.setValue("ColorNormal", App::ColorNormal);
    set.setValue("ColorPlotBg", App::ColorPlotBg);
    set.setValue("ColorPlotText", App::ColorPlotText);
    set.setValue("ColorPlotLine", App::ColorPlotLine);
    set.setValue("ColorOther", App::ColorOther);
    set.endGroup();

    set.beginGroup("NetLink");
    set.setValue("UseNetSend", App::UseNetSend);
    set.setValue("NetSendInfo", App::NetSendInfo);
    set.setValue("UseNetReceive", App::UseNetReceive);
    set.setValue("NetReceivePort", App::NetReceivePort);
    set.endGroup();

    set.beginGroup("MsgLink");
    set.setValue("MsgPortName", App::MsgPortName);
    set.setValue("MsgBaudRate", App::MsgBaudRate);
    set.setValue("MsgInterval", App::MsgInterval);
    set.setValue("MsgTel", App::MsgTel);
    set.endGroup();

    set.beginGroup("EmailLink");
    set.setValue("SendEmailAddr", App::SendEmailAddr);
    set.setValue("SendEmailPwd", App::SendEmailPwd);
    set.setValue("EmailInterval", App::EmailInterval);
    set.setValue("ReceiveEmailAddr", App::ReceiveEmailAddr);
    set.setValue("SendEmailPwd", QUIHelper::getXorEncryptDecrypt(App::SendEmailPwd, 255));
    set.endGroup();

    set.beginGroup("LocalDBConfig");
    set.setValue("LocalDBType", App::LocalDBType);
    set.setValue("LocalDBIP", App::LocalDBIP);
    set.setValue("LocalDBPort", App::LocalDBPort);
    set.setValue("LocalDBName", App::LocalDBName);
    set.setValue("LocalUserName", App::LocalUserName);
    set.setValue("LocalUserPwd", QUIHelper::getXorEncryptDecrypt(App::LocalUserPwd, 255));
    set.endGroup();

    set.beginGroup("NetDBConfig");
    set.setValue("UseNetDB", App::UseNetDB);
    set.setValue("NetDBType", App::NetDBType);
    set.setValue("NetDBIP", App::NetDBIP);
    set.setValue("NetDBPort", App::NetDBPort);
    set.setValue("NetDBName", App::NetDBName);
    set.setValue("NetUserName", App::NetUserName);
    set.setValue("NetUserPwd", QUIHelper::getXorEncryptDecrypt(App::NetUserPwd, 255));
    set.endGroup();

    set.beginGroup("DebugConfig");
    set.setValue("PortNameIndex", App::PortNameIndex);
    set.setValue("DeviceNameIndex", App::DeviceNameIndex);
    set.setValue("KeyValue1", App::KeyValue1);
    set.setValue("KeyColor1", App::KeyColor1);
    set.setValue("KeyValue2", App::KeyValue2);
    set.setValue("KeyColor2", App::KeyColor2);
    set.setValue("SelectDirName", App::SelectDirName);
    set.endGroup();

    set.beginGroup("ToolConfig");
    set.setValue("DeviceType", App::DeviceType);
    set.setValue("DeviceAddr", App::DeviceAddr);
    set.setValue("PortName", App::PortName);
    set.setValue("BaudRate", App::BaudRate);
    set.setValue("ListenPort", App::ListenPort);
    set.setValue("NodeValues", App::NodeValues);
    set.setValue("AutoData", App::AutoData);
    set.setValue("TabIndex", App::TabIndex);
    set.endGroup();
}

void App::newConfig()
{
    if (QUIHelper::deskWidth() < 1440) {
        App::MsgCount = 23;
        App::PageCount = 23;
    }

    writeConfig();
}

bool App::checkConfig()
{
    //如果配置文件大小为0,则以初始值继续运行,并生成配置文件
    QFile file(App::ConfigFile);
    if (file.size() == 0) {
        newConfig();
        return false;
    }

    //如果配置文件不完整,则以初始值继续运行,并生成配置文件
    if (file.open(QFile::ReadOnly)) {
        bool ok = true;
        while (!file.atEnd()) {
            QString line = file.readLine();
            line = line.replace("\r", "");
            line = line.replace("\n", "");
            QStringList list = line.split("=");

            if (list.count() == 2) {
                if (list.at(1) == "") {
                    ok = false;
                    break;
                }
            }
        }

        if (!ok) {
            newConfig();
            return false;
        }
    } else {
        newConfig();
        return false;
    }

    return true;
}
