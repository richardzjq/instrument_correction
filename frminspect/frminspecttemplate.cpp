﻿#include "frminspecttemplate.h"
#include "ui_frminspecttemplate.h"
#include "inspect_db_global.h"
#include "freetreewidget.h"

frmInspectTemplate::frmInspectTemplate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmInspectTemplate)
{
    ui->setupUi(this);
    this->initData();
    this->initForm();
}

frmInspectTemplate::~frmInspectTemplate()
{
    this->uninitForm();
    delete ui;
}

void frmInspectTemplate::initData(void)
{
    map_string_db.insert("direct_current", dbInspect_template_direct_current);
    map_string_db.insert("direct_voltage", dbInspect_template_direct_voltage);
    map_string_db.insert("alternating_current", dbInspect_template_alternating_current);
    map_string_db.insert("alternating_voltage", dbInspect_template_alternating_voltage);
    map_string_db.insert("resistance", dbInspect_template_resistance);
    map_string_db.insert("capacitance", dbInspect_template_capacitance);

    struct template_header template_header_current;
    template_header_current.template_rang = "range_A";
    template_header_current.template_standard_value = "standard_A";
    template_header_current.template_standard_max_error = "error";
    maps_template_header.insert("direct_current", template_header_current);
    maps_template_header.insert("alternating_current", template_header_current);

    struct template_header template_header_voltage;
    template_header_voltage.template_rang = "range_V";
    template_header_voltage.template_standard_value = "standard_V";
    template_header_voltage.template_standard_max_error = "error";
    maps_template_header.insert("direct_voltage", template_header_voltage);
    maps_template_header.insert("alternating_voltage", template_header_voltage);

    struct template_header template_header_resistance;
    template_header_resistance.template_rang = "range_O";
    template_header_resistance.template_standard_value = "standard_O";
    template_header_resistance.template_standard_max_error = "error";
    maps_template_header.insert("resistance", template_header_resistance);

    struct template_header template_header_capacitance;
    template_header_capacitance.template_rang = "range_pf";
    template_header_capacitance.template_standard_value = "standard_pf";
    template_header_capacitance.template_standard_max_error = "error";
    maps_template_header.insert("capacitance", template_header_capacitance);
}

