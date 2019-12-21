#include "frminspecttemplate.h"
#include "ui_frminspecttemplate.h"

frmInspectTemplate::frmInspectTemplate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmInspectTemplate)
{
    ui->setupUi(this);
    this->initForm();
}

frmInspectTemplate::~frmInspectTemplate()
{
    this->uninitForm();
    delete ui;
}

void frmInspectTemplate::initForm(void)
{
    /* 初始化各项 */
    QStringList strList;

    ui->lineEdit_name->setText("");

    strList.clear();
    strList << "" << "直流电流" << "直流电压" << "交流电流" << "交流电压" << "电阻" << "电容";
    ui->comboBox_type->addItems(strList);

    ui->radioButton_fixed_output->setChecked(true);
    ui->radioButton_adjust_output->setChecked(false);

    ui->checkBox_other_setting->setChecked(false);

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

    /* 设置treeWidget */
    ui->treeWidget_template->setColumnCount(1);

    strList.clear();
    strList << "模板类别";
    ui->treeWidget_template->setHeaderLabels(strList);

    QTreeWidgetItem* leaf;
    QStringList tables;
    int tables_count;

    /* 电阻 */
    QTreeWidgetItem*  root_capacitance = new QTreeWidgetItem(ui->treeWidget_template, QStringList(QString("电阻")));
    dbInspect_template_capacitance.get_tables(&tables, &tables_count);
    QStringListIterator itr_template_capacitance(tables);

    while (itr_template_capacitance.hasNext())
    {
        QString template_capacitance = itr_template_capacitance.next().toLocal8Bit();
        leaf = new QTreeWidgetItem(root_capacitance, QStringList(template_capacitance));
        root_capacitance->addChild(leaf);
        qDebug() << template_capacitance;
    }

    /* 电容 */
    QTreeWidgetItem*  root_resistance = new QTreeWidgetItem(ui->treeWidget_template, QStringList(QString("电容")));
    dbInspect_template_resistance.get_tables(&tables, &tables_count);
    QStringListIterator itr_template_resistance(tables);

    while (itr_template_resistance.hasNext())
    {
        QString template_resistance = itr_template_resistance.next().toLocal8Bit();
        leaf = new QTreeWidgetItem(root_resistance, QStringList(template_resistance));
        root_resistance->addChild(leaf);
        qDebug() << template_resistance;
    }

    /* 直流电流 */
    QTreeWidgetItem*  root_direct_current = new QTreeWidgetItem(ui->treeWidget_template, QStringList(QString("直流电流")));
    dbInspect_template_direct_current.get_tables(&tables, &tables_count);
    QStringListIterator itr_template_direct_current(tables);

    while (itr_template_direct_current.hasNext())
    {
        QString template_direct_current = itr_template_direct_current.next().toLocal8Bit();
        leaf = new QTreeWidgetItem(root_direct_current, QStringList(template_direct_current));
        root_direct_current->addChild(leaf);
        qDebug() << template_direct_current;
    }

    /* 直流电压 */
    QTreeWidgetItem*  root_direct_voltage = new QTreeWidgetItem(ui->treeWidget_template, QStringList(QString("直流电压")));
    dbInspect_template_direct_voltage.get_tables(&tables, &tables_count);
    QStringListIterator itr_template_direct_voltage(tables);

    while (itr_template_direct_voltage.hasNext())
    {
        QString template_direct_voltage = itr_template_direct_voltage.next().toLocal8Bit();
        leaf = new QTreeWidgetItem(root_direct_voltage, QStringList(template_direct_voltage));
        root_direct_voltage->addChild(leaf);
        qDebug() << template_direct_voltage;
    }

    /* 交流电流 */
    QTreeWidgetItem*  root_alternating_current = new QTreeWidgetItem(ui->treeWidget_template, QStringList(QString("交流电流")));
    dbInspect_template_alternating_current.get_tables(&tables, &tables_count);
    QStringListIterator itr_template_alternating_current(tables);

    while (itr_template_alternating_current.hasNext())
    {
        QString template_alternating_current = itr_template_alternating_current.next().toLocal8Bit();
        leaf = new QTreeWidgetItem(root_alternating_current, QStringList(template_alternating_current));
        root_alternating_current->addChild(leaf);
        qDebug() << template_alternating_current;
    }

    /* 交流电压 */
    QTreeWidgetItem*  root_alternating_voltage = new QTreeWidgetItem(ui->treeWidget_template, QStringList(QString("交流电压")));
    dbInspect_template_alternating_voltage.get_tables(&tables, &tables_count);
    QStringListIterator itr_template_alternating_voltage(tables);

    while (itr_template_alternating_voltage.hasNext())
    {
        QString template_alternating_voltage = itr_template_alternating_voltage.next().toLocal8Bit();
        leaf = new QTreeWidgetItem(root_alternating_voltage, QStringList(template_alternating_voltage));
        root_alternating_voltage->addChild(leaf);
        qDebug() << template_alternating_voltage;
    }

    QList<QTreeWidgetItem *> rootList;
    rootList << root_capacitance << root_resistance << root_direct_current << root_direct_voltage << root_alternating_current << root_alternating_voltage;

    ui->treeWidget_template->insertTopLevelItems(0, rootList);

    ui->treeWidget_template->expandAll();
}

void frmInspectTemplate::uninitForm(void)
{
    dbInspect_template_resistance.close_database();
    dbInspect_template_capacitance.close_database();
    dbInspect_template_direct_current.close_database();
    dbInspect_template_direct_voltage.close_database();
    dbInspect_template_alternating_current.close_database();
    dbInspect_template_alternating_voltage.close_database();
}

void frmInspectTemplate::on_btn_create_clicked()
{
    /* 获取模板名称 */
    QString template_name = ui->lineEdit_name->text();

    /* 获取模板类别 */
    QString template_type = ui->comboBox_type->currentText();


}

void frmInspectTemplate::on_treeWidget_template_itemClicked(QTreeWidgetItem *item, int column)
{
    /* 先判断是不是root item， 如果是，什么是也不做，返回 */

    /* 获取item text */
    QString project_name = item->text(column);

    /* 获取root */
}
