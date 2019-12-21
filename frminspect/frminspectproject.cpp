#include "frminspectproject.h"
#include "ui_frminspectproject.h"

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
    delete ui;
}

void frmInspectProject::initData(void)
{
    map_string_db.insert("直流电流", dbInspect_template_direct_current);
    map_string_db.insert("直流电压", dbInspect_template_direct_voltage);
    map_string_db.insert("交流电流", dbInspect_template_alternating_current);
    map_string_db.insert("交流电压", dbInspect_template_alternating_voltage);
    map_string_db.insert("电阻", dbInspect_template_resistance);
    map_string_db.insert("电容", dbInspect_template_capacitance);
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

    /* 打开数据库 */
    bool open_db;
    const QString filePath = QCoreApplication::applicationDirPath();
    QString fileName_db;

    fileName_db = filePath + "/db/inpsect_template/电容.db";
    open_db = dbInspect_template_resistance.open_database(fileName_db);
    if(open_db == false)
    {
        /* 关闭数据库 */
        dbInspect_template_resistance.close_database();
        qDebug() << __FUNCTION__ << "Open databas fail!";
    }

    fileName_db = filePath + "/db/inpsect_template/电阻.db";
    open_db = dbInspect_template_capacitance.open_database(fileName_db);
    if(open_db == false)
    {
        /* 关闭数据库 */
        dbInspect_template_capacitance.close_database();
        qDebug() << __FUNCTION__ << "Open databas fail!";
    }

    fileName_db = filePath + "/db/inpsect_template/直流电流.db";
    open_db = dbInspect_template_direct_current.open_database(fileName_db);
    if(open_db == false)
    {
        /* 关闭数据库 */
        dbInspect_template_direct_current.close_database();
        qDebug() << __FUNCTION__ << "Open databas fail!";
    }

    fileName_db = filePath + "/db/inpsect_template/直流电压.db";
    open_db = dbInspect_template_direct_voltage.open_database(fileName_db);
    if(open_db == false)
    {
        /* 关闭数据库 */
        dbInspect_template_direct_voltage.close_database();
        qDebug() << __FUNCTION__ << "Open databas fail!";
    }

    fileName_db = filePath + "/db/inpsect_template/交流电流.db";
    open_db = dbInspect_template_alternating_current.open_database(fileName_db);
    if(open_db == false)
    {
        /* 关闭数据库 */
        dbInspect_template_alternating_current.close_database();
        qDebug() << __FUNCTION__ << "Open databas fail!";
    }

    fileName_db = filePath + "/db/inpsect_template/交流电压.db";
    open_db = dbInspect_template_alternating_voltage.open_database(fileName_db);
    if(open_db == false)
    {
        /* 关闭数据库 */
        dbInspect_template_alternating_voltage.close_database();
        qDebug() << __FUNCTION__ << "Open databas fail!";
    }

    /* 设置检测结果table */
    ui->tableWidget_inspect_result->setWindowTitle("检定值");
    ui->tableWidget_inspect_result->setColumnCount(4);

    //将表头写入表格
    strList.clear();
    strList << "量程" << "标准值" << "示值" << "误差(%)";
    ui->tableWidget_inspect_result->setHorizontalHeaderLabels(strList);
    //自动调整宽度
    ui->tableWidget_inspect_result->horizontalHeader()->setStretchLastSection(true);

    ui->btn_gpib_disconnect->setEnabled(false);
    ui->btn_debug_mode->setEnabled(false);
}

void frmInspectProject::uninitForm(void)
{
    dbInspect_template_resistance.close_database();
    dbInspect_template_capacitance.close_database();
    dbInspect_template_direct_current.close_database();
    dbInspect_template_direct_voltage.close_database();
    dbInspect_template_alternating_current.close_database();
    dbInspect_template_alternating_voltage.close_database();
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
