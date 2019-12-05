#ifndef FRMLOGOUT_H
#define FRMLOGOUT_H

#include <QDialog>

namespace Ui
{
class frmLogout;
}

class frmLogout : public QDialog
{
	Q_OBJECT

public:
	explicit frmLogout(QWidget *parent = 0);
	~frmLogout();

private slots:
	void on_btnLogout_clicked();

private:
	Ui::frmLogout *ui;

signals:
	void signal_exit();

private slots:
	void initStyle();       //初始化无边框窗体
	void initForm();        //初始化窗体数据

};

#endif // FRMLOGOUT_H
