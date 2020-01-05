#include <QDebug>
#include "gpibctl.h"


ViStatus AutoConnectGPIB(ViPSession instrSesn)
{
    ViSession defaultRM,vi;
    ViStatus status;
    ViFindList  find_list;
	ViUInt32 retCnt = 0;
    ViChar  instrDesc[100]={0};

    qDebug() << __FUNCTION__;

	//����Ĭ����Դ������Դ������ֵ��״̬status,�����������Դָ��&defaultRM
	status = viOpenDefaultRM (&defaultRM);
	//������VI_SUCCESSֵΪ0
    if (status < VI_SUCCESS)
    {
        qDebug() << "��Ĭ����Դ������Դʧ��";
        return status;
    }
   
	//������Դ������ֵ��״̬status�������������"GPIB?*INSTR"��ʾ����GPIB�ĵ�ַ�ַ���
	status = viFindRsrc(defaultRM,"GPIB?*INSTR",&find_list, &retCnt, instrDesc);
	//������
    if (status < VI_SUCCESS)
    {
        qDebug() << "���Ҷ˿�ʧ��";
        viClose(defaultRM);

        return status;
    }
   
	//��ָ����Դ�ĻỰ������ֵstatus�������������&viΪ�Ự��Դָ��
    status = viOpen(defaultRM,instrDesc,VI_NULL,VI_NULL, &vi); 
	//������
    if (status < VI_SUCCESS)
    {
          qDebug() << "�򿪶˿���Դʧ��";
          viClose(defaultRM);

          return status;
    }

	viPrintf(vi, "*CLS\n");   //����Ǳ�������
	viPrintf(vi, "*RST\n");   //���͸�λ����
	char receiveBufferArrary[256] = {0};  //���Ͷ�ȡ�豸��Ϣ����
	viPrintf(vi, "*IDN?\n"); 
	viScanf(vi, "%t", &receiveBufferArrary);
	//����Ǳ���Ϣ,�ֱ���������̡���Ʒ�ͺš���Ʒ���к��Լ��̼��汾��
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

//ֱ����������
ViStatus Set_DC_Current(ViPSession m_ViSession, double Current)
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
        qDebug() << "����ֱ������ʧ��";

        return status;
    } 

    viScanf(*m_ViSession, "%t", buf);
    qDebug() << buf;

	return VI_SUCCESS;
 } 

//ֱ����ѹ����
ViStatus Set_DC_Voltage(ViPSession m_ViSession, double Voltage)
{
    qDebug() << __FUNCTION__ << "Voltage: " << Voltage;

	ViStatus status;

    status = viPrintf(*m_ViSession, "OUT %f V \n",Voltage);
    if (status < VI_SUCCESS)
    {
        qDebug() << "����ֱ����ʧ��";

        return status;
    } 

	return VI_SUCCESS;
 } 

//������������
ViStatus Set_AC_Current(ViPSession m_ViSession, double Current, int Frequency)
{
    qDebug() << __FUNCTION__ << "current: " << Current << "frequency" << Frequency;

    ViStatus status = viPrintf(*m_ViSession, "OUT %f A ,%d HZ\n", Current, Frequency);
    if (status < VI_SUCCESS)
    {
        qDebug() << "����ֱ����ʧ��";
        return status;
    } 
	
	return VI_SUCCESS;
 } 

//������ѹ����
ViStatus Set_AC_Voltage(ViPSession m_ViSession, double Voltage, int Frequency)
{
    qDebug() << __FUNCTION__ << "Voltage: " << Voltage << "frequency" << Frequency;

	ViStatus status;
    status = viPrintf(*m_ViSession, "OUT %f V,%d HZ\n", Voltage, Frequency);
    if (status < VI_SUCCESS)
    {
        qDebug() << "����ֱ����ʧ��";
        return status;
    } 
	
	return VI_SUCCESS;
 } 

//��������
ViStatus Set_Resistance(ViPSession m_ViSession, double Resistance)
{
    qDebug() << __FUNCTION__ << "Resistance: " << Resistance;

	ViStatus status;
    status = viPrintf(*m_ViSession, "OUT %f F \n",Resistance);
    if (status < VI_SUCCESS)
    {
        qDebug() << "����ֱ����ʧ��";
        return status;
    } 

	return VI_SUCCESS;
 } 
