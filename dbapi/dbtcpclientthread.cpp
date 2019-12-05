#pragma execution_character_set("utf-8")
#include "dbtcpclientthread.h"

QScopedPointer<DbTcpClientThread> DbTcpClientThread::self;
DbTcpClientThread *DbTcpClientThread::Instance()
{
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new DbTcpClientThread);
        }
    }

    return self.data();
}

DbTcpClientThread::DbTcpClientThread(QObject *parent) : QThread(parent)
{
    stopped = false;

    dbOkNet = false;
    errorCount = 0;
    lastCheckTimeNet = QDateTime::currentDateTime();

    checkConn = false;
    checkInterval = 30;

    dbTypeNet = DbType_MySql;
    connNameNet = "dbClient";
    hostNameNet = "127.0.0.1";
    portNet = 3306;
    dbNameNet = "db_mysql";
    userNameNet = "root";
    userPwdNet = "root";

    maxCount = 1000;
    listSql.clear();
    initSql.clear();
}

DbTcpClientThread::~DbTcpClientThread()
{
    this->stop();
    this->wait();
}

void DbTcpClientThread::run()
{
    while (!stopped) {
        //定时执行一次校验数据库连接是否正常
        QDateTime now = QDateTime::currentDateTime();

        if (checkConn && lastCheckTimeNet.secsTo(now) >= checkInterval) {
            checkDb();
            lastCheckTimeNet = now;
            continue;
        }

        //如果数据库连接正常则处理数据,不正常则重连数据库
        if (dbOkNet) {
            checkData();
            msleep(500);
        } else if (errorCount >= 3) {
            closeDb();
            msleep(3000);
            openDb();
            emit debug(QString("重连云端实时数据库%1").arg(dbOkNet ? "成功" : "失败"));
            msleep(3000);
        }
    }

    stopped = false;
}

bool DbTcpClientThread::openDb()
{
    //可以自行增加其他数据库的支持
    if (dbTypeNet == DbType_MySql) {
        dbConnNet = QSqlDatabase::addDatabase("QMYSQL", connNameNet);
        dbConnNet.setConnectOptions("MYSQL_OPT_RECONNECT=1;MYSQL_OPT_CONNECT_TIMEOUT=1;MYSQL_OPT_READ_TIMEOUT=1;MYSQL_OPT_WRITE_TIMEOUT=1");
        dbConnNet.setHostName(hostNameNet);
        dbConnNet.setPort(portNet);
        dbConnNet.setDatabaseName(dbNameNet);
        dbConnNet.setUserName(userNameNet);
        dbConnNet.setPassword(userPwdNet);
    }

    errorCount = 0;
    dbOkNet = dbConnNet.open();
    return dbOkNet;
}

bool DbTcpClientThread::checkDb()
{
    QDateTime dtStart = QDateTime::currentDateTime();

    QString sql = "select 1";
    QSqlQuery query(dbConnNet);
    dbOkNet = query.exec(sql);
    dbOkNet ? (errorCount = 0) : errorCount++;

    QDateTime dtEnd = QDateTime::currentDateTime();
    double ms = dtStart.msecsTo(dtEnd);
    emit debug(QString("检查云端实时数据库连接(共 %1 条/用时 %2 秒)").arg(1).arg(QString::number(ms / 1000, 'f', 3)));

    return dbOkNet;
}

void DbTcpClientThread::closeDb()
{
    dbConnNet.close();
    QSqlDatabase::removeDatabase(connNameNet);
    dbOkNet = false;
    emit debug("关闭云端实时数据库");
}

void DbTcpClientThread::initDb()
{
    //保证至少有一条数据
    int count = initSql.count();
    if (dbOkNet && count > 1) {
        QDateTime dtStart = QDateTime::currentDateTime();

        //严格规定第一条sql语句为清空该表所有数据
        QSqlQuery query(dbConnNet);
        query.exec(initSql.first());

        //新建初始值数据
        for (int i = 1; i < count; i++) {
            query.clear();
            query.exec(initSql.at(i));
        }

        QDateTime dtEnd = QDateTime::currentDateTime();
        double ms = dtStart.msecsTo(dtEnd);
        emit debug(QString("远程清空并新建数据(共 %1 条/用时 %2 秒)").arg(count).arg(QString::number(ms / 1000, 'f', 3)));
    }
}

void DbTcpClientThread::stop()
{
    stopped = true;
}

void DbTcpClientThread::append(const QString &sql)
{
    mutex.lock();

    //只有不一样的sql语句才需要处理
    if (!listSql.contains(sql)) {
        //如果达到最大数量,则移除最后一条,追加到末尾
        if (listSql.count() >= maxCount) {
            listSql.removeLast();
        }

        listSql.append(sql);
    }

    mutex.unlock();
}

void DbTcpClientThread::clear()
{
    mutex.lock();
    listSql.clear();
    mutex.unlock();
}

bool DbTcpClientThread::checkData()
{
    mutex.lock();
    QStringList sqls = listSql;
    mutex.unlock();

    int count = sqls.count();
    if (count == 0) {
        return false;
    }

    //取出队列中的所有sql语句,一次性事务提交到远程数据库
    if (dbOkNet) {
        QDateTime dtStart = QDateTime::currentDateTime();
        dbConnNet.transaction();

        //逐个执行sql语句
        foreach (QString sql, sqls) {
            QSqlQuery query(dbConnNet);
            query.exec(sql);
        }

        //提交成功后将sql语句队列清空
        //提交失败则需要回滚事务
        dbOkNet = dbConnNet.commit();

        if (dbOkNet) {
            clear();
            QDateTime dtEnd = QDateTime::currentDateTime();
            double ms = dtStart.msecsTo(dtEnd);
            emit debug(QString("远程实时更新数据(共 %1 条/用时 %2 秒)").arg(count).arg(QString::number(ms / 1000, 'f', 3)));
        } else {
            dbConnNet.rollback();
            emit debug(QString("提交事务失败: %1").arg(dbConnNet.lastError().text()));
        }
    }

    return dbOkNet;
}

void DbTcpClientThread::setConnInfo(DbType dbType, const QString &connName, const QString &hostName, int port,
                                    const QString &dbName, const QString &userName, const QString &userPwd)
{
    this->dbTypeNet = dbType;
    this->connNameNet = connName;
    this->hostNameNet = hostName;
    this->portNet = port;
    this->dbNameNet = dbName;
    this->userNameNet = userName;
    this->userPwdNet = userPwd;
}

void DbTcpClientThread::setCheckConn(bool checkConn)
{
    this->checkConn = checkConn;
}

void DbTcpClientThread::setCheckInterval(int checkInterval)
{
    if (checkInterval > 5 && this->checkInterval != checkInterval) {
        this->checkInterval = checkInterval;
    }
}

void DbTcpClientThread::setInitSql(const QStringList &initSql)
{
    this->initSql = initSql;
}
