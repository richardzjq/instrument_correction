#ifndef DBINSPECT_H
#define DBINSPECT_H

#include "quiwidget.h"
#include "buttondefence.h"

class DBInspect : public QObject
{
    Q_OBJECT
public:
    explicit DBInspect(QObject *parent = 0);
    ~DBInspect();

    /* 打开数据库 */
    bool open_database(QString database_name);

    /* 返回数据库对象成员 */
    QSqlDatabase* get_database(void);

    /* 关闭数据库 */
    void close_database(void);
	
    /* 获得一个数据库文件中的table和数量 */
    void get_tables(QStringList* p_tables, int* p_tables_count);

    /* 获得一个数据库文件中table中的字段和数量 */
    void get_columns(QString tab_name, QStringList* p_columns, int* p_columns_count);
	
    /* 查询一个表中的所有记录 */
    void get_table_content(QString tab_name, QStringList* p_table_content);

    /* 查询一个表中某一字段的所有记录 */
    void get_column_content(QString tab_name, QString col_name, QStringList* p_col_content);

    /* 根据条件查询一个表中某一字段的记录 */
    void get_table_content_by_condition(QString select_rule, QStringList* p_col_content);

    /* 向数据库中加入一个table */
    void add_one_table(QString tab_name, QString columns);

    /* 向数据库table加入一行内容 */
    void add_one_line_into_table(QString add_line_instruction);

    /* 从数据库table删除一行内容 */
    void delete_one_line_into_table(QString delete_line_instruction);

private:

    QSqlDatabase* m_db;


};

#endif // DBINSPECT_H
