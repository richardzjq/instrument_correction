#include <QDebug>
#include <QTime>
#include "gpibctl.h"
#include "device_mix_definition.h"


ViStatus AutoConnectGPIB_Fluke_5502A(ViPSession instrSesn, QString addr)
{
    ViSession defaultRM,vi;
    ViStatus status;
    ViFindList  find_list;
	ViUInt32 retCnt = 0;
    ViChar  instrDesc[100]={0};

    qDebug() << __FUNCTION__;

	//打来默认资源管理资源，返回值是状态status,输入参数是资源指针&defaultRM
	status = viOpenDefaultRM (&defaultRM);
	//错误处理，VI_SUCCESS值为0
    if (status < VI_SUCCESS)
    {
        qDebug() << "打开默认资源管理资源失败" << "status: " << status;
        return status;
    }
   
	//查找资源，返回值是状态status，其中输入参数"GPIB?*INSTR"表示所有GPIB的地址字符串
    status = viFindRsrc(defaultRM,"GPIB?*INSTR",&find_list, &retCnt, instrDesc);
	//错误处理
    if (status < VI_SUCCESS)
    {
        qDebug() << "查找资源失败" << "status: " << status;
        viClose(defaultRM);

        return status;
    }
   
	//打开指定资源的会话，返回值status，其中输入参数&vi为会话资源指针
    status = viOpen(defaultRM,instrDesc,VI_NULL,VI_NULL, &vi); 
	//错误处理
    if (status < VI_SUCCESS)
    {
          qDebug() << "打开资源失败" << "status: " << status;
          viClose(defaultRM);

          return status;
    }

	viPrintf(vi, "*CLS\n");   //清除仪表中命令
	viPrintf(vi, "*RST\n");   //发送复位命令
	char receiveBufferArrary[256] = {0};  //发送读取设备信息命令
	viPrintf(vi, "*IDN?\n"); 
	viScanf(vi, "%t", &receiveBufferArrary);
	//输出仪表信息,分别代表制造商、产品型号、产品序列号以及固件版本号
    qDebug() << "device infomation: " << receiveBufferArrary;
	*instrSesn = vi;

	return VI_SUCCESS;
}

ViStatus CloseConnectGPIB(ViPSession instrSesn)
{
    qDebug() << __FUNCTION__;
	ViStatus status;
	status = viClose(*instrSesn);

    return status;
}

//直流电流设置
ViStatus Set_DC_Current_GPIB_Fluke_5502A(ViPSession m_ViSession, double Current)
{
    qDebug() << __FUNCTION__ << "Current: " << Current;

    ViStatus status;
    char buf[256] = {0};

    status = viPrintf(*m_ViSession, "OUT %f A \n", Current);
    //viPrintf(m_ViSession, "OUT?\n");
    //status = viPrintf(*m_ViSession, "ADC \n");
    //status = viPrintf(*m_ViSession, "MEAS1? \n");
    //viScanf(*m_ViSession, "%t", &res);

    if (status < VI_SUCCESS)
    {
        qDebug() << "设置直流电流失败";

        return status;
    } 

    viScanf(*m_ViSession, "%t", buf);
    qDebug() << buf;

	return VI_SUCCESS;
 } 

//直流电压设置
ViStatus Set_DC_Voltage_GPIB_Fluke_5502A(ViPSession m_ViSession, double Voltage)
{
    qDebug() << __FUNCTION__ << "Voltage: " << Voltage;

	ViStatus status;

    status = viPrintf(*m_ViSession, "OUT %f V \n",Voltage);
    if (status < VI_SUCCESS)
    {
        qDebug() << "设置直流电失败";

        return status;
    } 

	return VI_SUCCESS;
 } 

//交流电流设置
ViStatus Set_AC_Current_GPIB_Fluke_5502A(ViPSession m_ViSession, double Current, int Frequency)
{
    qDebug() << __FUNCTION__ << "current: " << Current << "frequency" << Frequency;

    ViStatus status = viPrintf(*m_ViSession, "OUT %f A ,%d HZ\n", Current, Frequency);
    if (status < VI_SUCCESS)
    {
        qDebug() << "设置直流电失败";
        return status;
    } 
	
	return VI_SUCCESS;
 } 

