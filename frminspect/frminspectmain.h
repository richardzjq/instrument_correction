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

};

#endif // FRMINSPECTMAIN_H
