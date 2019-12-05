#include "frmconfiguser.h"
#include "ui_frmconfiguser.h"
#include "quiwidget.h"
#include "dbhelper.h"
#include "dbdelegate.h"
#include "excelapi.h"
#include "printapi.h"
#include "api.h"

frmConfigUser::frmConfigUser(QWidget *parent) : QWidget(parent), ui(new Ui::frmConfigUser)
{
    ui->setupUi(this);
    this->initForm();
    this->initData();
    this->changeStyle();
}

frmConfigUser::~frmConfigUser()
{
    delete ui;
}

void frmConfigUser::initForm()
{    
    API::initTableView(ui->tableView);
    ui->widgetTop->setProperty("flag", "navbtn");
    ui->labTip->setText("提示 → 用户权限不一致");
}

void frmConfigUser::initData()
{
    model = new QSqlTableModel(this);    
    model->setTable("UserInfo");
    model->setSort(0, Qt::AscendingOrder);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    ui->tableView->setModel(model);
    ui->tableView->setProperty("model", true);

    columnNames.clear();
    columnNames << "用户姓名" << "用户密码" << "用户类型" << "用户权限";

    columnWidths.clear();
    columnWidths << 120 << 150 << 130 << 120;

    for (int i = 0; i < columnNames.count(); i++) {
        model->setHeaderData(i, Qt::Horizontal, columnNames.at(i));
        ui->tableView->setColumnWidth(i, columnWidths.at(i));
    }

    //用户名委托
    DbDelegate *d_txt_userName = new DbDelegate(this);
    d_txt_userName->setDelegateType("QLineEdit");
    d_txt_userName->setDelegatePwd(false);
    ui->tableView->setItemDelegateForColumn(0, d_txt_userName);

    //用户密码委托
    DbDelegate *d_txt_userPwd = new DbDelegate(this);
    d_txt_userPwd->setDelegateType("QLineEdit");
    d_txt_userPwd->setDelegatePwd(true);
    d_txt_userPwd->setDelegateColumn(1);
    ui->tableView->setItemDelegateForColumn(1, d_txt_userPwd);

    //用户类型委托
    QStringList userType;
    userType << "管理员" << "操作员";
    DbDelegate *d_cbox_userType = new DbDelegate(this);
    d_cbox_userType->setDelegateType("QComboBox");
    d_cbox_userType->setDelegateValue(userType);
    ui->tableView->setItemDelegateForColumn(2, d_cbox_userType);

    //用来切换样式自动改变颜色
    delegates << d_txt_userName;
    delegates << d_txt_userPwd;
    delegates << d_cbox_userType;
}

void frmConfigUser::changeStyle()
{
    foreach (DbDelegate *delegate, delegates) {
        delegate->setTextColor(QUIConfig::TextColor);
        delegate->setSelectBgColor(QUIConfig::NormalColorStart);
    }
}

void frmConfigUser::on_btnAdd_clicked()
{
    int count = model->rowCount();
    model->insertRow(count);

    QString userName = model->index(count - 1, 0).data().toString();
    QString userType = model->index(count - 1, 2).data().toString();

    if (count == 0) {
        userName="user";
        userType="操作员";
    }

    //设置新增加的行默认值
    model->setData(model->index(count, 0), userName);
    model->setData(model->index(count, 2), userType);
    ui->tableView->setCurrentIndex(model->index(count, 0));
}

void frmConfigUser::on_btnSave_clicked()
{
    model->database().transaction();
    if (model->submitAll()) {
        model->database().commit();
    } else {
        model->database().rollback();
        QUIHelper::showMessageBoxError("保存信息失败,信息不能为空,请重新填写!");
    }
}

void frmConfigUser::on_btnDelete_clicked()
{
    int row = ui->tableView->currentIndex().row();
    if (row < 0) {
        QUIHelper::showMessageBoxError("请选择要删除的用户!");
        return;
    }

    QString userName = model->index(row, 0).data().toString();
    if (userName == "admin") {
        QUIHelper::showMessageBoxError("admin用户不能被删除!", 3);
        return;
    }

    if (QUIHelper::showMessageBoxQuestion("确定要删除该用户吗!") == QMessageBox::Yes) {
        model->removeRow(row);
        model->submitAll();

        int count = model->rowCount();
        ui->tableView->setCurrentIndex(model->index(count - 1, 0));
    }
}

void frmConfigUser::on_btnReturn_clicked()
{
    model->revertAll();
}

void frmConfigUser::on_btnClear_clicked()
{

}

void frmConfigUser::on_btnExcel_clicked()
{

}

void frmConfigUser::on_btnPrint_clicked()
{

}

void frmConfigUser::on_btnInput_clicked()
{

}

void frmConfigUser::on_btnOutput_clicked()
{

}
