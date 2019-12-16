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
};

#endif // FRMINSPECTTEMPLATE_H
