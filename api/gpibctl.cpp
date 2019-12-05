
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

	//����Ĭ����Դ������Դ������ֵ��״̬status,�����������Դָ��&defaultRM
	status = viOpenDefaultRM (&defaultRM);
	//������VI_SUCCESSֵΪ0
    if (status < VI_SUCCESS)
    {
     //MessageBox(TEXT("��Ĭ����Դ������Դʧ��"));
	  return status;
    }
   
	//������Դ������ֵ��״̬status�������������"GPIB?*INSTR"��ʾ����GPIB�ĵ�ַ�ַ���
	status = viFindRsrc(defaultRM,"GPIB?*INSTR",&find_list, &retCnt, instrDesc);
	//������
    if (status < VI_SUCCESS)
    {
      //MessageBox(TEXT("���Ҷ˿�ʧ��"));
	  viClose(defaultRM);
	  return status;
    }
   
	//��ָ����Դ�ĻỰ������ֵstatus�������������&viΪ�Ự��Դָ��
    status = viOpen(defaultRM,instrDesc,VI_NULL,VI_NULL, &vi); 
	//������
    if (status < VI_SUCCESS)
    {
  //   MessageBox("�򿪶˿���Դʧ��");
	 viClose(defaultRM);
	 return status;
    }

	viPrintf(vi, "*CLS\n");   //����Ǳ�������
	viPrintf(vi, "*RST\n");   //���͸�λ����
	char receiveBufferArrary[256] = {0};  //���Ͷ�ȡ�豸��Ϣ����
	viPrintf(vi, "*IDN?\n"); 
	viScanf(vi, "%t", &receiveBufferArrary);
	//����Ǳ���Ϣ,�ֱ���������̡���Ʒ�ͺš���Ʒ���к��Լ��̼��汾��
	//printf("device infomation: %s \n",receiveBufferArrary);
	*instrSesn = vi;
	return VI_SUCCESS;
}

ViStatus CloseConnectGPIB(ViPSession instrSesn)
{
	ViStatus status;
	status = viClose(*instrSesn);
}

//ֱ����������
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
      //MessageBox(TEXT("����ֱ������ʧ��"));
      viClose (*m_ViSession);
	  return status;
    } 
	//printf("device infomation: %f \n",receiveResultArrary);
	return VI_SUCCESS;
 } 

//ֱ����ѹ����
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

//������������
ViStatus Set_AC_Current(ViPSession m_ViSession , double Current, string Unit_Current��int Frequency,string Unit_Frequency)
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

//������ѹ����
ViStatus Set_AC_Voltage(ViPSession m_ViSession , double Voltage, string Unit_Voltage��int Frequency,string Unit_Frequency)
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

//��������
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

//��������
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
