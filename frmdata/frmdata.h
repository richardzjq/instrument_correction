#ifndef FRMDATA_H
#define FRMDATA_H

#include <QWidget>

class QPushButton;

namespace Ui {
class frmData;
}

class frmData : public QWidget
{
    Q_OBJECT

public:
    explicit frmData(QWidget *parent = 0);
    ~frmData();

private:
    Ui::frmData *ui;
    QList<QPushButton *> btns;

private slots:
    void initForm();
    void initNav();
    void initIcon();
    void buttonClicked();
};

#endif // FRMDATA_H
