#include "printthread.h"
#include "qapplication.h"
#include "qprintpreviewdialog.h"
#include "qdatetime.h"
#include "qstringlist.h"
#include "qdebug.h"

QScopedPointer<PrintThread> PrintThread::self;
PrintThread *PrintThread::Instance()
{
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new PrintThread);
        }
    }

    return self.data();
}

PrintThread::PrintThread(QObject *parent) : QThread(parent)
{    
    init();
}

void PrintThread::run()
{
    takeTime = 0;
    time.restart();

    while (!stopped) {
        take();
        msleep(10);
    }

    printView(&printer);

    takeTime = time.elapsed();
    stopped = false;
}

int PrintThread::getTakeTime() const
{
    return this->takeTime;
}

void PrintThread::appendHead()
{
    //清空原有数据,确保每次都是新的数据
    html.clear();

    //表格开始
    html.append("<table border='0.5' cellspacing='0' cellpadding='3'>");
}

void PrintThread::appendTitle()
{
    int columnCount = columnNames.count();

    //标题占一行,居中显示
    if (!title.isEmpty()) {
        html.append(QString("<tr><td align='center' style='vertical-align:middle;font-weight:bold;' colspan='%1'>").arg(columnCount));
        html.append(title);
        html.append("</td></tr>");
    }

    //副标题占一行,左对齐显示
    if (!subTitle.isEmpty()) {
        html.append(QString("<tr><td align='left' style='vertical-align:middle;' colspan='%1'>").arg(columnCount));
        html.append(subTitle);
        html.append("</td></tr>");
    }
}

void PrintThread::appendColumn()
{
    int columnCount = columnNames.count();

    //循环写入字段名,字段名占一行,居中显示
    if (columnCount > 0) {
        html.append("<tr>");

        for (int i = 0; i < columnCount; i++) {
            html.append(QString("<td width='%1' bgcolor='lightgray' align='center' style='vertical-align:middle;'>").arg(columnWidths.at(i)));
            html.append(columnNames.at(i));
            html.append("</td>");
        }

        html.append("</tr>");
    }
}

void PrintThread::appendEnd()
{
    html.append("</table>");
}

void PrintThread::appendContent(const QStringList &listContent)
{
    //循环一行行构建数据
    int rowCount = listContent.count();
    for (int i = 0; i < rowCount; i++) {
        html.append("<tr>");

        QString temp = listContent.at(i);
        QStringList value = temp.split(separator);
        int valueCount = value.count();

        //如果启用了过滤数据,则将符合条件的数据突出颜色显示
        bool existCheck = false;

        if (checkColumnIndex >= 0) {
            if (checkType == "==") {
                if (value.at(checkColumnIndex) == checkValue) {
                    existCheck = true;
                }
            } else if (checkType == ">") {
                if (value.at(checkColumnIndex) > checkValue) {
                    existCheck = true;
                }
            } else if (checkType == ">=") {
                if (value.at(checkColumnIndex) >= checkValue) {
                    existCheck = true;
                }
            } else if (checkType == "<") {
                if (value.at(checkColumnIndex) < checkValue) {
                    existCheck = true;
                }
            } else if (checkType == "<=") {
                if (value.at(checkColumnIndex) <= checkValue) {
                    existCheck = true;
                }
            } else if (checkType == "!=") {
                if (value.at(checkColumnIndex) != checkValue) {
                    existCheck = true;
                }
            }
        }

        if (existCheck) {
            for (int j = 0; j < valueCount; j++) {
                html.append(QString("<td width='%1' align='center' style='vertical-align:middle;'><font color='red'>").arg(columnWidths.at(j)));
                html.append(value.at(j));
                html.append("</font></td>");
            }
        } else {
            for (int j = 0; j < valueCount; j++) {
                html.append(QString("<td width='%1' align='center' style='vertical-align:middle;'>").arg(columnWidths.at(j)));
                html.append(value.at(j));
                html.append("</td>");
            }
        }

        html.append("</tr>");
    }
}

