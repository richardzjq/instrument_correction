#ifndef FRMCONFIGSYSTEM_H
#define FRMCONFIGSYSTEM_H

#include <QWidget>

namespace Ui {
class frmConfigSystem;
}

class frmConfigSystem : public QWidget
{
    Q_OBJECT

public:
    explicit frmConfigSystem(QWidget *parent = 0);
    ~frmConfigSystem();

private:
    Ui::frmConfigSystem *ui;

private slots:
    void initForm();
    void initConfig();
    void saveConfig();
    void initColor();
    void saveColor();
    void initTime();
    void saveTime();    
};

#endif // FRMCONFIGSYSTEM_H
