#include "dbhelper.h"
#include "quiwidget.h"

#define loadfromdb true

DBHelper::DBHelper(QObject *parent) : QObject(parent)
{
}

QStringList DBHelper::getContent(const QString &tableName,
                                 const QString &columnNames,
                                 const QString &whereSql,
                                 const QString &title,
                                 const QString &spliter)
{
    QStringList content;

    //如果标题字段名称不为空则先添加标题
    if (!title.isEmpty()) {
        content << title;
    }

    QString sql = QString("select %1 from %2 %3").arg(columnNames).arg(tableName).arg(whereSql);
    QSqlQuery query;
    query.exec(sql);

    int columnCount = query.record().count();
    while (query.next()) {
        QStringList list;
        for (int i = 0; i < columnCount; i++) {
            list << query.value(i).toString();
        }

        content << list.join(spliter);
    }

    return content;
}

bool DBHelper::inputData(int columnCount,
                         const QString &filter,
                         const QString &tableName,
                         QString &fileName,
                         const QString &defaultDir,
                         bool existTitle)
{
    bool result = true;
    fileName = QUIHelper::getFileName(filter, defaultDir);

    if (fileName.length() != 0) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QFile::Text)) {
            //先删除原来的数据
            QString sql = QString("delete from %1").arg(tableName);
            QSqlQuery query;
            query.exec(sql);

            QSqlDatabase::database().transaction();
            bool isremoveTitle = false;

            //cvs格式需要gbk编码才能正常
            QTextStream in(&file);
            if (fileName.endsWith(".csv")) {
                in.setCodec("gbk");
            }

            bool firstline = true;
            while(!in.atEnd()) {
                QString line = in.readLine();
                QStringList list = line.split(App::FileSpliter);

                //先读取第一行判断列数是否和目标列数一致,不一致则返回
                if (firstline) {
                    if (list.count() != columnCount) {
                        return false;
                    }

                    firstline = false;
                }

                //如果存在标题则不需要处理第一行标题
                if (existTitle && !isremoveTitle) {
                    isremoveTitle = true;
                    continue;
                }

                if (list.count() == columnCount) {
                    sql = QString("insert into %1 values('").arg(tableName);
                    for (int i = 0; i < columnCount - 1; i++) {
                        sql = sql + list.at(i).trimmed() + "','";
                    }

                    sql = sql + list.at(columnCount - 1).trimmed() + "')";
                    query.clear();
                    query.exec(sql);
                }
            }

            if (!QSqlDatabase::database().commit()) {
                QSqlDatabase::database().rollback();
                result = false;
            }

            file.close();
        }
    }

    return result;
}

bool DBHelper::outputData(const QString &defaultName,
                          const QString &filter,
                          const QStringList &content,
                          QString &fileName,
                          const QString &defaultDir)
{
    bool result = true;
    QString name = QString("%1/%2").arg(defaultDir).arg(defaultName);
    fileName = QUIHelper::getSaveName(filter, name);
    outputData(fileName, content);
    return result;
}

bool DBHelper::outputData(QString &fileName,
                          const QStringList &content)
{
    if (fileName.length() != 0) {
        int rowCount = content.count();
        if (rowCount == 0) {
            QUIHelper::showMessageBoxError("没有要导出的数据!", 3);
            fileName.clear();
            return false;
        }

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QFile::Text)) {
            //cvs格式需要gbk编码才能正常
            QTextStream out(&file);
            if (fileName.endsWith(".csv")) {
                out.setCodec("gbk");
            }

            for (int i = 0; i < rowCount; i++) {
                out << content.at(i) << "\n";
            }

            file.close();
        }
    }

    return true;
}

void DBHelper::loadUserInfo()
{
    DBData::UserInfo_Count = 0;
    DBData::UserInfo_UserName.clear();
    DBData::UserInfo_UserPwd.clear();
    DBData::UserInfo_UserType.clear();
    DBData::UserInfo_UserAdmin.clear();

    QString sql = "select UserName,UserPwd,UserType,UserAdmin from UserInfo";
    QSqlQuery query;
    if (!query.exec(sql)) {
        return;
    }

    while(query.next()) {
        QString userName = query.value(0).toString();
        QString userPwd = query.value(1).toString();
        QString userType = query.value(2).toString();
        QString userAdmin = query.value(3).toString();

        DBData::UserInfo_Count++;
        DBData::UserInfo_UserName << userName;
        DBData::UserInfo_UserPwd << userPwd;
        DBData::UserInfo_UserType << userType;
        DBData::UserInfo_UserAdmin << userAdmin;
    }
}

