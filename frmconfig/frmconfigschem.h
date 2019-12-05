#ifndef FRMCONFIGSCHEM_H
#define FRMCONFIGSCHEM_H

#include <QWidget>

namespace Ui {
class frmConfigSchem;
}

class frmConfigSchem : public QWidget
{
    Q_OBJECT

public:
    explicit frmConfigSchem(QWidget *parent = nullptr);
    ~frmConfigSchem();

private:
    Ui::frmConfigSchem *ui;
};

#endif // FRMCONFIGSCHEM_H
