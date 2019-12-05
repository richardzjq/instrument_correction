#include "frminspect.h"
#include "ui_frminspect.h"
#include "quiwidget.h"
#include "iconfont.h"
#include "frminspectcheck.h"


frmInspect::frmInspect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmInspect)
{
    ui->setupUi(this);
    this->initForm();
    this->initNav();
    this->initIcon();
}

frmInspect::~frmInspect()
{
    delete ui;
}

void frmInspect::initForm()
{
    ui->widgetLeft->setProperty("flag", "btn");
    ui->widgetLeft->setFixedWidth(App::LeftWidth);
}

void frmInspect::initNav()
{
    frmInspectCheck *inspect_check = new frmInspectCheck;
    ui->stackedWidget_inspect->addWidget(inspect_check);

    QList<QString> texts;
    btns << ui->btn_check;
    texts << "检       测";

    for (int i = 0; i < btns.count(); i++) {
        QPushButton *btn = (QPushButton *)btns.at(i);
        btn->setIconSize(QSize(30, 20));
        btn->setCheckable(true);
        btn->setText(texts.at(i) + "   ");
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));

        if (btn->objectName() == App::LastFormData) {
            btn->click();
        }
    }
}

void frmInspect::initIcon()
{
    //从图形字体库中设置图标
    int size = 20;
    int pixWidth = 30;
    int pixHeight = 20;

    ui->btn_check->setIcon(IconFont::Instance()->getPixmap(QUIConfig::TextColor, 0xea59, size, pixWidth, pixHeight));
}

void frmInspect::buttonClicked()
{
    QPushButton *btn = (QPushButton *)sender();
    int index = btns.indexOf(btn);
    ui->stackedWidget_inspect->setCurrentIndex(index);
    App::LastFormData = btn->objectName();
    App::writeConfig();

    //取消其他按钮选中
    foreach (QPushButton *b, btns) {
        b->setChecked(b == btn);
    }
}