void DBHelper::clearUserInfo()
{
    QString sql = "delete from UserInfo where UserName != 'admin'";
    execSql(sql);
}

void DBHelper::updateUserInfo(const QString &userName,
                              const QString &userPwd)
{
    QString sql = QString("update UserInfo set UserPwd='%1' where UserName='%2'").arg(userPwd).arg(userName);
    execSql(sql);
}

void DBHelper::getUserInfo(QString &userPwd,
                           QString &userType)
{
    QString sql = QString("select UserPwd,UserType from UserInfo where UserName='%1'").arg(App::LastLoginer);
    QSqlQuery query;
    if (!query.exec(sql)) {
        return;
    }

    if (query.next()) {
        userPwd = query.value(0).toString();
        userType = query.value(1).toString();
    }
}

//取出所有用户信息,包括用户名密码及类型
void DBHelper::getUserInfo(QStringList &userName,
                           QStringList &userPwd,
                           QStringList &userType)
{
    QString sql = "select UserName,UserPwd,UserType from UserInfo";
    QSqlQuery query;
    if (!query.exec(sql)) {
        return;
    }

    while (query.next()) {
        userName << query.value(0).toString();
        userPwd << query.value(1).toString();
        userType << query.value(2).toString();
    }
}

void DBHelper::loadPortInfo()
{
    DBData::PortInfo_Count = 0;
    DBData::PortInfo_PortID.clear();
    DBData::PortInfo_PortName.clear();
    DBData::PortInfo_ComName.clear();
    DBData::PortInfo_BaudRate.clear();
    DBData::PortInfo_TcpIP.clear();
    DBData::PortInfo_TcpPort.clear();
    DBData::PortInfo_ReadInterval.clear();
    DBData::PortInfo_ReadTimeout.clear();
    DBData::PortInfo_ReadMaxtime.clear();

    QString sql = "select PortID,PortName,ComName,BaudRate,TcpIP,TcpPort,ReadInterval,ReadTimeout,ReadMaxtime from PortInfo order by PortID asc";
    QSqlQuery query;
    if (!query.exec(sql)) {
        return;
    }

    while(query.next()) {
        int portID = query.value(0).toInt();
        QString portName = query.value(1).toString();
        QString comName = query.value(2).toString();
        int baudRate = query.value(3).toInt();
        QString tcpIP = query.value(4).toString();
        int tcpPort = query.value(5).toInt();
        float readInterval = query.value(6).toFloat();
        int readTimeout = query.value(7).toInt();
        int readMaxtime = query.value(8).toInt();

        //限定超时次数不能小于1
        readTimeout = readTimeout <= 1 ? 1 : readTimeout;

        DBData::PortInfo_Count++;
        DBData::PortInfo_PortID.append(portID);
        DBData::PortInfo_PortName.append(portName);
        DBData::PortInfo_ComName.append(comName);
        DBData::PortInfo_BaudRate.append(baudRate);
        DBData::PortInfo_TcpIP.append(tcpIP);
        DBData::PortInfo_TcpPort.append(tcpPort);
        DBData::PortInfo_ReadInterval.append(readInterval);
        DBData::PortInfo_ReadTimeout.append(readTimeout);
        DBData::PortInfo_ReadMaxtime.append(readMaxtime);
    }
}

void DBHelper::clearPortInfo()
{
    QString sql = "delete from PortInfo";
    execSql(sql);
}

