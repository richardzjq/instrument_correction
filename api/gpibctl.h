#ifndef GPIBCTL_H
#define GPIBCTL_H

#include <visa.h>

ViStatus AutoConnectGPIB(ViPSession instrSesn);
ViStatus CloseConnectGPIB(ViPSession instrSesn);
//直流电流设置
ViStatus Set_DC_Current(ViPSession m_ViSession, double Current);
//直流电压设置
ViStatus Set_DC_Voltage(ViPSession m_ViSession, double Voltage);
//交流电流设置
ViStatus Set_AC_Current(ViPSession m_ViSession, double Current, int Frequency);
//交流电压设置
ViStatus Set_AC_Voltage(ViPSession m_ViSession, double Voltage, int Frequency);
//电阻设置
ViStatus Set_Resistance(ViPSession m_ViSession, double Resistance);
//电容设置
ViStatus Set_Capacitance(ViPSession m_ViSession, double Capacitance);

#endif // GPIBCTL_H
