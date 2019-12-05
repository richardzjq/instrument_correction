#ifndef GPIB_H
#define GPIB_H

ViStatus AutoConnectGPIB(ViPSession instrSesn);
ViStatus CloseConnectGPIB(ViPSession instrSesn);
//直流电流设置
ViStatus Set_DC_Current(ViPSession m_ViSession , double Current, string Unit_Current);
//直流电压设置
ViStatus Set_DC_Voltage(ViPSession m_ViSession , double Voltage, string Unit_Voltage);
//交流电流设置
ViStatus Set_AC_Current(ViPSession m_ViSession , double Current, string Unit_Current，int Frequency,string Unit_Frequency);
//交流电压设置
ViStatus Set_AC_Voltage(ViPSession m_ViSession , double Voltage, string Unit_Voltage，int Frequency,string Unit_Frequency);
//电阻设置
ViStatus Set_Resistance(ViPSession m_ViSession , double Resistance, string Unit_Resistance);
//电容设置
ViStatus Set_Capacitance(ViPSession m_ViSession , double Capacitance, string Unit_Capacitance);

#endif // GPIB_H
