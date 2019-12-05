#ifndef FRMCONFIGINSTRUMENT_H
#define FRMCONFIGINSTRUMENT_H

#include <QWidget>

namespace Ui {
class frmConfigInstrument;
}

class frmConfigInstrument : public QWidget
{
    Q_OBJECT

public:
    explicit frmConfigInstrument(QWidget *parent = nullptr);
    ~frmConfigInstrument();

private:
    Ui::frmConfigInstrument *ui;
};

#endif // FRMCONFIGINSTRUMENT_H
