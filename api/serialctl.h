#ifndef SERIALCTL_H
#define SERIALCTL_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

/* 目前都按照trigger方式读取数值 */

void get_idn_RS232(QSerialPort* p_serialPort, QString* p_str_recv);
void get_dv_value_RS232_2700(QSerialPort* p_serialPort, double range, double* p_get_val);
void get_av_value_RS232_2700(QSerialPort* p_serialPort, double range, double* p_get_val);
void get_dc_value_RS232_2700(QSerialPort* p_serialPort, double range, double* p_get_val);
void get_ac_value_RS232_2700(QSerialPort* p_serialPort, double range, double* p_get_val);
void get_res_value_RS232_2700(QSerialPort* p_serialPort, double range, double* p_get_val);

void get_dv_value_RS232_34401(QSerialPort* p_serialPort, double range, double* p_get_val);
void get_av_value_RS232_34401(QSerialPort* p_serialPort, double range, double* p_get_val);
void get_dc_value_RS232_34401(QSerialPort* p_serialPort, double range, double* p_get_val);
void get_ac_value_RS232_34401(QSerialPort* p_serialPort, double range, double* p_get_val);
void get_res_value_RS232_34401(QSerialPort* p_serialPort, double range, double* p_get_val);


#endif // SERIALCTL_H
