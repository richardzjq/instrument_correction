#ifndef GPIBCTL_H
#define GPIBCTL_H

#include <visa.h>


ViStatus AutoConnectGPIB_Fluke_5502A(ViPSession instrSesn, QString addr);
ViStatus CloseConnectGPIB(ViPSession instrSesn);
//直流电流设置
ViStatus Set_DC_Current_Fluke_5502A(ViPSession m_ViSession, double Current);
//直流电压设置
ViStatus Set_DC_Voltage_Fluke_5502A(ViPSession m_ViSession, double Voltage);
//交流电流设置
ViStatus Set_AC_Current_Fluke_5502A(ViPSession m_ViSession, double Current, int Frequency);
//交流电压设置
ViStatus Set_AC_Voltage_Fluke_5502A(ViPSession m_ViSession, double Voltage, int Frequency);
//电阻设置
ViStatus Set_Resistance_Fluke_5502A(ViPSession m_ViSession, double Resistance);
//电容设置
ViStatus Set_Capacitance_Fluke_5502A(ViPSession m_ViSession, double Capacitance);

ViStatus AutoConnectGPIB_Agilent_34401A(ViPSession instrSesn, QString addr);
ViStatus get_instrument_value_gpib_34401A(ViPSession m_ViSession, int get_type, double* p_get_val);


#endif // GPIBCTL_H
