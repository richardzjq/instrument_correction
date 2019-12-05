#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include <QDialog>

namespace Ui
{
class frmLogin;
}

class frmLogin : public QDialog
{
	Q_OBJECT

public:
	explicit frmLogin(QWidget *parent = 0);
	~frmLogin();

private:
	Ui::frmLogin *ui;

private slots:
	void initStyle();   //初始化无边框窗体
	void initForm();    //初始化窗体数据

private slots:
	void on_btnLogin_clicked();
	void on_cboxUserName_activated(int);
};

#endif // FRMLOGIN_H
