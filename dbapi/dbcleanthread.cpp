#pragma execution_character_set("utf-8")
#include "dbcleanthread.h"

QScopedPointer<DbCleanThread> DbCleanThread::self;
DbCleanThread *DbCleanThread::Instance()
{
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new DbCleanThread);
        }
    }

    return self.data();
}

DbCleanThread::DbCleanThread(QObject *parent) : QThread(parent)
{
    stopped = false;
    isBusy = false;
    lastTime = QDateTime::currentDateTime();

    tableName = "DeviceLog";
    countName = "*";
    whereColumnName = "SaveTime";
    orderSql = "SaveTime asc";
    maxCount = 100000;
    interval = 30 * 60;

    dirPath = "";
    dirFileFilter = QStringList("*.jpg");
    dirMaxSize = 1024;

    dbType = DbType_MySql;
    connName = "qt_sql_default_connection";
    hostName = "127.0.0.1";
    port = 3306;
    dbName = "db_mysql";
    userName = "root";
    userPwd = "root";
}

DbCleanThread::~DbCleanThread()
{
    this->stop();
    this->wait();
}

void DbCleanThread::run()
{
    //在这里打开数据库,Qt5.10以后增加了安全性,不能用主线程创建的数据库连接
    QSqlDatabase dbConn;
    if (dbType == DbType_Sqlite) {
        dbConn = QSqlDatabase::addDatabase("QSQLITE", connName);
        dbConn.setDatabaseName(dbName);
    } else if (dbType == DbType_MySql) {
        dbConn = QSqlDatabase::addDatabase("QMYSQL", connName);
        dbConn.setConnectOptions("MYSQL_OPT_RECONNECT=1;MYSQL_OPT_CONNECT_TIMEOUT=1;MYSQL_OPT_READ_TIMEOUT=1;MYSQL_OPT_WRITE_TIMEOUT=1");
        dbConn.setHostName(hostName);
        dbConn.setPort(port);
        dbConn.setDatabaseName(dbName);
        dbConn.setUserName(userName);
        dbConn.setPassword(userPwd);
    }

    //数据库打开失败则不用处理
    if (!dbConn.open()) {
        qDebug() << this->objectName() << dbConn.lastError();
        return;
    }

    while (!stopped) {
        //定时执行一次校验时间是否到了该清理的时候
        QDateTime now = QDateTime::currentDateTime();
        if (lastTime.secsTo(now) >= interval) {
            if (isBusy) {
                qDebug() << this->objectName() << "DbCleanThread isBusy";
                lastTime = now;
                continue;
            }

            isBusy = true;
            clean();
            isBusy = false;
            lastTime = now;
            msleep(1);
        }

        msleep(100);
    }

    stopped = false;
}

void DbCleanThread::setTableName(const QString &tableName)
{
    this->tableName = tableName;
}

void DbCleanThread::setWhereColumnName(const QString &whereColumnName)
{
    this->whereColumnName = whereColumnName;
}

void DbCleanThread::setOrderSql(const QString &orderSql)
{
    this->orderSql = orderSql;
}

void DbCleanThread::setMaxCount(int maxCount)
{
    this->maxCount = maxCount;
}

void DbCleanThread::setInterval(int interval)
{
    this->interval = interval;
}

void DbCleanThread::setPar(const QString &tableName, const QString &countName,
                           const QString &whereColumnName, const QString &orderSql,
                           int maxCount, int interval)
{
    this->tableName = tableName;
    this->countName = countName;
    this->whereColumnName = whereColumnName;
    this->orderSql = orderSql;
    this->maxCount = maxCount;
    this->interval = interval;
}

void DbCleanThread::setDirPath(const QString &dirPath)
{
    this->dirPath = dirPath;
}

void DbCleanThread::setDirFileFilter(const QStringList &dirFileFilter)
{
    this->dirFileFilter = dirFileFilter;
}

void DbCleanThread::setDirMaxSize(int dirMaxSize)
{
    this->dirMaxSize = dirMaxSize;
}

void DbCleanThread::setConnInfo(DbCleanThread::DbType dbType, const QString &connName, const QString &hostName, int port,
                                const QString &dbName, const QString &userName, const QString &userPwd)
{
    this->dbType = dbType;
    this->connName = connName;
    this->hostName = hostName;
    this->port = port;
    this->dbName = dbName;
    this->userName = userName;
    this->userPwd = userPwd;
}

void DbCleanThread::deleteDirectory(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) {
        return;
    }

    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();

    foreach (QFileInfo fi, fileList) {
        if (fi.isFile()) {
            fi.dir().remove(fi.fileName());
        } else {
            deleteDirectory(fi.absoluteFilePath());
            dir.rmdir(fi.absoluteFilePath());
        }
    }
}

void DbCleanThread::stop()
{
    stopped = true;
}

void DbCleanThread::clean()
{
    //首先查找总记录数,如果总记录数超过限制,则将超出的部分按照字段排序进行删除
    QString sql = QString("select count(%1) from %2").arg(countName).arg(tableName);
    QSqlQuery query(QSqlDatabase::database(connName));
    query.exec(sql);
    query.next();

    int count = query.value(0).toInt();
    int cleanCount = (count - maxCount);
    if (cleanCount >= 100) {
        QDateTime dtStart = QDateTime::currentDateTime();

        //每次最大清理1000条数据
        cleanCount = cleanCount > 1000 ? 1000 : cleanCount;

        //将要删除的数据指定字段集合查询出来
        query.clear();
        sql = QString("select %2 from %1 order by %3 limit %4").arg(tableName).arg(whereColumnName).arg(orderSql).arg(cleanCount);
        query.exec(sql);

        QStringList list;
        while(query.next()) {
            list << query.value(0).toString();
        }

        //删除数据
        query.clear();
        sql = QString("delete from %1 where %2 in(%3)").arg(tableName).arg(whereColumnName).arg(list.join(","));
        bool ok = query.exec(sql);

        QDateTime dtEnd = QDateTime::currentDateTime();
        double msec = dtStart.msecsTo(dtEnd);
        emit cleanFinsh(ok, cleanCount, msec);
    }

    //自动清理文件夹
    if (!dirPath.isEmpty()) {
        //找出该文件夹下的所有文件夹
        QDir dir(dirPath);
        if (!dir.exists()) {
            return;
        }

        //按照目录查找,过滤文件夹,按照文件名称排序
        dir.setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        dir.setSorting(QDir::Name);
        QStringList list = dir.entryList();

        //遍历所有目录,对所有文件大小相加得到总大小,文件就在文件夹下,不会再有子目录
        qint64 size = 0;
        foreach (QString path, list) {
            QDir d(dirPath + "/" + path);
            QFileInfoList infos = d.entryInfoList(dirFileFilter);
            foreach (QFileInfo info, infos) {
                size += info.size();
            }

            //转化成MB,超过预定大小自动删除第一个文件夹,跳出循环无需继续判断
            int sizeMB = size / (1024 * 1024);
            if (sizeMB >= dirMaxSize) {
                //删除该目录下的所有文件
                QString firstDir = dirPath + "/" + list.at(0);
                QDir dir(firstDir);
                dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
                QStringList list = dir.entryList();
                foreach (QString file, list) {
                    dir.remove(file);
                    qDebug() << "remove file" << firstDir << file;
                }

                //删除文件夹本身
                dir.rmdir(firstDir);
                qDebug() << "remove dir" << firstDir;

                emit cleanDir(firstDir);
                break;
            }
        }
    }
}