//交流电压设置
ViStatus Set_AC_Voltage_GPIB_Fluke_5502A(ViPSession m_ViSession, double Voltage, int Frequency)
{
    qDebug() << __FUNCTION__ << "Voltage: " << Voltage << "frequency" << Frequency;

	ViStatus status;
    status = viPrintf(*m_ViSession, "OUT %f V,%d HZ\n", Voltage, Frequency);
    if (status < VI_SUCCESS)
    {
        qDebug() << "设置直流电失败";
        return status;
    } 
	
	return VI_SUCCESS;
 } 

//电阻设置
ViStatus Set_Resistance_GPIB_Fluke_5502A(ViPSession m_ViSession, double Resistance)
{
    qDebug() << __FUNCTION__ << "Resistance: " << Resistance;

	ViStatus status;
    status = viPrintf(*m_ViSession, "OUT %f F \n",Resistance);
    if (status < VI_SUCCESS)
    {
        qDebug() << "设置直流电失败";
        return status;
    } 

	return VI_SUCCESS;
 } 

ViStatus AutoConnectGPIB_Agilent_34401A(ViPSession instrSesn, QString addr)
{
    ViSession defaultRM,vi;
    ViStatus status;
    ViFindList  find_list;
    ViUInt32 retCnt = 0;
    ViChar  instrDesc[100]={0};

    qDebug() << __FUNCTION__;

    //打来默认资源管理资源，返回值是状态status,输入参数是资源指针&defaultRM
    status = viOpenDefaultRM (&defaultRM);
    //错误处理，VI_SUCCESS值为0
    if (status < VI_SUCCESS)
    {
        qDebug() << "打开默认资源管理资源失败" << "status: " << status;
        return status;
    }

    //查找资源，返回值是状态status，其中输入参数"GPIB?*INSTR"表示所有GPIB的地址字符串
    status = viFindRsrc(defaultRM,"GPIB?*INSTR",&find_list, &retCnt, instrDesc);
    //错误处理
    if (status < VI_SUCCESS)
    {
        qDebug() << "查找资源失败" << "status: " << status;
        viClose(defaultRM);

        return status;
    }

    //打开指定资源的会话，返回值status，其中输入参数&vi为会话资源指针
    status = viOpen(defaultRM,instrDesc,VI_NULL,VI_NULL, &vi);
    //错误处理
    if (status < VI_SUCCESS)
    {
          qDebug() << "打开资源失败" << "status: " << status;
          viClose(defaultRM);

          return status;
    }

    viPrintf(vi, "*CLS\n");   //清除仪表中命令
    viPrintf(vi, "*RST\n");   //发送复位命令
    char receiveBufferArrary[256] = {0};  //发送读取设备信息命令
    viPrintf(vi, "*IDN?\n");
    viScanf(vi, "%t", &receiveBufferArrary);
    //输出仪表信息,分别代表制造商、产品型号、产品序列号以及固件版本号
    qDebug() << "device infomation: " << receiveBufferArrary;
    *instrSesn = vi;

    return VI_SUCCESS;
}

#if 0
static void send_data_GPIB(int dev, QString str_send)
{
    ibwrt(dev, str_send.toLatin1(), str_send.size());
    qDebug() << str_send.size();
    qDebug() << str_send.toLatin1();
}

static void recv_data_GPIB(int dev, QString* p_str_recv)
{
    char recv_data[MAX_DATA_LENGTH];
    qint64 recv_size;
    int i;
    QString str_data;

    recv_size = ibrd(dev, recv_data, MAX_DATA_LENGTH);
    qDebug() << "read_size: " << recv_size;
    //for(i = 0; i < recv_size; i++)
    //{
    //   qDebug() << "sequence:  " << i << "data:  " << recv_data[i];
    //}

    *p_str_recv =  QString::fromUtf8(recv_data);

    qDebug() << *p_str_recv;
}

