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

	//����Ĭ����Դ������Դ������ֵ��״̬status,�����������Դָ��&defaultRM
	status = viOpenDefaultRM (&defaultRM);
	//������VI_SUCCESSֵΪ0
    if (status < VI_SUCCESS)
    {
        qDebug() << "��Ĭ����Դ������Դʧ��" << "status: " << status;
        return status;
    }
   
	//������Դ������ֵ��״̬status�������������"GPIB?*INSTR"��ʾ����GPIB�ĵ�ַ�ַ���
    status = viFindRsrc(defaultRM,"GPIB?*INSTR",&find_list, &retCnt, instrDesc);
	//������
    if (status < VI_SUCCESS)
    {
        qDebug() << "������Դʧ��" << "status: " << status;
        viClose(defaultRM);

        return status;
    }
   
	//��ָ����Դ�ĻỰ������ֵstatus�������������&viΪ�Ự��Դָ��
    status = viOpen(defaultRM,instrDesc,VI_NULL,VI_NULL, &vi); 
	//������
    if (status < VI_SUCCESS)
    {
          qDebug() << "����Դʧ��" << "status: " << status;
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
        qDebug() << "����ֱ������ʧ��";

        return status;
    } 

    viScanf(*m_ViSession, "%t", buf);
    qDebug() << buf;

	return VI_SUCCESS;
 } 

//ֱ����ѹ����
ViStatus Set_DC_Voltage_Fluke_5502A(ViPSession m_ViSession, double Voltage)
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
ViStatus Set_AC_Current_Fluke_5502A(ViPSession m_ViSession, double Current, int Frequency)
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
ViStatus Set_AC_Voltage_Fluke_5502A(ViPSession m_ViSession, double Voltage, int Frequency)
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
ViStatus Set_Resistance_Fluke_5502A(ViPSession m_ViSession, double Resistance)
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

ViStatus AutoConnectGPIB_Agilent_34401A(ViPSession instrSesn, QString addr)
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
        qDebug() << "��Ĭ����Դ������Դʧ��" << "status: " << status;
        return status;
    }

    //������Դ������ֵ��״̬status�������������"GPIB?*INSTR"��ʾ����GPIB�ĵ�ַ�ַ���
    status = viFindRsrc(defaultRM,"GPIB?*INSTR",&find_list, &retCnt, instrDesc);
    //������
    if (status < VI_SUCCESS)
    {
        qDebug() << "������Դʧ��" << "status: " << status;
        viClose(defaultRM);

        return status;
    }

    //��ָ����Դ�ĻỰ������ֵstatus�������������&viΪ�Ự��Դָ��
    status = viOpen(defaultRM,instrDesc,VI_NULL,VI_NULL, &vi);
    //������
    if (status < VI_SUCCESS)
    {
          qDebug() << "����Դʧ��" << "status: " << status;
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

ViStatus get_instrument_value_gpib_34401A(ViPSession m_ViSession, int get_type, double* p_get_val)
{
    qDebug() << __FUNCTION__ << "get_type: " << get_type;

    ViStatus status;

    /* ������ */
    status = viPrintf(*m_ViSession, "SYST:REM\n");
    if (status < VI_SUCCESS)
    {
        qDebug() << "SYST:REM ʧ��";
        return status;
    }
    qt_sleep(30);

    //������ñ���ʾ����Ϣ
    status = viPrintf(*m_ViSession, "*CLS\n");
    if (status < VI_SUCCESS)
    {
        qDebug() << "*CLS ʧ��";
        return status;
    }
    qt_sleep(30);

    status = viPrintf(*m_ViSession, "TRIG:SOUR IMM\n");
    if (status < VI_SUCCESS)
    {
        qDebug() << "TRIG:SOUR IMM ʧ��";
        return status;
    }
    qt_sleep(30);

    /* ��ȡֵ */
    switch(get_type)
    {
        case DIRECT_VOLT_TYPE:
        case ALTERNATING_VOLT_TYPE:
            status = viPrintf(*m_ViSession, "MEAS:VOLT?\n");
            if (status < VI_SUCCESS)
            {
                qDebug() << "MEAS:VOLT? ʧ��";
                return status;
            }
            break;
        case DIRECT_CURRENT_TYPE:
        case ALTERNATING_CURRENT_TYPE:
            status = viPrintf(*m_ViSession, "MEAS:CURR?\n");
            if (status < VI_SUCCESS)
            {
                qDebug() << "MEAS:CURR? ʧ��";
                return status;
            }
            break;
        case RESISTANCE_TYPE:
            status = viPrintf(*m_ViSession, "MEAS:RES?\n");
            if (status < VI_SUCCESS)
            {
                qDebug() << "MEAS:RES? ʧ��";
                return status;
            }
            break;
        default:
            break;
    }
    qt_sleep(1000);

    //��ȡ�����ֵ
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
