﻿#ifndef FRMINSPECTTEMPLATE_H
#define FRMINSPECTTEMPLATE_H

#include <QWidget>
#include "dbinspect.h"

namespace Ui {
class frmInspectTemplate;
}

class frmInspectTemplate : public QWidget
{
    Q_OBJECT

public:
    explicit frmInspectTemplate(QWidget *parent = nullptr);
    ~frmInspectTemplate();

private:
    Ui::frmInspectTemplate *ui;
    DBInspect dbInspect_template_resistance;
    DBInspect dbInspect_template_capacitance;
    DBInspect dbInspect_template_direct_current;
    DBInspect dbInspect_template_alternating_current;
    DBInspect dbInspect_template_direct_voltage;
    DBInspect dbInspect_template_alternating_voltage;

private slots:
    void initForm(void);
    void uninitForm(void);
    void on_btn_create_clicked();
    void on_treeWidget_template_itemClicked(QTreeWidgetItem *item, int column);
};

#endif // FRMINSPECTTEMPLATE_H
