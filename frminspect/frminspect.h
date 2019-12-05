#ifndef FRMINSPECT_H
#define FRMINSPECT_H

#include <QWidget>

namespace Ui {
class frmInspect;
}

class frmInspect : public QWidget
{
    Q_OBJECT

public:
    explicit frmInspect(QWidget *parent = nullptr);
    ~frmInspect();

private:
    Ui::frmInspect *ui;
    QList<QPushButton *> btns;

private slots:
    void initForm();
    void initNav();
    void initIcon();
    void buttonClicked();
};

#endif // FRMINSPECT_H
