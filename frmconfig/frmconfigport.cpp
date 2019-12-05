#include "frmconfigport.h"
#include "ui_frmconfigport.h"
#include "quiwidget.h"
#include "dbhelper.h"
#include "dbdelegate.h"
#include "excelapi.h"
#include "printapi.h"
#include "api.h"

frmConfigPort::frmConfigPort(QWidget *parent) : QWidget(parent), ui(new Ui::frmConfigPort)
{
    ui->setupUi(this);
    this->initForm();
    this->initData();
    this->changeStyle();
}

frmConfigPort::~frmConfigPort()
{
    delete ui;
}

void frmConfigPort::showEvent(QShowEvent *)
{
    model->select();
}

void frmConfigPort::initForm()
{
    API::initTableView(ui->tableView);
    ui->widgetTop->setProperty("flag", "navbtn");
    if (QUIHelper::deskWidth() < 1440) {
        ui->labTip->setText("提示 → 改动后需重启应用");
    } else {
        ui->labTip->setText("提示 → 必须和现场端口信息一致,改动后需重启应用");
    }    
}

void frmConfigPort::initData()
{
    model = new QSqlTableModel(this);    
    model->setTable("PortInfo");
    model->setSort(0, Qt::AscendingOrder);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    ui->tableView->setModel(model);
    ui->tableView->setProperty("model", true);

    columnNames.clear();
    columnNames << "编号" << "端口名称" << "串口号" << "波特率" << "IP地址" << "网络端口" << "采集周期(秒)" << "通讯超时(次)" << "重连时间(秒)";

    columnWidths.clear();
    columnWidths << 60 << 110 << 60 << 60 << 100 << 70 << 80 << 80 << 80;

    for (int i = 0; i < columnNames.count(); i++) {
        model->setHeaderData(i, Qt::Horizontal, columnNames.at(i));
        ui->tableView->setColumnWidth(i, columnWidths.at(i));
    }

    //编号委托
    QStringList portID;
    for (int i = 1; i <= 30; i++) {
        portID.append(QString::number(i));
    }

    DbDelegate *d_cbox_portID = new DbDelegate(this);
    d_cbox_portID->setDelegateType("QComboBox");
    d_cbox_portID->setDelegateValue(portID);
    //ui->tableView->setItemDelegateForColumn(0, d_cbox_portID);

    //用来切换样式自动改变颜色
    delegates << d_cbox_portID;
}

void frmConfigPort::changeStyle()
{
    foreach (DbDelegate *delegate, delegates) {
        delegate->setTextColor(QUIConfig::TextColor);
        delegate->setSelectBgColor(QUIConfig::NormalColorStart);
    }
}

void frmConfigPort::on_btnAdd_clicked()
{
    int count = model->rowCount();
    model->insertRow(count);

    int portID = model->index(count - 1, 0).data().toInt() + 1;
    QString portName = model->index(count - 1, 1).data().toString();
    QString comName = model->index(count - 1, 2).data().toString();
    int baudRate = model->index(count - 1, 3).data().toInt();
    QString tcpIP = model->index(count - 1, 4).data().toString();
    int tcpPort = model->index(count - 1, 5).data().toInt();
    int readInterval = model->index(count - 1, 6).data().toInt();
    int readTimeout = model->index(count - 1, 7).data().toInt();
    int readMaxtime = model->index(count - 1, 8).data().toInt();

    if (portName.startsWith("通信端口-")) {
        portName = QString("通信端口-%1").arg(portID);
    }

    QStringList list = tcpIP.split(".");
    if (list.count() == 4) {
        tcpIP = QString("%1.%2.%3.%4").arg(list.at(0)).arg(list.at(1)).arg(list.at(2)).arg(list.at(3).toInt() + 1);
    }

    if (count == 0) {
        portID = 1;
        portName = "通信端口-1";
        comName = "COM1";
        baudRate = 9600;
        tcpIP = "";
        tcpPort = 502;
        readInterval = 1;
        readTimeout = 3;
        readMaxtime = 60;
    }

    //设置新增加的行默认值
    model->setData(model->index(count, 0), portID);
    model->setData(model->index(count, 1), portName);
    model->setData(model->index(count, 2), comName);
    model->setData(model->index(count, 3), baudRate);
    model->setData(model->index(count, 4), tcpIP);
    model->setData(model->index(count, 5), tcpPort);
    model->setData(model->index(count, 6), readInterval);
    model->setData(model->index(count, 7), readTimeout);
    model->setData(model->index(count, 8), readMaxtime);
    ui->tableView->setCurrentIndex(model->index(count, 0));
}

