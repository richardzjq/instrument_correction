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
        qDebug() << "����ֱ������ʧ��";

        return status;
    } 

    viScanf(*m_ViSession, "%t", buf);
    qDebug() << buf;

	return VI_SUCCESS;
 } 

//ֱ����ѹ����
ViStatus Set_DC_Voltage_GPIB_Fluke_5502A(ViPSession m_ViSession, double Voltage)
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
ViStatus Set_AC_Current_GPIB_Fluke_5502A(ViPSession m_ViSession, double Current, int Frequency)
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
ViStatus Set_AC_Voltage_GPIB_Fluke_5502A(ViPSession m_ViSession, double Voltage, int Frequency)
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
ViStatus Set_Resistance_GPIB_Fluke_5502A(ViPSession m_ViSession, double Resistance)
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

    send_data_GPIB(dev, "TRIG:COUN 1\n"); //��������1�������ƽ��ģʽ��������������1ʱ��ȥ���д�����ƽ��ֵ�����紥��Ϊ5,2700�Զ���5�Σ�Ȼ�����ƽ��ֵ
    qt_sleep(100);

    send_data_GPIB(dev, "READ?\n"); 		 //д�����ָ��
    qt_sleep(100);

    //��ȡ�����ֵ
    recv_data_GPIB(dev, &str_recv);
    viResult = str_recv.toDouble();
    *p_get_val = viResult;
    qDebug() << "viResult: " << viResult;
}

void get_dv_value_GPIB_2700(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* ������ */
    send_data_GPIB(dev, "*RST\n"); //��λ
    qt_sleep(100);

    send_data_GPIB(dev, "INIT:CONT OFF;:ABOR\n"); //�ڲ������ر�
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:FUNC 'VOLT:DC'\n"); //����ֱ����ѹ����ģʽ
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:VOLT:DC:RANG 10\n"); //��ѹ������Χ10v,��SENS:CURR:DC:RANG:AUTO ON\n���Ը�Ϊ�Զ�����
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:VOLT:DC:NPLC 0.01\n"); 	//�����ٶ�0.1PLC��1��PLC = 20ms
    qt_sleep(100);
    send_data_GPIB(dev, "DISP:ENAB ON\n"); 		//��2700�����ʾ��OFF�ǹر�
    qt_sleep(100);
    send_data_GPIB(dev, "SYST:AZER:STAT OFF\r\n"); //�ر��Զ�����
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:CURR:DC:AVER:STAT OFF\r\n"); //�ر�ƽ��ģʽ
    qt_sleep(1000);

    /* ��ȡֵ */
    read_value_GPIB_2700(dev, p_get_val);
}

void get_av_value_GPIB_2700(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* ������ */
    send_data_GPIB(dev, "*RST\n"); //��λ
    qt_sleep(100);

    send_data_GPIB(dev, "INIT:CONT OFF;:ABOR\n"); //�ڲ������ر�
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:FUNC 'VOLT:AC'\n"); //���ý�����ѹ����ģʽ
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:VOLT:AC:RANG 10\n"); //��ѹ������Χ10v,��SENS:CURR:DC:RANG:AUTO ON\n���Ը�Ϊ�Զ�����
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:VOLT:AC:NPLC 0.01\n"); 	//�����ٶ�0.1PLC��1��PLC = 20ms
    qt_sleep(100);
    send_data_GPIB(dev, "DISP:ENAB ON\n"); 		//��2700�����ʾ��OFF�ǹر�
    qt_sleep(100);
    send_data_GPIB(dev, "SYST:AZER:STAT OFF\r\n"); //�ر��Զ�����
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:CURR:AC:AVER:STAT OFF\r\n"); //�ر�ƽ��ģʽ
    qt_sleep(1000);

    /* ��ȡֵ */
    read_value_GPIB_2700(dev, p_get_val);
}

void get_dc_value_GPIB_2700(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* ������ */
    send_data_GPIB(dev, "*RST\n"); //��λ
    qt_sleep(100);

    send_data_GPIB(dev, "INIT:CONT OFF;:ABOR\n"); //�ڲ������ر�
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:FUNC 'CURR:DC'\n"); //����ֱ����������ģʽ
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:CURR:DC:RANG 0.01\n"); //����������Χ10mA,��SENS:CURR:DC:RANG:AUTO ON\n���Ը�Ϊ�Զ�����
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:CURR:DC:NPLC 0.01\n"); //�����ٶ�0.1PLC��1��PLC = 20ms
    qt_sleep(100);
    send_data_GPIB(dev, "DISP:ENAB ON\n"); 		  //��2700�����ʾ��OFF�ǹر�
    qt_sleep(100);
    send_data_GPIB(dev, "SYST:AZER:STAT OFF\r\n");  //�ر��Զ�����
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:CURR:DC:AVER:STAT OFF\r\n"); //�ر�ƽ��ģʽ
    qt_sleep(1000);

    /* ��ȡֵ */
    read_value_GPIB_2700(dev, p_get_val);
}