void DBHelper::loadDeviceInfo()
{
    DBData::DeviceInfo_Count = 0;
    DBData::DeviceInfo_DeviceID.clear();
    DBData::DeviceInfo_PortName.clear();
    DBData::DeviceInfo_DeviceName.clear();
    DBData::DeviceInfo_DeviceAddr.clear();
    DBData::DeviceInfo_DeviceType.clear();
    DBData::DeviceInfo_NodeNumber.clear();

    QString sql = "select DeviceID,PortName,DeviceName,DeviceAddr,DeviceType,NodeNumber from DeviceInfo order by DeviceID asc";
    QSqlQuery query;
    if (!query.exec(sql)) {
        return;
    }

    while(query.next()) {
        int deviceID = query.value(0).toInt();
        QString portName = query.value(1).toString();
        QString deviceName = query.value(2).toString();
        quint8 deviceAddr = query.value(3).toInt();
        QString deviceType = query.value(4).toString();
        int nodeNumber = query.value(5).toInt();

        DBData::DeviceInfo_Count++;
        DBData::DeviceInfo_DeviceID.append(deviceID);
        DBData::DeviceInfo_PortName.append(portName);
        DBData::DeviceInfo_DeviceName.append(deviceName);
        DBData::DeviceInfo_DeviceAddr.append(deviceAddr);
        DBData::DeviceInfo_DeviceType.append(deviceType);
        DBData::DeviceInfo_NodeNumber.append(nodeNumber);
    }
}

void DBHelper::clearDeviceInfo()
{
    QString sql = "delete from DeviceInfo";
    execSql(sql);
}

void DBHelper::deleteDeviceInfo(const QString &portName)
{
    QString sql = QString("delete from DeviceInfo where PortName='%1'").arg(portName);
    execSql(sql);
}

QString DBHelper::getPortName(const QString &deviceName)
{
    QString portName;
    for (int i = 0; i < DBData::DeviceInfo_Count; i++) {
        if (DBData::DeviceInfo_DeviceName.at(i) == deviceName) {
            portName = DBData::DeviceInfo_PortName.at(i);
            break;
        }
    }

    return portName;
}

QString DBHelper::getDeviceName(const QString &portName, quint8 addr)
{
    QString deviceName;
    for (int i = 0; i < DBData::DeviceInfo_Count; i++) {
        if (DBData::DeviceInfo_PortName.at(i) == portName) {
            if (DBData::DeviceInfo_DeviceAddr.at(i) == addr) {
                deviceName = DBData::DeviceInfo_DeviceName.at(i);
                break;
            }
        }
    }

    return deviceName;
}

int DBHelper::getNodeNumber(const QString &portName, quint8 addr)
{
    int nodeNumber = 0;
    for (int i = 0; i < DBData::DeviceInfo_Count; i++) {
        if (DBData::DeviceInfo_PortName.at(i) == portName) {
            if (DBData::DeviceInfo_DeviceAddr.at(i) == addr) {
                nodeNumber = DBData::DeviceInfo_NodeNumber.at(i);
                break;
            }
        }
    }

    return nodeNumber;
}

quint16 DBHelper::getNodeMinAddr(const QString &portName, quint8 addr)
{
    quint16 nodeMinAddr = 65535;
    for (int i = 0; i < DBData::DeviceInfo_Count; i++) {
        if (DBData::DeviceInfo_PortName.at(i) == portName) {
            if (DBData::DeviceInfo_DeviceAddr.at(i) == addr) {
                QString deviceName = DBData::DeviceInfo_DeviceName.at(i);
                //再从探测器表中找该控制器对应的探测器的最小的地址
                for (int j = 0; j < DBData::NodeInfo_Count; j++) {
                    if (deviceName == DBData::NodeInfo_DeviceName.at(j)) {
                        quint16 nodeAddr = DBData::NodeInfo_NodeAddr.at(j);
                        if (nodeAddr < nodeMinAddr) {
                            nodeMinAddr = nodeAddr;
                        }
                    }
                }

                break;
            }
        }
    }

    return nodeMinAddr - 1;
}

