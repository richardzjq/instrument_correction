#include "frminspectproject.h"
#include "ui_frminspectproject.h"
#include "inspect_db_global.h"
#include "inspect_algorithm.h"
#include "freetreewidget.h"
#include <QtGlobal>
#include <cmath>
#include <QDebug>

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
   record_number = data;
}

void frmInspectProject::initData(void)
{
    map_string_db.insert("直流电流", dbInspect_template_direct_current);
    map_string_db.insert("直流电压", dbInspect_template_direct_voltage);
    map_string_db.insert("交流电流", dbInspect_template_alternating_current);
    map_string_db.insert("交流电压", dbInspect_template_alternating_voltage);
    map_string_db.insert("电阻", dbInspect_template_resistance);
    map_string_db.insert("电容", dbInspect_template_capacitance);

    /* 打开串口 */
    isComOk = false;

    com = new QextSerialPort("com1", QextSerialPort::Polling);
    isComOk = com->open(QIODevice::ReadWrite);
    if (isComOk) {
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
    strList << "" << "直流电流" << "直流电压" << "交流电流" << "交流电压" << "电阻" << "电容";
    ui->comboBox_template_type->addItems(strList);

    strList.clear();
    strList << "";
    ui->comboBox_template_type->addItems(strList);

    ui->lineEdit_gpib_address->setText("0");
    gpib_address = "0";

    ui->lineEdit_assist_parameter->setText("60HZ");

    ui->lineEdit_read_method->setText("被检表");    

    /* 设置检测结果table */
    ui->tableWidget_inspect_result->setWindowTitle("检定值");
    ui->tableWidget_inspect_result->setColumnCount(4);

    //将表头写入表格
    strList.clear();
    strList << "量程" << "标准值" << "示值" << "误差(%)";
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
void frmInspectProject::set_standard_source(int set_type, double set_val)
{
    set_type = set_type;
    set_val = set_val;
}

/**
 * 获取多用表输出值，set_type为直流电压，直流电流，交流电压，交流电流，电阻
 */
void frmInspectProject::get_instrument_value(int get_type, double* p_get_val)
{
    QByteArray bytes;
    get_type = get_type;
    *p_get_val = 10;

    //bytes = com->readAll();

    /* parse byte array*/
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

void frmInspectProject::on_btn_begin_inspect_clicked()
{
    /* 根据检定模板，按次序进行检定 */

    /* 获得检定方案数据库文件中的table和数量 */
    QString  template_type = ui->comboBox_template_type->currentText();
    DBInspect dbInspect_template = map_string_db[template_type];
    QString template_name = ui->comboBox_template_name->currentText();

    /* 在inspect_data数据库中创建一个新table，名称是检定编号，保存检定数据 */
    QString columns = "量程 double, 标准值 double, 显示值 double, 允许误差 double";
    dbInspect_inspect_data_record.add_one_table(record_number, columns);
    qDebug() << record_number;

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

        /* 解析出表中一行各个字段的值，量程-标准值-下限值-显示值-上限值-扩展不确定度 */
        range_val               = line_content.section('-', 0, 0).toDouble();
        standard_val            = line_content.section('-', 1, 1).toDouble();
        max_error_val           = line_content.section('-', 2, 2).toDouble();

        qDebug() << "range_val " << range_val;
        qDebug() << "standard_val " << standard_val;
        qDebug() << "max_error_val " << max_error_val;

        for(int check_count = 0; check_count < CHECK_TIMES; check_count++)
        {
            set_standard_source(0, standard_val);
            get_instrument_value(0, &check_val[check_count]);
        }

        average_value               = average(check_val, CHECK_TIMES);
        //average_value               = standard_val;
        standard_deviation_value    = standard_deviation(check_val, CHECK_TIMES);
        max_allowable_error         = 0.0000001;
        standard_uncertainty_value  = standard_uncertainty(max_allowable_error);
        synthesis_uncertainty_value = synthesis_uncertainty(standard_deviation_value, standard_uncertainty_value);
        extend_uncertainty_value    = extend_uncertainty(synthesis_uncertainty_value);

        /* 向检定结果数据库table中加入一行 */
        s_range_value = QString::number(range_val, 'g', 10);
        s_standard_value = QString::number(standard_val, 'g', 10);
        s_show_value = QString::number(average_value, 'g', 10);
        s_max_error_value = QString::number(max_error_val, 'g', 10);
        add_line_instruction = "INSERT INTO " + record_number + " (量程, 标准值, 显示值, 允许误差) VALUES ("
                                           + s_range_value + ", " + s_standard_value  + ", " + s_show_value + ", " +  s_max_error_value + ")";

        dbInspect_inspect_data_record.add_one_line_into_table(add_line_instruction);
    }
}

void frmInspectProject::on_btn_save_clicked()
{

}
