#include "frminspectmain.h"
#include "ui_frminspectmain.h"
#include "inspect_db_global.h"
#include "frminspectcheck.h"

/* 所有的数据库在inspect main中打开，关闭 */
DBInspect dbInspect_record;
DBInspect dbInspect_inspect_data_record;

DBInspect dbInspect_template_resistance;
DBInspect dbInspect_template_capacitance;
DBInspect dbInspect_template_direct_current;
DBInspect dbInspect_template_alternating_current;
DBInspect dbInspect_template_direct_voltage;
DBInspect dbInspect_template_alternating_voltage;


frmInspectMain::frmInspectMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmInspectMain)
{
    ui->setupUi(this);
    this->initData();
    this->initForm();
}

frmInspectMain::~frmInspectMain()
{
    this->uninitForm();
    this->uninitData();
    delete ui;
}

static int QStringList_removeDuplicates(QStringList *that)
{
    int n = that->size();
    int j = 0;
    QSet<QString> seen;
    seen.reserve(n);

    int setSize = 0;
    for (int i = 0; i < n; ++i)
    {
        const QString &s = that->at(i);

        seen.insert(s);

        if (setSize == seen.size()) // unchanged size => was already seen
            continue;

        ++setSize;

        if (j != i)
            that->swap(i, j);    //将不重复项与重复项交换

        ++j;
    }

    if (n != j)
        that->erase(that->begin() + j, that->end());

    return n - j;
}

void frmInspectMain::initForm(void)
{
    QStringList tables;
    int tables_count;

    /* 获得检定记录数据库文件中的table和数量 */
    dbInspect_record.get_tables(&tables, &tables_count);
    QStringListIterator itr_tables(tables);

    record_table_name = itr_tables.next().toLocal8Bit();
    qDebug() << QString::fromLocal8Bit("表名：") + record_table_name;

    /* 根据第一张表，初始化过滤条件各列表 */
    /* 送检单位 */
    QStringList column_content;
    dbInspect_record.get_column_content(record_table_name, "送检单位",  &column_content);
    QStringList_removeDuplicates(&column_content);
    column_content.insert(0, "全部");
    ui->comboBox_inspected_institution->addItems(column_content);

    /* 检定日期 */
    column_content.clear();
    dbInspect_record.get_column_content(record_table_name, "检定日期",  &column_content);
    QStringList_removeDuplicates(&column_content);
    column_content.insert(0, "全部");
    ui->comboBox_inspected_date->addItems(column_content);

    /* 检定结论 */
    column_content.clear();
    dbInspect_record.get_column_content(record_table_name, "检定结论",  &column_content);
    QStringList_removeDuplicates(&column_content);
    column_content.insert(0, "全部");
    ui->comboBox_inspect_conclusion->addItems(column_content);

    /* 仪器名称 */
    column_content.clear();
    dbInspect_record.get_column_content(record_table_name, "仪器名称",  &column_content);
    QStringList_removeDuplicates(&column_content);
    column_content.insert(0, "全部");
    ui->comboBox_sample_name->addItems(column_content);

    /* 型号规格 */
    column_content.clear();
    dbInspect_record.get_column_content(record_table_name, "型号规格",  &column_content);
    QStringList_removeDuplicates(&column_content);
    column_content.insert(0, "全部");
    ui->comboBox_sample_specification->addItems(column_content);

    /* 检定员 */
    column_content.clear();
    dbInspect_record.get_column_content(record_table_name, "检定员",  &column_content);
    QStringList_removeDuplicates(&column_content);
    column_content.insert(0, "全部");
    ui->comboBox_inspector->addItems(column_content);

    /* 获得第一张表中的column和数量 */
    QStringList columns;
    dbInspect_record.get_columns(record_table_name, &columns, &columns_count);
    qDebug() << columns << columns_count;

    ui->tableWidget_record->setWindowTitle(record_table_name);
    ui->tableWidget_record->setColumnCount(columns_count);

    //将表头写入表格
    ui->tableWidget_record->setHorizontalHeaderLabels(columns);
    //自动调整宽度
    ui->tableWidget_record->horizontalHeader()->setStretchLastSection(true);

    refresh_record_table();
}

void frmInspectMain::uninitForm(void)
{
    dbInspect_record.close_database();
}

