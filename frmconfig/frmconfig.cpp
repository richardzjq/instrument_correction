#include "frmconfig.h"
#include "ui_frmconfig.h"
#include "quiwidget.h"
#include "iconfont.h"

#include "frmconfigsystem.h"
#include "frmconfigport.h"
#include "frmconfiguser.h"
#include "frmconfiginstrument.h"
#include "frmconfigmodel.h"
#include "frmconfigschem.h"

frmConfig::frmConfig(QWidget *parent) : QWidget(parent), ui(new Ui::frmConfig)
{
    ui->setupUi(this);
    this->initForm();
    this->initNav();
    this->initIcon();
}

frmConfig::~frmConfig()
{
    delete ui;
}

void frmConfig::initForm()
{
    ui->widgetLeft->setProperty("flag", "btn");
    ui->widgetLeft->setFixedWidth(App::LeftWidth);
}

void frmConfig::initNav()
{
    frmConfigSystem *configSystem = new frmConfigSystem;
    ui->stackedWidget->addWidget(configSystem);

    frmConfigModel *config_model = new frmConfigModel;
    ui->stackedWidget->addWidget(config_model);

    frmConfigInstrument *config_instrument = new frmConfigInstrument;
    ui->stackedWidget->addWidget(config_instrument);

    frmConfigSchem *config_schem = new frmConfigSchem;
    ui->stackedWidget->addWidget(config_schem);

    frmConfigPort *configPort = new frmConfigPort;
    ui->stackedWidget->addWidget(configPort);

    frmConfigUser *configUser = new frmConfigUser;
    ui->stackedWidget->addWidget(configUser);

    connect(AppEvent::Instance(), SIGNAL(changeStyle()), this, SLOT(initIcon()));
    connect(AppEvent::Instance(), SIGNAL(changeStyle()), configPort, SLOT(changeStyle()));
    connect(AppEvent::Instance(), SIGNAL(changeStyle()), configUser, SLOT(changeStyle()));

    QList<QString> texts;
    btns << ui->btnConfigSystem << ui->btnConfigModel << ui->btnConfigInstrument << ui->btnConfigSchem << ui->btnConfigPort << ui->btnConfigUser;
    texts << "基 本 设 置" << "系 统 设 置" << "送检设备信息" << "检 定 方 案" << "端 口 管 理" << "用 户 管 理";


    for (int i = 0; i < btns.count(); i++) {
        QPushButton *btn = (QPushButton *)btns.at(i);
        btn->setIconSize(QSize(30, 20));
        btn->setCheckable(true);
        btn->setText(texts.at(i) + "   ");
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));

        if (btn->objectName() == App::LastFormConfig) {
            btn->click();
        }
    }

    //如果数据库出错则主动切换到系统设置页面
    if (App::DbError) {
        ui->btnConfigSystem->click();
    }
}

void frmConfig::initIcon()
{
    //从图形字体库中设置图标
    int size = 20;
    int pixWidth = 30;
    int pixHeight = 20;
	
    ui->btnConfigSystem->setIcon(IconFont::Instance()->getPixmap(QUIConfig::TextColor, 0xe60d, size, pixWidth, pixHeight));
	ui->btnConfigModel->setIcon(IconFont::Instance()->getPixmap(QUIConfig::TextColor, 0xea5b, size, pixWidth, pixHeight));
    ui->btnConfigInstrument->setIcon(IconFont::Instance()->getPixmap(QUIConfig::TextColor, 0xea44, size, pixWidth, pixHeight));
    ui->btnConfigSchem->setIcon(IconFont::Instance()->getPixmap(QUIConfig::TextColor, 0xea50, size, pixWidth, pixHeight));
    ui->btnConfigPort->setIcon(IconFont::Instance()->getPixmap(QUIConfig::TextColor, 0xe9c1, size, pixWidth, pixHeight));
    ui->btnConfigUser->setIcon(IconFont::Instance()->getPixmap(QUIConfig::TextColor, 0xea59, size, pixWidth, pixHeight));
}

void frmConfig::buttonClicked()
{
    QPushButton *btn = (QPushButton *)sender();
    int index = btns.indexOf(btn);
    ui->stackedWidget->setCurrentIndex(index);
    App::LastFormConfig = btn->objectName();
    App::writeConfig();

    //取消其他按钮选中
    foreach (QPushButton *b, btns) {
        b->setChecked(b == btn);
    }
}
