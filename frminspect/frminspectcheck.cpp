#include "frminspectcheck.h"
#include "ui_frminspectcheck.h"
#include "dbinspect.h"
#include "inspect_flow.h"
#include "gpibctl.h"
#include "wordapi.h"
#include <QDateTime>
#include<QDir>
#include<QDebug>


frmInspectCheck::frmInspectCheck(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmInspectCheck)
{
    ui->setupUi(this);
    this->initData();
}

frmInspectCheck::~frmInspectCheck()
{
    this->uninitData();
    delete ui;
}

void frmInspectCheck::initData()
{
    /* 初始化combobox */
    QStringList strList;
    const QString filePath = QCoreApplication::applicationDirPath();
    QString fileName_db_instruments = filePath + "/db/instruments.db";
    DBInspect dbInspect_instruments;
    QStringList tables;
    QStringList all_columns;
    int tables_count;

    /* 打开数据库 */
    bool open_instruments_db;

    open_instruments_db = dbInspect_instruments.open_database(fileName_db_instruments);

    if(open_instruments_db == false)
    {
        /* 关闭数据库 */
        dbInspect_instruments.close_database();
        qDebug() << "Open databas fail!";
    }

    /* 获得仪表数据库文件中的table和数量 */
    dbInspect_instruments.get_tables(&tables, &tables_count);
    QStringListIterator itr_tables(tables);

    /* 遍历表，根据表内容添加支持的仪表 */
    all_columns.clear();
    while (itr_tables.hasNext())
    {
        QStringList columns;
        QString table_name = itr_tables.next().toLocal8Bit();

        dbInspect_instruments.get_column_content(table_name, "仪表型号", &columns);

        all_columns << columns;
    }

    ui->comboBox_check_instrument->addItems(all_columns);

    strList.clear();
    strList<<"方案一"<<"方案二"<<"方案三";
    ui->comboBox_check_schem->addItems(strList);
}

void frmInspectCheck::uninitData()
{

}

void frmInspectCheck::on_pushButton_check_check_clicked()
{
    const QString filePath = QCoreApplication::applicationDirPath();
    QString current_Date_Time = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz");
    QString fileName_db_schem = filePath + "/db/inspect_schem.db";
    QString fileName_db_result = filePath + "/db/instruments-" + current_Date_Time +".db";

    inspect_follow_schem(fileName_db_schem, fileName_db_result);
}

static void save_inspect_result_to_word()
{
    const QString filePath = QCoreApplication::applicationDirPath();
    QString current_Date_Time = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz");
    QString fileName_last = "数字表校准原始记录格式" + current_Date_Time;
    QString fileName;

    qDebug() << __FUNCTION__;

    QDir dirReportPath(filePath);
    if (!dirReportPath.exists())
    {
        if (dirReportPath.mkpath(filePath))
        {
            fileName = filePath + "/doc/" + fileName_last + ".doc";
        }
    }
    else
    {
        fileName = filePath + "/doc/" + fileName_last + ".doc";
    }

    qDebug() << fileName;

    WordApi wordApi;
    if( !wordApi.createNewWord(fileName) )
    {
        qDebug() << "Failed to export report," + wordApi.getStrErrorInfo();
        return;
    }

    wordApi.setPageOrientation(0);			//页面方向
    wordApi.setWordPageView(3);			    //页面视图
    wordApi.setParagraphAlignment(0);		//下面文字位置
    wordApi.setFontSize(20);				    //字体大小

    wordApi.setFontBold(true);				    //字体加粗
    wordApi.insertText("数字表校准原始记录格式");

    wordApi.setFontBold(false);
    wordApi.insertMoveDown();
    wordApi.setFontSize(10);
    wordApi.setParagraphAlignment(1);

    QString current_Time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    wordApi.insertText("Report Generation Date:");
    wordApi.insertText(current_Time);
    wordApi.insertMoveDown();

    //第一个表
    wordApi.intsertTable(5,3);
    wordApi.setTableAutoFitBehavior(0);

    //合并单元格一 二 四行
    wordApi.MergeCells(1, 1, 1, 1, 2);
    wordApi.MergeCells(1, 2, 1, 2, 2);
    wordApi.MergeCells(1, 4, 1, 4, 2);

    wordApi.setCellString(1,1,1, "委托单位：");
    wordApi.setCellString(1,1,2, "校准证书编号：");
    wordApi.setCellString(1,2,1, "委托单位地址：");
    wordApi.setCellString(1,2,2, "标准依据：");
    wordApi.setCellString(1,3,1, "仪器名称：");
    wordApi.setCellString(1,3,2, "型号规格：");
    wordApi.setCellString(1,3,3, "出厂编号：");
    wordApi.setCellString(1,4,1, "制造单位：");
    wordApi.setCellString(1,4,2, "仪器状况：");
    wordApi.setCellString(1,5,1, "校准地点：");
    wordApi.setCellString(1,5,2, "环境温度：");
    wordApi.setCellString(1,5,3, "相对湿度：");
    wordApi.moveForEnd();
    wordApi.insertMoveDown();
    wordApi.insertMoveDown();

    wordApi.insertText("校准用主要计量标准器具");
    wordApi.insertMoveDown();

    //第二个表
    wordApi.intsertTable(4,6);
    wordApi.setCellString(2,1,1, "名称");
    wordApi.setCellString(2,1,2, "型号规格");
    wordApi.setCellString(2,1,3, "不确定度／准确度等级／最大允许误差");
    wordApi.setCellString(2,1,4, "出厂编号");
    wordApi.setCellString(2,1,5, "证书编号");
    wordApi.setCellString(2,1,6, "有效期");
    wordApi.setTableAutoFitBehavior(0);
    wordApi.moveForEnd();
    wordApi.insertMoveDown();

    wordApi.setVisible(true);
    wordApi.saveAs();
}

void frmInspectCheck::on_pushButton_check_print_clicked()
{
    save_inspect_result_to_word();
}
