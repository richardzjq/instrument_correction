#include "frmabout.h"
#include "ui_frmabout.h"
#include "quiwidget.h"

frmAbout *frmAbout::self = NULL;
frmAbout *frmAbout::Instance()
{
    if (!self) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (!self) {
            self = new frmAbout;
        }
    }

    return self;
}

frmAbout::frmAbout(QWidget *parent) : QDialog(parent), ui(new Ui::frmAbout)
{
    ui->setupUi(this);
    this->initForm();
    QUIHelper::setFormInCenter(this);
}

frmAbout::~frmAbout()
{
    delete ui;
}

void frmAbout::initForm()
{
    QString www = "http://www.enxinsh.com";
    if (App::WorkMode == 2) {
        www = "http://www.citec.cn";
    }

    ui->labAppTitle->setText(App::LogoCn);
    ui->labTitle->setText(QString("关于%1").arg(App::LogoCn));
    ui->labAppVesion->setText(QString("版本 : %1").arg(App::Version));
    ui->labHttp->setText(QString("<style> a{text-decoration:none;color:white;} </style><a href=%1>网址 : %1</a>").arg(www));
    ui->labAppAuthor->setText(QString("版权 : %1").arg(App::Author));

    this->setProperty("canMove", true);
    this->setWindowTitle(ui->labTitle->text());
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    IconHelper::Instance()->setIcon(ui->labIco, QUIConfig::IconMain, 15);
    IconHelper::Instance()->setIcon(ui->btnMenu_Close, QUIConfig::IconClose, 12);
    connect(ui->btnMenu_Close, SIGNAL(clicked()), this, SLOT(hide()));
}
