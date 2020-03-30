#ifndef GPIBCTL_H
#define GPIBCTL_H

#include <visa.h>


ViStatus AutoConnectGPIB_Fluke_5502A(ViPSession instrSesn, QString addr);
ViStatus CloseConnectGPIB(ViPSession instrSesn);
//直流电流设置
ViStatus Set_DC_Current_GPIB_Fluke_5502A(ViPSession m_ViSession, double Current);
//直流电压设置
ViStatus Set_DC_Voltage_GPIB_Fluke_5502A(ViPSession m_ViSession, double Voltage);
//交流电流设置
ViStatus Set_AC_Current_GPIB_Fluke_5502A(ViPSession m_ViSession, double Current, int Frequency);
//交流电压设置
ViStatus Set_AC_Voltage_GPIB_Fluke_5502A(ViPSession m_ViSession, double Voltage, int Frequency);
//电阻设置
ViStatus Set_Resistance_GPIB_Fluke_5502A(ViPSession m_ViSession, double Resistance);
//电容设置
ViStatus Set_Capacitance_GPIB_Fluke_5502A(ViPSession m_ViSession, double Capacitance);

ViStatus AutoConnectGPIB_Agilent_34401A(ViPSession instrSesn, QString addr);

/* 以后再实现GPIB读取2700，34401万用表的数值，过程，命令通RS232一样，只是通道不同 */
/* 目前都按照trigger方式读取数值 */


#endif // GPIBCTL_H
