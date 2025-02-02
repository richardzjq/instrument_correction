﻿#include "frminspectcheck.h"
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

static void get_string_list_from_file(QString file_name, QStringList *p_stringList)
{
    QString line;

    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
       return;

    QTextStream file_text(&file);

    //in.setCodec("GBK");
    while (!file_text.atEnd())
    {
        line = file_text.readLine();
        *p_stringList << line;
    }

    file.close();
    qDebug() << *p_stringList;
}

void frmInspectCheck::get_inspect_template_list(QStringList *p_stringList)
{

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
    strList << "";
    get_string_list_from_file("txt/送检单位.txt", &strList);
    //qDebug() << strList;
    ui->comboBox_inspected_institution->addItems(strList);

    strList.clear();
    strList << "";
    get_string_list_from_file("txt/检验员.txt", &strList);
    ui->comboBox_inspector->addItems(strList);

    strList.clear();
    strList << "";
    get_string_list_from_file("txt/校验员.txt", &strList);
    ui->comboBox_verifier->addItems(strList);

    strList.clear();
    strList << "";
    get_string_list_from_file("txt/样例名称.txt", &strList);
    ui->comboBox_sample_name->addItems(strList);

    strList.clear();
    strList << "";
    get_string_list_from_file("txt/校验规格.txt", &strList);
    ui->comboBox_model_specification->addItems(strList);

    ui->lineEdit_temperature->setText("0");
    ui->lineEdit_humidity->setText("0");

    strList.clear();
    strList << "";
    get_string_list_from_file("txt/生产厂家.txt", &strList);
    ui->comboBox_manufacture->addItems(strList);

    ui->lineEdit_manufacted_number->setText("");

    strList.clear();
    strList << "";
    get_string_list_from_file("txt/标准设备.txt", &strList);
    ui->comboBox_standard_device->addItems(strList);

    strList.clear();
    strList << "";
    get_string_list_from_file("txt/参考标准.txt", &strList);
    ui->comboBox_refered_rule->addItems(strList);

    strList.clear();
    strList << "";
    get_string_list_from_file("txt/校准模板.txt", &strList);
    /* 以后从模板数据库里获取 */
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

void frmInspectCheck::print_certificate()
{
    /* 打印证书 */
    QString current_Date_Time = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString cert_number = ui->lineEdit_record_number->text();
    QString fileName = tr("校准证书") + "_" + cert_number + "_" + current_Date_Time;
    QString filePath = QCoreApplication::applicationDirPath() + "/doc/";
    QDate Cur_Date= QDate::currentDate();
    QString strCurDate = Cur_Date.toString("yyyy-MM-dd");

    QDir dirReportPath(filePath);
    if (!dirReportPath.exists())
    {
        if (dirReportPath.mkpath(filePath))
        {
            filePath += fileName + tr(".doc");
        }
    }
    else
    {
        filePath += fileName + tr(".doc");
    }

    WordApi word;
    if( !word.createNewWord(filePath) )
    {
        QString error = tr("Failed to export report,") + word.getStrErrorInfo();
        qDebug() << error;
        return;
    }

    /* 设置格式 */
    word.setPageOrientation(0);			//页面方向
    word.setWordPageView(3);			//页面视图
    //word.setFontName(QString::fromLocal8Bit("宋体"));
    word.setParagraphAlignment(0);		//下面文字位置
    word.setFontSize(20);				//字体大小
    word.setFontBold(true);				//字体加粗
    word.insertText(tr("校准证书内页"));
    word.setFontBold(false);
    word.insertMoveDown();
    word.insertMoveDown();
    word.setFontSize(10);

    word.setParagraphAlignment(1);
    word.insertText(tr("证书编号：   "));
    word.insertText(cert_number);
    word.insertMoveDown();

    //first table
    word.intsertTable(2, 1);
    word.setFontSize(10);
    //word.setColumnWidth(1,1,120);

    //word.MergeCells(1, 1, 1, 1, 5);//合并第一行单元格
    //word.MergeCells(1, 2, 1, 2, 5);//合并第二行单元格
    //word.MergeCells(1, 3, 4, 3, 5);//合并第三行单元格
    //word.MergeCells(1, 4, 4, 4, 5);//合并第四行单元格
    //word.MergeCells(1, 5, 1, 5, 5);//合并第五行单元格
    //word.MergeCells(1, 6, 1, 6, 5);//合并第六行单元格

    //word.setRowHeight(1, 1, 120);
    //word.setRowHeight(1, 2, 240);

    //word.setRowAlignment(1,1,1);
    word.setCellString(1,1,1,tr("校准机构授权说明"));

    //word.setRowAlignment(1,2,1);
    word.setCellString(1,2,1,tr("校准环境条件及地点："));

    word.moveForEnd();
    word.intsertTable(1, 2, 4);

    //word.setRowAlignment(1,3,1);
    word.setCellString(1,3,1,tr("温度"));
    QString tempera = ui->lineEdit_temperature->text() + "  °C";
    word.setCellString(1,3,2,tempera);
    word.setCellString(1,3,3,tr("地点"));

    //word.setRowAlignment(1,4,1);
    word.setCellString(1,4,1,tr("相对湿度"));
    QString humidity = ui->lineEdit_humidity->text() + "  %";
    word.setCellString(1,4,2,humidity);
    word.setCellString(1,4,3,tr("其他"));

    word.moveForEnd();
    word.intsertTable(1, 2, 1);

    //word.setRowAlignment(1,5,1);
    word.setCellString(1,5,1,tr("校准所依据的技术文件（代号、名称）:  "));

    //word.setRowAlignment(1,6,1);
    word.setCellString(1,6,1,tr("校准所使用的主要测量标准：  "));

    word.moveForEnd();
    word.intsertTable(1, 2, 5);

    //word.setRowAlignment(1,7,1);
    word.setCellString(1,7,1,tr("名称"));
    word.setCellString(1,7,2,tr("测量范围"));
    word.setCellString(1,7,3,tr("最大允许误差"));
    word.setCellString(1,7,4,tr("证书编号"));
    word.setCellString(1,7,5,tr("证书有效期"));

    word.moveForEnd();
    word.insertMoveDown();

    word.setVisible(true);
    word.saveAs();
}

void frmInspectCheck::print_check_result()
{
    /* 打印测试结果 */
    QString current_Date_Time = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString cert_number = ui->lineEdit_record_number->text();
    QString fileName = tr("测试结果") + "_" + cert_number + "_" + current_Date_Time;
    QString filePath = QCoreApplication::applicationDirPath() + "/doc/";
    QDate Cur_Date= QDate::currentDate();
    QString strCurDate = Cur_Date.toString("yyyy-MM-dd");

    QDir dirReportPath(filePath);
    if (!dirReportPath.exists())
    {
        if (dirReportPath.mkpath(filePath))
        {
            filePath += fileName + tr(".doc");
        }
    }
    else
    {
        filePath += fileName + tr(".doc");
    }

    WordApi word;
    if( !word.createNewWord(filePath) )
    {
        QString error = tr("Failed to export report,") + word.getStrErrorInfo();
        qDebug() << error;
        return;
    }

    /* 设置格式 */
    word.setPageOrientation(0);			//页面方向
    word.setWordPageView(3);			//页面视图
    //word.setFontName(QString::fromLocal8Bit("宋体"));
    word.setParagraphAlignment(0);		//下面文字位置
    word.setFontSize(10);

    word.setParagraphAlignment(1);
    word.insertText(tr("证书编号：   "));
    word.insertText(cert_number);
    word.insertMoveDown();

    word.insertText(tr("校准结果"));
    word.insertMoveDown();
    word.insertMoveDown();


    //first table
    word.insertText(tr("1.直流电压"));
    word.insertMoveDown();

    word.intsertTable(4, 4);

    word.MergeCells(1, 2, 1, 4, 1);//合并第一列单元格

    word.setCellString(1,1,1,tr("量程"));
    word.setCellString(1,1,2,tr("标准值"));
    word.setCellString(1,1,1,tr("显示值"));
    word.setCellString(1,1,2,tr("测量不确定度"));

    word.setCellString(1,2,1,tr("10V"));

    //second table
    word.insertText(tr("2.直流电流"));
    word.insertMoveDown();

    word.moveForEnd();
    word.insertMoveDown();
    word.intsertTable(4, 4);

    word.MergeCells(2, 2, 1, 4, 1);//合并第一列单元格

    word.setCellString(2,1,1,tr("量程"));
    word.setCellString(2,1,2,tr("标准值"));
    word.setCellString(2,1,1,tr("显示值"));
    word.setCellString(2,1,2,tr("测量不确定度"));

    word.setCellString(2,2,1,tr("10V"));

    word.moveForEnd();
    word.insertMoveDown();

    word.setVisible(true);
    word.saveAs();
}

void frmInspectCheck::on_btn_print_clicked()
{
    //print_certificate();
    print_check_result();
}