void PrintThread::take()
{
    int count = content.count();
    if (count > 0) {
        //取出当前所有行内容
        static QMutex mutex;
        mutex.lock();
        QStringList listContent = content;
        QStringList listSubTitle1 = subTitle1;
        QStringList listSubTitle2 = subTitle2;
        content.clear();
        subTitle1.clear();
        subTitle2.clear();
        mutex.unlock();

        int columnCount = columnNames.count();
        int contentCount = listContent.count();
        int subTitle1Count = listSubTitle1.count();
        int subTitle2Count = listSubTitle2.count();

        if (subTitle1Count > 0 && subTitle2Count > 0 && subTitle1Count == subTitle2Count) {
            //循环添加子标题/字段名/内容
            for (int i = 0; i < contentCount; i++) {
                //加个空行隔开
                html.append(QString("<tr><td colspan='%1'>").arg(columnCount));
                html.append("</td></tr>");

                //子标题1
                if (subTitle1Count > i) {
                    if (!listSubTitle1.at(i).isEmpty()) {
                        html.append(QString("<tr><td align='left' style='vertical-align:middle;' colspan='%1'>").arg(columnCount));
                        html.append(listSubTitle1.at(i));
                        html.append("</td></tr>");
                    }
                }

                //子标题2
                if (subTitle2Count > i) {
                    if (!listSubTitle2.at(i).isEmpty()) {
                        html.append(QString("<tr><td align='left' style='vertical-align:middle;' colspan='%1'>").arg(columnCount));
                        html.append(listSubTitle2.at(i));
                        html.append("</td></tr>");
                    }
                }

                //逐个添加字段名称
                appendColumn();

                //逐行添加该分类下的内容
                appendContent(listContent.at(i).split(subSeparator));
            }
        } else {
            appendContent(listContent);
        }
    }
}

void PrintThread::printView(QPrinter *printer)
{
    //输出结尾信息
    appendEnd();

    textDocument.setHtml(html.join(""));
    textDocument.setPageSize(printer->pageRect().size());
    textDocument.print(printer);
    textDocument.end();

    //清空字符串
    html.clear();
}

void PrintThread::setFileName(const QString &fileName)
{
    if (this->fileName != fileName) {
        this->fileName = fileName;
    }
}

void PrintThread::setTitle(const QString &title)
{
    if (this->title != title) {
        this->title = title;
    }
}

void PrintThread::setSubTitle(const QString &subTitle)
{
    if (this->subTitle != subTitle) {
        this->subTitle = subTitle;
    }
}

void PrintThread::setSubTitle1(const QStringList &subTitle1)
{
    this->subTitle1 = subTitle1;
}

void PrintThread::setSubTitle2(const QStringList &subTitle2)
{
    this->subTitle2 = subTitle2;
}

void PrintThread::setContent(const QStringList &content)
{
    this->content = content;
}

void PrintThread::setSeparator(const QString &separator)
{
    if (this->separator != separator) {
        this->separator = separator;
    }
}

void PrintThread::setSubSeparator(const QString &subSeparator)
{
    if (this->subSeparator != subSeparator) {
        this->subSeparator = subSeparator;
    }
}

void PrintThread::setColumnNames(const QList<QString> &columnNames)
{
    if (this->columnNames != columnNames) {
        this->columnNames = columnNames;
    }
}

void PrintThread::setColumnWidths(const QList<int> &columnWidths)
{
    if (this->columnWidths != columnWidths) {
        this->columnWidths = columnWidths;
    }
}

void PrintThread::setBorderWidth(int borderWidth)
{
    if (this->borderWidth != borderWidth) {
        this->borderWidth = borderWidth;
    }
}

void PrintThread::setCheckColumnIndex(int checkColumnIndex)
{
    if (this->checkColumnIndex != checkColumnIndex) {
        this->checkColumnIndex = checkColumnIndex;
    }
}

