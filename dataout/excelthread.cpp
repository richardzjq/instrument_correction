#pragma execution_character_set("utf-8")

#include "excelthread.h"
#include "qfile.h"
#include "qtextstream.h"
#include "qdatetime.h"
#include "qstringlist.h"
#include "qdebug.h"
#include "qapplication.h"

#define DATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))

QScopedPointer<ExcelThread> ExcelThread::self;
ExcelThread *ExcelThread::Instance()
{
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new ExcelThread);
        }
    }

    return self.data();
}

ExcelThread::ExcelThread(QObject *parent) : QThread(parent)
{
    init();
}

void ExcelThread::run()
{
    takeTime = 0;
    time.restart();

    while (!stopped) {
        take();
        msleep(10);
    }

    takeTime = time.elapsed();
    stopped = false;
}

int ExcelThread::getTakeTime() const
{
    return this->takeTime;
}

void ExcelThread::appendHead()
{
    //固定头部信息
    stream << ("<?xml version=\"1.0\"?>\n");
    stream << ("<?mso-application progid=\"Excel.Sheet\"?>\n");
    stream << ("<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\n");
    stream << (" xmlns:o=\"urn:schemas-microsoft-com:office:office\"\n");
    stream << (" xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n");
    stream << (" xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\n");
    stream << (" xmlns:html=\"http://www.w3.org/TR/REC-html40\">\n");
}

void ExcelThread::appendDocument()
{
    //文档信息
    stream << (" <DocumentProperties xmlns=\"urn:schemas-microsoft-com:office:office\">\n");
    stream << ("  <LastAuthor></LastAuthor>\n");
    stream << (QString("  <Created>%1</Created>\n").arg(DATETIME));
    stream << ("  <Version>11.6360</Version>\n");
    stream << (" </DocumentProperties>\n");

    stream << (" <OfficeDocumentSettings xmlns=\"urn:schemas-microsoft-com:office:office\">\n");
    stream << ("  <AllowPNG/>\n");
    stream << (" </OfficeDocumentSettings>\n");

    stream << (" <ExcelWorkbook xmlns=\"urn:schemas-microsoft-com:office:excel\">\n");
    stream << ("  <WindowHeight>9795</WindowHeight>\n");
    stream << ("  <WindowWidth>21435</WindowWidth>\n");
    stream << ("  <WindowTopX>120</WindowTopX>\n");
    stream << ("  <WindowTopY>75</WindowTopY>\n");
    stream << ("  <WindowTopY>75</WindowTopY>\n");
    stream << ("  <ProtectWindows>False</ProtectWindows>\n");
    stream << (" </ExcelWorkbook>\n");
}

void ExcelThread::appendBorder()
{
    //如果边框宽度为0则不显示边框
    if (borderWidth > 0) {
        stream << ("  <Borders>\n");
        stream << QString("   <Border ss:Position=\"Bottom\" ss:LineStyle=\"Continuous\" ss:Weight=\"%1\"/>\n").arg(borderWidth);
        stream << QString("   <Border ss:Position=\"Left\" ss:LineStyle=\"Continuous\" ss:Weight=\"%1\"/>\n").arg(borderWidth);
        stream << QString("   <Border ss:Position=\"Right\" ss:LineStyle=\"Continuous\" ss:Weight=\"%1\"/>\n").arg(borderWidth);
        stream << QString("   <Border ss:Position=\"Top\" ss:LineStyle=\"Continuous\" ss:Weight=\"%1\"/>\n").arg(borderWidth);
        stream << ("  </Borders>\n");
    }
}