static void read_value_GPIB_2700(int dev, double* p_get_val)
{
    QString str_recv;
    double viResult;

    send_data_GPIB(dev, "TRIG:COUN 1\n"); //触发个数1，如果打开平均模式，触发个数大于1时，去所有触发的平均值，比如触发为5,2700自动测5次，然后输出平均值
    qt_sleep(100);

    send_data_GPIB(dev, "READ?\n"); 		 //写入读数指令
    qt_sleep(100);

    //读取输出数值
    recv_data_GPIB(dev, &str_recv);
    viResult = str_recv.toDouble();
    *p_get_val = viResult;
    qDebug() << "viResult: " << viResult;
}

void get_dv_value_GPIB_2700(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* 发命令 */
    send_data_GPIB(dev, "*RST\n"); //复位
    qt_sleep(100);

    send_data_GPIB(dev, "INIT:CONT OFF;:ABOR\n"); //内部触发关闭
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:FUNC 'VOLT:DC'\n"); //设置直流电压测量模式
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:VOLT:DC:RANG 10\n"); //电压测量范围10v,用SENS:CURR:DC:RANG:AUTO ON\n可以改为自动量程
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:VOLT:DC:NPLC 0.01\n"); 	//测试速度0.1PLC，1个PLC = 20ms
    qt_sleep(100);
    send_data_GPIB(dev, "DISP:ENAB ON\n"); 		//打开2700面板显示，OFF是关闭
    qt_sleep(100);
    send_data_GPIB(dev, "SYST:AZER:STAT OFF\r\n"); //关闭自动归零
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:CURR:DC:AVER:STAT OFF\r\n"); //关闭平均模式
    qt_sleep(1000);

    /* 读取值 */
    read_value_GPIB_2700(dev, p_get_val);
}

void get_av_value_GPIB_2700(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* 发命令 */
    send_data_GPIB(dev, "*RST\n"); //复位
    qt_sleep(100);

    send_data_GPIB(dev, "INIT:CONT OFF;:ABOR\n"); //内部触发关闭
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:FUNC 'VOLT:AC'\n"); //设置交流电压测量模式
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:VOLT:AC:RANG 10\n"); //电压测量范围10v,用SENS:CURR:DC:RANG:AUTO ON\n可以改为自动量程
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:VOLT:AC:NPLC 0.01\n"); 	//测试速度0.1PLC，1个PLC = 20ms
    qt_sleep(100);
    send_data_GPIB(dev, "DISP:ENAB ON\n"); 		//打开2700面板显示，OFF是关闭
    qt_sleep(100);
    send_data_GPIB(dev, "SYST:AZER:STAT OFF\r\n"); //关闭自动归零
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:CURR:AC:AVER:STAT OFF\r\n"); //关闭平均模式
    qt_sleep(1000);

    /* 读取值 */
    read_value_GPIB_2700(dev, p_get_val);
}

void get_dc_value_GPIB_2700(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* 发命令 */
    send_data_GPIB(dev, "*RST\n"); //复位
    qt_sleep(100);

    send_data_GPIB(dev, "INIT:CONT OFF;:ABOR\n"); //内部触发关闭
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:FUNC 'CURR:DC'\n"); //设置直流电流测量模式
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:CURR:DC:RANG 0.01\n"); //电流测量范围10mA,用SENS:CURR:DC:RANG:AUTO ON\n可以改为自动量程
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:CURR:DC:NPLC 0.01\n"); //测试速度0.1PLC，1个PLC = 20ms
    qt_sleep(100);
    send_data_GPIB(dev, "DISP:ENAB ON\n"); 		  //打开2700面板显示，OFF是关闭
    qt_sleep(100);
    send_data_GPIB(dev, "SYST:AZER:STAT OFF\r\n");  //关闭自动归零
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:CURR:DC:AVER:STAT OFF\r\n"); //关闭平均模式
    qt_sleep(1000);

    /* 读取值 */
    read_value_GPIB_2700(dev, p_get_val);
}