void DBHelper::loadNodeInfo()
{
    //先记住之前的值 Online NodeValue AlarmUpper AlarmLimit AlarmOther SaveTime
    //防止在动态改变探测器信息以后误报,例如以前处于报警,重新保存的时候不报警,此时重新采集数据又是报警,会认为是新的报警
    QList<QString> NodeInfo_PositionID;
    QList<bool> NodeInfo_Online;
    QList<bool> NodeInfo_AlarmUpper;
    QList<bool> NodeInfo_AlarmLimit;
    QList<bool> NodeInfo_AlarmOther;
    QList<float> NodeInfo_NodeValue;
    QList<QDateTime> NodeInfo_SaveTime;

    NodeInfo_PositionID = DBData::NodeInfo_PositionID;
    NodeInfo_Online = DBData::NodeInfo_Online;
    NodeInfo_AlarmUpper = DBData::NodeInfo_AlarmUpper;
    NodeInfo_AlarmLimit = DBData::NodeInfo_AlarmLimit;
    NodeInfo_AlarmOther = DBData::NodeInfo_AlarmOther;
    NodeInfo_NodeValue = DBData::NodeInfo_NodeValue;
    NodeInfo_SaveTime = DBData::NodeInfo_SaveTime;

    DBData::NodeInfo_Count = 0;
    DBData::NodeInfo_NodeID.clear();
    DBData::NodeInfo_PositionID.clear();
    DBData::NodeInfo_DeviceName.clear();
    DBData::NodeInfo_NodeName.clear();
    DBData::NodeInfo_NodeAddr.clear();
    DBData::NodeInfo_NodeType.clear();
    DBData::NodeInfo_NodeClass.clear();
    DBData::NodeInfo_NodeSign.clear();
    DBData::NodeInfo_NodeUpper.clear();
    DBData::NodeInfo_NodeLimit.clear();
    DBData::NodeInfo_NodeMax.clear();
    DBData::NodeInfo_NodeMin.clear();
    DBData::NodeInfo_NodeRange.clear();
    DBData::NodeInfo_NodeEnable.clear();
    DBData::NodeInfo_NodeSound.clear();
    DBData::NodeInfo_NodeImage.clear();
    DBData::NodeInfo_SaveInterval.clear();
    DBData::NodeInfo_DotCount.clear();
    DBData::NodeInfo_AlarmDelay.clear();
    DBData::NodeInfo_AlarmType.clear();
    DBData::NodeInfo_NodeX.clear();
    DBData::NodeInfo_NodeY.clear();

    DBData::NodeInfo_Online.clear();    
    DBData::NodeInfo_AlarmUpper.clear();
    DBData::NodeInfo_AlarmLimit.clear();
    DBData::NodeInfo_AlarmOther.clear();
    DBData::NodeInfo_NodeValue.clear();
    DBData::NodeInfo_SaveTime.clear();

    //根据气种升序+位号升序排列
    QString sql = "select NodeID,PositionID,DeviceName,NodeName,NodeAddr,NodeType,NodeClass,NodeSign,NodeUpper,NodeLimit,NodeMax,NodeMin,NodeRange,"
                  "NodeEnable,NodeSound,NodeImage,SaveInterval,DotCount,AlarmDelay,AlarmType,NodeX,NodeY from NodeInfo order by NodeClass asc,NodeID asc";
    QSqlQuery query;
    if (!query.exec(sql)) {
        return;
    }

    while(query.next()) {
        int nodeID = query.value(0).toInt();
        QString positionID = query.value(1).toString();
        QString deviceName = query.value(2).toString();
        QString nodeName = query.value(3).toString();
        quint16 nodeAddr = query.value(4).toInt();
        QString nodeType = query.value(5).toString();
        QString nodeClass = query.value(6).toString();
        QString nodeSign = query.value(7).toString();
        float nodeUpper = query.value(8).toFloat();
        float nodeLimit = query.value(9).toFloat();
        float nodeMax = query.value(10).toFloat();
        float nodeMin = query.value(11).toFloat();
        float nodeRange = query.value(12).toFloat();
        QString nodeEnable = query.value(13).toString();
        QString nodeSound = query.value(14).toString();
        QString nodeImage = query.value(15).toString();
        int saveInterval = query.value(16).toInt();
        int dotCount = query.value(17).toInt();
        int alarmDelay = query.value(18).toInt();
        QString alarmType = query.value(19).toString();
        int nodeX = query.value(20).toInt();
        int nodeY = query.value(21).toInt();

        //过滤禁用的探测器
        if (nodeEnable == "禁用") {
            continue;
        }

        DBData::NodeInfo_Count++;
        DBData::NodeInfo_NodeID.append(nodeID);
        DBData::NodeInfo_PositionID.append(positionID);
        DBData::NodeInfo_DeviceName.append(deviceName);
        DBData::NodeInfo_NodeName.append(nodeName);
        DBData::NodeInfo_NodeAddr.append(nodeAddr);
        DBData::NodeInfo_NodeType.append(nodeType);
        DBData::NodeInfo_NodeClass.append(nodeClass);
        DBData::NodeInfo_NodeSign.append(nodeSign);
        DBData::NodeInfo_NodeUpper.append(nodeUpper);
        DBData::NodeInfo_NodeLimit.append(nodeLimit);
        DBData::NodeInfo_NodeMax.append(nodeMax);
        DBData::NodeInfo_NodeMin.append(nodeMin);
        DBData::NodeInfo_NodeRange.append(nodeRange);
        DBData::NodeInfo_NodeEnable.append(nodeEnable);
        DBData::NodeInfo_NodeSound.append(nodeSound);
        DBData::NodeInfo_NodeImage.append(nodeImage);
        DBData::NodeInfo_SaveInterval.append(saveInterval);
        DBData::NodeInfo_DotCount.append(dotCount);
        DBData::NodeInfo_AlarmDelay.append(alarmDelay);
        DBData::NodeInfo_AlarmType.append(alarmType);
        DBData::NodeInfo_NodeX.append(nodeX);
        DBData::NodeInfo_NodeY.append(nodeY);

        DBData::NodeInfo_Online.append(false);
        DBData::NodeInfo_AlarmUpper.append(false);
        DBData::NodeInfo_AlarmLimit.append(false);
        DBData::NodeInfo_AlarmOther.append(false);
        DBData::NodeInfo_NodeValue.append(0);
        DBData::NodeInfo_SaveTime.append(QDateTime::currentDateTime());
    }

    //重新更新值和状态等
    for (int i = 0; i < DBData::NodeInfo_Count; i++) {
        QString positionID = DBData::NodeInfo_PositionID.at(i);
        int index = NodeInfo_PositionID.indexOf(positionID);
        if (index >= 0) {
            DBData::NodeInfo_Online[i] = NodeInfo_Online.at(index);
            DBData::NodeInfo_AlarmUpper[i] = NodeInfo_AlarmUpper.at(index);
            DBData::NodeInfo_AlarmLimit[i] = NodeInfo_AlarmLimit.at(index);
            DBData::NodeInfo_AlarmOther[i] = NodeInfo_AlarmOther.at(index);
            DBData::NodeInfo_NodeValue[i] = NodeInfo_NodeValue.at(index);
            DBData::NodeInfo_SaveTime[i] = NodeInfo_SaveTime.at(index);
        }
    }
}

