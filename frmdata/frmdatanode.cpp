﻿#include "frmdatanode.h"
#include "ui_frmdatanode.h"
#include "quiwidget.h"
#include "dbhelper.h"
#include "dbpage.h"
#include "excelapi.h"
#include "printapi.h"
#include "pdfapi.h"
#include "excelthread.h"
#include "printthread.h"
#include "api.h"

frmDataNode::frmDataNode(QWidget *parent) : QWidget(parent), ui(new Ui::frmDataNode)
{
    ui->setupUi(this);
    this->initForm();
    this->initData();
}

frmDataNode::~frmDataNode()
{
    delete ui;
}

void frmDataNode::initForm()
{
    ui->frameRight->setProperty("flag", "navbtn");
    ui->frameBottom->setProperty("flag", "navlab");
    ui->cboxDeviceName->addItems(DBData::DeviceInfo_DeviceName);

    QUIHelper::setLabStyle(ui->labPageCurrent, 2);
    QUIHelper::setLabStyle(ui->labPageCount, 2);
    QUIHelper::setLabStyle(ui->labResultCount, 2);
    QUIHelper::setLabStyle(ui->labResultCurrent, 2);
    QUIHelper::setLabStyle(ui->labResult, 0);

    ui->dateStart->setDate(QDate::currentDate());
    ui->dateEnd->setDate(QDate::currentDate().addDays(1));
    ui->dateStart->calendarWidget()->setLocale(QLocale::Chinese);
    ui->dateEnd->calendarWidget()->setLocale(QLocale::Chinese);
    API::initTableView2(ui->tableView);
}

void frmDataNode::initData()
{
    whereSql = "where 1=1";
    columnNames << "编号" << "位号" << "控制器名称" << "探测器名称" << "浓度值" << "气体符号" << "保存时间";
    columnWidths << 60 << 100 << 130 << 130 << 60 << 70 << 150;

    if (App::WorkMode == 2) {
        columnNames[5] = "符号";
    }

    //设置需要显示数据的表格和翻页的按钮,最后一列自动填充,奇偶行不同颜色显示
    dbPage = new DbPage(this);
    dbPage->setAllCenter(true);
    dbPage->setColumnNames(columnNames);
    dbPage->setColumnWidths(columnWidths);
    dbPage->setResultCurrent(App::PageCount);
    dbPage->setTableName("NodeLog");
    dbPage->setOrderSql(QString("LogID %1").arg(App::NodeLogOrder));
    dbPage->setControl(ui->tableView, ui->labPageCount, ui->labPageCurrent, ui->labResultCount, ui->labResultCurrent, ui->labResult, 0,
                       ui->btnFirst, ui->btnPre, ui->btnNext, ui->btnLast, "LogID");

    dbPage->setWhereSql(whereSql);
    dbPage->select();
}

void frmDataNode::on_btnSelect_clicked()
{
    QDateTime dateStart = ui->dateStart->dateTime();
    QDateTime dateEnd = ui->dateEnd->dateTime();

    if (dateStart > dateEnd) {
        QUIHelper::showMessageBoxError("开始时间不能大于结束时间!", 3);
        return;
    }

    //构建SQL语句
    QString sql = "where 1=1";

    if (ui->ckTimeStart->isChecked()) {
        if (App::LocalDBType.toUpper() == "SQLITE") {
            sql += " and datetime(SaveTime)>='" + dateStart.toString("yyyy-MM-dd HH:mm:ss") + "'";
            sql += " and datetime(SaveTime)<='" + dateEnd.toString("yyyy-MM-dd HH:mm:ss") + "'";
        } else if (App::LocalDBType.toUpper() == "MYSQL") {
            sql += " and unix_timestamp(SaveTime)>=unix_timestamp('" + dateStart.toString("yyyy-MM-dd HH:mm:ss") + "')";
            sql += " and unix_timestamp(SaveTime)<=unix_timestamp('" + dateEnd.toString("yyyy-MM-dd HH:mm:ss") + "')";
        }
    }

    if (ui->ckDeviceName->isChecked()) {
        sql += " and DeviceName='" + ui->cboxDeviceName->currentText() + "'";
    }

    if (ui->ckNodeName->isChecked()) {
        sql += " and NodeName='" + ui->cboxNodeName->currentText() + "'";
    }

    if (ui->ckPositionID->isChecked()) {
        sql += " and PositionID='" + ui->txtPositionID->text() + "'";
    }

    //绑定数据到表格
    whereSql = sql;
    dbPage->setWhereSql(whereSql);
    dbPage->select();
}

void frmDataNode::on_btnExcel_clicked()
{
    //先判断行数,超过一定大小弹出提示
    QString str = ui->labResultCount->text();
    QStringList list = str.split(" ");
    int rowCount = list.at(1).toInt();
    if (rowCount > 100000) {
        QUIHelper::showMessageBoxError("不支持大量数据,请重新查询!", 3);
        return;
    }

    if (rowCount > 10000) {
        QString msg = QString("要导出的数据共 %1 条,请耐心等待!确定要导出数据吗?").arg(rowCount);
        if (QUIHelper::showMessageBoxQuestion(msg) != QMessageBox::Yes) {
            return;
        }
    }

    QString fileName = API::getFileName("保存文件(*.xls)", QUIHelper::appPath() + "/db", QString("nodelog_%1.xls").arg(STRDATETIME));
    if (fileName.isEmpty()) {
        return;
    }

    //设定导出数据字段及排序字段
    QString columns = "LogID,PositionID,DeviceName,NodeName,NodeValue,NodeSign,SaveTime";
    QString where = whereSql + " order by " + QString("LogID %1").arg(App::NodeLogOrder);
    QStringList content = DBHelper::getContent("NodeLog", columns, where, "", ";");

    QString name = ui->ckDeviceName->isChecked() ? ui->cboxDeviceName->currentText() : "所有探测器";
    QString type = name + "运行记录";
    ExcelAPI::Instance()->saveExcel(fileName, type, type, "", columnNames, columnWidths, content);

    QString msg = QString("导出%1").arg(type);
    DBHelper::addUserLog(msg);

    if (QUIHelper::showMessageBoxQuestion(msg + "成功!确定现在就打开吗?") == QMessageBox::Yes) {
        QString url = QString("file:///%1").arg(fileName);
        QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
    }
}

