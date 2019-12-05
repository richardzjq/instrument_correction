#ifndef DBHELPER_H
#define DBHELPER_H

#include "quiwidget.h"
#include "buttondefence.h"

class DBHelper : public QObject
{
    Q_OBJECT
public:
    explicit DBHelper(QObject *parent = 0);

    //获取指定表名字段数据内容
    static QStringList getContent(const QString &tableName,
                                  const QString &columnNames,
                                  const QString &whereSql = QString(),
                                  const QString &title = QString(),
                                  const QString &spliter = QString(","));

    //导入数据文件
    static bool inputData(int columnCount,
                          const QString &filter,
                          const QString &tableName,
                          QString &fileName,
                          const QString &defaultDir = QCoreApplication::applicationDirPath(),
                          bool existTitle = false);
    //导出数据文件
    static bool outputData(const QString &defaultName,
                           const QString &filter,
                           const QStringList &content,
                           QString &fileName,
                           const QString &defaultDir = QCoreApplication::applicationDirPath());
    //导出数据
    static bool outputData(QString &fileName,
                           const QStringList &content);


    //载入用户信息
    static void loadUserInfo();
    //清空用户信息
    static void clearUserInfo();
    //更新密码
    static void updateUserInfo(const QString &userName,
                               const QString &userPwd);

    //取出最后一次登录用户的密码及类型
    static void getUserInfo(QString &userPwd,
                            QString &userType);
    //取出所有用户信息,包括用户名密码及类型
    static void getUserInfo(QStringList &userName,
                            QStringList &userPwd,
                            QStringList &userType);

    //载入端口信息
    static void loadPortInfo();
    //清空端口信息
    static void clearPortInfo();

    //载入设备信息
    static void loadDeviceInfo();
    //清空设备信息
    static void clearDeviceInfo();
    //删除指定端口设备
    static void deleteDeviceInfo(const QString &portName);

    //根据设备名称查询端口名称
    static QString getPortName(const QString &deviceName);
    //根据端口名称+设备地址获取对应的设备的名称
    static QString getDeviceName(const QString &portName, quint8 addr);
    //根据端口名称+设备地址获取对应的设备的探测器个数
    static int getNodeNumber(const QString &portName, quint8 addr);
    //根据端口名称+设备地址获取寄存器其实地址
    static quint16 getNodeMinAddr(const QString &portName, quint8 addr);

    //载入探测器信息
    static void loadNodeInfo();
    //清空探测器信息
    static void clearNodeInfo();
    //删除指定设备名称的探测器
    static void deleteNodeInfo(const QString &deviceName);

    //插入运行记录
    static void addNodeLog(const QString &positionID, const QString &deviceName,
                           const QString &nodeName, float nodeValue,
                           const QString &nodeSign, const QString &saveTime = DATETIME);
    //删除运行记录
    static void deleteNodeLog(const QString &timeStart, const QString &timeEnd);
    static void clearNodeLog();

    //载入报警联动信息
    static void loadAlarmLink();
    //清空报警联动信息
    static void clearAlarmLink();

    //载入类型信息
    static void loadTypeInfo();
    //清空类型信息
    static void clearTypeInfo();

    //插入报警记录
    static void addAlarmLog(const QString &positionID, const QString &deviceName,
                            const QString &nodeName, float nodeValue, const QString &nodeSign,
                            const QString &content, const QString &startTime);
    //确认报警记录
    static void confirmAlarmLog(const QString &positionID, const QString &confirmContent);
    //更新报警记录
    static void updateAlarmLog(const QString &positionID, const QString &content, const QString &endTime);
    //删除报警记录
    static void deleteAlarmLog(const QString &timeStart, const QString &timeEnd);
    //删除报警记录指定id号集合
    static void deleteAlarmLog(const QString &ids);
    //清空报警记录
    static void clearAlarmLog();

    //插入用户记录
    static void addUserLog(const QString &triggerType,
                           const QString &triggerContent);
    static void addUserLog(const QString &triggerContent);
    //删除用户记录
    static void deleteUserLog(const QString &timeStart, const QString &timeEnd);
    static void clearUserLog();

    //删除设备临时记录表
    static void clearNodeData();

    //更新按钮位置
    static void updatePosition(ButtonDefence *btn);
    //执行sql语句
    static void execSql(const QString &sql);
    //清空表数据并重置自增ID
    static void clearData(const QString &tableName, const QString &dbType);

};

#endif // DBHELPER_H
