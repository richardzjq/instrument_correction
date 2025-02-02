﻿#include "frmmain.h"
#include "ui_frmmain.h"
#include "quiwidget.h"
#include "iconfont.h"

#include "frmlogout.h"
#include "frminspectmain.h"
#include "frminspectcheck.h"
#include "frmconfig.h"
#include "frmabout.h"

frmMain::frmMain(QWidget *parent) : QWidget(parent), ui(new Ui::frmMain)
{
    ui->setupUi(this);
    this->initForm();
    this->initText();
    this->initNav();
    this->initIcon();
    QUIHelper::setFormInCenter(this);
    on_btnMenu_Max_clicked();
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::closeEvent(QCloseEvent *e)
{
    if (!App::AutoLogin) {
        frmLogout logout;
        logout.exec();
        e->ignore();
    } else {
        //如果不采用暴力退出则会打印出现 QThread: Destroyed while thread is still running
        //如果在关闭的时候出现崩溃现象,建议启用下面这行代码
        exit(0);
        //qApp->quit();
    }
}

bool frmMain::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        if (watched == ui->labLogoBg) {
            frmAbout::Instance()->show();
        }
    } else if (event->type() == QEvent::MouseButtonDblClick) {
        //双击标题栏最大化
        if (watched == ui->widgetTop) {
            on_btnMenu_Max_clicked();
        }
    }

    return QWidget::eventFilter(watched, event);
}

void frmMain::initForm()
{
    ui->widgetMain->setProperty("form", true);
    ui->widgetTop->setProperty("form", "title");
    ui->widgetTop->setFixedHeight(App::TopHeight);
    ui->bottomWidget->setFixedHeight(App::BottomHeight);

    this->setProperty("form", true);
    this->setProperty("canMove", true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    ui->widgetTop->installEventFilter(this);
    ui->labLogoBg->installEventFilter(this);

    //设置右上角菜单,图形字体
    IconHelper::Instance()->setIcon(ui->btnMenu_Min, QUIConfig::IconMin);
    IconHelper::Instance()->setIcon(ui->btnMenu_Max, QUIConfig::IconNormal);
    IconHelper::Instance()->setIcon(ui->btnMenu_Close, QUIConfig::IconClose);

    //设置底部状态栏
    ui->bottomWidget->setVersion(App::Version);
    ui->bottomWidget->setCurrentUserName(App::CurrentUserName);
    ui->bottomWidget->setCurrentUserType(App::CurrentUserType);
    ui->bottomWidget->setFontName(QUIConfig::FontName);
    int fontSize = (QUIConfig::FontSize > 12 ? QUIConfig::FontSize - 1 : QUIConfig::FontSize);
    ui->bottomWidget->setFontSize(fontSize);
    ui->bottomWidget->setLineFixedWidth(true);
    ui->bottomWidget->start();
}

void frmMain::initText()
{
    //从配置文件读取logo名称
    ui->labLogoCn->setText(App::LogoCn);
    ui->labLogoEn->setText(App::LogoEn);
    this->setWindowTitle(ui->labLogoCn->text());

    //设置标题+版本+版权
    ui->bottomWidget->setAuthor(App::Author);
    ui->bottomWidget->setTitle(ui->labLogoCn->text());
}

void frmMain::initNav()
{
    frmInspectMain *inspectMain = new frmInspectMain;
    ui->stackedWidget->addWidget(inspectMain);

    frmInspectCheck *inspectCheck = new frmInspectCheck;
    ui->stackedWidget->addWidget(inspectCheck);

    frmConfig *config = new frmConfig;
    ui->stackedWidget->addWidget(config);

    connect(AppEvent::Instance(), SIGNAL(changeText()), this, SLOT(initText()));
    connect(AppEvent::Instance(), SIGNAL(changeStyle()), this, SLOT(initIcon()));
    //connect(AppEvent::Instance(), SIGNAL(changeStyle()), view, SLOT(initTree()));
    //connect(this, SIGNAL(setIndex(int)), view, SLOT(setIndex(int)));

    QList<QString> texts;

    btns << ui->btn_main << ui->btn_check << ui->btn_config;
    texts << "主界面" << "检定" << "系统设置";

    for (int i = 0; i < btns.count(); i++) {
        QPushButton *btn = (QPushButton *)btns.at(i);
        btn->setIconSize(QSize(25, 20));
        btn->setFixedWidth(115);
        btn->setCheckable(true);
        btn->setText(texts.at(i));
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));

        if (btn->objectName() == App::LastFormMain) {
            btn->click();
        }
    }

    //如果数据库出错则主动切换到系统设置页面
    if (App::DbError) {
        ui->btn_main->click();
    }
}

void frmMain::initIcon()
{
    //如果logo图片不存在则隐藏标签,否则显示logo图片
    ui->labLogoBg->setAlignment(Qt::AlignCenter);
    QPixmap pixLogo(QUIHelper::appPath() + "/" + App::LogoBg);
    if (pixLogo.isNull()) {
        //ui->labLogoBg->setVisible(false);
        //设置图形字体作为logo
        int size = 60;
        ui->labLogoBg->setFixedWidth(size + 20);
        IconFont::Instance()->setIcon(ui->labLogoBg, 0xe6f2, size);
    } else {
        int size = 150;
        ui->labLogoBg->setFixedWidth(size + 20);
        pixLogo = pixLogo.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->labLogoBg->setPixmap(pixLogo);
    }

    //从图形字体库中设置图标
    ui->btn_main->setIcon(IconHelper::Instance()->getPixmap(QUIConfig::TextColor, 0xf002, 20, 20, 20));
    ui->btn_check->setIcon(IconHelper::Instance()->getPixmap(QUIConfig::TextColor, 0xf044, 20, 20, 20));
    ui->btn_config->setIcon(IconHelper::Instance()->getPixmap(QUIConfig::TextColor, 0xf085, 20, 20, 20));
}

void frmMain::buttonClicked()
{
    QPushButton *btn = (QPushButton *)sender();

    //如果是操作员,不能点系统设置
    if (!App::CurrentUserType.contains("管理员") && btn == ui->btn_config) {
        QUIHelper::showMessageBoxError("只有管理员才有系统设置权限!");
        btn->setChecked(false);
        return;
    }

    if(btn == ui->btn_main) {
        ui->stackedWidget->setCurrentIndex(0);
    }else if (btn == ui->btn_check) {
        ui->stackedWidget->setCurrentIndex(1);
    }else if (btn == ui->btn_config) {
        ui->stackedWidget->setCurrentIndex(2);
    }

    App::writeConfig();

    //取消其他按钮选中
    foreach (QPushButton *b, btns) {
        b->setChecked(b == btn);
    }
}

void frmMain::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void frmMain::on_btnMenu_Max_clicked()
{
    static bool max = false;
    static QRect location = this->geometry();

    if (max) {
        this->setGeometry(location);
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
    }

    IconHelper::Instance()->setIcon(ui->btnMenu_Max, max ? QUIConfig::IconNormal : QUIConfig::IconMax);
    this->setProperty("canMove", max);
    max = !max;

    //最大化以后有个BUG,悬停样式没有取消掉,需要主动模拟鼠标动一下
    if (max) {
        QEvent event(QEvent::Leave);
        QApplication::sendEvent(ui->btnMenu_Max, &event);
    }
}

void frmMain::on_btnMenu_Close_clicked()
{
    this->close();
}