void ExcelThread::appendStyle()
{
    //样式表
    stream << (" <Styles>\n");
    stream << ("  <Style ss:ID=\"Default\" ss:Name=\"Normal\">\n");
    stream << ("   <Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n");
    stream << ("   <Borders/>\n");
    stream << ("   <Font ss:FontName=\"微软雅黑\" x:CharSet=\"134\" ss:Size=\"10\" ss:Color=\"#000000\"/>\n");
    stream << ("   <Interior/>\n");
    stream << ("   <NumberFormat/>\n");
    stream << ("   <Protection/>\n");
    stream << ("  </Style>\n");

    //标题样式--居中大号加粗字体
    stream << ("  <Style ss:ID=\"s_title\">\n");
    stream << ("   <Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n");
    appendBorder();
    stream << ("   <Font ss:FontName=\"微软雅黑\" x:CharSet=\"134\" ss:Size=\"15\" ss:Color=\"#000000\" ss:Bold=\"1\"/>\n");
    stream << ("  </Style>\n");

    //标题样式--左对齐普通字体
    stream << ("  <Style ss:ID=\"s_subtitle\">\n");
    stream << ("   <Alignment ss:Horizontal=\"Left\" ss:Vertical=\"Center\"/>\n");
    appendBorder();
    stream << ("   <Font ss:FontName=\"微软雅黑\" x:CharSet=\"134\" ss:Size=\"10\" ss:Color=\"#000000\" ss:Bold=\"0\"/>\n");
    stream << ("  </Style>\n");

    //正文样式--默认颜色
    stream << ("  <Style ss:ID=\"s_normal\">\n");
    stream << ("   <Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n");
    appendBorder();
    stream << (" </Style>\n");

    //正文样式--文字红色
    stream << ("  <Style ss:ID=\"s_red\">\n");
    appendBorder();
    stream << ("  <Font ss:FontName=\"微软雅黑\" x:CharSet=\"134\" ss:Size=\"10\" ss:Color=\"#FF0000\" ss:Bold=\"0\"/>\n");
    stream << (" </Style>\n");

    //生成多个带背景色的样式
    if (randomColor) {
        QStringList colorName;
        colorName << "#FF99CC" << "#FFCC99" << "#FF8080" << "#8DB4E2" << "#EBF1DE" << "#CC99FF" << "#99CC00" << "#99CCFF" << "#FFFF00" << "#FF6600";

        for (int i = 0; i < 10; i++) {
            stream << (QString("  <Style ss:ID=\"s_color_%1\">\n").arg(i));
            stream << ("   <Alignment ss:Horizontal=\"Left\" ss:Vertical=\"Center\"/>\n");
            appendBorder();
            stream << (QString("   <Interior ss:Color=\"%1\" ss:Pattern=\"Solid\"/>\n").arg(colorName.at(i)));
            stream << (" </Style>\n");
        }
    }

    stream << (" </Styles>\n");
}

void ExcelThread::appendTable()
{
    int columnCount = columnNames.count();

    //工作表名称
    stream << (QString(" <Worksheet ss:Name=\"%1\">\n").arg(sheetName));

    //表格开始
    stream << (QString("  <Table ss:ExpandedColumnCount=\"%1\" x:FullColumns=\"1\"\n").arg(columnCount));
    stream << ("   x:FullRows=\"1\" ss:DefaultColumnWidth=\"54\" ss:DefaultRowHeight=\"18\">\n");

    //设置字段宽度
    for (int i = 0; i < columnCount; i++) {
        stream << (QString("   <Column ss:AutoFitWidth=\"0\" ss:Width=\"%1\"/>\n").arg(columnWidths.at(i)));
    }

    //表格标题,如果没有则不输出
    if (!title.isEmpty()) {
        stream << ("   <Row ss:AutoFitHeight=\"0\" ss:Height=\"22\">\n");
        stream << (QString("    <Cell ss:MergeAcross=\"%1\" ss:StyleID=\"s_title\"><Data ss:Type=\"String\">%2</Data></Cell>\n")
                   .arg(columnCount - 1).arg(title));
        stream << ("   </Row>");
    }

    //表格副标题,如果没有则不输出
    if (!subTitle.isEmpty()) {
        stream << ("   <Row ss:AutoFitHeight=\"0\" ss:Height=\"18\">\n");
        stream << (QString("    <Cell ss:MergeAcross=\"%1\" ss:StyleID=\"s_subtitle\"><Data ss:Type=\"String\">%2</Data></Cell>\n")
                   .arg(columnCount - 1).arg(subTitle));
        stream << ("   </Row>");
    }
}

void ExcelThread::appendColumn()
{
    //逐个添加字段名称
    int columnCount = columnNames.count();
    if (columnCount > 0) {
        stream << ("   <Row ss:AutoFitHeight=\"0\">\n");

        for (int i = 0; i < columnCount; i++) {
            stream << (QString("    <Cell ss:StyleID=\"s_normal\"><Data ss:Type=\"String\">%1</Data></Cell>\n")
                       .arg(columnNames.at(i)));
        }

        stream << ("   </Row>\n");
    }
}

