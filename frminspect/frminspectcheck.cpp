#include "frminspectcheck.h"
#include "frminspectproject.h"
#include "frminspecttemplate.h"
#include "ui_frminspectcheck.h"
#include "gpibctl.h"
#include "wordapi.h"
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
}

void frmInspectCheck::uninitForm(void)
{
    dbInspect_record.close_database();
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
    QString add_line_instruction = "INSERT INTO 检定记录 (检定记录编号, 送检单位, 仪器名称, 型号规格, 生产厂家, 出厂编号, 检定日期, 检定员, 检定结论, 核验员, 环境温度, 相对湿度, 依据规程, 检定模板) VALUES ("
                                   + record_number + ", " + inspected_institution  + ", " + sample_name + ", " + model_specification + ", " +  manufacture + ", " + manufacted_number+ ", "
                                   + inspect_date + ", " + inspector + ", " + inspect_conclusion + ", " + verifier + ", " + temperature + ", " + humidity + ", " + refered_rule + ", " + inpsect_template + ")";

    dbInspect_record.add_one_line_into_table(add_line_instruction);

    /* 已检定项目TreeWidget加入一项 */
    QTreeWidgetItem* root = ui->treeWidget_inpsected_project->topLevelItem(0);
    QTreeWidgetItem* leaf = new QTreeWidgetItem(root, QStringList(ui->lineEdit_record_number->text()));
    root->addChild(leaf);

    ui->treeWidget_inpsected_project->expandAll();
}

void frmInspectCheck::on_btn_delete_inspected_project_clicked()
{
    /* 获得当前节点 */
    QTreeWidgetItem* item= ui->treeWidget_inpsected_project->currentItem();
    /* 删除 */
    ui->treeWidget_inpsected_project->removeItemWidget(item, 0);

    /* 从数据库中删除该行 */
    QString delete_line_instruction = QString("DELETE FROM 检定记录 WHERE 检定记录编号 == '%1'").arg(ui->treeWidget_inpsected_project->currentItem()->text(0));
    dbInspect_record.delete_one_line_into_table(delete_line_instruction);

    this->clearForm();
}

void frmInspectCheck::on_btn_inspect_template_manage_clicked()
{
    //this->hide();
    frmInspectTemplate *frm = new frmInspectTemplate;
    frm->show();
}

void frmInspectCheck::on_btn_inspect_new_project_clicked()
{
    //this->hide();
    frmInspectProject *frm = new frmInspectProject;
    frm->show();
}
