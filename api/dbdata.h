#ifndef DBDATA_H
#define DBDATA_H

#include <QStringList>
#include <QDateTime>

class DBData
{
public:

    //用户信息链表
    static int UserInfo_Count;
    static QList<QString> UserInfo_UserName;
    static QList<QString> UserInfo_UserPwd;
    static QList<QString> UserInfo_UserType;
    static QList<QString> UserInfo_UserAdmin;

    //端口信息表
    static int PortInfo_Count;
    static QList<int> PortInfo_PortID;
    static QList<QString> PortInfo_PortName;
    static QList<QString> PortInfo_ComName;
    static QList<int> PortInfo_BaudRate;
    static QList<QString> PortInfo_TcpIP;
    static QList<int> PortInfo_TcpPort;
    static QList<float> PortInfo_ReadInterval;
    static QList<int> PortInfo_ReadTimeout;
    static QList<int> PortInfo_ReadMaxtime;

    //设备信息表
    static int DeviceInfo_Count;
    static QList<int> DeviceInfo_DeviceID;
    static QList<QString> DeviceInfo_PortName;
    static QList<QString> DeviceInfo_DeviceName;
    static QList<quint8> DeviceInfo_DeviceAddr;
    static QList<QString> DeviceInfo_DeviceType;
    static QList<int> DeviceInfo_NodeNumber;

    //探测器信息表
    static int NodeInfo_Count;
    static QList<int> NodeInfo_NodeID;
    static QList<QString> NodeInfo_PositionID;
    static QList<QString> NodeInfo_DeviceName;
    static QList<QString> NodeInfo_NodeName;
    static QList<quint16> NodeInfo_NodeAddr;
    static QList<QString> NodeInfo_NodeType;
    static QList<QString> NodeInfo_NodeClass;
    static QList<QString> NodeInfo_NodeSign;
    static QList<float> NodeInfo_NodeUpper;
    static QList<float> NodeInfo_NodeLimit;
    static QList<float> NodeInfo_NodeMax;
    static QList<float> NodeInfo_NodeMin;
    static QList<float> NodeInfo_NodeRange;
    static QList<QString> NodeInfo_NodeEnable;
    static QList<QString> NodeInfo_NodeSound;
    static QList<QString> NodeInfo_NodeImage;
    static QList<int> NodeInfo_SaveInterval;
    static QList<int> NodeInfo_DotCount;
    static QList<int> NodeInfo_AlarmDelay;
    static QList<QString> NodeInfo_AlarmType;
    static QList<int> NodeInfo_NodeX;
    static QList<int> NodeInfo_NodeY;

    //探测器状态报警信息
    static QList<bool> NodeInfo_Online;
    static QList<bool> NodeInfo_AlarmUpper;
    static QList<bool> NodeInfo_AlarmLimit;
    static QList<bool> NodeInfo_AlarmOther;
    static QList<float> NodeInfo_NodeValue;
    static QList<QDateTime> NodeInfo_SaveTime;

    //联动信息表
    static int AlarmLink_Count;
    static QList<QString> AlarmLink_PositionID;
    static QList<QString> AlarmLink_ComName;
    static QList<int> AlarmLink_BaudRate;
    static QList<quint8> AlarmLink_ModelAddr;
    static QList<QString> AlarmLink_LinkAddr;
    static QList<QString> AlarmLink_LinkEnable;

    //类型信息表 控制器型号+探测器数量+探测器型号+气体种类+气体符号
    static int TypeInfo_Count;
    static QList<int> TypeInfo_TypeID;
    static QList<QString> TypeInfo_DeviceType;
    static QList<int> TypeInfo_NodeNumber;
    static QList<QString> TypeInfo_NodeType;
    static QList<QString> TypeInfo_NodeClass;
    static QList<QString> TypeInfo_NodeSign;

    //地图+声音等
    static QString MapPath;
    static QList<QString> MapNames;
    static QString SoundPath;
    static QList<QString> SoundNames;
    static QList<QString> PortNames;
};

#endif // DBDATA_H
