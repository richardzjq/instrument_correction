#pragma execution_character_set("utf-8")
#include "dblocalthread.h"

QScopedPointer<DbLocalThread> DbLocalThread::self;
DbLocalThread *DbLocalThread::Instance()
{
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new DbLocalThread);
        }
    }

    return self.data();
}

DbLocalThread::DbLocalThread(QObject *parent) : QThread(parent)
{
    stopped = false;

    dbOkLocal = false;
    errorCount = 0;
    lastCheckTimeLocal = QDateTime::currentDateTime();

    checkConn = false;
    checkInterval = 30;

    dbTypeLocal = DbType_MySql;
    connNameLocal = "qt_sql_default_connection";
    hostNameLocal = "127.0.0.1";
    portLocal = 3306;
    dbNameLocal = "db_mysql";
    userNameLocal = "root";
    userPwdLocal = "root";
}

DbLocalThread::~DbLocalThread()
{
    this->stop();
    this->wait();
}

void DbLocalThread::run()
{
    while (!stopped) {
        //定时执行一次校验数据库连接是否正常
        QDateTime now = QDateTime::currentDateTime();
        if (checkConn && lastCheckTimeLocal.secsTo(now) >= checkInterval) {
            checkDb();
            lastCheckTimeLocal = now;
            continue;
        }

        //如果数据库连接正常则处理数据,不正常则重连数据库
        if (!dbOkLocal && errorCount >= 3) {
            closeDb();
            msleep(3000);
            openDb();
            emit debug(QString("重连本地数据库%1").arg(dbOkLocal ? "成功" : "失败"));            
            msleep(3000);
        } else {
            msleep(100);
        }
    }

    stopped = false;
}

bool DbLocalThread::openDb()
{
    //可以自行增加其他数据库的支持
    if (dbTypeLocal == DbType_Sqlite) {
        dbConnLocal = QSqlDatabase::addDatabase("QSQLITE", connNameLocal);
        dbConnLocal.setDatabaseName(dbNameLocal);
    } else if (dbTypeLocal == DbType_MySql) {
        dbConnLocal = QSqlDatabase::addDatabase("QMYSQL", connNameLocal);
        dbConnLocal.setConnectOptions("MYSQL_OPT_RECONNECT=1;MYSQL_OPT_CONNECT_TIMEOUT=1;MYSQL_OPT_READ_TIMEOUT=1;MYSQL_OPT_WRITE_TIMEOUT=1");
        dbConnLocal.setHostName(hostNameLocal);
        dbConnLocal.setPort(portLocal);
        dbConnLocal.setDatabaseName(dbNameLocal);
        dbConnLocal.setUserName(userNameLocal);
        dbConnLocal.setPassword(userPwdLocal);
    }

    errorCount = 0;
    dbOkLocal = dbConnLocal.open();
    return dbOkLocal;
}

bool DbLocalThread::checkDb()
{
    QDateTime dtStart = QDateTime::currentDateTime();

    QString sql = "select 1";
    QSqlQuery query(dbConnLocal);
    dbOkLocal = query.exec(sql);
    dbOkLocal ? (errorCount = 0) : errorCount++;

    QDateTime dtEnd = QDateTime::currentDateTime();
    double ms = dtStart.msecsTo(dtEnd);
    emit debug(QString("检查本地数据库连接(共 %1 条/用时 %2 秒)").arg(1).arg(QString::number(ms / 1000, 'f', 3)));

    return dbOkLocal;
}

void DbLocalThread::closeDb()
{
    dbConnLocal.close();
    QSqlDatabase::removeDatabase(connNameLocal);
    dbOkLocal = false;
    emit debug("关闭本地数据库");
}

void DbLocalThread::stop()
{
    stopped = true;
}

void DbLocalThread::setConnInfo(DbLocalThread::DbType dbType, const QString &connName, const QString &hostName, int port, const QString &dbName, const QString &userName, const QString &userPwd)
{
    this->dbTypeLocal = dbType;
    this->connNameLocal = connName;
    this->hostNameLocal = hostName;
    this->portLocal = port;
    this->dbNameLocal = dbName;
    this->userNameLocal = userName;
    this->userPwdLocal = userPwd;
}

void DbLocalThread::setCheckConn(bool checkConn)
{
    this->checkConn = checkConn;
}

void DbLocalThread::setCheckInterval(int checkInterval)
{
    if (checkInterval > 5 && this->checkInterval != checkInterval) {
        this->checkInterval = checkInterval;
    }
}

QSqlDatabase DbLocalThread::getDb()
{
    return dbConnLocal;
}

bool DbLocalThread::getOk()
{
    return dbOkLocal;
}
