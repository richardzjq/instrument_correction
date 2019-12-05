#ifndef API_H
#define API_H

#include <QObject>

class QTableView;

class API : public QObject
{
    Q_OBJECT
public:
    explicit API(QObject *parent = 0);

    static void reboot();

    //初始化表格
    static void initTableView(QTableView *tableView);
    static void initTableView2(QTableView *tableView);

    //获取保存文件名,传入默认文件名和过滤符
    static QString getSaveFileNames(const QString &defaultName, const QString &filter);
    static QString getFileName(const QString &filter, const QString &defaultDir = "", const QString &fileName = "");
    static QString getDefaultDir();

};

#endif // API_H
