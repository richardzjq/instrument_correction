#include "dbinspect.h"
#include "quiwidget.h"
#include <QDebug>

#define loadfromdb true

DBInspect::DBInspect()
{
    m_db = nullptr;
}

DBInspect::~DBInspect()
{
    m_db = nullptr;
}

/* 打开数据库 */
bool DBInspect::open_database(QString database_name)
{
    //qDebug() << database_name;

    m_db= new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", database_name));
    if(nullptr == m_db)
    {
        qDebug() <<"new database fail!";
        return false;
    }
    m_db->setDatabaseName(database_name);

    if(true != m_db->open())
    {  
        qDebug() << m_db->lastError();
        qFatal("failed to connect.");

        return false;
    }

    //qDebug() << __FUNCTION__ << *m_db;
    return true;
}

void DBInspect::close_database(void)
{
    if(nullptr != m_db)
    {
        m_db->close();
        delete m_db;
    }
}

QSqlDatabase* DBInspect::get_database(void)
{
    return m_db;
}

/* 获得一个数据库文件中的table和数量 */
void DBInspect::get_tables(QStringList* p_tables, int* p_tables_count)
{
    if(nullptr == m_db)
    {
        qDebug() <<"database is not set!";
        return ;
    }

    if(!p_tables || !p_tables_count)
	{
         qDebug() << "传入空指针";
         return ;
	}
	 
    *p_tables       = m_db->tables();  //获取数据库中的table
    *p_tables_count = p_tables->count();

    qDebug() << __FUNCTION__ << *m_db;
    qDebug() << *p_tables;
    //qDebug() << *p_tables_count;
}

/* 获得一个数据库文件中table中的字段和数量 */
void DBInspect::get_columns(QString tab_name, QStringList* p_columns, int* p_columns_count)
{
    if(nullptr == m_db)
    {
        qDebug() <<"database is not set!";
        return ;
    }

    QSqlQuery query(*m_db);
	QString str = "PRAGMA table_info(" + tab_name + ")";        
	query.prepare(str);   

    //qDebug() << __FUNCTION__ << *m_db;
    //qDebug() << str;
	
    if(!p_columns || !p_columns_count)
	{
         qDebug() << "传入空指针";
         return ;
	}
	
	if (query.exec())
	{        
        while (query.next())
		{            
            *p_columns << QString(QString::fromLocal8Bit("%2")).arg(query.value(1).toString());
		}
        //qDebug() << *p_columns;
		
		*p_columns_count = p_columns->count(); 
        //qDebug() << *p_columns_count;
	}    
	else
	{                
		*p_columns_count = 0;
        qDebug() << query.lastError();       
	}
}

/* 查询一个表中的所有记录 */
void DBInspect::get_table_content(QString tab_name, QStringList* p_table_content)
{
    if(nullptr == m_db)
    {
        qDebug() <<"database is not set!";
        return ;
    }

    QSqlQuery query(*m_db);
    QString str = "select * from " + tab_name;
    qDebug() << __FUNCTION__ << *m_db;
    qDebug() << str;

	query.prepare(str);   
	
    if(!p_table_content)
	{
         qDebug() << "传入空指针";
         return ;
	}
	
	if (query.exec())
	{   
        QSqlRecord rec = query.record();
		
        int line_count = rec.count();
        qDebug() << line_count;
        int line;
    
        //for(line = 0; line < line_count; line++)
        //   qDebug() << QString( "line %1: %2" ).arg(line).arg( rec.fieldName(line) );
      
        while(query.next())
		{
			QString line_content;
			
			line_content.clear();
            for(line = 0; line < line_count; line++)
			{
                line_content += query.value(line).toString() + ",";
	        }
			
            qDebug() << line_content;
			*p_table_content << line_content;
		}
        //qDebug() << *p_table_content;
	}
	else    
	{        
        qDebug() << query.lastError();          
	}	
}

/* 查询一个表中某一字段的所有记录 */
void DBInspect::get_column_content(QString tab_name, QString col_name, QStringList* p_col_content)
{
    if(nullptr == m_db)
    {
        qDebug() <<"database is not set!";
        return ;
    }

    QSqlQuery query(*m_db);
    QString str = "select " + col_name + " from " + tab_name;
    qDebug() << __FUNCTION__ << *m_db;
    qDebug() << str;

    query.prepare(str);

    if(!p_col_content)
    {
         qDebug() << "传入空指针";
         return ;
    }

    if (query.exec())
    {
        QSqlRecord rec = query.record();

        int line_count = rec.count();
        qDebug() << line_count;
        int line;

        //for(line = 0; line < line_count; line++)
        //   qDebug() << QString( "line %1: %2" ).arg(line).arg( rec.fieldName(line) );

        while(query.next())
        {
            QString line_content;

            line_content.clear();
            for(line = 0; line < line_count; line++)
            {
                line_content += query.value(line).toString();
            }

            qDebug() << line_content;
            *p_col_content << line_content;
        }
        qDebug() << *p_col_content;
    }
    else
    {
        qDebug() << query.lastError();
    }
}

