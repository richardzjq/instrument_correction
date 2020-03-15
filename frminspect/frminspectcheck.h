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


private:
    Ui::frmInspectCheck *ui;
    QString record_table_name;
    int columns_count;

private slots:
    void initForm(void);
    void uninitForm(void);
    void clearForm(void);
    void get_inspect_template_list(QStringList *p_stringList);

    void on_btn_create_clicked();
    void on_btn_save_clicked();
    void on_btn_delete_inspected_project_clicked();
    void on_btn_inspect_new_project_clicked();
    void on_treeWidget_inpsected_project_itemClicked(QTreeWidgetItem *item, int column);
    void on_btn_print_clicked();
};

#endif // FRMINSPECTCHECK_H