void DBHelper::clearNodeInfo()
{
    QString sql = "delete from NodeInfo";
    execSql(sql);
}

void DBHelper::deleteNodeInfo(const QString &deviceName)
{
    QString sql = QString("delete from NodeInfo where DeviceName='%1'").arg(deviceName);
    execSql(sql);
}

void DBHelper::addNodeLog(const QString &positionID, const QString &deviceName,
                          const QString &nodeName, float nodeValue,
                          const QString &nodeSign, const QString &saveTime)
{
    QString sql = "insert into NodeLog(PositionID,DeviceName,NodeName,NodeValue,NodeSign,SaveTime) values('";
    sql += positionID + "','";
    sql += deviceName + "','";
    sql += nodeName + "','";
    sql += QString::number(nodeValue) + "','";
    sql += nodeSign + "','";
    sql += saveTime + "')";
    execSql(sql);
}

void DBHelper::deleteNodeLog(const QString &timeStart, const QString &timeEnd)
{
    QString sql = "delete from NodeLog where 1=1";
    if (App::LocalDBType.toUpper() == "SQLITE") {
        sql += " and datetime(SaveTime)>='" + timeStart + "'";
        sql += " and datetime(SaveTime)<='" + timeEnd + "'";
    } else if (App::LocalDBType.toUpper() == "MYSQL") {
        sql += " and unix_timestamp(SaveTime)>=unix_timestamp('" + timeStart + "')";
        sql += " and unix_timestamp(SaveTime)<=unix_timestamp('" + timeEnd + "')";
    }

    execSql(sql);
}

void DBHelper::clearNodeLog()
{
    clearData("NodeLog", App::LocalDBType);
}

