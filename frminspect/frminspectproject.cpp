#include "frminspectproject.h"
#include "ui_frminspectproject.h"

frmInspectProject::frmInspectProject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmInspectProject)
{
    ui->setupUi(this);
}

frmInspectProject::~frmInspectProject()
{
    delete ui;
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
    if(0 == arg1.compare("电容"))
    {
        dbInspect_template_resistance.get_tables(&tables, &tables_count);
    }

    if(0 == arg1.compare("电阻"))
    {
        dbInspect_template_capacitance.get_tables(&tables, &tables_count);
    }

    if(0 == arg1.compare("直流电流"))
    {
        dbInspect_template_direct_current.get_tables(&tables, &tables_count);
    }

    if(0 == arg1.compare("直流电压"))
    {
        dbInspect_template_direct_voltage.get_tables(&tables, &tables_count);
    }

    if(0 == arg1.compare("交流电流"))
    {
        dbInspect_template_alternating_current.get_tables(&tables, &tables_count);
    }

    if(0 == arg1.compare("交流电压"))
    {
        dbInspect_template_alternating_voltage.get_tables(&tables, &tables_count);
    }

    ui->comboBox_template_name->addItems(tables);
}

void frmInspectProject::on_comboBox_template_name_currentIndexChanged(const QString &arg1)
{
    /* 设置模板tableWidget */
}