void frmInspectTemplate::initForm(void)
{
    /* 初始化各项 */
    QStringList strList;

    ui->lineEdit_name->setText("");

    strList.clear();
    strList << "" << "direct_current" << "direct_voltage" << "alternating_current" << "alternating_voltage" << "resistance" << "capacitance";
    ui->comboBox_type->addItems(strList);

    ui->radioButton_fixed_output->setChecked(true);
    ui->radioButton_adjust_output->setChecked(false);

    ui->checkBox_other_setting->setChecked(false);

    /* 设置treeWidget */
    ui->treeWidget_template->setColumnCount(1);

    strList.clear();
    strList << "模板类别";
    ui->treeWidget_template->setHeaderLabels(strList);

    QTreeWidgetItem* leaf;
    QStringList tables;
    int tables_count;

    /* 直流电流 */
    QTreeWidgetItem*  root_direct_current = new QTreeWidgetItem(ui->treeWidget_template, QStringList(QString("direct_current")));
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
    QTreeWidgetItem*  root_direct_voltage = new QTreeWidgetItem(ui->treeWidget_template, QStringList(QString("direct_voltage")));
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
    QTreeWidgetItem*  root_alternating_current = new QTreeWidgetItem(ui->treeWidget_template, QStringList(QString("alternating_current")));
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
    QTreeWidgetItem*  root_alternating_voltage = new QTreeWidgetItem(ui->treeWidget_template, QStringList(QString("alternating_voltage")));
    dbInspect_template_alternating_voltage.get_tables(&tables, &tables_count);
    QStringListIterator itr_template_alternating_voltage(tables);

    while (itr_template_alternating_voltage.hasNext())
    {
        QString template_alternating_voltage = itr_template_alternating_voltage.next().toLocal8Bit();
        leaf = new QTreeWidgetItem(root_alternating_voltage, QStringList(template_alternating_voltage));
        root_alternating_voltage->addChild(leaf);
        qDebug() << template_alternating_voltage;
    }

    /* 电阻 */
    QTreeWidgetItem*  root_resistance = new QTreeWidgetItem(ui->treeWidget_template, QStringList(QString("resistance")));
    dbInspect_template_resistance.get_tables(&tables, &tables_count);
    QStringListIterator itr_template_resistance(tables);

    while (itr_template_resistance.hasNext())
    {
        QString template_resistance = itr_template_resistance.next().toLocal8Bit();
        leaf = new QTreeWidgetItem(root_resistance, QStringList(template_resistance));
        root_resistance->addChild(leaf);
        qDebug() << template_resistance;
    }


    /* 电容 */
    QTreeWidgetItem*  root_capacitance = new QTreeWidgetItem(ui->treeWidget_template, QStringList(QString("capacitance")));
    dbInspect_template_capacitance.get_tables(&tables, &tables_count);
    QStringListIterator itr_template_capacitance(tables);

    while (itr_template_capacitance.hasNext())
    {
        QString template_capacitance = itr_template_capacitance.next().toLocal8Bit();
        leaf = new QTreeWidgetItem(root_capacitance, QStringList(template_capacitance));
        root_capacitance->addChild(leaf);
        qDebug() << template_capacitance;
    }

    QList<QTreeWidgetItem *> rootList;
    rootList << root_direct_current << root_direct_voltage << root_alternating_current << root_alternating_voltage << root_resistance << root_capacitance;

    ui->treeWidget_template->insertTopLevelItems(0, rootList);

    ui->treeWidget_template->expandAll();

    ui->btn_edit_line->setEnabled(false);
    ui->btn_move_up->setEnabled(false);
    ui->btn_move_down->setEnabled(false);
    ui->btn_delete->setEnabled(false);
}

void frmInspectTemplate::uninitForm(void)
{
    int top_level_count = ui->treeWidget_template->topLevelItemCount();
    int top_leve_count_each = ui->treeWidget_template->topLevelItemCount();
    qDebug() << top_leve_count_each;
    QTreeWidgetItem* root = nullptr;

    while(top_level_count > 0)
    {
        top_level_count--;
        top_leve_count_each = ui->treeWidget_template->topLevelItemCount();
        root= ui->treeWidget_template->takeTopLevelItem(0);
        qDebug() << top_leve_count_each << root;
        freeTreeWidget(root);
    }
}

void frmInspectTemplate::on_treeWidget_template_itemClicked(QTreeWidgetItem *item, int column)
{
    /* 获取当前item的父item */
    QTreeWidgetItem * parent_item = item->parent();

    /* 先判断是不是root item， 如果是，什么是也不做，返回 */
    if(nullptr == parent_item)
        return;

    /* 获取item text */
    QString template_name = item->text(column);
    QString template_type = parent_item->text(column);

    ui->lineEdit_name->setText(template_name);
    ui->comboBox_type->setCurrentText(template_type);

    /* 获取模板数据库里的数据，显示在tableWidget中 */
    /* 得到数据库 */
    QStringList columns, table_content;
    int columns_count;
    DBInspect dbInspect_template;

    dbInspect_template = map_string_db[template_type];
    dbInspect_template.get_columns(template_name, &columns, &columns_count);
    qDebug() << columns << columns_count;

    ui->tableWidget_template->setWindowTitle(template_name);
    ui->tableWidget_template->setColumnCount(columns_count);

    //将表头写入表格
    ui->tableWidget_template->setHorizontalHeaderLabels(columns);
    //自动调整宽度
    ui->tableWidget_template->horizontalHeader()->setStretchLastSection(true);

    dbInspect_template.get_table_content(template_name, &table_content);
    ui->tableWidget_template->setRowCount(table_content.count());

    QStringListIterator itr_table_content(table_content);

    int row_inc = 0, columm_inc = 0;
    while(itr_table_content.hasNext())
    {
        QString line_content = itr_table_content.next().toLocal8Bit();
        //qDebug() << __FUNCTION__ << line_content;

        for(columm_inc = 0; columm_inc < columns_count; columm_inc++)
        {
            ui->tableWidget_template->setItem(row_inc, columm_inc, new QTableWidgetItem(line_content.section(',', columm_inc, columm_inc)));
            //qDebug() << __FUNCTION__ << row_inc << columm_inc << line_content.section(',', columm_inc, columm_inc);
        }

        row_inc++;
    }
}

