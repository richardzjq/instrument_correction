#ifndef PRINTAPI_H
#define PRINTAPI_H

#include <QObject>
#include <QStringList>

#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtPrintSupport>
#else
#include <QPrinter>
#endif

class PrintAPI : public QObject
{
	Q_OBJECT
public:
    static PrintAPI *Instance();
    explicit PrintAPI(QObject *parent = 0);    

private:
    static QScopedPointer<PrintAPI> self;
	QStringList html;

private slots:
	void printView(QPrinter *printer);

public slots:
    //打印数据
    void print(const QString &title, const QString &subTitle,
               const QList<QString> &columnNames, const QList<int> &columnWidths,
               const QStringList &content, bool landscape = false, bool check = false,
               int checkColumn = 0, const QString &checkType = "==", const QString &checkValue = "0",
               const QPrinter::PageSize &pageSize = QPrinter::A4);

    //打印数据,分段形式,带子标题
    void print(const QString &title, const QList<QString> &columnNames, const QList<int> &columnWidths,
               const QStringList &subTitle1, const QStringList &subTitle2,
               const QStringList &content, bool landscape = false, bool check = false,
               int checkColumn = 0, const QString &checkType = "==", const QString &checkValue = "0",
               const QPrinter::PageSize &pageSize = QPrinter::A4);

};

#endif // PRINTAPI_H
