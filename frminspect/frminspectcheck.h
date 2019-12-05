#ifndef FRMINSPECTCHECK_H
#define FRMINSPECTCHECK_H

#include <QWidget>

namespace Ui {
class frmInspectCheck;
}

class frmInspectCheck : public QWidget
{
    Q_OBJECT

public:
    explicit frmInspectCheck(QWidget *parent = nullptr);
    ~frmInspectCheck();

private slots:
    void initData();
    void uninitData();

private slots:
    void on_pushButton_check_check_clicked();
    void on_pushButton_check_print_clicked();

private:
    Ui::frmInspectCheck *ui;
};

#endif // FRMINSPECTCHECK_H
