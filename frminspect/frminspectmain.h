#ifndef FRMINSPECTMAIN_H
#define FRMINSPECTMAIN_H

#include <QWidget>
#include "dbinspect.h"

namespace Ui {
class frmInspectMain;
}

class frmInspectMain : public QWidget
{
    Q_OBJECT

public:
    explicit frmInspectMain(QWidget *parent = nullptr);
    ~frmInspectMain();

private:
    Ui::frmInspectMain *ui;
    DBInspect dbInspect_record;
    QString record_table_name;
    int columns_count;

private slots:
    void initForm(void);
    void uninitForm(void);
    void initData(void);
    void refresh_record_table(void);

    void on_comboBox_inspected_institution_currentIndexChanged(const QString &arg1);
    void on_comboBox_inspected_date_currentIndexChanged(const QString &arg1);
    void on_comboBox_inspect_conclusion_currentIndexChanged(const QString &arg1);
    void on_comboBox_sample_name_currentIndexChanged(const QString &arg1);
    void on_comboBox_sample_specification_currentIndexChanged(const QString &arg1);
    void on_comboBox_inspector_currentIndexChanged(const QString &arg1);
};

#endif // FRMINSPECTMAIN_H
