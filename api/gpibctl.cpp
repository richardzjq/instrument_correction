
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <iostream>
#include <string>
#include "C://Program Files//IVI Foundation//VISA//WinNT//Include//visa.h"
#pragma comment(lib,"C://Program Files//IVI Foundation//VISA//WinNT//lib//msc//visa32.lib")
//#pragma comment(lib,"C://Program Files//IVI Foundation//VISA//WinNT//lib//bc//visa32.lib")


using namespace std;


ViStatus AutoConnectGPIB(ViPSession instrSesn)
{
    ViSession defaultRM,vi;
    ViStatus status;
    ViFindList  find_list;
	ViUInt32 retCnt = 0;
    char buf [256] = {0};
    ViChar  instrDesc[100]={0};

	//打来默认资源管理资源，返回值是状态status,输入参数是资源指针&defaultRM
	status = viOpenDefaultRM (&defaultRM);
	//错误处理，VI_SUCCESS值为0
    if (status < VI_SUCCESS)
    {
     //MessageBox(TEXT("打开默认资源管理资源失败"));
	  return status;
    }
   
	//查找资源，返回值是状态status，其中输入参数"GPIB?*INSTR"表示所有GPIB的地址字符串
	status = viFindRsrc(defaultRM,"GPIB?*INSTR",&find_list, &retCnt, instrDesc);
	//错误处理
    if (status < VI_SUCCESS)
    {
      //MessageBox(TEXT("查找端口失败"));
	  viClose(defaultRM);
	  return status;
    }
   
	//打开指定资源的会话，返回值status，其中输入参数&vi为会话资源指针
    status = viOpen(defaultRM,instrDesc,VI_NULL,VI_NULL, &vi); 
	//错误处理
    if (status < VI_SUCCESS)
    {
  //   MessageBox("打开端口资源失败");
	 viClose(defaultRM);
	 return status;
    }

	viPrintf(vi, "*CLS\n");   //清除仪表中命令
	viPrintf(vi, "*RST\n");   //发送复位命令
	char receiveBufferArrary[256] = {0};  //发送读取设备信息命令
	viPrintf(vi, "*IDN?\n"); 
	viScanf(vi, "%t", &receiveBufferArrary);
	//输出仪表信息,分别代表制造商、产品型号、产品序列号以及固件版本号
	//printf("device infomation: %s \n",receiveBufferArrary);
	*instrSesn = vi;
	return VI_SUCCESS;
}

ViStatus CloseConnectGPIB(ViPSession instrSesn)
{
	ViStatus status;
	status = viClose(*instrSesn);
}

//直流电流设置
ViStatus Set_DC_Current(ViPSession m_ViSession , double Current, string Unit_Current)
{
	if(Unit_Current == "mA")
	{
		Current*=1e-3;
		//printf("c= %f ma",Current);
	}
	else if(Unit_Current == "uA")
	{
		Current*=1e-6;
		//printf("c= %f uA",Current);
	}

	ViStatus status;
	char receiveResultArrary[256] = {0};
	//sprintf(s_EditCurrent, "%f", Current); 

	status = viPrintf(m_ViSession, "OUT %f A \n",Current); 
	//viPrintf(m_ViSession, "OUT?\n"); 
	//status = viPrintf(*m_ViSession, "ADC \n"); 
	//status = viPrintf(*m_ViSession, "MEAS1? \n"); 
	//viScanf(*m_ViSession, "%t", &res);

    if (status < VI_SUCCESS)
    {
      //MessageBox(TEXT("设置直流电流失败"));
      viClose (*m_ViSession);
	  return status;
    } 
	//printf("device infomation: %f \n",receiveResultArrary);
	return VI_SUCCESS;
 } 

//直流电压设置
ViStatus Set_DC_Voltage(ViPSession m_ViSession , double Voltage, string Unit_Voltage)
{
	if(Unit_Voltage == "mV")
	{
		Voltage*=1e-3;
	}
	else if(Unit_Current == "uV")
	{
		Voltage*=1e-6;
	}

	ViStatus status;
	char receiveResultArrary[256] = {0};

	status = viPrintf(m_ViSession, "OUT %f V \n",Voltage); 
    if (status < VI_SUCCESS)
    {
      viClose (*m_ViSession);
	  return status;
    } 

	return VI_SUCCESS;
 } 

//交流电流设置
ViStatus Set_AC_Current(ViPSession m_ViSession , double Current, string Unit_Current，int Frequency,string Unit_Frequency)
{
	if(Unit_Current == "mA")
	{
		Current*=1e-3;
	}
	else if(Unit_Current == "uA")
	{
		Current*=1e-6;
	}

	if(Unit_Frequency == "KHz")
	{
		Frequency*=1e3;
	}
	else if(Unit_Frequency == "MHz")
	{
		Frequency*=1e6;
	}

	ViStatus status;
	char receiveResultArrary[256] = {0};
	status = viPrintf(m_ViSession, "OUT %f A ,%d HZ\n",Current,Frequency); 
    if (status < VI_SUCCESS)
    {
      viClose (*m_ViSession);
	  return status;
    } 
	
	return VI_SUCCESS;
 } 

//交流电压设置
ViStatus Set_AC_Voltage(ViPSession m_ViSession , double Voltage, string Unit_Voltage，int Frequency,string Unit_Frequency)
{
	if(Unit_Voltage == "mV")
	{
		Voltage*=1e-3;
	}
	else if(Unit_Voltage == "uV")
	{
		Voltage*=1e-6;
	}

	if(Unit_Frequency == "KHz")
	{
		Frequency*=1e3;
	}
	else if(Unit_Frequency == "MHz")
	{
		Frequency*=1e6;
	}

	ViStatus status;
	char receiveResultArrary[256] = {0};
	status = viPrintf(m_ViSession, "OUT %f V,%d HZ\n",Voltage,Frequency); 
    if (status < VI_SUCCESS)
    {
      viClose (*m_ViSession);
	  return status;
    } 
	
	return VI_SUCCESS;
 } 

//电阻设置
ViStatus Set_Resistance(ViPSession m_ViSession , double Resistance, string Unit_Resistance)
{
	if(Unit_Capacitance == "Kohm")
	{
		Resistance*=1e3;
	}
	else if(Unit_Capacitance == "Mohm")
	{
		Resistance*=1e6;
	}
	
	ViStatus status;
	char receiveResultArrary[256] = {0};

	status = viPrintf(m_ViSession, "OUT %f OHM \n",Resistance); 
    if (status < VI_SUCCESS)
    {
      viClose (*m_ViSession);
	  return status;
    } 

	return VI_SUCCESS;
 } 

//电容设置
ViStatus Set_Capacitance(ViPSession m_ViSession , double Capacitance, string Unit_Capacitance)
{
	if(Unit_Capacitance == "mF")
	{
		Capacitance*=1e-3;
	}
	else if(Unit_Capacitance == "uF")
	{
		Capacitance*=1e-6;
	}
	else if(Unit_Capacitance == "nF")
	{
		Capacitance*=1e-9;
	}
	else if(Unit_Capacitance == "pF")
	{
		Capacitance*=1e-12;
	}

	ViStatus status;
	char receiveResultArrary[256] = {0};

	status = viPrintf(m_ViSession, "OUT %f F \n",Capacitance); 
    if (status < VI_SUCCESS)
    {
      viClose (*m_ViSession);
	  return status;
    } 

	return VI_SUCCESS;
 } 