void DBHelper::loadAlarmLink()
{
    DBData::AlarmLink_Count = 0;
    DBData::AlarmLink_PositionID.clear();
    DBData::AlarmLink_ComName.clear();
    DBData::AlarmLink_BaudRate.clear();
    DBData::AlarmLink_ModelAddr.clear();
    DBData::AlarmLink_LinkAddr.clear();
    DBData::AlarmLink_LinkEnable.clear();

    QSqlQuery query;
    QString sql = "select PositionID,ComName,BaudRate,ModelAddr,LinkAddr,LinkEnable from AlarmLink order by PositionID asc";
    if (!query.exec(sql)) {
        return;
    }

    while(query.next()) {
        QString positionID = query.value(0).toString();
        QString comName = query.value(1).toString();
        int baudRate = query.value(2).toInt();
        quint8 modelAddr = query.value(3).toInt();
        QString linkAddr = query.value(4).toString();
        QString linkEnable = query.value(5).toString();

        //过滤禁用的报警联动
        if (linkEnable == "禁用") {
            continue;
        }

        DBData::AlarmLink_Count++;
        DBData::AlarmLink_PositionID.append(positionID);
        DBData::AlarmLink_ComName.append(comName);
        DBData::AlarmLink_BaudRate.append(baudRate);
        DBData::AlarmLink_ModelAddr.append(modelAddr);
        DBData::AlarmLink_LinkAddr.append(linkAddr);
        DBData::AlarmLink_LinkEnable.append(linkEnable);
    }
}

void DBHelper::clearAlarmLink()
{
    QString sql = "delete from AlarmLink";
    execSql(sql);
}

void DBHelper::loadTypeInfo()
{
    DBData::TypeInfo_Count = 0;
    DBData::TypeInfo_TypeID.clear();
    DBData::TypeInfo_DeviceType.clear();
    DBData::TypeInfo_NodeNumber.clear();
    DBData::TypeInfo_NodeType.clear();
    DBData::TypeInfo_NodeClass.clear();
    DBData::TypeInfo_NodeSign.clear();

    QSqlQuery query;
    QString sql = "select TypeID,DeviceType,NodeNumber,NodeType,NodeClass,NodeSign from TypeInfo";
    if (!query.exec(sql)) {
        return;
    }

    while(query.next()) {
        int typeID = query.value(0).toInt();
        QString deviceType = query.value(1).toString();
        int nodeNumber = query.value(2).toInt();
        QString nodeType = query.value(3).toString();
        QString nodeClass = query.value(4).toString();
        QString nodeSign = query.value(5).toString();

        DBData::TypeInfo_Count++;
        DBData::TypeInfo_TypeID.append(typeID);

        //需要过滤空的
        if (!deviceType.isEmpty()) {
            DBData::TypeInfo_DeviceType.append(deviceType);
            DBData::TypeInfo_NodeNumber.append(nodeNumber);
        }

        if (!nodeType.isEmpty()) {
            DBData::TypeInfo_NodeType.append(nodeType);
        }

        if (!nodeClass.isEmpty()) {
            DBData::TypeInfo_NodeClass.append(nodeClass);
        }

        if (!nodeSign.isEmpty()) {
            DBData::TypeInfo_NodeSign.append(nodeSign);
        }
    }
}

void DBHelper::clearTypeInfo()
{
    QString sql = "delete from TypeInfo";
    execSql(sql);
}

void DBHelper::addAlarmLog(const QString &positionID, const QString &deviceName,
                           const QString &nodeName, float nodeValue, const QString &nodeSign,
                           const QString &content, const QString &startTime)
{
    QString sql = "insert into AlarmLog(PositionID,DeviceName,NodeName,NodeValue,NodeSign,"
                  "Content,StartTime,ConfirmUser,ConfirmTime,ConfirmContent) values('";
    sql += positionID + "','";
    sql += deviceName + "','";
    sql += nodeName + "','";
    sql += QString::number(nodeValue) + "','";
    sql += nodeSign + "','";
    sql += content + "','";
    sql += startTime + "','";
    sql += App::CurrentUserName + "','";
    sql += QString(DATETIME) + "','";

    //如果开启了自动确认则自动添加确认信息
    if (App::AutoConfirm) {
        sql += QString("已确认并解决") + "')";
    } else {
        sql += QString("") + "')";
    }

    execSql(sql);
}

void DBHelper::confirmAlarmLog(const QString &logID, const QString &confirmContent)
{
    QString sql = QString("update AlarmLog set ConfirmUser='%1',ConfirmTime='%2',ConfirmContent='%3' where LogID='%4'")
                  .arg(App::CurrentUserName).arg(DATETIME).arg(confirmContent).arg(logID);
    execSql(sql);
}