/* 查询一个表中某些字段的所有记录 */
void DBInspect::get_columns_content(QString tab_name, QString cols_name, QStringList* p_col_content)
{
    if(nullptr == m_db)
    {
        qDebug() <<"database is not set!";
        return ;
    }

    QSqlQuery query(*m_db);
    QString str = "select " + cols_name + " from " + tab_name;
    qDebug() << __FUNCTION__ << *m_db;
    qDebug() << str;

    query.prepare(str);

    if(!p_col_content)
    {
         qDebug() << "传入空指针";
         return ;
    }

    if (query.exec())
    {
        QSqlRecord rec = query.record();

        int line_count = rec.count();
        qDebug() << line_count;
        int line;

        //for(line = 0; line < line_count; line++)
        //   qDebug() << QString( "line %1: %2" ).arg(line).arg( rec.fieldName(line) );

        while(query.next())
        {
            QString line_content;

            line_content.clear();
            for(line = 0; line < line_count; line++)
            {
                line_content += query.value(line).toString();
            }

            qDebug() << line_content;
            *p_col_content << line_content;
        }
        qDebug() << *p_col_content;
    }
    else
    {
        qDebug() << query.lastError();
    }
}

/* 根据条件查询一个表中的记录 */
void DBInspect::get_table_content_by_condition(QString select_rule, QStringList* p_col_content)
{
    if(nullptr == m_db)
    {
        qDebug() <<"database is not set!";
        return ;
    }

    QSqlQuery query(*m_db);
    qDebug() << __FUNCTION__ << *m_db;
    qDebug() << select_rule;

    query.prepare(select_rule);

    if(!p_col_content)
    {
         qDebug() << "传入空指针";
         return ;
    }

    if (query.exec())
    {
        QSqlRecord rec = query.record();

        int line_count = rec.count();
        qDebug() << line_count;
        int line;

        //for(line = 0; line < line_count; line++)
        //   qDebug() << QString( "line %1: %2" ).arg(line).arg( rec.fieldName(line) );

        while(query.next())
        {
            QString line_content;

            line_content.clear();
            for(line = 0; line < line_count; line++)
            {
                line_content += query.value(line).toString() + ",";
            }

            qDebug() << line_content;
            *p_col_content << line_content;
        }
        qDebug() << *p_col_content;
    }
    else
    {
        qDebug() << query.lastError();
    }
}

/* 向数据库中加入一个table */
void DBInspect::add_one_table(QString tab_name, QString columns)
{
    if(nullptr == m_db)
    {
        qDebug() <<"database is not set!";
        return ;
    }

    QSqlQuery query(*m_db);
    QString str = "create table " + tab_name + " (" + columns + ")";
    //qDebug() << __FUNCTION__ << *m_db;
    //qDebug() << str;
	query.prepare(str);   
	
	qDebug() << str;
	
	if (!query.exec())
	{                
        qDebug() << query.lastError();       
	}
}

/* 从数据库中删除一个table */
void DBInspect::delete_one_table(QString tab_name)
{
    if(nullptr == m_db)
    {
        qDebug() <<"database is not set!";
        return ;
    }

    QSqlQuery query(*m_db);
    QString str = "drop table " + tab_name;
    //qDebug() << __FUNCTION__ << *m_db;
    //qDebug() << str;
    query.prepare(str);

    qDebug() << str;

    if (!query.exec())
    {
        qDebug() << query.lastError();
    }
}

/* 向数据库table加入一行内容 */
void DBInspect::add_one_line_into_table(QString add_line_instruction)
{
    if(nullptr == m_db)
    {
        qDebug() <<"database is not set!";
        return ;
    }

    QSqlQuery query(*m_db);
    query.prepare(add_line_instruction);
    qDebug() << __FUNCTION__ << *m_db;
    qDebug() << add_line_instruction;
	
	if (!query.exec())
	{                
        qDebug() << query.lastError();       
	}
}

/* 从数据库table删除一行内容 */
void DBInspect::delete_one_line_into_table(QString delete_line_instruction)
{
    if(nullptr == m_db)
    {
        qDebug() <<"database is not set!";
        return ;
    }

    QSqlQuery query(*m_db);
    query.prepare(delete_line_instruction);
    qDebug() << __FUNCTION__ << *m_db;
    qDebug() << delete_line_instruction;

    if (!query.exec())
    {
        qDebug() << query.lastError();
    }
}
