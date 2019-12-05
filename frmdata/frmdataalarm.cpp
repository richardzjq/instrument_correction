#include "frmdataalarm.h"
#include "ui_frmdataalarm.h"
#include "quiwidget.h"
#include "dbhelper.h"
#include "dbpage.h"
#include "excelapi.h"
#include "printapi.h"
#include "pdfapi.h"
#include "api.h"

frmDataAlarm::frmDataAlarm(QWidget *parent) : QWidget(parent), ui(new Ui::frmDataAlarm)
{
    ui->setupUi(this);
    this->initForm();
    this->initData();
}

frmDataAlarm::~frmDataAlarm()
{
    delete ui;
}

void frmDataAlarm::initForm()
{
    ui->frameRight->setProperty("flag", "navbtn");
    ui->frameBottom->setProperty("flag", "navlab");
    ui->cboxDeviceName->addItems(DBData::DeviceInfo_DeviceName);

    QStringList content;
    if (App::WorkMode == 2) {
        content << "报警" << "高报" << "失效";
    } else {
        content << "浓度上限报警" << "浓度下限报警";
    }

    ui->cboxContent->addItems(content);

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

    //增加鼠标右键删除
    QAction *action = new QAction("删除记录", this);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(doAction()));
    ui->tableView->addAction(action);
    if (App::CurrentUserType.contains("管理员")) {
        ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    }
}

void frmDataAlarm::initData()
{
    whereSql = "where 1=1";
    columnNames << "编号" << "位号" << "控制器名称" << "探测器名称" << "报警值" << "单位" << "报警类型"
                << "触发时间" << "结束时间" << "确认用户" << "确认时间" << "确认信息";
    if (QUIHelper::deskWidth() < 1440) {
        columnWidths << 40 << 80 << 120 << 80 << 50 << 0 << 100 << 135 << 0 << 80 << 135 << 100;
    } else {
        columnWidths << 60 << 90 << 130 << 100 << 50 << 0 << 100 << 135 << 0 << 80 << 135 << 110;
    }

    //设置需要显示数据的表格和翻页的按钮,最后一列自动填充,奇偶行不同颜色显示
    dbPage = new DbPage(this);
    dbPage->setAllCenter(true);
    dbPage->setColumnNames(columnNames);
    dbPage->setColumnWidths(columnWidths);
    dbPage->setResultCurrent(App::PageCount);
    dbPage->setTableName("AlarmLog");
    dbPage->setOrderSql(QString("LogID %1").arg(App::AlarmLogOrder));
    dbPage->setControl(ui->tableView, ui->labPageCount, ui->labPageCurrent, ui->labResultCount, ui->labResultCurrent, ui->labResult, 0,
                       ui->btnFirst, ui->btnPre, ui->btnNext, ui->btnLast, "LogID");
    dbPage->setWhereSql(whereSql);
    dbPage->select();

    //2019-6-24 隐藏结束时间,大部分时候不需要
    ui->tableView->setColumnHidden(5, true);
    ui->tableView->setColumnHidden(8, true);
}

void frmDataAlarm::doAction()
{
    QAction *action = (QAction *)sender();
    QString text = action->text();
    if (text == "删除记录") {
        if (QUIHelper::showMessageBoxQuestion("确定要删除选中的记录吗?") == QMessageBox::Yes) {
            //统计当前选中了多少行记录,找出唯一标识,逐个删除
            QStringList ids;
            QItemSelectionModel *selections = ui->tableView->selectionModel();
            QModelIndexList selected = selections->selectedIndexes();
            foreach (QModelIndex index, selected) {
                if (index.column() == 0) {
                    ids << index.data().toString();
                }
            }

            DBHelper::deleteAlarmLog(ids.join(","));
            on_btnSelect_clicked();
            QString msg = "删除报警记录成功";
            DBHelper::addUserLog(msg);
            QUIHelper::showMessageBoxError(msg, 3);
        }
    }
}