void DBHelper::updateAlarmLog(const QString &positionID, const QString &content, const QString &endTime)
{
    //找到最近的该设备该种类型报警的LogID
    QString sql = QString("select LogID,EndTime from AlarmLog where PositionID='%1' and Content='%2' order by LogID desc limit 1")
                  .arg(positionID).arg(content);
    QSqlQuery query;
    if (!query.exec(sql)) {
        return;
    }

    if (query.next()) {
        QString logID = query.value(0).toString();
        //如果存在结束时间说明已经恢复过不需要恢复
        if (query.value(1).toString().isEmpty()) {
            sql = QString("update AlarmLog set EndTime='%1' where LogID='%2'").arg(endTime).arg(logID);
            query.clear();
            query.exec(sql);
        }
    }
}

void DBHelper::deleteAlarmLog(const QString &timeStart, const QString &timeEnd)
{
    QString sql = "delete from AlarmLog where 1=1";
    if (App::LocalDBType.toUpper() == "SQLITE") {
        sql += " and datetime(StartTime)>='" + timeStart + "'";
        sql += " and datetime(StartTime)<='" + timeEnd + "'";
    } else if (App::LocalDBType.toUpper() == "MYSQL") {
        sql += " and unix_timestamp(StartTime)>=unix_timestamp('" + timeStart + "')";
        sql += " and unix_timestamp(StartTime)<=unix_timestamp('" + timeEnd + "')";
    }

    execSql(sql);
}

void DBHelper::deleteAlarmLog(const QString &ids)
{
    //合并成一条语句删除
    QString sql = QString("delete from AlarmLog where LogID in (%1)").arg(ids);
    QSqlQuery query;
    query.exec(sql);
}

void DBHelper::clearAlarmLog()
{
    clearData("AlarmLog", App::LocalDBType);
}

void DBHelper::addUserLog(const QString &triggerType,
                          const QString &triggerContent)
{
    QString sql = "insert into UserLog(TriggerTime,UserName,UserType,TriggerType,TriggerContent) values('";
    sql += QString("%1").arg(DATETIME) + "','";
    sql += App::CurrentUserName + "','";
    sql += App::CurrentUserType + "','";
    sql += triggerType + "','";
    sql += triggerContent + "')";
    execSql(sql);
}

void DBHelper::addUserLog(const QString &triggerContent)
{
    addUserLog("用户操作", triggerContent);
}

void DBHelper::deleteUserLog(const QString &timeStart, const QString &timeEnd)
{
    QString sql = "delete from UserLog where 1=1";
    if (App::LocalDBType.toUpper() == "SQLITE") {
        sql += " and datetime(TriggerTime)>='" + timeStart + "'";
        sql += " and datetime(TriggerTime)<='" + timeEnd + "'";
    } else if (App::LocalDBType.toUpper() == "MYSQL") {
        sql += " and unix_timestamp(TriggerTime)>=unix_timestamp('" + timeStart + "')";
        sql += " and unix_timestamp(TriggerTime)<=unix_timestamp('" + timeEnd + "')";
    }

    execSql(sql);
}

void DBHelper::clearUserLog()
{
    clearData("UserLog", App::LocalDBType);
}

void DBHelper::clearNodeData()
{
    clearData("NodeData", App::LocalDBType);
}

void DBHelper::updatePosition(ButtonDefence *btn)
{
    QString sql = QString("update NodeInfo set NodeX='%1',NodeY='%2' where NodeID='%3'")
                  .arg(btn->geometry().x()).arg(btn->geometry().y()).arg(btn->property("nodeID").toString());
    execSql(sql);
}

void DBHelper::execSql(const QString &sql)
{
    QSqlQuery query;
    query.exec(sql);
}

void DBHelper::clearData(const QString &tableName, const QString &dbType)
{
    QString sql;
    QSqlQuery query;
    if (dbType.toUpper() == "SQLITE") {
        //SQLite数据库自增列信息存储在 sqlite_sequence 表,表包含两个列：name记录自增列所在的表,seq记录当前序号
        sql = QString("delete from %1").arg(tableName);
        query.clear();
        query.exec(sql);

        sql = QString("delete from sqlite_sequence where name='%1'").arg(tableName);
        query.clear();
        query.exec(sql);
    } else if (dbType.toUpper() == "MYSQL") {
        //重置自增主键,用 truncate table table_name 效率高,而不是 alter table table_name auto_increment = 1
        sql = QString("truncate table %1").arg(tableName);
        query.clear();
        query.exec(sql);
    }
}
