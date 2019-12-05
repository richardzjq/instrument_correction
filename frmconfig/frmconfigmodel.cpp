#include "frmconfigmodel.h"
#include "ui_frmconfigmodel.h"
#include "quiwidget.h"
#include "api.h"

frmConfigModel::frmConfigModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmConfigModel)
{
    ui->setupUi(this);

    /* 初始化combobox */
    QStringList strList;

    strList.clear();
    strList << "多功能标准源" << "交流电压源" << "交流电流源" << "直流电压源" << "直流电流源" << "标准电阻";
    ui->comboBox_model_standard_source->addItems(strList);
}

frmConfigModel::~frmConfigModel()
{
    delete ui;
}
