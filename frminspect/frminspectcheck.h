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
};

#endif // FRMINSPECTCHECK_H
