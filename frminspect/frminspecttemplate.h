#ifndef FRMINSPECTTEMPLATE_H
#define FRMINSPECTTEMPLATE_H

#include <QWidget>

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

private slots:
    void initForm(void);
    void uninitForm(void);
};

#endif // FRMINSPECTTEMPLATE_H