void ExcelThread::appendContent(const QStringList &listContent)
{
    //逐个添加数据
    int rowCount = listContent.count();
    for (int i = 0; i < rowCount; i++) {
        stream << ("   <Row ss:AutoFitHeight=\"0\">\n");

        QString temp = listContent.at(i);
        QStringList value = temp.split(separator);
        int valueCount = value.count();

        if (randomColor) {
            //如果启用了随机背景颜色则随机取背景颜色样式
            QString style = "s_normal";
            static int styleIndex = 0;

            //如果没有指定列随机背景颜色则对整行
            for (int j = 0; j < valueCount; j++) {
                if (colorColumn.count() == 0) {
                    style = QString("s_color_%1").arg(styleIndex);
                } else {
                    if (colorColumn.contains(j)) {
                        style = QString("s_color_%1").arg(styleIndex);
                    } else {
                        style = "s_normal";
                    }
                }

                stream << (QString("    <Cell ss:StyleID=\"%1\"><Data ss:Type=\"String\">%2</Data></Cell>\n").arg(style).arg(value.at(j)));
            }

            styleIndex++;

            if (styleIndex > 9) {
                styleIndex = 0;
            }
        } else {
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
                } else if (checkType == "contains") {
                    if (value.at(checkColumnIndex).contains(checkValue)) {
                        existCheck = true;
                    }
                }
            }

            if (existCheck) {
                for (int j = 0; j < valueCount; j++) {
                    stream << (QString("    <Cell ss:StyleID=\"s_red\"><Data ss:Type=\"String\">%1</Data></Cell>\n").arg(value.at(j)));
                }
            } else {
                for (int j = 0; j < valueCount; j++) {
                    stream << (QString("    <Cell ss:StyleID=\"s_normal\"><Data ss:Type=\"String\">%1</Data></Cell>\n").arg(value.at(j)));
                }
            }
        }

        stream << ("   </Row>\n");
    }
}

