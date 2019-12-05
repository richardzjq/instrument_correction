#ifndef EXCELTHREAD_H
#define EXCELTHREAD_H

/**
 * 导出数据到Excel 作者:feiyangqingyun(QQ:517216493) 2017-2-17
 * 原理:写入xml格式的数据,10万行数据9个字段只需要2秒钟完成
 * 只需要四个步骤即可开始急速导出大量数据到Excel
 * 第一步:设置文件路径,标题等信息; 第二步:打开文件,启动线程; 第三步:逐行输入数据; 第四步:关闭文件,关闭线程;
 * 1:多线程写入数据
 * 2:可设置标题/副标题/表名
 * 3:可设置字段名称和列宽度
 * 4:可设置是否启用校验过滤数据,校验的列,校验规则和校验值
 * 5:可设置随机背景颜色及需要随机背景色的列集合
 * 6:可分组输出数据
 * 7:可设置行内容分隔符
 * 8:可追加数据形式写入数据,建议每次追加的数据小于10000条
 * 9:可直接设置表格数据,一次性写入
 * 10:灵活性超高,可自由更改源码设置对齐方式/文字颜色/背景颜色等
 * 11:支持任意excel表格软件,包括但不限于excel2003/2007/2010/2013 wps openoffice
 */

#include <QThread>
#include <QMutex>
#include <QFile>
#include <QTextStream>
#include <QTime>
#include <QStringList>

class ExcelThread : public QThread
{
    Q_OBJECT
public:
    static ExcelThread *Instance();
    explicit ExcelThread(QObject *parent = 0);    

protected:
    void run();

private:
    static QScopedPointer<ExcelThread> self;

    volatile bool stopped;      //标志位用于停止线程
    int takeTime;               //执行用时
    QTime time;                 //计算用时对象

    QFile file;                 //文件对象
    QTextStream stream;         //数据流对象

    QString fileName;           //文件名称
    QString sheetName;          //表名称

    QString title;              //主标题
    QString subTitle;           //副标题

    QStringList subTitle1;      //子标题1集合
    QStringList subTitle2;      //子标题2集合
    QStringList content;        //行内容集合
    QString separator;          //行内容分隔符
    QString subSeparator;       //子内容分隔符

    QList<QString> columnNames; //字段名称集合
    QList<int> columnWidths;    //字段宽度集合

    bool randomColor;           //行背景随机颜色
    QList<int> colorColumn;     //随机颜色列索引集合

    int borderWidth;            //边框宽度
    int checkColumnIndex;       //校验列的索引,为-1则不校验
    QString checkType;          //校验规则,例如>=
    QString checkValue;         //检验值

public:
    int getTakeTime() const;    //获取执行所用时间

private slots:
    void appendHead();          //输出头部信息
    void appendDocument();      //输出文档信息
    void appendBorder();        //输出边框样式
    void appendStyle();         //输出样式信息
    void appendTable();         //输出表结构
    void appendColumn();        //输出字段名称
    void appendEnd();           //输出结尾信息

    //输出当前队列内容
    void appendContent(const QStringList &listContent);

    //核心处理
    void take();

public slots:
    //设置文件名称
    void setFileName(const QString &fileName);
    //设置表名
    void setSheetName(const QString &sheetName);

    //设置主标题
    void setTitle(const QString &title);
    //设置副标题
    void setSubTitle(const QString &subTitle);

    //设置子标题1集合
    void setSubTitle1(const QStringList &subTitle1);
    //设置子标题2集合
    void setSubTitle2(const QStringList &subTitle2);
    //设置行内容集合
    void setContent(const QStringList &content);
    //设置行内容分隔符
    void setSeparator(const QString &separator);
    //设置子行内容分隔符
    void setSubSeparator(const QString &subSeparator);

    //设置字段名称集合
    void setColumnNames(const QList<QString> &columnNames);
    //设置字段宽度集合
    void setColumnWidths(const QList<int> &columnWidths);

    //设置行背景随机颜色
    void setRandomColor(bool randomColor);
    //设置随机背景颜色的列索引集合
    void setColorColumn(const QList<int> &colorColumn);

    //设置边框宽度
    void setBorderWidth(int borderWidth);
    //设置需要检验列的索引
    void setCheckColumnIndex(int checkColumnIndex);
    //设置校验规则
    void setCheckType(const QString &checkType);
    //设置校验内容
    void setCheckValue(const QString &checkValue);

    //初始化
    void init();

    //打开文件准备输出数据
    void open();

    //关闭文件
    void close();

    //暂停线程
    void stop();

    //追加子标题1
    void appendSubTitle1(const QString &str);
    //追加子标题2
    void appendSubTitle2(const QString &str);
    //追加行数据
    void appendContent(const QString &str);
};

#endif // EXCELTHREAD_H
