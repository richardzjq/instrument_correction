#include "frminspectproject.h"
#include "ui_frminspectproject.h"
#include "inspect_db_global.h"
#include "inspect_algorithm.h"
#include "freetreewidget.h"
#include <QtGlobal>
#include <cmath>
#include <QApplication>
#include <QTime>
#include <QDebug>


typedef enum instrument_check_type
{
    MAX_FLUKE_DIRECT_VOLT = 2000,
    MAX_FLUKE_ALTERNATING_VOLT = 2000,
    MAX_FLUKE_DIRECT_CURRENT = 200,
    MAX_FLUKE_ALTERNATING_CURRENT = 200,
    MAX_FLUKE_RESISTANCE = 20000,
    MAX_FLUKE_CAPACITANCE = 200000,

    MAX_34401A_DIRECT_VOLT = 2000,
    MAX_34401A_ALTERNATING_VOLT = 2000,
    MAX_34401A_DIRECT_CURRENT = 200,
    MAX_34401A_ALTERNATING_CURRENT = 200,
    MAX_34401A_RESISTANCE = 20000,
} check_type;

typedef enum instrument_max_value
{
    DIRECT_VOLT_TYPE = 0,
    ALTERNATING_VOLT_TYPE,
    DIRECT_CURRENT_TYPE,
    ALTERNATING_CURRENT_TYPE,
    RESISTANCE_TYPE,
    CAPACITANCE_TYPE,
} max_value;

frmInspectProject::frmInspectProject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmInspectProject)
{
    ui->setupUi(this);
    this->initForm();
    this->initData();
}

frmInspectProject::~frmInspectProject()
{
    this->uninitForm();
    this->unInitData();
    delete ui;
}

void frmInspectProject::getData(QString data)
{   //保存传递的数据
   record_number = "INS_" + data;
}

void frmInspectProject::initData(void)
{
    map_string_db.insert("direct_current", dbInspect_template_direct_current);
    map_string_db.insert("direct_voltage", dbInspect_template_direct_voltage);
    map_string_db.insert("alternating_current", dbInspect_template_alternating_current);
    map_string_db.insert("alternating_voltage", dbInspect_template_alternating_voltage);
    map_string_db.insert("resistance", dbInspect_template_resistance);
    map_string_db.insert("capacitance", dbInspect_template_capacitance);

    map_tempate_type.insert("direct_current", DIRECT_CURRENT_TYPE);
    map_tempate_type.insert("direct_voltage", DIRECT_VOLT_TYPE);
    map_tempate_type.insert("alternating_current", ALTERNATING_CURRENT_TYPE);
    map_tempate_type.insert("alternating_voltage", ALTERNATING_VOLT_TYPE);
    map_tempate_type.insert("resistance", RESISTANCE_TYPE);
    map_tempate_type.insert("capacitance", CAPACITANCE_TYPE);

    /* 打开串口 */
    isComOk = false;

    com = new QextSerialPort("com1", QextSerialPort::Polling);
    isComOk = com->open(QIODevice::ReadWrite);
    if (isComOk) {
        qDebug() << "com1 opened success";
        com->setBaudRate(BAUD115200);
        com->setFlowControl(FLOW_OFF);
        com->setTimeout(10);
    }
}

void frmInspectProject::unInitData(void)
{
    /* 关闭串口 */
    if(com)
    {
        com->close();
        com->deleteLater();
        isComOk = false;
        delete com;
        com = nullptr;
    }
}

void frmInspectProject::initForm(void)
{
    /* 初始化各项 */
    QStringList strList;

    strList.clear();
    strList << "" << "direct_current" << "direct_voltage" << "alternating_current" << "alternating_voltage" << "resistance" << "capacitance";
    ui->comboBox_template_type->addItems(strList);

    strList.clear();
    strList << "";
    ui->comboBox_template_type->addItems(strList);

    ui->lineEdit_gpib_address->setText("0");
    gpib_address = "0";

    ui->lineEdit_assist_parameter->setText("60");

    ui->lineEdit_read_method->setText("被检表");    

    /* 设置检测结果table */
    ui->tableWidget_inspect_result->setWindowTitle("检定值");
    ui->tableWidget_inspect_result->setColumnCount(4);

    //将表头写入表格
    strList.clear();
    strList << "range" << "standard" << "show" << "error(%)";
    ui->tableWidget_inspect_result->setHorizontalHeaderLabels(strList);
    //自动调整宽度
    ui->tableWidget_inspect_result->horizontalHeader()->setStretchLastSection(true);

    ui->btn_inspect_mode->setEnabled(false);
    ui->btn_save->setEnabled(false);
}

void frmInspectProject::uninitForm(void)
{

}

/**
 * 设置标准源，set_type为直流电压，直流电流，交流电压，交流电流，电阻
 */