void get_ac_value_GPIB_2700(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* ������ */
    send_data_GPIB(dev, "*RST\n"); //��λ
    qt_sleep(100);

    send_data_GPIB(dev, "INIT:CONT OFF;:ABOR\n"); //�ڲ������ر�
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:FUNC 'CURR:AC'\n"); //���ý�����������ģʽ
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:CURR:AC:RANG 0.01\n"); //����������Χ10mA,��SENS:CURR:AC:RANG:AUTO ON\n���Ը�Ϊ�Զ�����
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:CURR:AC:NPLC 0.01\n"); //�����ٶ�0.1PLC��1��PLC = 20ms
    qt_sleep(100);
    send_data_GPIB(dev, "DISP:ENAB ON\n"); 		  //��2700�����ʾ��OFF�ǹر�
    qt_sleep(100);
    send_data_GPIB(dev, "SYST:AZER:STAT OFF\r\n");  //�ر��Զ�����
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:CURR:AC:AVER:STAT OFF\r\n"); //�ر�ƽ��ģʽ
    qt_sleep(1000);

    /* ��ȡֵ */
    read_value_GPIB_2700(dev, p_get_val);
}

void get_res_value_GPIB_2700(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* ������ */
    send_data_GPIB(dev, "*RST\n"); //��λ
    qt_sleep(100);

    send_data_GPIB(dev, "INIT:CONT OFF;:ABOR\n"); //�ڲ������ر�
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:FUNC 'RES'\n"); 	//����ֱ���������ģʽ
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:RES:RANG 1E6\n");   //���������Χ1e6,��SENS:RES:RANG:AUTO ON\n���Ը�Ϊ�Զ�����
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:RES:NPLC 0.01\n");  //�����ٶ�0.1PLC��1��PLC = 20m
    qt_sleep(100);
    send_data_GPIB(dev, "DISP:ENAB ON\n"); 		//��2700�����ʾ��OFF�ǹر�
    qt_sleep(100);
    send_data_GPIB(dev, "SYST:AZER:STAT OFF\r\n"); //�ر��Զ�����
    qt_sleep(100);
    send_data_GPIB(dev, "SENS:RES:AVER:STAT OFF\r\n"); //�ر�ƽ��ģʽ
    qt_sleep(1000);

    /* ��ȡֵ */
    read_value_GPIB_2700(dev, p_get_val);
}

static void read_value_GPIB_34401(int dev, double* p_get_val)
{
    QString str_recv;
    double viResult;

    send_data_GPIB(dev, "READ?\n"); 		 //д�����ָ��
    qt_sleep(200); //��ȴ�һ�����

    //��ȡ�����ֵ
    recv_data_GPIB(dev, &str_recv);
    viResult = str_recv.toDouble();
    *p_get_val = viResult;
    qDebug() << "viResult: " << viResult;
}

void get_dv_value_GPIB_34401(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* ������ */
    send_data_GPIB(dev, "*RST\n"); //��λ
    qt_sleep(100);

    //send_data_GPIB(dev, "SYST:REM\n"); //Զ��ģʽ
    //qt_sleep(100);

    send_data_GPIB(dev, "CONF:VOLT:DC\n"); //����ֱ����ѹ����ģʽ
    qt_sleep(100);
    send_data_GPIB(dev, "VOLT:RANG:AUTO ON\n"); //����ֱ����ѹ������ѹ��Χ10  �����á�VOLT:RANG:AUTO ON������Ϊ�Զ�����
    qt_sleep(100);
    send_data_GPIB(dev, "VOLT:RES 0.01\n"); //��������0.00001/10V
    qt_sleep(100);
    send_data_GPIB(dev, "VOLT:NPLC 1\n"); //�����ٶ�1 ��PLC��1��PLC = 20ms����ѡ����0.02,0.2,1,10,100
    qt_sleep(100);
    send_data_GPIB(dev, "DISP ON\n"); //��34401�����ʾ��OFF�ǹر�
    qt_sleep(100);
    send_data_GPIB(dev, "TRIG:SOUR IMM\n");
    qt_sleep(100);

    /* ��ȡֵ */
    read_value_GPIB_34401(dev, p_get_val);
}

