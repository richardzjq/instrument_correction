void on_btn_read_clicked();
void on_btn_inspect_mode_clicked();
#ifndef FRMINSPECTPROJECT_H
#define FRMINSPECTPROJECT_H

#include <QWidget>
#include <QMap>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "gpibctl.h"
#include "serialctl.h"
#include "dbinspect.h"


/* 每个点检测次数，最后求平均值 */
#define CHECK_TIMES 10

/* 最大仪表获取值函数数量 每个仪表提供6个函数，假定支持20个仪表类型 6*20= 120 */
#define MAX_INSTRUMENTS_SUPPORT 20
#define ONE_INSTRUMENT_FUNCS 6

//自定义一个map类型，值为模板类型对应int型数据
typedef QMap<QString, int> TemplateTypeMap;

/* 定义RS232获取值得函数指针 */
typedef void (*get_value_RS232_fun_ptr)(QSerialPort*, double, double*);


namespace Ui {
class frmInspectProject;
}

class frmInspectProject : public QWidget
{
    Q_OBJECT

public:
    explicit frmInspectProject(QWidget *parent = nullptr);
    ~frmInspectProject();

public:
    //在子窗口创建public函数用来获取传递的数据
    void getData(QString data);

private:
    Ui::frmInspectProject *ui;
    DBInspectMap map_string_db;
    TemplateTypeMap map_tempate_type;
    QString record_number;
    QSerialPort m_serialPort;
    ViSession viSession_standard_source;
    ViSession viSession_instrument;
    get_value_RS232_fun_ptr m_get_value_RS232_fun_ptr[MAX_INSTRUMENTS_SUPPORT * ONE_INSTRUMENT_FUNCS];

private slots:
    void initData(void);
    void unInitData(void);
    void initForm(void);
    void uninitForm(void);
    /* 设置标准源，set_type为直流电压，直流电流，交流电压，交流电流，电阻 */
    void set_standard_source(int set_type, double set_val, int freq);
    /* 获取多用表输出值，set_type为直流电压，直流电流，交流电压，交流电流，电阻*/
    void get_instrument_value(int instrument_type, int get_type, double range_val, double* p_get_val);
    void readData();
    void on_comboBox_template_type_currentIndexChanged(const QString &arg1);
    void on_comboBox_template_name_currentIndexChanged(const QString &arg1);
    void on_btn_gpib_connect_clicked();
    void on_btn_gpib_disconnect_clicked();
    void on_btn_begin_inspect_clicked();
    void on_btn_save_clicked();
    void on_btn_inspect_mode_clicked();
};


#endif // FRMINSPECTPROJECT_H
