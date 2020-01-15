#include "frminspectcheck.h"
#include "frminspectproject.h"
#include "ui_frminspectcheck.h"
#include "gpibctl.h"
#include "inspect_db_global.h"
#include "wordapi.h"
#include "freetreewidget.h"
#include <QDateTime>
#include<QDir>
#include<QDebug>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QModelIndex>


frmInspectCheck::frmInspectCheck(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmInspectCheck)
{
    ui->setupUi(this);
    this->initForm();
}

frmInspectCheck::~frmInspectCheck()
{
    this->uninitForm();
    delete ui;
}

void frmInspectCheck::initForm(void)
{
    /* 初始化记录编号等各项 */
    ui->lineEdit_record_number->setText("0");
    QString current_Date_Time = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    ui->lineEdit_inspect_date->setText(current_Date_Time);

    QStringList strList;

    strList.clear();
    strList << "" << "合格" << "不合格";
    ui->comboBox_inspect_conclusion->addItems(strList);

    strList.clear();
    strList << "" << "湖北钢铁厂" << "湖北电科院";
    ui->comboBox_inspected_institution->addItems(strList);

    strList.clear();
    strList << "" << "刘慈欣" << "逻辑" << "史强";
    ui->comboBox_inspector->addItems(strList);

    strList.clear();
    strList << "" << "程心" << "韦德" << "乔恩斯";
    ui->comboBox_verifier->addItems(strList);

    strList.clear();
    strList << "" << "数字式万用表" << "指针式万用表";
    ui->comboBox_sample_name->addItems(strList);

    strList.clear();
    strList << "" << "UT52" << "UT58";
    ui->comboBox_model_specification->addItems(strList);

    ui->lineEdit_temperature->setText("0");
    ui->lineEdit_humidity->setText("0");

    strList.clear();
    strList << "" << "UNI-T" << "UNI-S";
    ui->comboBox_manufacture->addItems(strList);

    ui->lineEdit_manufacted_number->setText("");

    strList.clear();
    strList << "" << "UNI-T" << "UNI-S";
    ui->comboBox_standard_device->addItems(strList);

    strList.clear();
    strList << "" << "JJG315-83" << "JJG315-8398";
    ui->comboBox_refered_rule->addItems(strList);

    strList.clear();
    strList << "" << "数字式万用表200-200M" << "数字式万用表2n-200u" << "数字式万用表2mA-20A";
    ui->comboBox_template->addItems(strList);    

    /* 设置treeWidget */
    ui->treeWidget_inpsected_project->setColumnCount(1);

    strList.clear();
    strList << "已检定项目";
    ui->treeWidget_inpsected_project->setHeaderLabels(strList);

    QTreeWidgetItem* root = new QTreeWidgetItem(ui->treeWidget_inpsected_project, QStringList(QString("已检定项目")));

    /* 查询  检定记录编号  的所有记录 */
    strList.clear();
    dbInspect_record.get_column_content("检定记录", "检定记录编号", &strList);
    QStringListIterator itr_inspected_projects(strList);
    QTreeWidgetItem* leaf;

    while (itr_inspected_projects.hasNext())
    {
        QString project_name = itr_inspected_projects.next().toLocal8Bit();
        leaf = new QTreeWidgetItem(root, QStringList(project_name));
        root->addChild(leaf);
        qDebug() << project_name;
    }

    QList<QTreeWidgetItem *> rootList;
    rootList << root;
    ui->treeWidget_inpsected_project->insertTopLevelItems(0, rootList);

    ui->treeWidget_inpsected_project->expandAll();

    /* 设置tableWidget */
    /* 获得第一张表中的column和数量 */
    QStringList columns;
    columns << "量程" << "标准值" << "显示值" << "误差%";

    ui->tableWidget_inspect_result->setWindowTitle("检定模板和结果");
    ui->tableWidget_inspect_result->setColumnCount(4);

    //将表头写入表格
    ui->tableWidget_inspect_result->setHorizontalHeaderLabels(columns);
    //自动调整宽度
    ui->tableWidget_inspect_result->horizontalHeader()->setStretchLastSection(true);

    ui->btn_cancel_modification->setEnabled(false);
    ui->btn_print->setEnabled(false);
}

void frmInspectCheck::uninitForm(void)
{
    QTreeWidgetItem* root = ui->treeWidget_inpsected_project->takeTopLevelItem(0);
    freeTreeWidget(root);
}

