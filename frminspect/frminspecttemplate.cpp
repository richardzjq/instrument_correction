#include "frminspecttemplate.h"
#include "ui_frminspecttemplate.h"

frmInspectTemplate::frmInspectTemplate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmInspectTemplate)
{
    ui->setupUi(this);
}

frmInspectTemplate::~frmInspectTemplate()
{
    delete ui;
}
