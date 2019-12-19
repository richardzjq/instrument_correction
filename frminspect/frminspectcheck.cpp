#include "frminspectcheck.h"
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
    strList << "合格" << "不合格";
    ui->comboBox_inspect_conclusion->addItems(strList);

    strList.clear();
    strList << "湖北钢铁厂" << "湖北电科院";
    ui->comboBox_inspected_institution->addItems(strList);

    strList.clear();
    strList << "刘慈欣" << "逻辑" << "史强";
    ui->comboBox_inspector->addItems(strList);

    strList.clear();
    strList << "程心" << "韦德" << "乔恩斯";
    ui->comboBox_verifier->addItems(strList);

    strList.clear();
    strList << "数字式万用表" << "指针式万用表";
    ui->comboBox_sample_name->addItems(strList);

    strList.clear();
    strList << "UT52" << "UT58";
    ui->comboBox_model_specification->addItems(strList);

    ui->lineEdit_temperature->setText("20");
    ui->lineEdit_humidity->setText("60");

    strList.clear();
    strList << "UNI-T" << "UNI-S";
    ui->comboBox_manufacture->addItems(strList);

    ui->lineEdit_manufacted_number->setText("ZFL9008");

    strList.clear();
    strList << "UNI-T" << "UNI-S";
    ui->comboBox_standard_device->addItems(strList);

    strList.clear();
    strList << "JJG315-83" << "JJG315-8398";
    ui->comboBox_refered_rule->addItems(strList);

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

    /* 查询  检定记录编号  的所有记录 */
    dbInspect_record.get_column_content("检定记录", "检定记录编号", &strList);


    ui->treeWidget_inpsected_project->setColumnCount(1);

    strList.clear();
    strList << "已检定项目";
    ui->treeWidget_inpsected_project->setHeaderLabels(strList);

    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidget_inpsected_project, QStringList(QString("已检定项目")));

    QTreeWidgetItem *leaf1 = new QTreeWidgetItem(root, QStringList(QString("Leaf 1")));
    root->addChild(leaf1);
    QTreeWidgetItem *leaf2 = new QTreeWidgetItem(root, QStringList(QString("Leaf 2")));
    root->addChild(leaf2);

    QList<QTreeWidgetItem *> rootList;
    rootList << root;
    ui->treeWidget_inpsected_project->insertTopLevelItems(0, rootList);
}

void frmInspectCheck::uninitForm(void)
{
    dbInspect_record.close_database();
}