void frmInspectMain::initData(void)
{
    /* 在此打开所有数据库文件 */
    bool open_db;
    const QString filePath = QCoreApplication::applicationDirPath();
    QString fileName_db = filePath + "/db/inspect_record.db";

    open_db = dbInspect_record.open_database(fileName_db);

    if(open_db == false)
    {
        /* 关闭数据库 */
        dbInspect_record.close_database();

        qDebug() << __FUNCTION__ << "Open databas fail!";
    }

    fileName_db = filePath + "/db/inspect_data.db";
    open_db = dbInspect_inspect_data_record.open_database(fileName_db);
    if(open_db == false)
    {
        /* 关闭数据库 */
        dbInspect_inspect_data_record.close_database();
        qDebug() << __FUNCTION__ << "Open databas fail!";
    }

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

void frmInspectMain::uninitData(void)
{
    dbInspect_template_resistance.close_database();
    dbInspect_template_capacitance.close_database();
    dbInspect_template_direct_current.close_database();
    dbInspect_template_direct_voltage.close_database();
    dbInspect_template_alternating_current.close_database();
    dbInspect_template_alternating_voltage.close_database();

    dbInspect_record.close_database();
    dbInspect_inspect_data_record.close_database();
}

/* 根据筛选条件，刷新检定显示的记录表 */
void frmInspectMain::refresh_record_table(void)
{
    QString inspected_institution = ui->comboBox_inspected_institution->currentText();
    QString inspected_date        = ui->comboBox_inspected_date->currentText();
    QString inspect_conclusion    = ui->comboBox_inspect_conclusion->currentText();
    QString sample_name           = ui->comboBox_sample_name->currentText();
    QString sample_specification  = ui->comboBox_sample_specification->currentText();
    QString inspector             = ui->comboBox_inspector->currentText();

    QString select_rule;
    QString select_rule_condition = "";

    if(0 == inspected_institution.compare("全部") && 0 == inspected_date.compare("全部") && 0 == inspect_conclusion.compare("全部")
       && 0 == sample_name.compare("全部") && 0 == sample_specification.compare("全部") && 0 == inspector.compare("全部"))
    {
         select_rule = "select * from " + record_table_name;
    }
    else
    {
        if(0 != inspected_institution.compare("全部"))
        {
            if(true == select_rule_condition.isEmpty())
                select_rule_condition = QString("送检单位 == '%1'").arg(inspected_institution);
            else
                select_rule_condition = select_rule_condition + QString(" and 送检单位 == '%1'").arg(inspected_institution);
        }

        if(0 != inspected_date.compare("全部"))
        {
            if(true == select_rule_condition.isEmpty())
                select_rule_condition = QString("检定日期 == '%1'").arg(inspected_date);
            else
                select_rule_condition = select_rule_condition + QString(" and 检定日期 == '%1'").arg(inspected_date);
        }

        if(0 != inspect_conclusion.compare("全部"))
        {
            if(true == select_rule_condition.isEmpty())
                select_rule_condition = QString("检定结论 == '%1'").arg(inspect_conclusion);
            else
                select_rule_condition = select_rule_condition + QString(" and 检定结论 == '%1'").arg(inspect_conclusion);
        }

        if(0 != sample_name.compare("全部"))
        {
            if(true == select_rule_condition.isEmpty())
                select_rule_condition = QString("仪器名称 == '%1'").arg(sample_name);
            else
                select_rule_condition = select_rule_condition + QString(" and 仪器名称 == '%1'").arg(sample_name);
        }

        if(0 != sample_specification.compare("全部"))
        {
            if(true == select_rule_condition.isEmpty())
                select_rule_condition = QString("型号规格 == '%1'").arg(sample_specification);
            else
                select_rule_condition = select_rule_condition + QString(" and 型号规格 == '%1'").arg(sample_specification);
        }

        if(0 != inspector.compare("全部"))
        {
            if(true == select_rule_condition.isEmpty())
                select_rule_condition = QString("检定员 == '%1'").arg(inspector);
            else
                select_rule_condition = select_rule_condition + QString(" and 检定员 == '%1'").arg(inspector);
        }

        select_rule = "select * from " + record_table_name + " where " + select_rule_condition;
    }
    qDebug() << select_rule;

    QStringList table_content;

    dbInspect_record.get_table_content_by_condition(select_rule, &table_content);

    ui->tableWidget_record->setRowCount(table_content.count());

    QStringListIterator itr_table_content(table_content);

    int row_inc = 0, columm_inc = 0;
    while(itr_table_content.hasNext())
    {
        QString line_content = itr_table_content.next().toLocal8Bit();
        qDebug() << __FUNCTION__ << line_content;

        for(columm_inc = 0; columm_inc < columns_count; columm_inc++)
        {
            ui->tableWidget_record->setItem(row_inc, columm_inc, new QTableWidgetItem(line_content.section(',', columm_inc, columm_inc)));
            //qDebug() << __FUNCTION__ << row_inc << columm_inc << line_content.section(',', columm_inc, columm_inc);
        }

        row_inc++;
    }
}

void frmInspectMain::on_comboBox_inspected_institution_currentIndexChanged(const QString &arg1)
{
    refresh_record_table();
}

void frmInspectMain::on_comboBox_inspected_date_currentIndexChanged(const QString &arg1)
{
    refresh_record_table();
}

void frmInspectMain::on_comboBox_inspect_conclusion_currentIndexChanged(const QString &arg1)
{
    refresh_record_table();
}

void frmInspectMain::on_comboBox_sample_name_currentIndexChanged(const QString &arg1)
{
    refresh_record_table();
}

void frmInspectMain::on_comboBox_sample_specification_currentIndexChanged(const QString &arg1)
{
    refresh_record_table();
}

void frmInspectMain::on_comboBox_inspector_currentIndexChanged(const QString &arg1)
{
    refresh_record_table();
}

void frmInspectMain::on_btn_new_inspect_clicked()
{
    //this->hide();
    frmInspectCheck *frm = new frmInspectCheck;
    frm->show();
}
