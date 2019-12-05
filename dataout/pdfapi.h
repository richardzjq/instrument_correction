#ifndef PDFAPI_H
#define PDFAPI_H

#include <QObject>
#include <QStringList>

#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtPrintSupport>
#else
#include <QPrinter>
#endif

class PdfAPI : public QObject
{
    Q_OBJECT
public:
    static PdfAPI *Instance();
    explicit PdfAPI(QObject *parent = 0);

private:
    static QScopedPointer<PdfAPI> self;
    QStringList html;

public slots:
    //导出数据到pdf
    void savePdf(const QString &fileName, const QString &title, const QString &subTitle,
                 const QList<QString> &columnNames, const QList<int> &columnWidths,
                 const QStringList &content, bool landscape = false, bool check = false,
                 int checkColumn = 0, const QString &checkType = "==", const QString &checkValue = "0",
                 const QPrinter::PageSize &pageSize = QPrinter::A4);

    //导出数据到pdf,分段形式,带子标题
    void savePdf(const QString &fileName, const QString &title,
                 const QList<QString> &columnNames, const QList<int> &columnWidths,
                 const QStringList &subTitle1, const QStringList &subTitle2,
                 const QStringList &content, bool landscape = false, bool check = false,
                 int checkColumn = 0, const QString &checkType = "==", const QString &checkValue = "0",
                 const QPrinter::PageSize &pageSize = QPrinter::A4);
};

#endif // PDFAPI_H
