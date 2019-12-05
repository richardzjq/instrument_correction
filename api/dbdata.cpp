#include "dbdata.h"

//用户信息链表
int DBData::UserInfo_Count = 0;
QList<QString> DBData::UserInfo_UserName = QList<QString>();
QList<QString> DBData::UserInfo_UserPwd = QList<QString>();
QList<QString> DBData::UserInfo_UserType = QList<QString>();
QList<QString> DBData::UserInfo_UserAdmin = QList<QString>();

int DBData::PortInfo_Count = 0;
QList<int> DBData::PortInfo_PortID = QList<int>();
QList<QString> DBData::PortInfo_PortName = QList<QString>();
QList<QString> DBData::PortInfo_ComName = QList<QString>();
QList<int> DBData::PortInfo_BaudRate = QList<int>();
QList<QString> DBData::PortInfo_TcpIP = QList<QString>();
QList<int> DBData::PortInfo_TcpPort = QList<int>();
QList<float> DBData::PortInfo_ReadInterval = QList<float>();
QList<int> DBData::PortInfo_ReadTimeout = QList<int>();
QList<int> DBData::PortInfo_ReadMaxtime = QList<int>();

int DBData::DeviceInfo_Count = 0;
QList<int> DBData::DeviceInfo_DeviceID = QList<int>();
QList<QString> DBData::DeviceInfo_PortName = QList<QString>();
QList<QString> DBData::DeviceInfo_DeviceName = QList<QString>();
QList<quint8> DBData::DeviceInfo_DeviceAddr = QList<quint8>();
QList<QString> DBData::DeviceInfo_DeviceType = QList<QString>();
QList<int> DBData::DeviceInfo_NodeNumber = QList<int>();

int DBData::NodeInfo_Count = 0;
QList<int> DBData::NodeInfo_NodeID = QList<int>();
QList<QString> DBData::NodeInfo_PositionID = QList<QString>();
QList<QString> DBData::NodeInfo_DeviceName = QList<QString>();
QList<QString> DBData::NodeInfo_NodeName = QList<QString>();
QList<quint16> DBData::NodeInfo_NodeAddr = QList<quint16>();
QList<QString> DBData::NodeInfo_NodeType = QList<QString>();
QList<QString> DBData::NodeInfo_NodeClass = QList<QString>();
QList<QString> DBData::NodeInfo_NodeSign = QList<QString>();
QList<float> DBData::NodeInfo_NodeUpper = QList<float>();
QList<float> DBData::NodeInfo_NodeLimit = QList<float>();
QList<float> DBData::NodeInfo_NodeMax = QList<float>();
QList<float> DBData::NodeInfo_NodeMin = QList<float>();
QList<float> DBData::NodeInfo_NodeRange = QList<float>();
QList<QString> DBData::NodeInfo_NodeEnable = QList<QString>();
QList<QString> DBData::NodeInfo_NodeSound = QList<QString>();
QList<QString> DBData::NodeInfo_NodeImage = QList<QString>();
QList<int> DBData::NodeInfo_SaveInterval = QList<int>();
QList<int> DBData::NodeInfo_DotCount = QList<int>();
QList<int> DBData::NodeInfo_AlarmDelay = QList<int>();
QList<QString> DBData::NodeInfo_AlarmType = QList<QString>();
QList<int> DBData::NodeInfo_NodeX = QList<int>();
QList<int> DBData::NodeInfo_NodeY = QList<int>();

QList<bool> DBData::NodeInfo_Online = QList<bool>();
QList<bool> DBData::NodeInfo_AlarmUpper = QList<bool>();
QList<bool> DBData::NodeInfo_AlarmLimit = QList<bool>();
QList<bool> DBData::NodeInfo_AlarmOther = QList<bool>();
QList<float> DBData::NodeInfo_NodeValue = QList<float>();
QList<QDateTime> DBData::NodeInfo_SaveTime = QList<QDateTime>();

int DBData::AlarmLink_Count = 0;
QList<QString> DBData::AlarmLink_PositionID = QList<QString>();
QList<QString> DBData::AlarmLink_ComName = QList<QString>();
QList<int> DBData::AlarmLink_BaudRate = QList<int>();
QList<quint8> DBData::AlarmLink_ModelAddr = QList<quint8>();
QList<QString> DBData::AlarmLink_LinkAddr = QList<QString>();
QList<QString> DBData::AlarmLink_LinkEnable = QList<QString>();

int DBData::TypeInfo_Count = 0;
QList<int> DBData::TypeInfo_TypeID = QList<int>();
QList<QString> DBData::TypeInfo_DeviceType = QList<QString>();
QList<int> DBData::TypeInfo_NodeNumber = QList<int>();
QList<QString> DBData::TypeInfo_NodeType = QList<QString>();
QList<QString> DBData::TypeInfo_NodeClass = QList<QString>();
QList<QString> DBData::TypeInfo_NodeSign = QList<QString>();

QString DBData::MapPath = QString();
QList<QString> DBData::MapNames = QList<QString>();
QString DBData::SoundPath = QString();
QList<QString> DBData::SoundNames = QList<QString>();
QList<QString> DBData::PortNames = QList<QString>();
