#include "frmconfigsystem.h"
#include "ui_frmconfigsystem.h"
#include "quiwidget.h"
#include "dbhelper.h"
#include "appinit.h"
#include "sendserver.h"
#include "soundapi.h"
#include "excelapi.h"
#include "api.h"
#include "savelog.h"

frmConfigSystem::frmConfigSystem(QWidget *parent) : QWidget(parent), ui(new Ui::frmConfigSystem)
{
    ui->setupUi(this);
    this->initForm();
    this->initConfig();
    this->initColor();
    this->initTime();
}

frmConfigSystem::~frmConfigSystem()
{
    delete ui;
}

void frmConfigSystem::initForm()
{
    int fixWidth = 440;
    QList<QGroupBox *> gboxs = this->findChildren<QGroupBox *>();
    foreach (QGroupBox *gbox, gboxs) {
        gbox->setFixedWidth(fixWidth);
    }

    //禁用工作模式
    if (App::WorkMode > 0) {
        //ui->cboxWorkMode->setEnabled(false);
    }
}

void frmConfigSystem::initConfig()
{
    ui->txtLogoCn->setText(App::LogoCn);
    ui->txtLogoCn->setSelection(0, 0);
    connect(ui->txtLogoCn, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtLogoEn->setText(App::LogoEn);
    ui->txtLogoEn->setSelection(0, 0);
    connect(ui->txtLogoEn, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtAuthor->setText(App::Author);
    ui->txtAuthor->setSelection(0, 0);
    connect(ui->txtAuthor, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->cboxWorkMode->addItem("从设备采集");
    ui->cboxWorkMode->addItem("从数据库采集");
    ui->cboxWorkMode->addItem("从数据库读取");
    ui->cboxWorkMode->setCurrentIndex(App::WorkMode);
    connect(ui->cboxWorkMode, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));

    ui->btnAutoRun->setChecked(App::AutoRun);
    connect(ui->btnAutoRun, SIGNAL(checkedChanged(bool)), this, SLOT(saveConfig()));

    ui->btnAutoLogin->setChecked(App::AutoLogin);
    connect(ui->btnAutoLogin, SIGNAL(checkedChanged(bool)), this, SLOT(saveConfig()));

    ui->btnAutoPwd->setChecked(App::AutoPwd);
    connect(ui->btnAutoPwd, SIGNAL(checkedChanged(bool)), this, SLOT(saveConfig()));

    ui->btnAutoConfirm->setChecked(App::AutoConfirm);
    connect(ui->btnAutoConfirm, SIGNAL(checkedChanged(bool)), this, SLOT(saveConfig()));

    QStringList rowCount;
    for (int i = 12; i <= 40; i++) {
        rowCount << QString("%1行").arg(i);
    }
    rowCount << "45行" << "50行" << "55行" << "60行" << "70行" << "80行" << "90行" << "100行";

    QStringList deviceColumn;
    for (int i = 2; i <= 20; i++) {
        deviceColumn << QString("%1列").arg(i);
    }

   QStringList styleDatas;
    styleDatas << "银色" << "蓝色" << "浅蓝色" << "深蓝色" << "灰色" << "浅灰色"
               << "深灰色" << "黑色" << "浅黑色" << "深黑色" << "PS黑色" << "黑色扁平"
               << "白色扁平" << "蓝色扁平" << "紫色" << "黑蓝色" << "视频黑";

    QStringList styleNames;
    styleNames << ":/qss/silvery.css" << ":/qss/blue.css" << ":/qss/lightblue.css" << ":/qss/darkblue.css" << ":/qss/gray.css" << ":/qss/lightgray.css"
               << ":/qss/darkgray.css" << ":/qss/black.css" << ":/qss/lightblack.css" << ":/qss/darkblack.css" << ":/qss/psblack.css" << ":/qss/flatblack.css"
               << ":/qss/flatwhite.css" << ":/qss/flatblue.css" << ":/qss/purple.css" << ":/qss/blackblue.css" << ":/qss/blackvideo.css";

    for (int i = 0; i < styleNames.count(); i++) {
        ui->cboxStyleName->addItem(styleDatas.at(i), styleNames.at(i));
    }

    ui->cboxStyleName->setCurrentIndex(ui->cboxStyleName->findData(App::StyleName));
    connect(ui->cboxStyleName, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));
}

void frmConfigSystem::saveConfig()
{
    QString logoCn = ui->txtLogoCn->text();
    if (App::LogoCn != logoCn) {
        App::LogoCn = logoCn;
        AppEvent::Instance()->slot_changeText();
    }

    QString logoEn = ui->txtLogoEn->text();
    if (App::LogoEn != logoEn) {
        App::LogoEn = logoEn;
        AppEvent::Instance()->slot_changeText();
    }

    QString author = ui->txtAuthor->text();
    if (App::Author != author) {
        App::Author = author;
        AppEvent::Instance()->slot_changeText();
    }

    int workMode = ui->cboxWorkMode->currentIndex();
    if (App::WorkMode != workMode) {
        App::WorkMode = workMode;
        App::writeConfig();
        API::reboot();
    }

    QString styleName = ui->cboxStyleName->itemData(ui->cboxStyleName->currentIndex()).toString();
    if (App::StyleName != styleName) {
        App::StyleName = styleName;
        AppInit::Instance()->initStyle();
        AppEvent::Instance()->slot_changeStyle();
    }

    App::writeConfig();
}

void frmConfigSystem::initColor()
{
    ui->cboxColorZero->setColorName(App::ColorZero);
    connect(ui->cboxColorZero, SIGNAL(colorChanged(QString)), this, SLOT(saveColor()));

    ui->cboxColorUpper->setColorName(App::ColorUpper);
    connect(ui->cboxColorUpper, SIGNAL(colorChanged(QString)), this, SLOT(saveColor()));

    ui->cboxColorLimit->setColorName(App::ColorLimit);
    connect(ui->cboxColorLimit, SIGNAL(colorChanged(QString)), this, SLOT(saveColor()));

    ui->cboxColorNormal->setColorName(App::ColorNormal);
    connect(ui->cboxColorNormal, SIGNAL(colorChanged(QString)), this, SLOT(saveColor()));

    ui->cboxColorPlotBg->setColorName(App::ColorPlotBg);
    connect(ui->cboxColorPlotBg, SIGNAL(colorChanged(QString)), this, SLOT(saveColor()));

    ui->cboxColorPlotText->setColorName(App::ColorPlotText);
    connect(ui->cboxColorPlotText, SIGNAL(colorChanged(QString)), this, SLOT(saveColor()));

    ui->cboxColorPlotLine->setColorName(App::ColorPlotLine);
    connect(ui->cboxColorPlotLine, SIGNAL(colorChanged(QString)), this, SLOT(saveColor()));

    ui->cboxColorOther->setColorName(App::ColorOther);
    connect(ui->cboxColorOther, SIGNAL(colorChanged(QString)), this, SLOT(saveColor()));
}

void frmConfigSystem::saveColor()
{
    App::ColorZero = ui->cboxColorZero->getColorName();
    App::ColorUpper = ui->cboxColorUpper->getColorName();
    App::ColorLimit = ui->cboxColorLimit->getColorName();
    App::ColorNormal = ui->cboxColorNormal->getColorName();
    App::ColorPlotBg = ui->cboxColorPlotBg->getColorName();
    App::ColorPlotText = ui->cboxColorPlotText->getColorName();
    App::ColorPlotLine = ui->cboxColorPlotLine->getColorName();
    App::ColorOther = ui->cboxColorOther->getColorName();
    App::writeConfig();
}

void frmConfigSystem::initTime()
{
    for (int i = 2018; i <= 2030; i++) {
        ui->cboxYear->addItem(QString::number(i));
    }

    for (int i = 1; i <= 12; i++) {
        ui->cboxMonth->addItem(QString::number(i));
    }

    for (int i = 1; i <= 31; i++) {
        ui->cboxDay->addItem(QString::number(i));
    }

    for (int i = 0; i < 24; i++) {
        ui->cboxHour->addItem(QString::number(i));
    }

    for (int i = 0; i < 60; i++) {
        ui->cboxMin->addItem(QString::number(i));
        ui->cboxSec->addItem(QString::number(i));
    }

    QString now = QDateTime::currentDateTime().toString("yyyy-M-d-H-m-s");
    QStringList str = now.split("-");

    int indexYear = ui->cboxYear->findText(str.at(0));
    ui->cboxYear->setCurrentIndex(indexYear < 0 ? 0 : indexYear);
    ui->cboxMonth->setCurrentIndex(ui->cboxMonth->findText(str.at(1)));
    ui->cboxDay->setCurrentIndex(ui->cboxDay->findText(str.at(2)));
    ui->cboxHour->setCurrentIndex(ui->cboxHour->findText(str.at(3)));
    ui->cboxMin->setCurrentIndex(ui->cboxMin->findText(str.at(4)));
    ui->cboxSec->setCurrentIndex(ui->cboxSec->findText(str.at(5)));

    connect(ui->btnSetTime, SIGNAL(clicked(bool)), this, SLOT(saveTime()));
}

void frmConfigSystem::saveTime()
{
    QString year = QString("%1").arg(ui->cboxYear->currentText());
    QString month = QString("%1").arg(ui->cboxMonth->currentText().toInt(), 2, 10, QChar('0'));
    QString day = QString("%1").arg(ui->cboxDay->currentText().toInt(), 2, 10, QChar('0'));
    QString hour = QString("%1").arg(ui->cboxHour->currentText().toInt(), 2, 10, QChar('0'));
    QString min = QString("%1").arg(ui->cboxMin->currentText().toInt(), 2, 10, QChar('0'));
    QString sec = QString("%1").arg(ui->cboxSec->currentText().toInt(), 2, 10, QChar('0'));
    QUIHelper::setSystemDateTime(year, month, day, hour, min, sec);
}