void frmDataAlarm::on_btnSelect_clicked()
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
            sql += " and datetime(StartTime)>='" + dateStart.toString("yyyy-MM-dd HH:mm:ss") + "'";
            sql += " and datetime(StartTime)<='" + dateEnd.toString("yyyy-MM-dd HH:mm:ss") + "'";
        } else if (App::LocalDBType.toUpper() == "MYSQL") {
            sql += " and unix_timestamp(StartTime)>=unix_timestamp('" + dateStart.toString("yyyy-MM-dd HH:mm:ss") + "')";
            sql += " and unix_timestamp(StartTime)<=unix_timestamp('" + dateEnd.toString("yyyy-MM-dd HH:mm:ss") + "')";
        }
    }

    if (ui->ckDeviceName->isChecked()) {
        sql += " and DeviceName='" + ui->cboxDeviceName->currentText() + "'";
    }

    if (ui->ckNodeName->isChecked()) {
        sql += " and NodeName='" + ui->cboxNodeName->currentText() + "'";
    }

    if (ui->ckContent->isChecked()) {
        sql += " and Content='" + ui->cboxContent->currentText() + "'";
    }

    if (ui->ckNodeValue->isChecked()) {
        sql += " and NodeValue='" + ui->txtNodeValue->text() + "'";
    }

    if (ui->ckPositionID->isChecked()) {
        sql += " and PositionID='" + ui->txtPositionID->text() + "'";
    }

    //绑定数据到表格
    whereSql = sql;
    dbPage->setWhereSql(whereSql);
    dbPage->select();
}

void frmDataAlarm::on_btnExcel_clicked()
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

    QString fileName = API::getFileName("保存文件(*.xls)", QUIHelper::appPath() + "/db", QString("alarmlog_%1.xls").arg(STRDATETIME));
    if (fileName.isEmpty()) {
        return;
    }

    //设定导出数据字段及排序字段
    QString columns = "LogID,PositionID,DeviceName,NodeName,NodeValue,NodeSign,Content,StartTime,ConfirmUser,ConfirmTime,ConfirmContent";
    QString where = whereSql + " order by " + QString("LogID %1").arg(App::AlarmLogOrder);
    QStringList content = DBHelper::getContent("AlarmLog", columns, where, "", ";");

    QList<QString> columnNames;
    QList<int> columnWidths;
    columnNames << "编号" << "位号" << "控制器名称" << "探测器名称" << "报警值" << "单位" << "报警类型"
                << "触发时间" << "确认用户" << "确认时间" << "确认信息";
    columnWidths << 50 << 80 << 120 << 100 << 50 << 50 << 100 << 135 << 80 << 135 << 120;

    QString name = ui->ckNodeName->isChecked() ? ui->cboxNodeName->currentText() : "所有探测器";
    QString type = name + "报警记录";
    ExcelAPI::Instance()->saveExcel(fileName, type, type, "", columnNames, columnWidths, content);

    QString msg = QString("导出%1").arg(type);
    DBHelper::addUserLog(msg);

    if (QUIHelper::showMessageBoxQuestion(msg + "成功!确定现在就打开吗?") == QMessageBox::Yes) {
        QString url = QString("file:///%1").arg(fileName);
        QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
    }
}

void frmDataAlarm::on_btnPrint_clicked()
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
    QString columns = "LogID,PositionID,DeviceName,NodeName,NodeValue,NodeSign,Content,StartTime,ConfirmUser,ConfirmTime,ConfirmContent";
    QString where = whereSql + " order by " + QString("LogID %1").arg(App::AlarmLogOrder);
    QStringList content = DBHelper::getContent("AlarmLog", columns, where, "", ";");

    QList<QString> columnNames;
    QList<int> columnWidths;
    columnNames << "编号" << "位号" << "控制器名称" << "探测器名称" << "报警值" << "单位" << "报警类型"
                << "触发时间" << "确认用户" << "确认时间" << "确认信息";
    columnWidths << 50 << 80 << 120 << 100 << 50 << 50 << 100 << 135 << 80 << 135 << 120;

    QString name = ui->ckNodeName->isChecked() ? ui->cboxNodeName->currentText() : "所有探测器";
    QString type = name + "报警记录";
    PrintAPI::Instance()->print(type, "", columnNames, columnWidths, content, true);

    QString msg = QString("打印%1").arg(type);
    DBHelper::addUserLog(msg);
}

