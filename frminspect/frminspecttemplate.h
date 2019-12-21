#ifndef FRMINSPECTTEMPLATE_H
#define FRMINSPECTTEMPLATE_H

#include <QWidget>
#include "dbinspect.h"

namespace Ui {
class frmInspectTemplate;
}

struct template_header {
     QString template_rang;
     QString template_standard_value;
     QString template_standard_max_error;
};

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
    DBInspectMap map_string_db;
    QMap<QString, template_header> maps_template_header;

private slots:
    void initData(void);
    void initForm(void);
    void uninitForm(void);
    void on_btn_create_clicked();
    void on_treeWidget_template_itemClicked(QTreeWidgetItem *item, int column);
    void on_btn_save_clicked();
};

#endif // FRMINSPECTTEMPLATE_H