void frmInspectProject::set_standard_source(int set_type, double set_val, int freq)
{
    switch(set_type)
    {
        case DIRECT_VOLT_TYPE:
            if(set_val > MAX_FLUKE_DIRECT_VOLT)
                set_val = MAX_FLUKE_DIRECT_VOLT;
            Set_DC_Voltage(&viSession, set_val);
            break;
        case ALTERNATING_VOLT_TYPE:
            if(set_val > MAX_FLUKE_ALTERNATING_VOLT)
                set_val = MAX_FLUKE_ALTERNATING_VOLT;
            Set_AC_Voltage(&viSession, set_val, freq);
            break;
        case DIRECT_CURRENT_TYPE:
            if(set_val > MAX_FLUKE_DIRECT_CURRENT)
                set_val = MAX_FLUKE_DIRECT_CURRENT;
            Set_DC_Current(&viSession, set_val);
            break;
        case ALTERNATING_CURRENT_TYPE:
            if(set_val > MAX_FLUKE_ALTERNATING_CURRENT)
                set_val = MAX_FLUKE_ALTERNATING_CURRENT;
            Set_AC_Current(&viSession, set_val, freq);
            break;
        case RESISTANCE_TYPE:
            if(set_val > MAX_FLUKE_CAPACITANCE)
                set_val = MAX_FLUKE_CAPACITANCE;
            Set_Resistance(&viSession, set_val);
            break;
        default:
            break;
    }
}

static void qt_sleep(int time_msecond)
{
    QTime t;
    t.start();
    while(t.elapsed() < time_msecond)
        QCoreApplication::processEvents();
}

void frmInspectProject::get_instrument_value_RS232_34401A(int get_type, double* p_get_val)
{
    /* 设置串口 */
    com->setBaudRate(BAUD9600);
    com->setDataBits(DATA_8);
    com->setParity(PAR_NONE);
    com->setDtr(true);
    com->setStopBits(STOP_1);
    //com->setFlowControl(FLOW_OFF);
    com->setTimeout(10);

    /* 发命令 */
    com->write("SYST:REM");
    qt_sleep(30);
    //清除万用表显示板信息
    com->write("*CLS");
    qt_sleep(30);
    com->write("TRIG:SOUR IMM");
    qt_sleep(30);

    /* 读取值 */
    switch(get_type)
    {
        case DIRECT_VOLT_TYPE:
        case ALTERNATING_VOLT_TYPE:
            com->write("MEAS:VOLT?");
            break;
        case DIRECT_CURRENT_TYPE:
        case ALTERNATING_CURRENT_TYPE:
            com->write("MEAS:CURR?");
            break;
        case RESISTANCE_TYPE:
            com->write("MEAS:RES");
            break;
        default:
            break;
    }
    qt_sleep(1000);
    //读取输出数值
    QByteArray data = com->readAll();
    QString str_data = data;
    double viResult = str_data.toDouble();
    *p_get_val = viResult;
    qDebug() << "data: " << data;
    qDebug() << "str_data: " << str_data;
    qDebug() << "viResult: " << viResult;
}

/**
 * 获取多用表输出值，set_type为直流电压，直流电流，交流电压，交流电流，电阻
 */
void frmInspectProject::get_instrument_value(int instrument_type, int get_type, double* p_get_val)
{
    switch(instrument_type)
    {
        /* RS232_34401A */
        case 0:
            get_instrument_value_RS232_34401A(get_type, p_get_val);
            break;
        default:
            break;
    }
}

void frmInspectProject::on_comboBox_template_type_currentIndexChanged(const QString &arg1)
{
    QStringList tables;
    int tables_count;

    /* 设置模板comb */
    DBInspect dbInspect_template;

    dbInspect_template = map_string_db[arg1];
    dbInspect_template.get_tables(&tables, &tables_count);

    ui->comboBox_template_name->addItems(tables);
}

void frmInspectProject::on_comboBox_template_name_currentIndexChanged(const QString &arg1)
{
    /* 设置模板tableWidget */
    QStringList table_content;
    QString  template_type = ui->comboBox_template_type->currentText();

    /* 查询一个表中的所有记录 */
    DBInspect dbInspect_template;

    dbInspect_template = map_string_db[template_type];
    dbInspect_template.get_table_content(arg1, &table_content);

    QStringListIterator itr_table_content(table_content);

    /* 根据表中内容，设置tableWidget */
    int row_inc = 0, columm_inc = 0;
    ui->tableWidget_inspect_result->setRowCount(table_content.count());
    while (itr_table_content.hasNext())
    {
        QString line_content = itr_table_content.next().toLocal8Bit();
        qDebug() << line_content;

        /* 量程, 标准值 */
        for(columm_inc = 0; columm_inc < 2; columm_inc++)
        {
            ui->tableWidget_inspect_result->setItem(row_inc, columm_inc, new QTableWidgetItem(line_content.section(',', columm_inc, columm_inc)));
            qDebug() << row_inc << columm_inc;
        }

        /* 允许误差 */
        columm_inc = 3;
        ui->tableWidget_inspect_result->setItem(row_inc, columm_inc, new QTableWidgetItem(line_content.section(',', columm_inc - 1, columm_inc - 1)));
        qDebug() << row_inc << columm_inc;

        row_inc++;
    }
}

