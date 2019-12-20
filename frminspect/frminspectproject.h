#ifndef FRMINSPECTPROJECT_H
#define FRMINSPECTPROJECT_H

#include <QWidget>
#include "dbinspect.h"

namespace Ui {
class frmInspectProject;
}

class frmInspectProject : public QWidget
{
    Q_OBJECT

public:
    explicit frmInspectProject(QWidget *parent = nullptr);
    ~frmInspectProject();

private:
    Ui::frmInspectProject *ui;
    DBInspect dbInspect_template_resistance;
    DBInspect dbInspect_template_capacitance;
    DBInspect dbInspect_template_direct_current;
    DBInspect dbInspect_template_alternating_current;
    DBInspect dbInspect_template_direct_voltage;
    DBInspect dbInspect_template_alternating_voltage;
    QString gpib_address;

private slots:
    void initForm(void);
    void uninitForm(void);
    void on_comboBox_template_type_currentIndexChanged(const QString &arg1);
    void on_comboBox_template_name_currentIndexChanged(const QString &arg1);
};

#endif // FRMINSPECTPROJECT_H