void frmDataNode::on_btnPrint_clicked()
{
    //先判断行数,超过一定大小弹出提示
    QString str = ui->labResultCount->text();
    QStringList list = str.split(" ");
    int rowCount = list.at(1).toInt();
    if (rowCount > 5000) {
        QUIHelper::showMessageBoxError("不支持大量数据,请重新查询!", 3);
        return;
    }

    if (rowCount > 500) {
        QString msg = QString("要导出的数据共 %1 条,请耐心等待!确定要打印数据吗?").arg(rowCount);
        if (QUIHelper::showMessageBoxQuestion(msg) != QMessageBox::Yes) {
            return;
        }
    }

    //设定导出数据字段及排序字段
    QString columns = "LogID,PositionID,DeviceName,NodeName,NodeValue,NodeSign,SaveTime";
    QString where = whereSql + " order by " + QString("LogID %1").arg(App::NodeLogOrder);
    QStringList content = DBHelper::getContent("NodeLog", columns, where, "", ";");

    QString name = ui->ckDeviceName->isChecked() ? ui->cboxDeviceName->currentText() : "所有探测器";
    QString type = name + "运行记录";
    PrintAPI::Instance()->print(type, "", columnNames, columnWidths, content);

    QString msg = QString("打印%1").arg(type);
    DBHelper::addUserLog(msg);
}

void frmDataNode::on_btnPdf_clicked()
{
    //先判断行数,超过一定大小弹出提示
    QString str = ui->labResultCount->text();
    QStringList list = str.split(" ");
    int rowCount = list.at(1).toInt();
    if (rowCount > 5000) {
        QUIHelper::showMessageBoxError("不支持大量数据,请重新查询!", 3);
        return;
    }

    if (rowCount > 500) {
        QString msg = QString("要导出的数据共 %1 条,请耐心等待!确定要导出数据吗?").arg(rowCount);
        if (QUIHelper::showMessageBoxQuestion(msg) != QMessageBox::Yes) {
            return;
        }
    }

    QString fileName = API::getFileName("保存文件(*.pdf)", QUIHelper::appPath() + "/db", QString("nodelog_%1.pdf").arg(STRDATETIME));
    if (fileName.isEmpty()) {
        return;
    }

    //设定导出数据字段及排序字段
    QString columns = "LogID,PositionID,DeviceName,NodeName,NodeValue,NodeSign,SaveTime";
    QString where = whereSql + " order by " + QString("LogID %1").arg(App::NodeLogOrder);
    QStringList content = DBHelper::getContent("NodeLog", columns, where, "", ";");

    QString name = ui->ckDeviceName->isChecked() ? ui->cboxDeviceName->currentText() : "所有探测器";
    QString type = name + "运行记录";
    PdfAPI::Instance()->savePdf(fileName, type, "", columnNames, columnWidths, content);

    QString msg = QString("导出%1").arg(type);
    DBHelper::addUserLog(msg);

    if (QUIHelper::showMessageBoxQuestion(msg + "成功!确定现在就打开吗?") == QMessageBox::Yes) {
        QString url = QString("file:///%1").arg(fileName);
        QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
    }
}

void frmDataNode::on_btnDelete_clicked()
{
    if (!App::CurrentUserType.contains("管理员")) {
        QUIHelper::showMessageBoxError("只有管理员才有权限!", 3);
        return;
    }

    QString timeStart, timeEnd;
    QUIHelper::showDateSelect(timeStart, timeEnd);
    if (!timeStart.isEmpty() && !timeEnd.isEmpty()) {
        if (QUIHelper::showMessageBoxQuestion("确定要删除该时间段的数据吗?") == QMessageBox::Yes) {
            DBHelper::deleteNodeLog(timeStart, timeEnd);
            QString msg = "删除运行记录成功";
            DBHelper::addUserLog(msg);
            QUIHelper::showMessageBoxInfo(QString("%1!").arg(msg), 3);

            //重新查询数据
            on_btnSelect_clicked();
        }
    }
}

void frmDataNode::on_btnClear_clicked()
{
    if (!App::CurrentUserType.contains("管理员")) {
        QUIHelper::showMessageBoxError("只有管理员才有权限!", 3);
        return;
    }

    if (QUIHelper::showMessageBoxQuestion("确定要清空所有数据吗?") == QMessageBox::Yes) {
        DBHelper::clearNodeLog();
        QString msg = "清空运行记录成功";
        DBHelper::addUserLog(msg);
        QUIHelper::showMessageBoxInfo(QString("%1!").arg(msg), 3);

        //重新查询数据
        on_btnSelect_clicked();
    }
}

void frmDataNode::on_cboxDeviceName_currentIndexChanged(const QString &arg1)
{
    ui->cboxNodeName->clear();
    for (int i = 0; i < DBData::NodeInfo_Count; i++) {
        if (DBData::NodeInfo_DeviceName.at(i) == arg1) {
            ui->cboxNodeName->addItem(DBData::NodeInfo_NodeName.at(i), DBData::NodeInfo_PositionID.at(i));
        }
    }
}

void frmDataNode::on_cboxNodeName_currentIndexChanged(int index)
{
    ui->txtPositionID->setText(ui->cboxNodeName->itemData(index).toString());
}