void get_av_value_GPIB_34401(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* ������ */
    send_data_GPIB(dev, "*RST\n"); //��λ
    qt_sleep(100);

    //send_data_GPIB(dev, "SYST:REM\n"); //Զ��ģʽ
    //qt_sleep(100);

    send_data_GPIB(dev, "CONF:VOLT:AC\n"); //���ý�����ѹ����ģʽ
    qt_sleep(100);
    send_data_GPIB(dev, "VOLT:RANG:AUTO ON\n"); //����ֱ����ѹ������ѹ��Χ10  �����á�VOLT:RANG:AUTO ON������Ϊ�Զ�����
    qt_sleep(100);
    send_data_GPIB(dev, "VOLT:RES 0.01\n"); //��������0.00001/10V
    qt_sleep(100);
    send_data_GPIB(dev, "VOLT:NPLC 1\n"); //�����ٶ�1 ��PLC��1��PLC = 20ms����ѡ����0.02,0.2,1,10,100
    qt_sleep(100);
    send_data_GPIB(dev, "DISP ON\n"); //��34401�����ʾ��OFF�ǹر�
    qt_sleep(100);
    send_data_GPIB(dev, "TRIG:SOUR IMM\n");
    qt_sleep(100);

    /* ��ȡֵ */
    read_value_GPIB_34401(dev, p_get_val);
}

void get_dc_value_GPIB_34401(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* ������ */
    send_data_GPIB(dev, "*RST\n"); //��λ
    qt_sleep(100);

    //send_data_GPIB(dev, "SYST:REM\n"); //Զ��ģʽ
    //qt_sleep(100);

    send_data_GPIB(dev, "CONF:CURR:DC\n"); //����ֱ����������ģʽ
    qt_sleep(100);
    send_data_GPIB(dev, "CURR:RANG:AUTO ON\n"); //����ֱ������������ѹ��Χ�Զ�
    qt_sleep(100);
    send_data_GPIB(dev, "CURR:RES MAX\n"); //�����������  0.00001/10
    qt_sleep(100);
    send_data_GPIB(dev, "CURR:NPLC 1\n"); //�����ٶ�1 ��PLC��1��PLC = 20ms����ѡ����0.02,0.2,1,10,100
    qt_sleep(100);
    send_data_GPIB(dev, "DISP ON\n"); //��34401�����ʾ��OFF�ǹر�
    qt_sleep(100);
    send_data_GPIB(dev, "TRIG:SOUR IMM\n");
    qt_sleep(100);

    /* ��ȡֵ */
    read_value_GPIB_34401(dev, p_get_val);
}

void get_ac_value_GPIB_34401(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* ������ */
    send_data_GPIB(dev, "*RST\n"); //��λ
    qt_sleep(100);

    //send_data_GPIB(dev, "SYST:REM\n"); //Զ��ģʽ
    //qt_sleep(100);

    send_data_GPIB(dev, "CONF:CURR:AC\n"); //���ý�����������ģʽ
    qt_sleep(100);
    send_data_GPIB(dev, "CURR:RANG:AUTO ON\n"); //����ֱ������������ѹ��Χ�Զ�
    qt_sleep(100);
    send_data_GPIB(dev, "CURR:RES MAX\n"); //�����������  0.00001/10
    qt_sleep(100);
    send_data_GPIB(dev, "CURR:NPLC 1\n"); //�����ٶ�1 ��PLC��1��PLC = 20ms����ѡ����0.02,0.2,1,10,100
    qt_sleep(100);
    send_data_GPIB(dev, "DISP ON\n"); //��34401�����ʾ��OFF�ǹر�
    qt_sleep(100);
    send_data_GPIB(dev, "TRIG:SOUR IMM\n");
    qt_sleep(100);

    /* ��ȡֵ */
    read_value_GPIB_34401(dev, p_get_val);
}

void get_res_value_GPIB_34401(int dev, double range, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* ������ */
    send_data_GPIB(dev, "*RST\n"); //��λ
    qt_sleep(100);

    //send_data_GPIB(dev, "SYST:REM\n"); //Զ��ģʽ
    //qt_sleep(100);

    send_data_GPIB(dev, "CONF:RES\n"); //���õ������ģʽ
    qt_sleep(100);
    send_data_GPIB(dev, "RES:RANG:AUTO ON\n"); //���õ��������ѹ��Χ�Զ�
    qt_sleep(100);
    send_data_GPIB(dev, "RES:RES 0.01\n"); //��������0.00001/10V
    qt_sleep(100);
    send_data_GPIB(dev, "RES:NPLC 1\n"); //�����ٶ�1 ��PLC��1��PLC = 20ms����ѡ����0.02,0.2,1,10,100
    qt_sleep(100);
    send_data_GPIB(dev, "DISP ON\n"); //��34401�����ʾ��OFF�ǹر�
    qt_sleep(100);
    send_data_GPIB(dev, "TRIG:SOUR IMM\n");
    qt_sleep(100);

    /* ��ȡֵ */
    read_value_GPIB_34401(dev, p_get_val);
}
#endif