void get_ac_value_GPIB_2700(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* 发命令 */
    send_data_GPIB(dev, "*RST\n"); //复位
    qt_sleep(100);

    send_data_GPIB(dev, "INIT:CONT OFF;:ABOR\n"); //内部触发关闭
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:FUNC 'CURR:AC'\n"); //设置交流电流测量模式
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:CURR:AC:RANG 0.01\n"); //电流测量范围10mA,用SENS:CURR:AC:RANG:AUTO ON\n可以改为自动量程
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:CURR:AC:NPLC 0.01\n"); //测试速度0.1PLC，1个PLC = 20ms
    qt_sleep(100);
    send_data_GPIB(dev, "DISP:ENAB ON\n"); 		  //打开2700面板显示，OFF是关闭
    qt_sleep(100);
    send_data_GPIB(dev, "SYST:AZER:STAT OFF\r\n");  //关闭自动归零
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:CURR:AC:AVER:STAT OFF\r\n"); //关闭平均模式
    qt_sleep(1000);

    /* 读取值 */
    read_value_GPIB_2700(dev, p_get_val);
}

void get_res_value_GPIB_2700(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* 发命令 */
    send_data_GPIB(dev, "*RST\n"); //复位
    qt_sleep(100);

    send_data_GPIB(dev, "INIT:CONT OFF;:ABOR\n"); //内部触发关闭
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:FUNC 'RES'\n"); 	//设置直流电阻测量模式
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:RES:RANG 1E6\n");   //电阻测量范围1e6,用SENS:RES:RANG:AUTO ON\n可以改为自动量程
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:RES:NPLC 0.01\n");  //测试速度0.1PLC，1个PLC = 20m
    qt_sleep(100);
    send_data_GPIB(dev, "DISP:ENAB ON\n"); 		//打开2700面板显示，OFF是关闭
    qt_sleep(100);
    send_data_GPIB(dev, "SYST:AZER:STAT OFF\r\n"); //关闭自动归零
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:RES:AVER:STAT OFF\r\n"); //关闭平均模式
    qt_sleep(1000);

    /* 读取值 */
    read_value_GPIB_2700(dev, p_get_val);
}

static void read_value_GPIB_34401(int dev, double* p_get_val)
{
    QString str_recv;
    double viResult;

    send_data_GPIB(dev, "READ?\n"); 		 //写入读数指令
    qt_sleep(200); //多等待一会儿！

    //读取输出数值
    recv_data_GPIB(dev, &str_recv);
    viResult = str_recv.toDouble();
    *p_get_val = viResult;
    qDebug() << "viResult: " << viResult;
}

void get_dv_value_GPIB_34401(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* 发命令 */
    send_data_GPIB(dev, "*RST\n"); //复位
    qt_sleep(100);

    //send_data_GPIB(dev, "SYST:REM\n"); //远程模式
    //qt_sleep(100);

    send_data_GPIB(dev, "CONF:VOLT:DC\n"); //设置直流电压测量模式
    qt_sleep(100);
    send_data_GPIB(dev, "VOLT:RANG:AUTO ON\n"); //设置直流电压测量电压范围10  可以用“VOLT:RANG:AUTO ON”设置为自动量程
    qt_sleep(100);
    send_data_GPIB(dev, "VOLT:RES 0.01\n"); //测量精度0.00001/10V
    qt_sleep(100);
    send_data_GPIB(dev, "VOLT:NPLC 1\n"); //测试速度1 个PLC，1个PLC = 20ms，可选参数0.02,0.2,1,10,100
    qt_sleep(100);
    send_data_GPIB(dev, "DISP ON\n"); //打开34401面板显示，OFF是关闭
    qt_sleep(100);
    send_data_GPIB(dev, "TRIG:SOUR IMM\n");
    qt_sleep(100);

    /* 读取值 */
    read_value_GPIB_34401(dev, p_get_val);
}

void get_av_value_GPIB_34401(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* 发命令 */
    send_data_GPIB(dev, "*RST\n"); //复位
    qt_sleep(100);

    //send_data_GPIB(dev, "SYST:REM\n"); //远程模式
    //qt_sleep(100);

    send_data_GPIB(dev, "CONF:VOLT:AC\n"); //设置交流电压测量模式
    qt_sleep(100);
    send_data_GPIB(dev, "VOLT:RANG:AUTO ON\n"); //设置直流电压测量电压范围10  可以用“VOLT:RANG:AUTO ON”设置为自动量程
    qt_sleep(100);
    send_data_GPIB(dev, "VOLT:RES 0.01\n"); //测量精度0.00001/10V
    qt_sleep(100);
    send_data_GPIB(dev, "VOLT:NPLC 1\n"); //测试速度1 个PLC，1个PLC = 20ms，可选参数0.02,0.2,1,10,100
    qt_sleep(100);
    send_data_GPIB(dev, "DISP ON\n"); //打开34401面板显示，OFF是关闭
    qt_sleep(100);
    send_data_GPIB(dev, "TRIG:SOUR IMM\n");
    qt_sleep(100);

    /* 读取值 */
    read_value_GPIB_34401(dev, p_get_val);
}