void ExcelThread::take()
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
                stream << ("   <Row ss:AutoFitHeight=\"0\">\n");
                stream << ("   </Row>");

                //子标题1
                if (subTitle1Count > i) {
                    if (!listSubTitle1.at(i).isEmpty()) {
                        stream << ("   <Row ss:AutoFitHeight=\"0\" ss:Height=\"18\">\n");
                        stream << (QString("    <Cell ss:MergeAcross=\"%1\" ss:StyleID=\"s_subtitle\"><Data ss:Type=\"String\">%2</Data></Cell>\n")
                                   .arg(columnCount - 1).arg(listSubTitle1.at(i)));
                        stream << ("   </Row>");
                    }
                }

                //子标题2
                if (subTitle2Count > i) {
                    if (!listSubTitle2.at(i).isEmpty()) {
                        stream << ("   <Row ss:AutoFitHeight=\"0\" ss:Height=\"18\">\n");
                        stream << (QString("    <Cell ss:MergeAcross=\"%1\" ss:StyleID=\"s_subtitle\"><Data ss:Type=\"String\">%2</Data></Cell>\n")
                                   .arg(columnCount - 1).arg(listSubTitle2.at(i)));
                        stream << ("   </Row>");
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

void ExcelThread::appendEnd()
{
    //表格结尾
    stream << ("  </Table>\n");

    //固定结尾格式
    stream << ("  <WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\">\n");
    stream << ("   <PageSetup>\n");
    stream << ("    <Header x:Margin=\"0.3\"/>\n");
    stream << ("    <Footer x:Margin=\"0.3\"/>\n");
    stream << ("    <PageMargins x:Bottom=\"0.75\" x:Left=\"0.7\" x:Right=\"0.7\" x:Top=\"0.75\"/>\n");
    stream << ("   </PageSetup>\n");
    stream << ("   <Unsynced/>\n");
    stream << ("   <Selected/>\n");
    stream << ("   <ProtectObjects>False</ProtectObjects>\n");
    stream << ("   <ProtectScenarios>False</ProtectScenarios>\n");
    stream << ("  </WorksheetOptions>\n");
    stream << (" </Worksheet>\n");
    stream << ("</Workbook>\n");    
}

void ExcelThread::setFileName(const QString &fileName)
{
    if (this->fileName != fileName) {
        this->fileName = fileName;
    }
}

void ExcelThread::setSheetName(const QString &sheetName)
{
    if (this->sheetName != sheetName) {
        this->sheetName = sheetName;
    }
}

void ExcelThread::setTitle(const QString &title)
{
    if (this->title != title) {
        this->title = title;
    }
}

void ExcelThread::setSubTitle(const QString &subTitle)
{
    if (this->subTitle != subTitle) {
        this->subTitle = subTitle;
    }
}

void ExcelThread::setSubTitle1(const QStringList &subTitle1)
{
    this->subTitle1 = subTitle1;
}

void ExcelThread::setSubTitle2(const QStringList &subTitle2)
{
    this->subTitle2 = subTitle2;
}

void ExcelThread::setContent(const QStringList &content)
{
    this->content = content;
}

void ExcelThread::setSeparator(const QString &separator)
{
    if (this->separator != separator) {
        this->separator = separator;
    }
}

void ExcelThread::setSubSeparator(const QString &subSeparator)
{
    if (this->subSeparator != subSeparator) {
        this->subSeparator = subSeparator;
    }
}

void ExcelThread::setColumnNames(const QList<QString> &columnNames)
{
    if (this->columnNames != columnNames) {
        this->columnNames = columnNames;
    }
}

void ExcelThread::setColumnWidths(const QList<int> &columnWidths)
{
    if (this->columnWidths != columnWidths) {
        this->columnWidths = columnWidths;
    }
}

void ExcelThread::setRandomColor(bool randomColor)
{
    if (this->randomColor != randomColor) {
        this->randomColor = randomColor;
    }
}

void ExcelThread::setColorColumn(const QList<int> &colorColumn)
{
    if (this->colorColumn != colorColumn) {
        this->colorColumn = colorColumn;
    }
}

void ExcelThread::setBorderWidth(int borderWidth)
{
    if (this->borderWidth != borderWidth) {
        this->borderWidth = borderWidth;
    }
}

void ExcelThread::setCheckColumnIndex(int checkColumnIndex)
{
    if (this->checkColumnIndex != checkColumnIndex) {
        this->checkColumnIndex = checkColumnIndex;
    }
}

void ExcelThread::setCheckType(const QString &checkType)
{
    if (this->checkType != checkType) {
        this->checkType = checkType;
    }
}

void ExcelThread::setCheckValue(const QString &checkValue)
{
    if (this->checkValue != checkValue) {
        this->checkValue = checkValue;
    }
}

void ExcelThread::init()
{
    stopped = false;    

    fileName = QString("%1.xls").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    sheetName = "Sheet1";

    title.clear();
    subTitle.clear();

    subTitle1.clear();
    subTitle2.clear();
    content.clear();
    separator = ";";
    subSeparator = "|";

    columnNames.clear();
    columnWidths.clear();

    randomColor = false;
    colorColumn.clear();

    borderWidth = 1;
    checkColumnIndex = -1;
    checkType = "==";
    checkValue = "0";
}

void ExcelThread::open()
{
    //校验字段名称和宽度,个数不能为0,而且字段名称的个数要和字段宽度的个数一致
    if (columnNames.count() == 0 || columnWidths.count() == 0 || columnNames.count() != columnWidths.count()) {
        return;
    }

    if (fileName.isEmpty()) {
        return;
    }

    if (file.isOpen()) {
        file.close();
    }

    //指定文件名称
    file.setFileName(fileName);
    if (!file.open(QFile::WriteOnly | QIODevice::Text)) {
        return;
    }

    //设置流对象及编码
    stream.setDevice(&file);
    stream.setCodec("utf-8");

    //输出固定头部信息
    appendHead();

    //输出文档信息
    appendDocument();

    //输出边框样式
    appendStyle();

    //输出表结构
    appendTable();

    //输出字段名称
    if (subTitle1.count() == 0) {
        appendColumn();
    }
}

void ExcelThread::close()
{
    if (file.isOpen()){
        appendEnd();
        file.close();
    }
}

void ExcelThread::stop()
{
    stopped = true;
}

void ExcelThread::appendSubTitle1(const QString &str)
{
    QMutex mutex;
    mutex.lock();
    subTitle1.append(str);
    mutex.unlock();
}

void ExcelThread::appendSubTitle2(const QString &str)
{
    QMutex mutex;
    mutex.lock();
    subTitle2.append(str);
    mutex.unlock();
}

void ExcelThread::appendContent(const QString &str)
{
    QMutex mutex;
    mutex.lock();
    content.append(str);
    mutex.unlock();
}
