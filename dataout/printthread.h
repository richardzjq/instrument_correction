#ifndef PRINTTHREAD_H
#define PRINTTHREAD_H

/**
 * 导出数据到pdf及打印数据 作者:feiyangqingyun(QQ:517216493) 2017-2-18
 * 原理:写入table格式的数据
 * 1:多线程写入数据
 * 2:可设置标题/副标题/表名
 * 3:可设置字段名称和列宽度
 * 4:可设置是否启用校验过滤数据,校验的列,校验规则和校验值
 * 5:可分组输出数据
 * 6:可设置行内容分隔符
 * 7:可追加数据形式写入数据,建议每次追加的数据小于1000条
 * 8:可直接设置表格数据,一次性写入
 * 9:灵活性超高,可自由更改源码设置对齐方式/文字颜色/背景颜色等
 * 10:打印数据目前不支持多线程
 */

#include <QThread>
#include <QMutex>
#include <QTextDocument>
#include <QTextBlock>
#include <QTime>
#include <QStringList>

#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtPrintSupport>
#else
#include <QPrinter>
#endif

class PrintThread : public QThread
{
    Q_OBJECT
public:
    static PrintThread *Instance();
    explicit PrintThread(QObject *parent = 0);    

protected:
    void run();

private:
    static QScopedPointer<PrintThread> self;

    volatile bool stopped;      //标志位用于停止线程
    int takeTime;               //执行用时
    QTime time;                 //计算用时对象

    QPrinter printer;           //打印对象
    QTextDocument textDocument; //文档对象

    QStringList html;           //构建html字符串
    QString fileName;           //文件名称

    QString title;              //主标题
    QString subTitle;           //副标题

    QStringList subTitle1;      //子标题1集合
    QStringList subTitle2;      //子标题2集合
    QStringList content;        //行内容集合
    QString separator;          //行内容分隔符
    QString subSeparator;       //子内容分隔符

    QList<QString> columnNames; //字段名称集合
    QList<int> columnWidths;    //字段宽度集合

    int borderWidth;            //边框宽度
    int checkColumnIndex;       //校验列的索引,为-1则不校验
    QString checkType;          //校验规则,例如>=
    QString checkValue;         //检验值

    //纸张方向,横向纵向
    QPrinter::Orientation orientation;
    //纸张大小 A4 A3
    QPrinter::PageSize pageSize;

public:
    int getTakeTime() const;    //获取执行所用时间

private slots:
    void appendHead();          //输出头部信息
    void appendTitle();         //输出标题信息
    void appendColumn();        //输出字段名称
    void appendEnd();           //输出结尾信息

    //输出当前队列内容
    void appendContent(const QStringList &listContent);

    //核心处理
    void take();

    //打印预览
    void printView(QPrinter *printer);

public slots:
    //设置文件名称
    void setFileName(const QString &fileName);
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

    //设置边框宽度
    void setBorderWidth(int borderWidth);
    //设置需要检验列的索引
    void setCheckColumnIndex(int checkColumnIndex);
    //设置校验规则
    void setCheckType(const QString &checkType);
    //设置校验内容
    void setCheckValue(const QString &checkValue);

    //设置纸张方向 横向纵向
    void setOrientation(const QPrinter::Orientation &orientation);
    //设置纸张大小 A4 A3
    void setPageSize(const QPrinter::PageSize &pageSize);

    //初始化
    void init();

    //打开文件准备输出数据
    void open();

    //暂停线程
    void stop();

    //打印到纸张
    void print();

    //追加子标题1
    void appendSubTitle1(const QString &str);
    //追加子标题2
    void appendSubTitle2(const QString &str);
    //追加行数据
    void appendContent(const QString &str);

};

#endif // PRINTTHREAD_H
