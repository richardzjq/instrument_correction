#include "frminspectmain.h"
#include "ui_frminspectmain.h"


frmInspectMain::frmInspectMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmInspectMain)
{
    ui->setupUi(this);

    this->initForm();
}

frmInspectMain::~frmInspectMain()
{
    this->uninitForm();
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

    /* 打开数据库 */
    bool open_record_db;
    const QString filePath = QCoreApplication::applicationDirPath();
    QString fileName_db_record = filePath + "/db/inspect_record.db";

    open_record_db = dbInspect_record.open_database(fileName_db_record);

    if(open_record_db == false)
    {
        /* 关闭数据库 */
        dbInspect_record.close_database();

        qDebug() << __FUNCTION__ << "Open databas fail!";
    }

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
            select_rule_condition = "送检单位 == " + inspected_institution;
        }

        if(0 != inspected_date.compare("全部"))
        {
            select_rule_condition = " and 检定日期 == " + inspected_date;
        }

        if(0 != inspect_conclusion.compare("全部"))
        {
            select_rule_condition = " and 检定结论 == " + inspect_conclusion;
        }

        if(0 != sample_name.compare("全部"))
        {
            select_rule_condition = " and 仪器名称 == " + sample_name;
        }

        if(0 != sample_specification.compare("全部"))
        {
            select_rule_condition = " and 型号规格 == " + sample_specification;
        }

        if(0 != inspector.compare("全部"))
        {
            select_rule_condition = " and 检定员 == " + inspector;
        }

        select_rule = "select * where " + select_rule_condition + " from " + record_table_name;
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
            qDebug() << __FUNCTION__ << row_inc << columm_inc << line_content.section(',', columm_inc, columm_inc);
        }

        row_inc++;
    }
}
