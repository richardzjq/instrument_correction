#ifndef FRMINSPECTPROJECT_H
#define FRMINSPECTPROJECT_H

#include <QWidget>

namespace Ui {
class frmInspectProject;
}

class frmInspectProject : public QWidget
{
    Q_OBJECT

public:
    explicit frmInspectProject(QWidget *parent = nullptr);
    ~frmInspectProject();

private:
    Ui::frmInspectProject *ui;
};

#endif // FRMINSPECTPROJECT_H
