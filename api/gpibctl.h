#ifndef GPIBCTL_H
#define GPIBCTL_H

#include <visa.h>

ViStatus AutoConnectGPIB(ViPSession instrSesn);
ViStatus CloseConnectGPIB(ViPSession instrSesn);
//直流电流设置
ViStatus Set_DC_Current(ViPSession m_ViSession, double Current, QString Unit_Current);
//直流电压设置
ViStatus Set_DC_Voltage(ViPSession m_ViSession, double Voltage, QString Unit_Voltage);
//交流电流设置
ViStatus Set_AC_Current(ViPSession m_ViSession, double Current, QString Unit_Current, int Frequency, QString Unit_Frequency);
//交流电压设置
ViStatus Set_AC_Voltage(ViPSession m_ViSession, double Voltage, QString Unit_Voltage, int Frequency, QString Unit_Frequency);
//电阻设置
ViStatus Set_Resistance(ViPSession m_ViSession, double Resistance, QString Unit_Resistance);
//电容设置
ViStatus Set_Capacitance(ViPSession m_ViSession, double Capacitance, QString Unit_Capacitance);

#endif // GPIBCTL_H