void PrintThread::setCheckType(const QString &checkType)
{
    if (this->checkType != checkType) {
        this->checkType = checkType;
    }
}

void PrintThread::setCheckValue(const QString &checkValue)
{
    if (this->checkValue != checkValue) {
        this->checkValue = checkValue;
    }
}

void PrintThread::setOrientation(const QPrinter::Orientation &orientation)
{
    if (this->orientation != orientation) {
        this->orientation = orientation;
    }
}

void PrintThread::setPageSize(const QPrinter::PageSize &pageSize)
{
    if (this->pageSize != pageSize) {
        this->pageSize = pageSize;
    }
}

void PrintThread::init()
{
    stopped = false;

    html.clear();
    fileName = QString("%1.pdf").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    title.clear();
    subTitle.clear();

    subTitle1.clear();
    subTitle2.clear();
    content.clear();
    separator = ";";
    subSeparator = "|";

    columnNames.clear();
    columnWidths.clear();

    borderWidth = 1;
    checkColumnIndex = -1;
    checkType = "==";
    checkValue = "0";

    orientation = QPrinter::Portrait;
    pageSize = QPrinter::A4;
}

void PrintThread::open()
{
    //校验字段名称和宽度,个数不能为0,而且字段名称的个数要和字段宽度的个数一致
    if (columnNames.count() == 0 || columnWidths.count() == 0 || columnNames.count() != columnWidths.count()) {
        return;
    }

    if (fileName.isEmpty()) {
        return;
    }

    textDocument.moveToThread(this);

    //输出固定头部信息
    appendHead();

    //输出标题信息
    appendTitle();

    //输出字段名称
    if (subTitle1.count() == 0) {
        appendColumn();
    }

    //设置纸张规格
    printer.setPageSize(pageSize);

    //设置横向纵向及页边距
    printer.setOrientation(orientation);
    printer.setPageMargins(10, 10, 10, 12, QPrinter::Millimeter);

    //设置输出格式
    printer.setOutputFormat(QPrinter::PdfFormat);
    //设置输出文件保存位置
    printer.setOutputFileName(fileName);
}

void PrintThread::stop()
{
    stopped = true;
}

void PrintThread::print()
{
    //校验字段名称和宽度,个数不能为0,而且字段名称的个数要和字段宽度的个数一致
    if (columnNames.count() == 0 || columnWidths.count() == 0 || columnNames.count() != columnWidths.count()) {
        return;
    }

    if (fileName.isEmpty()) {
        return;
    }

    //输出固定头部信息
    appendHead();

    //输出标题信息
    appendTitle();

    //输出字段名称
    if (subTitle1.count() == 0) {
        appendColumn();
    }

    take();

    //设置纸张规格
    printer.setPageSize(pageSize);

    //设置横向纵向及页边距
    printer.setOrientation(orientation);
    printer.setPageMargins(10, 10, 10, 11, QPrinter::Millimeter);

    //设置输出格式
    printer.setOutputFormat(QPrinter::NativeFormat);

    QPrintPreviewDialog preview(&printer);
    preview.setStyleSheet("QToolButton{background:none;margin:2px;padding:0px;border-width:0px;border-radius:0px;}QLineEdit{border-width:0px;}");
    preview.setWindowTitle("打印预览");
    preview.setWindowFlags(Qt::WindowStaysOnTopHint);
    connect(&preview, SIGNAL(paintRequested(QPrinter *)), this, SLOT(printView(QPrinter *)));
    preview.showMaximized();
    preview.exec();
}

void PrintThread::appendSubTitle1(const QString &str)
{
    QMutex mutex;
    mutex.lock();
    subTitle1.append(str);
    mutex.unlock();
}

void PrintThread::appendSubTitle2(const QString &str)
{
    QMutex mutex;
    mutex.lock();
    subTitle2.append(str);
    mutex.unlock();
}

void PrintThread::appendContent(const QString &str)
{
    QMutex mutex;
    mutex.lock();
    content.append(str);
    mutex.unlock();
}