void frmInspectProject::on_btn_begin_inspect_clicked()
{
    /* 根据检定模板，按次序进行检定 */

    /* 获取模板类型和辅助参数 */
    QString str_freq = ui->lineEdit_assist_parameter->text();
    int freq = str_freq.toInt();
    qDebug() << "frequence: " << freq;

    /* 获得检定方案数据库文件中的table和数量 */
    QString  template_type = ui->comboBox_template_type->currentText();
    DBInspect dbInspect_template = map_string_db[template_type];
    QString template_name = ui->comboBox_template_name->currentText();
    int int_template_type = map_tempate_type[template_type];
    qDebug() << "template_type: " << template_type;

    /* 在inspect_data数据库中创建一个新table，名称是检定编号，保存检定数据 */
    QString columns = "range real, standard real, show real, error real";
    dbInspect_inspect_data_record.add_one_table(record_number, columns);
    qDebug() << "record_number: " << record_number;

    /* 查询一个表中的所有记录 */
    QStringList table_content;
    dbInspect_template.get_table_content(template_name, &table_content);
    QStringListIterator itr_table_content(table_content);
    double check_val[CHECK_TIMES] = {0};

    /* 根据表中内容，进行检定 */
    double range_val, standard_val, max_error_val;
    double average_value, standard_deviation_value, max_allowable_error, standard_uncertainty_value, synthesis_uncertainty_value, extend_uncertainty_value;
    QString s_range_value, s_standard_value, s_show_value, s_max_error_value, add_line_instruction;

    while (itr_table_content.hasNext())
    {
        QString line_content = itr_table_content.next().toLocal8Bit();
        qDebug() << line_content;
        bool is_ok;

        /* 解析出表中一行各个字段的值，量程-标准值-下限值-显示值-上限值-扩展不确定度 */
        range_val               = line_content.section('-', 0, 0).toDouble(&is_ok);
        standard_val            = line_content.section('-', 1, 1).toDouble(&is_ok);
        max_error_val           = line_content.section('-', 2, 2).toDouble(&is_ok);

        qDebug() << "range_val " << range_val;
        qDebug() << "standard_val " << standard_val;
        qDebug() << "max_error_val " << max_error_val;

        for(int check_count = 0; check_count < CHECK_TIMES; check_count++)
        {
            set_standard_source(int_template_type, standard_val, freq);
            get_instrument_value(0, int_template_type, &check_val[check_count]);
        }

        average_value               = average(check_val, CHECK_TIMES);
        //average_value               = standard_val;
        standard_deviation_value    = standard_deviation(check_val, CHECK_TIMES);
        max_allowable_error         = 0.0000001;
        standard_uncertainty_value  = standard_uncertainty(max_allowable_error);
        synthesis_uncertainty_value = synthesis_uncertainty(standard_deviation_value, standard_uncertainty_value);
        extend_uncertainty_value    = extend_uncertainty(synthesis_uncertainty_value);

        /* 向检定结果数据库table中加入一行 */
        s_range_value = QString::number(range_val, 10, 10);
        s_standard_value = QString::number(standard_val, 10, 10);
        s_show_value = QString::number(average_value, 10, 10);
        s_max_error_value = QString::number(max_error_val, 10, 10);
        add_line_instruction = "INSERT INTO " + record_number + " (range, standard, show, error) VALUES ("
                                           + s_range_value + ", " + s_standard_value  + ", " + s_show_value + ", " +  s_max_error_value + ")";

        dbInspect_inspect_data_record.add_one_line_into_table(add_line_instruction);
    }
}

void frmInspectProject::on_btn_gpib_connect_clicked()
{
    /* 发送命令，和fluke标准源建立GPIB连接 */
    /* 打开GPIB */
    ViStatus status;

    status = AutoConnectGPIB(&viSession);
}

void frmInspectProject::on_btn_gpib_disconnect_clicked()
{
    /* 发送命令，和fluke标准源断开GPIB连接 */
    /* 关闭GPIB */
    CloseConnectGPIB(&viSession);
    viSession = 0;
}

void frmInspectProject::on_btn_save_clicked()
{

}

void frmInspectProject::on_btn_inspect_mode_clicked()
{
    /* 测试一下GPIB读写 */
    //Set_DC_Current(&viSession, 3.3);
}
