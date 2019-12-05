#ifndef FRMCONFIGMODEL_H
#define FRMCONFIGMODEL_H

#include <QWidget>

namespace Ui {
class frmConfigModel;
}

class frmConfigModel : public QWidget
{
    Q_OBJECT

public:
    explicit frmConfigModel(QWidget *parent = nullptr);
    ~frmConfigModel();

private:
    Ui::frmConfigModel *ui;

private slots:
};

#endif // FRMINSPECTMODEL_H
