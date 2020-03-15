#ifndef GPIBCTL_H
#define GPIBCTL_H

#include <visa.h>

typedef enum instrument_max_value
{
    MAX_FLUKE_DIRECT_VOLT = 200,
    MAX_FLUKE_ALTERNATING_VOLT = 200,
    MAX_FLUKE_DIRECT_CURRENT = 2,
    MAX_FLUKE_ALTERNATING_CURRENT = 20,
    MAX_FLUKE_RESISTANCE = 2000,
    MAX_FLUKE_CAPACITANCE = 20000,

    MAX_34401A_DIRECT_VOLT = 2000,
    MAX_34401A_ALTERNATING_VOLT = 2000,
    MAX_34401A_DIRECT_CURRENT = 200,
    MAX_34401A_ALTERNATING_CURRENT = 200,
    MAX_34401A_RESISTANCE = 20000,
} max_value;

typedef enum instrument_check_type
{
    DIRECT_VOLT_TYPE = 0,
    ALTERNATING_VOLT_TYPE,
    DIRECT_CURRENT_TYPE,
    ALTERNATING_CURRENT_TYPE,
    RESISTANCE_TYPE,
    CAPACITANCE_TYPE,
} check_type;


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
void qt_sleep(int time_msecond);
ViStatus get_instrument_value_gpib_34401A(ViPSession m_ViSession, int get_type, double* p_get_val);

#endif // GPIBCTL_H