void frmInspectTemplate::on_btn_create_clicked()
{
    /* 获取模板名称 */
    QString template_name = ui->lineEdit_name->text();

    /* 获取模板类别 */
    QString template_type = ui->comboBox_type->currentText();

    /* 在模板treeWidget加入一个节点 */
    QList<QTreeWidgetItem *> rootList;
    rootList = ui->treeWidget_template->findItems(template_type, nullptr, 0);
    QTreeWidgetItem* root = rootList.at(0);
    QTreeWidgetItem* leaf = new QTreeWidgetItem(root, QStringList(template_name));
    root->addChild(leaf);

    ui->treeWidget_template->expandAll();

    /* 设置tableWidget的header */
    QStringList strList;

    strList << maps_template_header[template_type].template_rang << maps_template_header[template_type].template_standard_value << maps_template_header[template_type].template_standard_max_error;

    ui->tableWidget_template->setWindowTitle(template_name);
    ui->tableWidget_template->setColumnCount(3);

    //将表头写入表格
    ui->tableWidget_template->setHorizontalHeaderLabels(strList);
    //自动调整宽度
    ui->tableWidget_template->horizontalHeader()->setStretchLastSection(true);

    ui->tableWidget_template->setRowCount(1);
}

void frmInspectTemplate::on_btn_save_clicked()
{
    /* 保存treeWidget的内容到数据库 */

    /* 获取模板名称 */
    QString template_name = ui->lineEdit_name->text();
    qDebug() << __FUNCTION__ << template_name;

    /* 获取模板类别 */
    QString template_type = ui->comboBox_type->currentText();
    qDebug() << __FUNCTION__ << template_type;

    /* 获取数据库 */
    DBInspect dbInspect_template;
    struct template_header template_header_col = maps_template_header[template_type];
    QString columns = template_header_col.template_rang + " double, " + template_header_col.template_standard_value + " double, " + template_header_col.template_standard_max_error + " double";

    dbInspect_template = map_string_db[template_type];

    /* 向数据库中加入一个table */
    dbInspect_template.add_one_table(template_name, columns);
    int row_count = ui->tableWidget_template->rowCount();
    QString add_line_instruction;

    for(int row_inc = 0; row_inc < row_count; row_inc++)
    {
        add_line_instruction = "INSERT INTO " + template_name + " (" + template_header_col.template_rang + ", " +  template_header_col.template_standard_value + ", " + template_header_col.template_standard_max_error + ") VALUES ("
                                   + ui->tableWidget_template->item(row_inc, 0)->text() + ", " + ui->tableWidget_template->item(row_inc, 1)->text()  + ", " + ui->tableWidget_template->item(row_inc, 2)->text() + ")";

        //qDebug() << __FUNCTION__ << add_line_instruction;
        dbInspect_template.add_one_line_into_table(add_line_instruction);
    }
}

void frmInspectTemplate::on_btn_add_line_clicked()
{
    /* tableWidget增加一行，放在最后面 */
    int row_count = ui->tableWidget_template->rowCount();

    ui->tableWidget_template->insertRow(row_count);
    ui->tableWidget_template->setRowCount(row_count + 1);
}

void frmInspectTemplate::on_btn_delete_line_clicked()
{
    /* tableWidget删除选中的行 */
    int row_index = ui->tableWidget_template->currentRow();
    int row_count = ui->tableWidget_template->rowCount();

    if (row_index >= 0)
        ui->tableWidget_template->removeRow(row_index);
    if (row_count >= 1)
        ui->tableWidget_template->setRowCount(row_count - 1);
}

void frmInspectTemplate::on_btn_delete_clicked()
{

}