void frmDataAlarm::on_btnPdf_clicked()
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

    QString fileName = API::getFileName("保存文件(*.pdf)", QUIHelper::appPath() + "/db", QString("alarmlog_%1.pdf").arg(STRDATETIME));
    if (fileName.isEmpty()) {
        return;
    }

    //设定导出数据字段及排序字段
    QString columns = "LogID,PositionID,DeviceName,NodeName,NodeValue,NodeSign,Content,StartTime,ConfirmUser,ConfirmTime,ConfirmContent";
    QString where = whereSql + " order by " + QString("LogID %1").arg(App::AlarmLogOrder);
    QStringList content = DBHelper::getContent("AlarmLog", columns, where, "", ";");

    QList<QString> columnNames;
    QList<int> columnWidths;
    columnNames << "编号" << "位号" << "控制器名称" << "探测器名称" << "报警值" << "单位" << "报警类型"
                << "触发时间" << "确认用户" << "确认时间" << "确认信息";
    columnWidths << 50 << 80 << 120 << 100 << 50 << 50 << 100 << 135 << 80 << 135 << 120;

    QString name = ui->ckNodeName->isChecked() ? ui->cboxNodeName->currentText() : "所有探测器";
    QString type = name + "报警记录";
    PdfAPI::Instance()->savePdf(fileName, type, "", columnNames, columnWidths, content, true);

    QString msg = QString("导出%1").arg(type);
    DBHelper::addUserLog(msg);

    if (QUIHelper::showMessageBoxQuestion(msg + "成功!确定现在就打开吗?") == QMessageBox::Yes) {
        QString url = QString("file:///%1").arg(fileName);
        QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
    }
}

void frmDataAlarm::on_btnDelete_clicked()
{
    if (!App::CurrentUserType.contains("管理员")) {
        QUIHelper::showMessageBoxError("只有管理员才有权限!", 3);
        return;
    }

    QString timeStart, timeEnd;
    QUIHelper::showDateSelect(timeStart, timeEnd);
    if (!timeStart.isEmpty() && !timeEnd.isEmpty()) {
        if (QUIHelper::showMessageBoxQuestion("确定要删除该时间段的数据吗?") == QMessageBox::Yes) {
            DBHelper::deleteAlarmLog(timeStart, timeEnd);
            QString msg = "删除报警记录成功";
            DBHelper::addUserLog(msg);
            QUIHelper::showMessageBoxInfo(QString("%1!").arg(msg), 3);

            //重新查询数据
            on_btnSelect_clicked();
        }
    }
}

void frmDataAlarm::on_btnClear_clicked()
{
    if (!App::CurrentUserType.contains("管理员")) {
        QUIHelper::showMessageBoxError("只有管理员才有权限!", 3);
        return;
    }

    if (QUIHelper::showMessageBoxQuestion("确定要清空所有数据吗?") == QMessageBox::Yes) {
        DBHelper::clearAlarmLog();
        QString msg = "清空报警记录成功";
        DBHelper::addUserLog(msg);
        QUIHelper::showMessageBoxInfo(QString("%1!").arg(msg), 3);

        //重新查询数据
        on_btnSelect_clicked();
    }
}

void frmDataAlarm::on_cboxDeviceName_currentIndexChanged(const QString &arg1)
{
    ui->cboxNodeName->clear();
    for (int i = 0; i < DBData::NodeInfo_Count; i++) {
        if (DBData::NodeInfo_DeviceName.at(i) == arg1) {
            ui->cboxNodeName->addItem(DBData::NodeInfo_NodeName.at(i), DBData::NodeInfo_PositionID.at(i));
        }
    }
}

void frmDataAlarm::on_cboxNodeName_currentIndexChanged(int index)
{
    ui->txtPositionID->setText(ui->cboxNodeName->itemData(index).toString());
}

void frmDataAlarm::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    if (row < 0) {
        return;
    }

    QAbstractItemModel *model = ui->tableView->model();
    QString logID = model->index(row, 0).data().toString();
    QString confirmContent = model->index(row, 11).data().toString();

    //弹出输入确认信息,为空说明单击了取消
    confirmContent = QUIHelper::showInputBox("请输入确认信息", 0, 0, "", false, confirmContent);
    if (!confirmContent.isEmpty()) {
        DBHelper::confirmAlarmLog(logID, confirmContent);
        on_btnSelect_clicked();
    }
}
