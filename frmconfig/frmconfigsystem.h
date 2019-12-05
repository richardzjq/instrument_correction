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
    void initLog();
    void saveLog();
    void initColor();
    void saveColor();
    void initTime();
    void saveTime();    
    void initLocalDB();
    void saveLocalDB();
    QString saveExcel();

private slots:
    void on_btnLocalDBConnect_clicked();
    void on_btnLocalDBInit_clicked();
};

#endif // FRMCONFIGSYSTEM_H
