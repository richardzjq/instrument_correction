#include "frminspectproject.h"
#include "ui_frminspectproject.h"

frmInspectProject::frmInspectProject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmInspectProject)
{
    ui->setupUi(this);
}

frmInspectProject::~frmInspectProject()
{
    delete ui;
}