void frmInspectCheck::clearForm()
{
    /* 各项清零 */
    ui->lineEdit_record_number->setText("0");
    QString current_Date_Time = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    ui->lineEdit_inspect_date->setText(current_Date_Time);

    ui->comboBox_inspect_conclusion->setCurrentIndex(0);
    ui->comboBox_inspected_institution->setCurrentIndex(0);
    ui->comboBox_inspector->setCurrentIndex(0);
    ui->comboBox_verifier->setCurrentIndex(0);
    ui->comboBox_sample_name->setCurrentIndex(0);
    ui->comboBox_model_specification->setCurrentIndex(0);

    ui->lineEdit_temperature->setText("0");
    ui->lineEdit_humidity->setText("0");

    ui->comboBox_manufacture->setCurrentIndex(0);

    ui->lineEdit_manufacted_number->setText("");

    ui->comboBox_refered_rule->setCurrentIndex(0);

    ui->comboBox_template->setCurrentIndex(0);
}

void frmInspectCheck::on_btn_create_clicked()
{
    this->clearForm();
}

void frmInspectCheck::on_btn_save_clicked()
{
    /* 获取各项值 */
    /* 检定记录编号 */
    QString record_number = ui->lineEdit_record_number->text();

    /* 检定日期 */
    QString inspect_date = ui->lineEdit_inspect_date->text();

    /* 检定结论 */
    QString inspect_conclusion = ui->comboBox_inspect_conclusion->currentText();

    /* 送检单位 */
    QString inspected_institution = ui->comboBox_inspected_institution->currentText();

    /* 检定员 */
    QString inspector = ui->comboBox_inspector->currentText();

    /* 核验员 */
    QString verifier = ui->comboBox_verifier->currentText();

    /* 仪器名称 */
    QString sample_name = ui->comboBox_sample_name->currentText();

    /* 型号规格 */
    QString model_specification = ui->comboBox_model_specification->currentText();

    /* 温度 */
    QString temperature = ui->lineEdit_temperature->text();

    /* 湿度 */
    QString humidity = ui->lineEdit_humidity->text();

    /* 制造商 */
    QString manufacture = ui->comboBox_manufacture->currentText();

    /* 出厂编号 */
    QString manufacted_number = ui->lineEdit_manufacted_number->text();

    /* 标准设备 */
    QString standard_device = ui->comboBox_standard_device->currentText();

    /* 依据规程 */
    QString refered_rule = ui->comboBox_refered_rule->currentText();

    /* 模板 */
    QString inpsect_template = ui->comboBox_template->currentText();

    /* 记录写入数据库 */
    QString add_line_instruction = QString("insert into 检定记录 (检定记录编号, 送检单位, 仪器名称, 型号规格, 生产厂家, 出厂编号, 检定日期, 检定员, 检定结论, 核验员, 环境温度, 相对湿度, 依据规程, 检定模板) VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12', '%13', '%14')")
                                                                .arg(record_number).arg(inspected_institution).arg(sample_name).arg(model_specification).arg(manufacture).arg(manufacted_number).arg(inspect_date).arg(inspector).arg(inspect_conclusion).arg(verifier).arg(temperature).arg(humidity).arg(refered_rule).arg(inpsect_template);
    dbInspect_record.add_one_line_into_table(add_line_instruction);

    /* 已检定项目TreeWidget加入一项 */
    QTreeWidgetItem* root = ui->treeWidget_inpsected_project->topLevelItem(0);
    QTreeWidgetItem* leaf = new QTreeWidgetItem(root, QStringList(ui->lineEdit_record_number->text()));
    root->addChild(leaf);

    ui->treeWidget_inpsected_project->expandAll();
}

void frmInspectCheck::on_btn_delete_inspected_project_clicked()
{
    ui->treeWidget_inpsected_project->expandAll();

    /* 从数据库中删除该行 */
    QString delete_line_instruction = QString("DELETE FROM 检定记录 WHERE 检定记录编号 == '%1'").arg(ui->treeWidget_inpsected_project->currentItem()->text(0));
    dbInspect_record.delete_one_line_into_table(delete_line_instruction);

    /* 获得当root节点 */
    QTreeWidgetItem* root = ui->treeWidget_inpsected_project->topLevelItem(0);
    /* 获得当前节点 */
    QTreeWidgetItem* item= ui->treeWidget_inpsected_project->currentItem();
    /* 删除 */
    root->removeChild(item);
    /* 释放item占用的内存 */
    delete item;

    this->clearForm();
}

