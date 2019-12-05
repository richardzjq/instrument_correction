#ifndef FRMCONFIG_H
#define FRMCONFIG_H

#include <QWidget>

class QPushButton;

namespace Ui {
class frmConfig;
}

class frmConfig : public QWidget
{
    Q_OBJECT

public:
    explicit frmConfig(QWidget *parent = 0);
    ~frmConfig();

private:
    Ui::frmConfig *ui;
    QList<QPushButton *> btns;

private slots:
    void initForm();
    void initNav();
    void initIcon();
    void buttonClicked();
};

#endif // FRMCONFIG_H
