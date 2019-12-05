#include "appkey.h"
#include "quiwidget.h"

QScopedPointer<AppKey> AppKey::self;
AppKey *AppKey::Instance()
{
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new AppKey);
        }
    }

    return self.data();
}

AppKey::AppKey(QObject *parent) : QObject(parent)
{
    keyData = "";
    keyUseDate = false;
    keyDate = "2017-01-01";
    keyUseRun = false;
    keyRun = 1;
    keyUseCount = false;
    keyCount = 10;

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkTime()));
    startTime = QDateTime::currentDateTime();
}

void AppKey::start()
{
    //判断密钥文件是否存在,不存在则从资源文件复制出来,同时需要设置文件写权限
    QString keyName = QUIHelper::appPath() + "/db/key.db";
    QFile keyFile(keyName);
    if (!keyFile.exists() || keyFile.size() == 0) {
        keyFile.remove();
        QUIHelper::copyFile(":/key.db", keyName);
    }

    //读取密钥文件    
    keyFile.open(QFile::ReadOnly);
    keyData = keyFile.readLine();
    keyFile.close();

    //将从注册码文件中的密文解密,与当前时间比较是否到期
    keyData = QUIHelper::getXorEncryptDecrypt(keyData, 110);
    QStringList data = keyData.split("|");

    if (data.count() != 6) {
        QUIHelper::showMessageBoxError("注册码文件已损坏,程序将自动关闭!", 8, true);
        exit(0);
    }

    keyUseDate = (data.at(0) == "1");
    keyDate = data.at(1);
    keyUseRun = (data.at(2) == "1");
    keyRun = data.at(3).toInt();
    keyUseCount = (data.at(4) == "1");
    keyCount = data.at(5).toInt();

    //如果启用了时间限制
    if (keyUseDate) {
        QString nowDate = QDate::currentDate().toString("yyyy-MM-dd");
        if (nowDate > keyDate) {
            QUIHelper::showMessageBoxError("软件已到期,请联系供应商更新注册码!", 8, true);
            exit(0);
        }
    }

    //如果启用了运行时间显示
    if (keyUseRun) {
        timer->start();
    }
}

void AppKey::stop()
{
    timer->stop();
}

void AppKey::checkTime()
{
    //找出当前时间与首次启动时间比较
    QDateTime now = QDateTime::currentDateTime();
    if (startTime.secsTo(now) >= (keyRun * 60)) {
        QUIHelper::showMessageBoxError("试运行时间已到,请联系供应商更新注册码!", 8, true);
        exit(0);
    }
}

bool AppKey::checkCount(int count)
{
    if (keyUseCount) {
        if (count >= keyCount) {
            QUIHelper::showMessageBoxError("设备数量超过限制,请联系供应商更新注册码!", 8, true);
            return false;
        }
    }

    return true;
}
