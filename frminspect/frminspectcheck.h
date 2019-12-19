#ifndef FRMINSPECTCHECK_H
#define FRMINSPECTCHECK_H

#include <QWidget>
#include "dbinspect.h"

namespace Ui {
class frmInspectCheck;
}

class frmInspectCheck : public QWidget
{
    Q_OBJECT

public:
    explicit frmInspectCheck(QWidget *parent = nullptr);
    ~frmInspectCheck();


private:
    Ui::frmInspectCheck *ui;
    DBInspect dbInspect_record;
    QString record_table_name;
    int columns_count;

private slots:
    void initForm(void);
    void uninitForm(void);
    void clearForm(void);

    void on_btn_create_clicked();
    void on_btn_save_clicked();
    void on_btn_delete_inspected_project_clicked();
    void on_btn_inspect_template_manage_clicked();
    void on_btn_inspect_new_project_clicked();
};

#endif // FRMINSPECTCHECK_H
