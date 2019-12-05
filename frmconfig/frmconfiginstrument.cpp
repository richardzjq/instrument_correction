#include "frmconfiginstrument.h"
#include "ui_frmconfiginstrument.h"

frmConfigInstrument::frmConfigInstrument(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmConfigInstrument)
{
    ui->setupUi(this);

    /* 初始化combobox */
    QStringList strList;

    strList.clear();
    strList<<"交流电压"<<"交流电流"<<"直流电压"<<"直流电流"<<"电阻";
    ui->comboBox_instrument_function->addItems(strList);

    strList.clear();
    strList<<"三位半"<<"四位半"<<"五位半"<<"六位半"<<"七位半"<<"八位半";
    ui->comboBox_instrument_show_degree->addItems(strList);

}

frmConfigInstrument::~frmConfigInstrument()
{
    delete ui;
}
