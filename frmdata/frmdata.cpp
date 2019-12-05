#include "frmdata.h"
#include "ui_frmdata.h"
#include "quiwidget.h"
#include "iconfont.h"
#include "frmdataalarm.h"
#include "frmdatanode.h"
#include "frmdatauser.h"

frmData::frmData(QWidget *parent) : QWidget(parent), ui(new Ui::frmData)
{
    ui->setupUi(this);
    this->initForm();
    this->initNav();
    this->initIcon();
}

frmData::~frmData()
{
    delete ui;
}

void frmData::initForm()
{
    ui->widgetLeft->setProperty("flag", "btn");
    ui->widgetLeft->setFixedWidth(App::LeftWidth);
}

void frmData::initNav()
{
    frmDataAlarm *dataAlarm = new frmDataAlarm;
    ui->stackedWidget->addWidget(dataAlarm);

    frmDataNode *dataNode = new frmDataNode;
    ui->stackedWidget->addWidget(dataNode);

    frmDataUser *dataUser = new frmDataUser;
    ui->stackedWidget->addWidget(dataUser);

    QList<QString> texts;
    btns << ui->btnDataAlarm << ui->btnDataNode << ui->btnDataUser;
    texts << "报 警 记 录" << "运 行 记 录" << "操 作 记 录";

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

void frmData::initIcon()
{
    //从图形字体库中设置图标
    int size = 20;
    int pixWidth = 30;
    int pixHeight = 20;
    ui->btnDataAlarm->setIcon(IconFont::Instance()->getPixmap(QUIConfig::TextColor, 0xea5b, size, pixWidth, pixHeight));
    ui->btnDataNode->setIcon(IconFont::Instance()->getPixmap(QUIConfig::TextColor, 0xea44, size, pixWidth, pixHeight));
    ui->btnDataUser->setIcon(IconFont::Instance()->getPixmap(QUIConfig::TextColor, 0xea59, size, pixWidth, pixHeight));
}

void frmData::buttonClicked()
{
    QPushButton *btn = (QPushButton *)sender();
    int index = btns.indexOf(btn);
    ui->stackedWidget->setCurrentIndex(index);
    App::LastFormData = btn->objectName();
    App::writeConfig();

    //取消其他按钮选中
    foreach (QPushButton *b, btns) {
        b->setChecked(b == btn);
    }
}
