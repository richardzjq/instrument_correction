#ifndef FRMCONFIGUSER_H
#define FRMCONFIGUSER_H

#include <QWidget>

class QSqlTableModel;
class DbDelegate;

namespace Ui {
class frmConfigUser;
}

class frmConfigUser : public QWidget
{
    Q_OBJECT

public:
    explicit frmConfigUser(QWidget *parent = 0);
    ~frmConfigUser();

private:
    Ui::frmConfigUser *ui;
    QSqlTableModel *model;
    QList<QString> columnNames;
    QList<int> columnWidths;

    QList<DbDelegate *> delegates;

private slots:
    void initForm();
    void initData();
    void changeStyle();

private slots:
    void on_btnAdd_clicked();
    void on_btnSave_clicked();
    void on_btnDelete_clicked();
    void on_btnReturn_clicked();
    void on_btnClear_clicked();
    void on_btnExcel_clicked();
    void on_btnPrint_clicked();
    void on_btnInput_clicked();
    void on_btnOutput_clicked();
};

#endif // FRMCONFIGUSER_H
