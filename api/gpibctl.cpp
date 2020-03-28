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
ViStatus Set_DC_Current_Fluke_5502A(ViPSession m_ViSession, double Current)
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
ViStatus Set_DC_Voltage_Fluke_5502A(ViPSession m_ViSession, double Voltage)
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
ViStatus Set_AC_Current_Fluke_5502A(ViPSession m_ViSession, double Current, int Frequency)
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
ViStatus Set_AC_Voltage_Fluke_5502A(ViPSession m_ViSession, double Voltage, int Frequency)
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
ViStatus Set_Resistance_Fluke_5502A(ViPSession m_ViSession, double Resistance)
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

ViStatus get_instrument_value_gpib_34401A(ViPSession m_ViSession, int get_type, double* p_get_val)
{
    qDebug() << __FUNCTION__ << "get_type: " << get_type;

    ViStatus status;

    /* 发命令 */
    status = viPrintf(*m_ViSession, "SYST:REM\n");
    if (status < VI_SUCCESS)
    {
        qDebug() << "SYST:REM 失败";
        return status;
    }
    qt_sleep(30);

    //清除万用表显示板信息
    status = viPrintf(*m_ViSession, "*CLS\n");
    if (status < VI_SUCCESS)
    {
        qDebug() << "*CLS 失败";
        return status;
    }
    qt_sleep(30);

    status = viPrintf(*m_ViSession, "TRIG:SOUR IMM\n");
    if (status < VI_SUCCESS)
    {
        qDebug() << "TRIG:SOUR IMM 失败";
        return status;
    }
    qt_sleep(30);

    /* 读取值 */
    switch(get_type)
    {
        case DIRECT_VOLT_TYPE:
        case ALTERNATING_VOLT_TYPE:
            status = viPrintf(*m_ViSession, "MEAS:VOLT?\n");
            if (status < VI_SUCCESS)
            {
                qDebug() << "MEAS:VOLT? 失败";
                return status;
            }
            break;
        case DIRECT_CURRENT_TYPE:
        case ALTERNATING_CURRENT_TYPE:
            status = viPrintf(*m_ViSession, "MEAS:CURR?\n");
            if (status < VI_SUCCESS)
            {
                qDebug() << "MEAS:CURR? 失败";
                return status;
            }
            break;
        case RESISTANCE_TYPE:
            status = viPrintf(*m_ViSession, "MEAS:RES?\n");
            if (status < VI_SUCCESS)
            {
                qDebug() << "MEAS:RES? 失败";
                return status;
            }
            break;
        default:
            break;
    }
    qt_sleep(1000);

    //读取输出数值
    char receiveBufferArrary[256] = {0};
    viScanf(*m_ViSession, "%t", &receiveBufferArrary);
    QString str_data = QString(QLatin1String(receiveBufferArrary));
    double viResult = str_data.toDouble();
    *p_get_val = viResult;
    qDebug() << "receiveBufferArrary: " << receiveBufferArrary;
    qDebug() << "str_data: " << str_data;
    qDebug() << "viResult: " << viResult;

    return VI_SUCCESS;
}