void get_dc_value_GPIB_34401(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* 发命令 */
    send_data_GPIB(dev, "*RST\n"); //复位
    qt_sleep(100);

    //send_data_GPIB(dev, "SYST:REM\n"); //远程模式
    //qt_sleep(100);

    send_data_GPIB(dev, "CONF:CURR:DC\n"); //设置直流电流测量模式
    qt_sleep(100);
    send_data_GPIB(dev, "CURR:RANG:AUTO ON\n"); //设置直流电流测量电压范围自动
    qt_sleep(100);
    send_data_GPIB(dev, "CURR:RES MAX\n"); //测量精度最大  0.00001/10
    qt_sleep(100);
    send_data_GPIB(dev, "CURR:NPLC 1\n"); //测试速度1 个PLC，1个PLC = 20ms，可选参数0.02,0.2,1,10,100
    qt_sleep(100);
    send_data_GPIB(dev, "DISP ON\n"); //打开34401面板显示，OFF是关闭
    qt_sleep(100);
    send_data_GPIB(dev, "TRIG:SOUR IMM\n");
    qt_sleep(100);

    /* 读取值 */
    read_value_GPIB_34401(dev, p_get_val);
}

void get_ac_value_GPIB_34401(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* 发命令 */
    send_data_GPIB(dev, "*RST\n"); //复位
    qt_sleep(100);

    //send_data_GPIB(dev, "SYST:REM\n"); //远程模式
    //qt_sleep(100);

    send_data_GPIB(dev, "CONF:CURR:AC\n"); //设置交流电流测量模式
    qt_sleep(100);
    send_data_GPIB(dev, "CURR:RANG:AUTO ON\n"); //设置直流电流测量电压范围自动
    qt_sleep(100);
    send_data_GPIB(dev, "CURR:RES MAX\n"); //测量精度最大  0.00001/10
    qt_sleep(100);
    send_data_GPIB(dev, "CURR:NPLC 1\n"); //测试速度1 个PLC，1个PLC = 20ms，可选参数0.02,0.2,1,10,100
    qt_sleep(100);
    send_data_GPIB(dev, "DISP ON\n"); //打开34401面板显示，OFF是关闭
    qt_sleep(100);
    send_data_GPIB(dev, "TRIG:SOUR IMM\n");
    qt_sleep(100);

    /* 读取值 */
    read_value_GPIB_34401(dev, p_get_val);
}

void get_res_value_GPIB_34401(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* 发命令 */
    send_data_GPIB(dev, "*RST\n"); //复位
    qt_sleep(100);

    //send_data_GPIB(dev, "SYST:REM\n"); //远程模式
    //qt_sleep(100);

    send_data_GPIB(dev, "CONF:RES\n"); //设置电阻测量模式
    qt_sleep(100);
    send_data_GPIB(dev, "RES:RANG:AUTO ON\n"); //设置电阻测量电压范围自动
    qt_sleep(100);
    send_data_GPIB(dev, "RES:RES 0.01\n"); //测量精度0.00001/10V
    qt_sleep(100);
    send_data_GPIB(dev, "RES:NPLC 1\n"); //测试速度1 个PLC，1个PLC = 20ms，可选参数0.02,0.2,1,10,100
    qt_sleep(100);
    send_data_GPIB(dev, "DISP ON\n"); //打开34401面板显示，OFF是关闭
    qt_sleep(100);
    send_data_GPIB(dev, "TRIG:SOUR IMM\n");
    qt_sleep(100);

    /* 读取值 */
    read_value_GPIB_34401(dev, p_get_val);
}
#endif
