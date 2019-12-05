#include "frmconfigschem.h"
#include "ui_frmconfigschem.h"

frmConfigSchem::frmConfigSchem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmConfigSchem)
{
    ui->setupUi(this);

    /* 初始化combobox */
    QStringList strList;

    strList.clear();
    strList<<"方案一"<<"方案二"<<"方案三";
    ui->comboBox_schem_custom->addItems(strList);

    strList.clear();
    strList<<"张三"<<"李四"<<"王二";
    ui->comboBox_schem_inspector->addItems(strList);

    strList.clear();
    strList<<"交流电压"<<"交流电流"<<"直流电压"<<"直流电流"<<"电阻";
    ui->comboBox_schem_refered_standard->addItems(strList);

    strList.clear();
    strList<<"三位半"<<"四位半"<<"五位半"<<"六位半"<<"七位半"<<"八位半";
    ui->comboBox_schem_checked_instrument->addItems(strList);
}

frmConfigSchem::~frmConfigSchem()
{
    delete ui;
}
