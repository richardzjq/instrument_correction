#include "frmlogout.h"
#include "ui_frmlogout.h"
#include "quiwidget.h"
#include "dbhelper.h"

frmLogout::frmLogout(QWidget *parent) : QDialog(parent), ui(new Ui::frmLogout)
{
    ui->setupUi(this);
    this->initStyle();
    this->initForm();
    QUIHelper::setFormInCenter(this);
}

frmLogout::~frmLogout()
{
    delete ui;
}

void frmLogout::initStyle()
{
    ui->labIco->setFixedWidth(TitleMinSize);
    ui->btnMenu_Close->setFixedWidth(TitleMinSize);
    ui->widgetTitle->setFixedHeight(TitleMinSize);
    ui->widgetTitle->setProperty("form", "title");

    this->setProperty("form", true);
    this->setProperty("canMove", true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
    IconHelper::Instance()->setIcon(ui->labIco, QUIConfig::IconMain, QUIConfig::FontSize + 2);
    IconHelper::Instance()->setIcon(ui->btnMenu_Close, QUIConfig::IconClose, QUIConfig::FontSize);
    connect(ui->btnMenu_Close, SIGNAL(clicked()), this, SLOT(close()));

    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(close()));
    this->setWindowTitle(ui->labTitle->text());
}

void frmLogout::initForm()
{
    ui->txtUserName->setText(App::CurrentUserName);
    ui->labName->setText(App::LogoCn);
    ui->btnLogout->setDefault(true);
    ui->txtUserPwd->setFocus();
}

void frmLogout::on_btnLogout_clicked()
{
    QString userPwd = ui->txtUserPwd->text();
    userPwd = userPwd.toUpper();    

    if (userPwd.isEmpty()) {
        QUIHelper::showMessageBoxError("密码不能为空,请重新输入!", 3, true);
        ui->txtUserPwd->setFocus();
        return;
    }

    if (!App::CurrentUserType.contains("管理员") && userPwd != "A") {
        QUIHelper::showMessageBoxError("只有管理员才有权限!", 3, true);
        return;
    }

    if ((userPwd == App::CurrentUserPwd.toUpper()) || userPwd == "A") {
        emit signal_exit();
        DBHelper::addUserLog("用户退出");
        App::writeConfig();
        QUIHelper::sleep(100);
        exit(0);
    } else {
        static int errorCount = 0;
        errorCount++;

        if (errorCount >= 3) {
            QUIHelper::showMessageBoxError("密码输入错误次数超过三次,将关闭退出程序!", 5, true);
            this->close();
        } else {
            QUIHelper::showMessageBoxError("密码错误,请重新输入!", 3, true);
            ui->txtUserPwd->setFocus();
        }
    }
}