void frmInspectCheck::on_btn_inspect_new_project_clicked()
{
    //this->hide();
    frmInspectProject *frm = new frmInspectProject;
    frm->getData(ui->lineEdit_record_number->text());
    frm->show();
}

void frmInspectCheck::on_treeWidget_inpsected_project_itemClicked(QTreeWidgetItem *item, int column)
{
    /* 获取item text */
    QString project_name = item->text(column);

    /* 获取各项值 */
    QString select_rule = QString("select * from 检定记录 where 检定记录编号 == '%1'").arg(project_name);
    QStringList row_content;

    dbInspect_record.get_table_content_by_condition(select_rule, &row_content);
    QStringListIterator itr_row_content(row_content);

    /* 只取第一行数据 */
    QString line_content = itr_row_content.next().toLocal8Bit();

    /* 检定记录编号 */
    QString record_number = line_content.section(',', 0, 0);
    ui->lineEdit_record_number->setText(record_number);

    /* 送检单位 */
    QString inspected_institution = line_content.section(',', 1, 1);
    ui->comboBox_inspected_institution->setCurrentText(inspected_institution);

    /* 仪器名称 */
    QString sample_name = line_content.section(',', 2, 2);
    ui->comboBox_sample_name->setCurrentText(sample_name);

    /* 型号规格 */
    QString model_specification = line_content.section(',', 3, 3);
    ui->comboBox_model_specification->setCurrentText(model_specification);

    /* 生产厂家 */
    QString manufacture = line_content.section(',', 4, 4);
    ui->comboBox_manufacture->setCurrentText(manufacture);

    /* 出厂编号 */
    QString manufacted_number = line_content.section(',', 5, 5);
    ui->lineEdit_manufacted_number->setText(manufacted_number);

    /* 检定日期 */
    QString inspect_date = line_content.section(',', 6, 6);
    ui->lineEdit_inspect_date->setText(inspect_date);

    /* 检定员 */
    QString inspector = line_content.section(',', 7, 7);
    ui->comboBox_inspector->setCurrentText(inspector);

    /* 检定结论 */
    QString inspect_conclusion = line_content.section(',', 8, 8);
    ui->comboBox_inspect_conclusion->setCurrentText(inspect_conclusion);

    /* 核验员 */
    QString verifier = line_content.section(',', 9, 9);
    ui->comboBox_verifier->setCurrentText(verifier);

    /* 温度 */
    QString temperature = line_content.section(',', 10, 10);
    ui->lineEdit_temperature->setText(temperature);

    /* 湿度 */
    QString humidity = line_content.section(',', 11, 11);
    ui->lineEdit_humidity->setText(humidity);

    /* 依据规程 */
    QString refered_rule = line_content.section(',', 12, 12);
    ui->comboBox_refered_rule->setCurrentText(refered_rule);

    /* 模板 */
    QString inpsect_template = line_content.section(',', 13, 13);
    ui->comboBox_template->setCurrentText(inpsect_template);

    /* tableWidget的内容展示 */
    /* 得到数据库 */
    QStringList columns, table_content;
    int columns_count;
    dbInspect_inspect_data_record.get_columns(record_number, &columns, &columns_count);
    qDebug() << columns << columns_count;

    ui->tableWidget_inspect_result->setWindowTitle(record_number);
    ui->tableWidget_inspect_result->setColumnCount(columns_count);

    //将表头写入表格
    ui->tableWidget_inspect_result->setHorizontalHeaderLabels(columns);
    //自动调整宽度
    ui->tableWidget_inspect_result->horizontalHeader()->setStretchLastSection(true);

    dbInspect_inspect_data_record.get_table_content(record_number, &table_content);
    ui->tableWidget_inspect_result->setRowCount(table_content.count());

    QStringListIterator itr_table_content(table_content);

    int row_inc = 0, columm_inc = 0;
    while(itr_table_content.hasNext())
    {
        QString line_content = itr_table_content.next().toLocal8Bit();
        qDebug() << __FUNCTION__ << line_content;

        for(columm_inc = 0; columm_inc < columns_count; columm_inc++)
        {
            ui->tableWidget_inspect_result->setItem(row_inc, columm_inc, new QTableWidgetItem(line_content.section(',', columm_inc, columm_inc)));
            //qDebug() << __FUNCTION__ << row_inc << columm_inc << line_content.section(',', columm_inc, columm_inc);
        }

        row_inc++;
    }
}