void frmConfigPort::on_btnSave_clicked()
{
    model->database().transaction();
    if (model->submitAll()) {
        model->database().commit();
        DBHelper::loadPortInfo();

        //先同步更新设备表对应的端口名称
        emit portNameChanged();
    } else {
        model->database().rollback();
        QUIHelper::showMessageBoxError("保存信息失败,信息不能为空,请重新填写!");
    }
}

void frmConfigPort::on_btnDelete_clicked()
{
    if (ui->tableView->currentIndex().row() < 0) {
        QUIHelper::showMessageBoxError("请选择要删除的端口!");
        return;
    }

    if (QUIHelper::showMessageBoxQuestion("确定要删除该端口吗? 对应端口的所有控制器会一并删除!") == QMessageBox::Yes) {
        int row = ui->tableView->currentIndex().row();
        QString portName = model->index(row, 1).data().toString();
        DBHelper::deleteDeviceInfo(portName);

        model->removeRow(row);
        model->submitAll();

        int count = model->rowCount();
        ui->tableView->setCurrentIndex(model->index(count - 1, 0));
    }
}

void frmConfigPort::on_btnReturn_clicked()
{
    model->revertAll();
}

void frmConfigPort::on_btnClear_clicked()
{
    int count = model->rowCount();
    if (count <= 0) {
        return;
    }

    if (QUIHelper::showMessageBoxQuestion("确定要清空所有端口信息吗?") == QMessageBox::Yes) {
        DBHelper::clearPortInfo();
        model->select();
    }
}

void frmConfigPort::on_btnExcel_clicked()
{
    QString name = "端口信息";
    QString fileName = QString("%1_%2").arg(name).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"));
    QString file = API::getSaveFileNames(fileName, "Excel(*.xls)");
    if (file.isEmpty()) {
        return;
    }

    QString columns = "PortID,PortName,ComName,BaudRate,TcpIP,TcpPort,ReadInterval,ReadTimeout,ReadMaxtime";
    QString where = "order by PortID asc";
    QStringList content = DBHelper::getContent("PortInfo", columns, where, "", ";");

    int rowCount = content.count();
    if (rowCount == 0) {
        QUIHelper::showMessageBoxError("没有要处理的数据!");
        return;
    }

    ExcelAPI::Instance()->saveExcel(file, name, name, "", columnNames, columnWidths, content);
    QString msg = QString("导出%1到Excel").arg(name);
    DBHelper::addUserLog("用户操作", msg);

    if (QUIHelper::showMessageBoxQuestion(msg + "成功!确定现在就打开吗?") == QMessageBox::Yes) {
        QString url = QString("file:///%1").arg(file);
        QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
    }
}

void frmConfigPort::on_btnPrint_clicked()
{
    QString name = "端口信息";
    QString columns = "PortID,PortName,ComName,BaudRate,TcpIP,TcpPort,ReadInterval,ReadTimeout,ReadMaxtime";
    QString where = "order by PortID asc";
    QStringList content = DBHelper::getContent("PortInfo", columns, where, "", ";");

    int rowCount = content.count();
    if (rowCount == 0) {
        QUIHelper::showMessageBoxError("没有要处理的数据!");
        return;
    }

    PrintAPI::Instance()->print(name, "", columnNames, columnWidths, content);
    QString msg = QString("打印%1").arg(name);
    DBHelper::addUserLog("用户操作", msg);
}

void frmConfigPort::on_btnInput_clicked()
{
    QString fileName;
    bool ok = DBHelper::inputData(columnNames.count(), App::FileFilter, "PortInfo", fileName, QUIHelper::appPath() + "/db", true);
    if (!fileName.isEmpty()) {
        DBHelper::addUserLog("用户操作", "导入端口信息");
        if (ok) {
            QUIHelper::showMessageBoxInfo("导入端口信息成功!", 3);
            model->select();
        } else {
            QUIHelper::showMessageBoxError("导入端口信息失败!", 3);
        }
    }
}

void frmConfigPort::on_btnOutput_clicked()
{
    QString columns = "*";
    QString where = "order by PortID asc";
    QString title = columnNames.join(App::FileSpliter);
    QStringList content = DBHelper::getContent("PortInfo", columns, where, title);

    QString fileName;
    bool ok = DBHelper::outputData("端口信息", App::FileFilter, content, fileName, QUIHelper::appPath() + "/db");
    if (!fileName.isEmpty()) {
        DBHelper::addUserLog("用户操作", "导出端口信息");
        if (ok) {
            QUIHelper::showMessageBoxInfo("导出端口信息成功!", 3);
        } else {
            QUIHelper::showMessageBoxError("导出端口信息失败!", 3);
        }
    }
}
