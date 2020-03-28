#include "serialctl.h"
#include "device_mix_definition.h"


void send_data_RS232(QSerialPort* p_serialPort, QString str_send)
{
    p_serialPort->write(str_send.toLatin1());
    qDebug() << str_send.toLatin1();
}

void recv_data_RS232(QSerialPort* p_serialPort, QString* p_str_recv)
{
    char recv_data[MAX_DATA_LENGTH];
    qint64 recv_size;
    int i;
    QString str_data;

    recv_size = p_serialPort->read(recv_data, MAX_DATA_LENGTH);
    qDebug() << "read_size: " << recv_size;
    //for(i = 0; i < recv_size; i++)
    //{
    //   qDebug() << "sequence:  " << i << "data:  " << recv_data[i];
    //}

    *p_str_recv =  QString::fromUtf8(recv_data);

    qDebug() << *p_str_recv;
}

void get_idn_RS232(QSerialPort* p_serialPort, double* p_get_val)
{
    QString str_recv;

    qDebug() << __FUNCTION__;

    /* 发命令 */
    send_data_RS232(p_serialPort, "*idn?\r\n");
    qt_sleep(100);

    recv_data_RS232(p_serialPort, &str_recv);
}

void read_value_RS232_2700(QSerialPort* p_serialPort, double* p_get_val)
{
    QString str_recv;
    double viResult;

    send_data_RS232(p_serialPort, "TRIG:COUN 1\n"); //触发个数1，如果打开平均模式，触发个数大于1时，去所有触发的平均值，比如触发为5,2700自动测5次，然后输出平均值
    qt_sleep(100);

    send_data_RS232(p_serialPort, "READ?\n"); 		 //写入读数指令
    qt_sleep(100);

    //读取输出数值
    recv_data_RS232(p_serialPort, &str_recv);
    viResult = str_recv.toDouble();
    *p_get_val = viResult;
    qDebug() << "viResult: " << viResult;
}
void get_dc_value_RS232_2700(QSerialPort* p_serialPort, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* 发命令 */
    send_data_RS232(p_serialPort, "*RST\n"); //复位
    qt_sleep(100);

    send_data_RS232(p_serialPort, "INIT:CONT OFF;:ABOR\n"); //内部触发关闭
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SENS:FUNC 'CURR:DC'\n"); //设置直流电流测量模式
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SENS:CURR:DC:RANG 0.01\n"); //电流测量范围10mA,用SENS:CURR:DC:RANG:AUTO ON\n可以改为自动量程
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SENS:CURR:DC:NPLC 0.01\n"); //测试速度0.1PLC，1个PLC = 20ms
    qt_sleep(100);
    send_data_RS232(p_serialPort, "DISP:ENAB ON\n"); 		  //打开2700面板显示，OFF是关闭
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SYST:AZER:STAT OFF\r\n");  //关闭自动归零
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SENS:CURR:DC:AVER:STAT OFF\r\n"); //关闭平均模式
    qt_sleep(1000);

    /* 读取值 */
    read_value_RS232_2700(p_serialPort, p_get_val)
}

void get_ac_value_RS232_2700(QSerialPort* p_serialPort, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* 发命令 */
    send_data_RS232(p_serialPort, "*RST\n"); //复位
    qt_sleep(100);

    send_data_RS232(p_serialPort, "INIT:CONT OFF;:ABOR\n"); //内部触发关闭
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SENS:FUNC 'CURR:AC'\n"); //设置交流电流测量模式
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SENS:CURR:AC:RANG 0.01\n"); //电流测量范围10mA,用SENS:CURR:AC:RANG:AUTO ON\n可以改为自动量程
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SENS:CURR:AC:NPLC 0.01\n"); //测试速度0.1PLC，1个PLC = 20ms
    qt_sleep(100);
    send_data_RS232(p_serialPort, "DISP:ENAB ON\n"); 		  //打开2700面板显示，OFF是关闭
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SYST:AZER:STAT OFF\r\n");  //关闭自动归零
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SENS:CURR:AC:AVER:STAT OFF\r\n"); //关闭平均模式
    qt_sleep(1000);

    /* 读取值 */
    read_value_RS232_2700(p_serialPort, p_get_val);
}

void get_dv_value_RS232_2700(QSerialPort* p_serialPort, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* 发命令 */
    send_data_RS232(p_serialPort, "*RST\n"); //复位
    qt_sleep(100);

    send_data_RS232(p_serialPort, "INIT:CONT OFF;:ABOR\n"); //内部触发关闭
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SENS:FUNC 'VOLT:DC'\n"); //设置直流电压测量模式
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SENS:VOLT:DC:RANG 10\n"); //电压测量范围10v,用SENS:CURR:DC:RANG:AUTO ON\n可以改为自动量程
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SENS:VOLT:DC:NPLC 0.01\n"); 	//测试速度0.1PLC，1个PLC = 20ms
    qt_sleep(100);
    send_data_RS232(p_serialPort, "DISP:ENAB ON\n"); 		//打开2700面板显示，OFF是关闭
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SYST:AZER:STAT OFF\r\n"); //关闭自动归零
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SENS:CURR:DC:AVER:STAT OFF\r\n"); //关闭平均模式
    qt_sleep(1000);

    /* 读取值 */
    read_value_RS232_2700(p_serialPort, p_get_val);
}

void get_av_value_RS232_2700(QSerialPort* p_serialPort, double* p_get_val)
{
    qDebug() << __FUNCTION__;

    /* 发命令 */
    send_data_RS232(p_serialPort, "*RST\n"); //复位
    qt_sleep(100);

    send_data_RS232(p_serialPort, "INIT:CONT OFF;:ABOR\n"); //内部触发关闭
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SENS:FUNC 'VOLT:AC'\n"); //设置交流电压测量模式
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SENS:VOLT:AC:RANG 10\n"); //电压测量范围10v,用SENS:CURR:DC:RANG:AUTO ON\n可以改为自动量程
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SENS:VOLT:AC:NPLC 0.01\n"); 	//测试速度0.1PLC，1个PLC = 20ms
    qt_sleep(100);
    send_data_RS232(p_serialPort, "DISP:ENAB ON\n"); 		//打开2700面板显示，OFF是关闭
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SYST:AZER:STAT OFF\r\n"); //关闭自动归零
    qt_sleep(100);
    send_data_RS232(p_serialPort, "SENS:CURR:AC:AVER:STAT OFF\r\n"); //关闭平均模式
    qt_sleep(1000);

    /* 读取值 */
    read_value_RS232_2700(p_serialPort, p_get_val);
}
