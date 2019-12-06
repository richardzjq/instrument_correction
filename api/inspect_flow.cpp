#include "inspect_flow.h"
#include "inspect_algorithm.h"
#include "dbinspect.h"
#include "qextserialport.h"
#include "gpibctl.h"
#include <QtGlobal>
#include <cmath>
#include <QDebug>


static bool isComOk;
static QextSerialPort *com;
//static QTimer *timerRead;
//static QTimer *timerData;

static ViPSession pViSession;


static void initData()
{
    /* 打开GPIB */
    ViStatus status;
    ViSession viSes;

    status = AutoConnectGPIB(&viSes);
    pViSession = &viSes;

    /* 打开串口 */
    isComOk = false;
    // timerRead = new QTimer();
    //timerRead->setInterval(100);
    //connect(timerRead, SIGNAL(timeout()), this, SLOT(readDataCom()));

    //timerData = new QTimer();
    //timerData->setInterval(1000);
    //connect(timerData, SIGNAL(timeout()), this, SLOT(loadData()));

    com = new QextSerialPort("com1", QextSerialPort::Polling);
    isComOk = com->open(QIODevice::ReadWrite);
    if (isComOk) {
        com->setBaudRate(BAUD115200);
        com->setFlowControl(FLOW_OFF);
        com->setTimeout(10);

        //timerRead->start();
        //timerData->start();
    }
}

static void uninitData()
{
    /* 关闭串口 */
    if(com)
    {
        com->close();
        com->deleteLater();
        isComOk = false;
        delete com;
        com = nullptr;
    }
    //timerData->stop();
    //timerRead->stop();

    /* 关闭GPIB */
    if(pViSession)
    {
        CloseConnectGPIB(pViSession);
        delete pViSession;
        pViSession = nullptr;
    }
}

/**
 * 设置标准源，set_type为直流电压，直流电流，交流电压，交流电流，电阻
 */
static void set_standard_source(int set_type, double set_val)
{
    set_type = set_type;
    set_val = set_val;
}

/**
 * 获取多用表输出值，set_type为直流电压，直流电流，交流电压，交流电流，电阻
 */
static void get_instrument_value(int get_type, double* p_get_val)
{
    QByteArray bytes;
    get_type = get_type;
	*p_get_val = 10;

    bytes = com->readAll();

    /* parse byte array*/
}

/**
 * 从一个db文件中读取测试方案，按照设置进行检定
 */
void inspect_follow_schem(QString schem_db_name, QString instrument_db_name)
{
    DBInspect dbInspect_schem;
    DBInspect dbInspect_result;

	QStringList tables;
	int tables_count;

    /* 打开数据库 */
    bool open_schem_db, open_result_db;

    open_schem_db = dbInspect_schem.open_database(schem_db_name);
    open_result_db = dbInspect_result.open_database(instrument_db_name);

    if(open_schem_db == false || open_result_db == false)
    {
        /* 关闭数据库 */
        dbInspect_schem.close_database();
        dbInspect_result.close_database();

        qDebug() << "Open databas fail!";
    }

	/* 获得检定方案数据库文件中的table和数量 */
    dbInspect_schem.get_tables(&tables, &tables_count);
	QStringListIterator itr_tables(tables);
	
	/* 遍历所有表，根据表内容检定 */
	while (itr_tables.hasNext())        
	{            
        QString table_name = itr_tables.next().toLocal8Bit();
		qDebug() << QString::fromLocal8Bit("表名：") + table_name;
		
		/* 向检定结果数据库中加入一个table */
        QString columns = "量程 real, 标准值 real, 下限值 real, 显示值 real, 上限值 real, 扩展不确定度 real";
        dbInspect_result.add_one_table(table_name, columns);

		QStringList table_content;
		
	    /* 查询一个表中的所有记录 */
        dbInspect_schem.get_table_content(table_name, &table_content);
	    QStringListIterator itr_table_content(table_content);
		
		/* 根据表中内容，进行检定 */
	    while (itr_table_content.hasNext())        
	    {            
            QString line_content = itr_table_content.next().toLocal8Bit();
	        qDebug() << line_content;
			
            /* 解析出表中一行各个字段的值，量程-标准值-下限值-显示值-上限值-扩展不确定度 */
            double range_val               = line_content.section('-', 0, 0).toDouble();
            double standard_val            = line_content.section('-', 1, 1).toDouble();
            double min_val                 = line_content.section('-', 2, 2).toDouble();
            double show_val                = line_content.section('-', 3, 3).toDouble();
            double max_val                 = line_content.section('-', 4, 4).toDouble();
            double extend_uncertainty_val  = line_content.section('-', 5, 5).toDouble();
		
            qDebug() << "range_val " << range_val;
            qDebug() << "standard_val " << standard_val;
            qDebug() << "min_val " << min_val;
            qDebug() << "show_val " << show_val;
            qDebug() << "max_val " << max_val;
            qDebug() << "extend_uncertainty_val " << extend_uncertainty_val;
		
            double check_val[CHECK_TIMES] = {0};
		
		    for(int check_count = 0; check_count < CHECK_TIMES; check_count++)
		    {
                set_standard_source(0, standard_val);
                get_instrument_value(0, &check_val[check_count]);
		    }
       
            //double average_value               = average(check_val, CHECK_TIMES);
            double average_value               = standard_val;
            double standard_deviation_value    = standard_deviation(check_val, CHECK_TIMES);
		    double max_allowable_error         = 0.0000001;
            double standard_uncertainty_value  = standard_uncertainty(max_allowable_error);
            double synthesis_uncertainty_value = synthesis_uncertainty(standard_deviation_value, standard_uncertainty_value);
            double extend_uncertainty_value    = extend_uncertainty(synthesis_uncertainty_value);
			
			/* 向检定结果数据库table中加入一行 */
            QString s_range_value = QString::number(range_val, 'g', 10);
            QString s_standard_value = QString::number(standard_val, 'g', 10);
            QString s_min_value = QString::number(min_val, 'g', 10);
            QString s_show_value = QString::number(average_value, 'g', 10);
            QString s_max_value = QString::number(max_val, 'g', 10);
            QString s_extend_uncertainty_value  = QString::number(extend_uncertainty_value, 'g', 10);
			QString add_line_instruction = "INSERT INTO " + table_name + " (量程, 标准值, 下限值, 显示值, 上限值, 扩展不确定度) VALUES (" 
                                           + s_range_value + ", " + s_standard_value  + ", " + s_min_value + ", " + s_show_value + ", " +  s_max_value + ", " + s_extend_uncertainty_value + ")";
			
            dbInspect_result.add_one_line_into_table(add_line_instruction);
	    }

    }
	
    /* 关闭数据库 */
    dbInspect_schem.close_database();
    dbInspect_result.close_database();
}

