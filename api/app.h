#ifndef APP_H
#define APP_H

#include "head.h"

class App
{
public:
    static QString ConfigFile;          //配置文件文件路径及名称

    //全局通用变量
    static int MapWidth;                //地图宽度
    static int MapHeight;               //地图高度
    static bool IsMove;                 //设备是否可以移动
    static bool DbError;                //数据库是否错误
    static QString CurrentImage;        //当前对应地图
    static QString CurrentUserName;     //当前用户名
    static QString CurrentUserPwd;      //当前用户密码
    static QString CurrentUserType;     //当前用户类型(值班员/管理员)
    static QString FileFilter;          //文件拓展名过滤
    static QString FileExtension;       //导出文件拓展名
    static QString FileSpliter;         //导出文件内容分隔符

    //全局布局宽度高度值
    static int RowHeight;               //行高
    static int LeftWidth;               //左边宽度
    static int RightWidth;              //右边宽度
    static int TopHeight;               //顶部高度
    static int BottomHeight;            //底部高度
    static int DeviceWidth;             //设备对象宽度
    static int DeviceHeight;            //设备对象高度

    static int BtnMinWidth;             //按钮最小宽度
    static int BtnMinHeight;            //左侧导航按钮最小高度
    static int BtnIconSize;             //按钮图标宽度高度
    static int BorderWidth;             //高亮边框宽度
    static int IconSize;                //图标字体大小
    static int IconWidth;               //图标宽度
    static int IconHeight;              //图标高度
    static int SwitchBtnWidth;          //开关按钮宽度
    static int SwitchBtnHeight;         //开关按钮高度

    //通用配置参数
    static bool AutoRun;                //开机自动运行
    static bool AutoPwd;                //记住密码
    static bool AutoLogin;              //自动登录
    static bool AutoConfirm;            //自动确认
    static bool SaveLog;                //启动日志调试
    static bool ShowTip;                //显示右下角弹框
    static int TipInterval;             //弹框显示的时间 单位秒 0表示一直显示除非手动关闭
    static int Precision;               //采集到的值的精确度
    static QString LogoCn;              //软件中文名称
    static QString LogoEn;              //软件英文名称
    static QString LogoBg;              //软件logo
    static QString Version;             //版本
    static QString Author;              //版权
    static QString StyleName;           //样式
    static QString LastLoginer;         //最后登录用户
    static QString LastFormMain;        //最后主界面
    static QString LastFormData;        //最后的数据查询界面
    static QString LastFormConfig;      //最后的系统设置界面

    //基本配置参数
    static int WorkMode;                //工作模式 0-从设备采集 1-从数据库采集
    static int PanelMode;               //面板模式
    static bool PlayAlarmSound;         //播放报警声音
    static int PlayCount;               //报警声音播放次数
    static int MsgCount;                //主界面警情状态条数
    static int DeviceColumn;            //主界面设备列数
    static int PageCount;               //表格分页时每页数据

    //记录控制配置参数
    static int AlarmLogMaxCount;        //报警记录最大数,单位万
    static int NodeLogMaxCount;         //运行记录最大数,单位万
    static int UserLogMaxCount;         //用户记录最大数,单位万
    static QString AlarmLogOrder;       //报警记录排序
    static QString NodeLogOrder;        //运行记录排序
    static QString UserLogOrder;        //用户记录排序

    //颜色配置参数
    static QString ColorZero;           //0值颜色
    static QString ColorUpper;          //高报颜色
    static QString ColorLimit;          //低报颜色
    static QString ColorNormal;         //正常颜色
    static QString ColorPlotBg;         //曲线背景颜色
    static QString ColorPlotText;       //曲线文字颜色
    static QString ColorPlotLine;       //曲线线条颜色
    static QString ColorOther;          //其他颜色

    //数据转发配置参数
    static bool UseNetSend;             //启用数据转发
    static QString NetSendInfo;         //转发ip地址+端口
    static bool UseNetReceive;          //启用数据接收
    static int NetReceivePort;          //数据接收端口

    //告警短信转发配置参数
    static QString MsgPortName;         //短信猫通信串口号
    static int MsgBaudRate;             //短信猫通信波特率
    static int MsgInterval;             //短信告警间隔
    static QString MsgTel;              //告警短信接收号码

    //报警邮件转发配置参数
    static QString SendEmailAddr;       //发件人地址
    static QString SendEmailPwd;        //发件人密码
    static int EmailInterval;           //邮件报警间隔
    static QString ReceiveEmailAddr;    //报警邮件接收地址

    //本地数据库配置
    static QString LocalDBType;         //本地数据库类型
    static QString LocalDBIP;           //本地数据库主机
    static int LocalDBPort;             //本地数据库端口
    static QString LocalDBName;         //本地数据库名称
    static QString LocalUserName;       //本地数据库用户名
    static QString LocalUserPwd;        //本地数据库密码

    //远程数据库配置参数
    static bool UseNetDB;               //启用远程数据同步
    static QString NetDBType;           //远程数据库类型
    static QString NetDBIP;             //远程数据库主机
    static int NetDBPort;               //远程数据库端口
    static QString NetDBName;           //远程数据库名称
    static QString NetUserName;         //远程数据库用户名
    static QString NetUserPwd;          //远程数据库密码

    //调试配置参数
    static int PortNameIndex;           //端口名称下拉框索引
    static int DeviceNameIndex;         //设备名称下拉框索引
    static QString KeyValue1;           //关键字1
    static QString KeyColor1;           //关键字颜色1
    static QString KeyValue2;           //关键字1
    static QString KeyColor2;           //关键字颜色1
    static QString SelectDirName;       //最后保存的文件夹

    //调试工具配置参数
    static QString DeviceType;          //设备类型
    static int DeviceAddr;              //设备地址
    static QString PortName;            //串口号
    static int BaudRate;                //波特率
    static int ListenPort;              //监听端口
    static QString NodeValues;          //节点的值集合
    static bool AutoData;               //自动模拟
    static int TabIndex;               //选项卡索引

    static void readConfig();           //读取配置文件,在main函数最开始加载程序载入
    static void writeConfig();          //写入配置文件,在更改配置文件程序关闭时调用
    static void newConfig();            //以初始值新建配置文件
    static bool checkConfig();          //校验配置文件

};

#endif // APP_H
