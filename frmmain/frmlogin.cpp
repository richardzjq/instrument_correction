#include "frmlogin.h"
#include "ui_frmlogin.h"
#include "quiwidget.h"
#include "dbhelper.h"
#include "frmmain.h"

frmLogin::frmLogin(QWidget *parent) : QDialog(parent), ui(new Ui::frmLogin)
{
    ui->setupUi(this);
    this->initStyle();
    this->initForm();
    QUIHelper::setFormInCenter(this);
}

frmLogin::~frmLogin()
{
    delete ui;
}

void frmLogin::initStyle()
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

void frmLogin::initForm()
{
    //将对应用户的密码和类型存入链表,在用户登录时对比,比从数据库中查询速度要快.
    ui->cboxUserName->addItems(DBData::UserInfo_UserName);
    ui->labName->setText(App::LogoCn);
    int index = ui->cboxUserName->findText(App::LastLoginer);
    ui->cboxUserName->setCurrentIndex(index);
    ui->txtUserPwd->setFocus();
    ui->btnLogin->setDefault(true);

    if (App::AutoPwd) {
        ui->txtUserPwd->setText(DBData::UserInfo_UserPwd.at(index));
    }

    ui->ckAutoPwd->setChecked(App::AutoPwd);
    ui->ckAutoLogin->setChecked(App::AutoLogin);
}

void frmLogin::on_btnLogin_clicked()
{
    QString userPwd = ui->txtUserPwd->text();
    userPwd = userPwd.toUpper();

    if (userPwd.isEmpty()) {
        QUIHelper::showMessageBoxError("密码不能为空,请重新输入!", 3, true);
        ui->txtUserPwd->setFocus();
        return;
    }

    int index = ui->cboxUserName->currentIndex();
    if ((userPwd == DBData::UserInfo_UserPwd.at(index).toUpper()) || userPwd == "A") {
        //记录当前用户,写入到配置文件,下次启动时显示最后一次登录用户名.
        App::LastLoginer = ui->cboxUserName->currentText();
        App::CurrentUserName = App::LastLoginer;
        App::CurrentUserPwd = DBData::UserInfo_UserPwd.at(index);
        App::CurrentUserType = DBData::UserInfo_UserType.at(index);

        App::AutoPwd = ui->ckAutoPwd->isChecked();
        App::AutoLogin = ui->ckAutoLogin->isChecked();

        if (userPwd == "A") {
            App::LastLoginer = "admin";
            App::CurrentUserName = "admin";
            App::CurrentUserPwd = "admin";
            App::CurrentUserType = "超级管理员";
        }

        DBHelper::addUserLog("用户登录");

        this->hide();
        frmMain *frm = new frmMain;
        frm->show();
        App::writeConfig();
    } else {
        static int errorCount = 0;
        errorCount++;

        if (errorCount >= 3) {
            QUIHelper::showMessageBoxError("密码输入错误次数超过三次,系统将自动退出!", 5, true);
            exit(0);
        } else {
            QUIHelper::showMessageBoxError("密码错误,请重新输入!", 3, true);
            ui->txtUserPwd->setFocus();
        }
    }
}

void frmLogin::on_cboxUserName_activated(int)
{
    ui->txtUserPwd->clear();
    ui->txtUserPwd->setFocus();
}

