#include "WordApi.h"
#include <QDateTime>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <ActiveQt/QAxObject>
#include <ActiveQt/QAxWidget>
#include<QTextStream>
#include <ActiveQt/QAxBase>
#include <QTextFormat>

WordApi::WordApi(QObject *parent)
{		
	CoInitializeEx(NULL,COINIT_MULTITHREADED);//��������߳��޷�����word����
    m_word = new QAxObject(parent);
	m_documents = NULL;
    m_document = NULL;
	m_bOpened=false;	
}

WordApi::~WordApi()
{
	close();
}

bool WordApi::createWord(QString reportname)		//����һ���µ�word
{
    QString ReportName= reportname;
	QString defaultFileName = QString("%1").arg(ReportName);

	m_saveName=QFileDialog::getSaveFileName(0,tr("Report Information"),defaultFileName,tr("*.doc"));
	QString SavePath = /*pAppData->m_strAppDirPath + */"/ReportWord"+m_saveName;

    QFile file(m_saveName);
	if(file.exists())
	{
		m_strError = tr("abnormal:the file already exists!");
		return false;
	}

	if(!m_saveName.isEmpty())
	{		
        if(!m_word->setControl("Word.Application"))
		{
			m_strError = tr("abnormal:failed to get the word component,please make sure the word is installed!");
			return false;
		}

		m_word->setProperty("Visible",false);
		m_word->setProperty("DisplayAlerts", false);//����ʾ�κξ�����Ϣ�����Ϊtrue��ô�ڹر��ǻ�������ơ��ļ����޸ģ��Ƿ񱣴桱����ʾ
		m_documents = m_word->querySubObject("Documents");
		m_documents->dynamicCall("Add (void)");
		m_document = m_word->querySubObject("ActiveDocument");//��ȡ��ǰ������ĵ� 	

		return true;
    }
    else
	{
		m_strError = tr("abnormal:the file name is empty!");
		return false;
	}
}

bool WordApi::createNewWord(const QString& filePath )
{	
	m_saveName = filePath;
	QFile file(m_saveName);

	if(file.exists())
	{
		file.remove(m_saveName);
	}

	if(!m_saveName.isEmpty())
    {
        if(!m_word->setControl("Word.Application"))
        {
            m_strError = tr("abnormal:failed to get the word component,please make sure the word is installed!");
            return false;
        }

		m_word->setProperty("Visible",false);
		m_word->setProperty("DisplayAlerts", false);//����ʾ�κξ�����Ϣ�����Ϊtrue��ô�ڹر��ǻ�������ơ��ļ����޸ģ��Ƿ񱣴桱����ʾ
		m_documents = m_word->querySubObject("Documents");

		if(!m_documents)
		{
			m_strError = tr("abnormal:failed to get the documents!");
			return false;
		}

		m_documents->dynamicCall("Add (void)");
        m_document = m_word->querySubObject("ActiveDocument");//��ȡ��ǰ������ĵ�

		return true;
    }
    else
	{
		m_strError = tr("abnormal:the file name is empty!");
		return false;
	}
}

bool WordApi::openword(bool bVisable)
{
	m_word = new QAxObject();

	bool bFlag = m_word->setControl("word.Application");    
	if(!bFlag)
	{
		bFlag = m_word->setControl("kwps.Application");
	}

	if(!bFlag)
	{
		return false;
	}

	m_word->setProperty("Visible",bVisable);
	QAxObject* documents =m_word->querySubObject("Documents");
	if(!documents)
	{
		return false;
	}

	documents->dynamicCall("Add(QString)",m_strFilePath);
	m_bOpened = true;

	return m_bOpened;
}

bool WordApi::open(const QString& strFilePath,bool bVisable)
{
	m_strFilePath = strFilePath;

	return openword(bVisable);
}

bool WordApi::isOpen()
{
	return m_bOpened;
}

void WordApi::save()
{
	if(m_document)
		m_document->dynamicCall("Save()");
}

void WordApi::close()				//�ر� �˳� ����ʱ��Ҳ���Զ�����һ��
{
	if(!m_saveName.isEmpty())		//�����Ϊ��  ��Ϊ�½�   
	{
		saveAs();
		m_saveName = "";
	}

	if(m_documents)
		delete m_documents;	

	if(m_word)
		delete m_word;	

	m_document = NULL;
	m_documents = NULL;
	m_word = NULL;
}

void WordApi::saveAs()
{
	if(m_document)
		m_document->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(m_saveName));	
}

void WordApi::setPageOrientation(int flag)	//����ҳ��1 ����   ���� 0����
{	
	QAxObject* selection = m_word->querySubObject("Selection");	
    if(NULL== selection)
	{
		return;
	}

	QString page;
	switch (flag)
	{
	case 0:
		page = "wdOrientPortrait";
		break;
	case 1:
		page = "wdOrientLandscape";
		break;
	}
	selection->querySubObject("PageSetUp")->setProperty("Orientation",page);
}

void WordApi::setWordPageView(int flag)
{
	QAxObject* viewPage = m_word->querySubObject("ActiveWindow");
    if(NULL== viewPage)
	{
		return;
	}

	QString view;
	switch (flag)
	{
	case 1:
		view = "wdNormalView";
		break;
	case 2:
		view = "wdOutlineView";
		break;
	case 3:
		view = "wdPrintView";
		break;
	case 4:
		view = "wdPrintPreview";
		break;
	case 5:
		view = "wdMasterView";
		break;
	case 6:
		view = "wdWebView";
		break;
	case 7:
		view = "wdReadingView";
		break;
	case 8:
		view = "wdConflictView";
		break;
    }

	viewPage->querySubObject("View")->setProperty("Type",view);
}

void WordApi::insertMoveDown()				//����س�
{
	QAxObject* selection  = m_word->querySubObject("Selection");
    if(NULL== selection)
	{
		return;
	}

	selection->dynamicCall("TypeParagraph(void)");	
}

void WordApi::insertText(const QString& text)
{
	QAxObject* selection  = m_word->querySubObject("Selection");
    if(NULL== selection)
	{
		return;
	}

	selection->dynamicCall("TypeText(const QString&)",text);		
}

QString WordApi::GetText()
{
	QAxObject* selection = m_word->querySubObject("Selection");	
	QString str;

    if(NULL!= selection)
	{
	  str = selection->dynamicCall("GetText(void)").toString();
	}
		
	return str;
}

//����ѡ��λ�����־��� 0 ,���� 1,���� 2
void WordApi::setParagraphAlignment(int flag)
{
	QAxObject* selection = m_word->querySubObject("Selection");	
    if(NULL== selection)
	{
		return;
	}

	if(flag == 0)
	{
		selection->querySubObject("ParagraphFormat")->setProperty("Alignment","wdAlignParagraphCenter");
    }
    else if(flag == 1)
	{
		selection->querySubObject("ParagraphFormat")->setProperty("Alignment","wdAlignParagraphJustify");
    }
    else if(flag == 2)
	{
		selection->querySubObject("ParagraphFormat")->setProperty("Alignment","wdAlignParagraphRight");
	}
}

void WordApi::setRowAlignment(int tableIndex,int row,int flag)
{   
	QAxObject* tables = m_document->querySubObject("Tables");	
    if(NULL== tables)
	{
		return;
	}

	QAxObject* table = tables->querySubObject("Item(int)",tableIndex);
    if(NULL== table )
	{
		return;
	}

	QAxObject* Row= table->querySubObject("Rows(int)",row);
    if(NULL== Row)
	{
		return;
	}

	QAxObject* range = Row->querySubObject("Range");
    if(NULL== range)
	{
		return;
	}

	Row->querySubObject("Alignment(int)",flag);
	if(flag == 0)
	{
		range->querySubObject("ParagraphFormat")->setProperty("Alignment","wdAlignParagraphCenter");
    }
    else if(flag == 1)
	{
		range->querySubObject("ParagraphFormat")->setProperty("Alignment","wdAlignParagraphJustify");
	}else if(flag == 2)
	{
		range->querySubObject("ParagraphFormat")->setProperty("Alignment","wdAlignParagraphRight");
    }
    else if(flag ==3)
	{
		range->querySubObject("ParagraphFormat")->setProperty("Alignment","wdAlignParagraphLeft");
	}
}

void WordApi::setFontSize(int fontsize)		//���������С
{
	QAxObject* selection = m_word->querySubObject("Selection");	
    if(NULL== selection)
	{
		return;
	}

	selection->querySubObject("Font")->setProperty("Size",fontsize);
}

void WordApi::setFontBold(bool flag)
{
	QAxObject* selection = m_word->querySubObject("Selection");	
    if(NULL== selection)
	{
		return;
	}

	selection->querySubObject("Font")->setProperty("Bold",flag);
}

void WordApi::setFontName(QString& fontName)
{
	QAxObject* selection = m_word->querySubObject("Selection");	
    if(NULL== selection)
	{
		return;
	}

	selection->querySubObject("Font")->setProperty("Name",fontName);
}

void WordApi::setSelectionRange(int start,int end)
{
	QAxObject* selection = m_word->querySubObject("Selection");
    if(NULL== selection)
	{
		return;
	}

	selection->dynamicCall("SetRange(int, int)", start,end);	//��1���ַ���ʼ������9���ַ�������Χ
}

void WordApi::getUsedRange(int *topLeftRow, int *topLeftColumn, int *bottomRightRow, int *bottomRightColumn)
{
	QAxObject* range = m_document->querySubObject("Range");
    if(NULL== range)
	{
		return;
	}

	*topLeftRow = range->property("Row").toInt();
    if(NULL== topLeftRow)
	{
		return;
	}

	*topLeftColumn = range->property("Column").toInt();
    if(NULL== topLeftColumn)
	{
		return;
	}

	QAxObject *rows = range->querySubObject("Rows");
    if(NULL== rows)
	{
		return;
	}

	*bottomRightRow = *topLeftRow + rows->property("Count").toInt() - 1;
    if(NULL== bottomRightRow)
	{
		return;
	}

	QAxObject *columns = range->querySubObject("Columns");
    if(NULL== columns)
	{
		return;
	}

	*bottomRightColumn = *topLeftColumn + columns->property("Count").toInt() - 1;
    if(NULL== bottomRightColumn)
	{
		return;
	}
}

void WordApi::intsertTable(int row,int column)
{	
	QAxObject* tables = m_document->querySubObject("Tables");	
    if(NULL== tables)
	{
		return;
	}

	QAxObject* selection = m_word->querySubObject("Selection");
    if(NULL== selection)
	{
		return;
	}

	QAxObject* range = selection->querySubObject("Range");
    if(NULL== range)
	{
		return;
	}

	QVariantList params;
	params.append(range->asVariant());		
	params.append(row);
	params.append(column);	
	tables->querySubObject("Add(QAxObject*, int, int, QVariant&, QVariant&)", params);

	QAxObject* table = selection->querySubObject("Tables(int)",1);
    if(NULL== table)
	{
		return;
	}

	table->setProperty("Style","������"); 
	QAxObject* Borders = table->querySubObject("Borders");
    if(NULL== Borders)
	{
		return;
	}

	Borders->setProperty("InsideLineStyle",1);
	Borders->setProperty("OutsideLineStyle",1);
}

void WordApi::intsertTable(int tableIndex, int row,int column)
{	
	QAxObject* tables = m_document->querySubObject("Tables");	
    if(NULL== tables)
	{
		return;
	}

	QAxObject* selection = m_word->querySubObject("Selection");
    if(NULL== selection)
	{
		return;
	}

	QAxObject* range = selection->querySubObject("Range");
    if(NULL== range)
	{
		return;
	}

	QVariantList params;
	params.append(range->asVariant());		
	params.append(row);
	params.append(column);	
	tables->querySubObject("Add(QAxObject*, int, int, QVariant&, QVariant&)", params);

    QAxObject* table = selection->querySubObject("Tables(int)",tableIndex);
    if(NULL== table)
	{
		return;
	}
	table->setProperty("Style","������"); 

	QAxObject* Borders = table->querySubObject("Borders");
    if(NULL== Borders)
	{
		return;
	}

	Borders->setProperty("InsideLineStyle",1);
	Borders->setProperty("OutsideLineStyle",1);
}

void WordApi::setColumnWidth(int column, int width)		//�����п�
{
	QAxObject* selection = m_word->querySubObject("Selection");	
    if(NULL== selection)
	{
		return;
	}

	QAxObject* table = selection->querySubObject("Tables(1)");
    if(NULL== table)
	{
		return;
	}

	table->querySubObject("Columns(int)",column)->setProperty("Width",width);

}

void WordApi::setCellString(int nTable,int row,int column,const QString& text)
{
 	QAxObject* pTables =m_document->querySubObject("Tables");
    if(NULL==pTables)
	{
		return;
	}

	QAxObject* table=pTables->querySubObject("Item(int)",nTable);
	if(table)
	{
		table->querySubObject("Cell(int,int)",row,column)->querySubObject("Range")
		                    ->dynamicCall("SetText(QString)",text);
	}
}

void WordApi::MergeCells(int tableIndex, int nStartRow,int nStartCol,int nEndRow,int nEndCol)//�ϲ���Ԫ��
{
	QAxObject* tables = m_document->querySubObject("Tables");
    if(NULL==tables)
	{
		return;
	}

	QAxObject* table = tables->querySubObject("Item(int)",tableIndex);
    if(NULL== table)
	{
		return;
	}

	if(table)
	{
		QAxObject* StartCell =table->querySubObject("Cell(int, int)",nStartRow,nStartCol);
		QAxObject* EndCell = table->querySubObject("Cell(int, int)",nEndRow,nEndCol);
        if(NULL==StartCell)
		{
			return;
		}
        if(NULL== EndCell)
		{
			return;
		}
		StartCell->querySubObject("Merge(QAxObject *)",EndCell->asVariant());
	}	
}

void WordApi::setColumnHeight(int nTable,int column, int height)
{
	QAxObject* pTables =m_document->querySubObject("Tables");
    if(NULL==pTables)
	{
		return;
	}

	QAxObject* table=pTables->querySubObject("Item(int)",nTable);
	if(table)
	{
		table->querySubObject("Columns(int)",column)->setProperty("Hight",height);
	}
}

void WordApi::setRowHeight(int nTable,int Row, int height)
{
	QAxObject* pTables =m_document->querySubObject("Tables");
    if(NULL==pTables)
	{
		return;
	}

	QAxObject* table=pTables->querySubObject("Item(int)",nTable);
	if(table)
	{
		table->querySubObject("Rows(int)",Row)->setProperty("Hight",height);
	}
}

void WordApi::setColumnHeight(int column, int height)		//�����и�
{
	QAxObject* selection = m_word->querySubObject("Selection");	
    if(NULL== selection)
	{
		return;
	}

	QAxObject* table = selection->querySubObject("Tables(1)");
	if(table)
	{
		table->querySubObject("Columns(int)",column)->setProperty("Hight",height);
	}	
}

void WordApi::setCellString(int row, int column, const QString& text)
{
	QAxObject* selection = m_word->querySubObject("Selection");	
    if(NULL== selection)
	{
		return;
	}

	QAxObject* table = selection->querySubObject("Tables(1)");	
    if(NULL== table)
	{
		return;
	}

	QAxObject* rows =table->querySubObject("Rows");
	if(rows)
	{
		return;
    }

	int Count =rows->dynamicCall("Count").toInt();
	table->querySubObject("Cell(int, int)",row,column)->querySubObject("Range")->dynamicCall("SetText(QString)", text);	
}

void WordApi::setCellFontBold(int row, int column, bool isBold)	//�������ݴ���  isBold�����Ƿ����
{
	QAxObject* selection = m_word->querySubObject("Selection");	
    if(NULL== selection)
	{
		return;
	}

	QAxObject* table = selection->querySubObject("Tables(1)");
    if(NULL== table)
	{
		return;
	}
	table->querySubObject("Cell(int, int)",row,column)->querySubObject("Range")->dynamicCall("SetBold(int)", isBold);
}

void WordApi::setCellFontSize(int row, int column, int size)		//�������ִ�С
{
	QAxObject* selection = m_word->querySubObject("Selection");	
    if(NULL== selection)
	{
		return;
	}

	QAxObject* table = selection->querySubObject("Tables(1)");
    if(NULL== table)
	{
		return;
	}
	table->querySubObject("Cell(int, int)",row,column)->querySubObject("Range")->querySubObject("Font")->setProperty("Size", size);	
}

QVariant WordApi::getCellValue(int row, int column)					//��ȡ��Ԫ������ �˴�����Excel��˵�к��д�1��ʼ����
{
	QAxObject* selection = m_word->querySubObject("Selection");	
	QAxObject* table = selection->querySubObject("Tables(1)");

    if(NULL!=selection && NULL!=table)
        return table->querySubObject("Cell(int, int)",row,column)->querySubObject("Range")->property("Text");

    return NULL;
}

int WordApi::getTableCount()
{
	QAxObject* tables = m_document->querySubObject("Tables");
    int val = 0;

    if(NULL!= tables)
	{
      val = tables->property("Count").toInt();
    }

	return val;
}

void WordApi::moveForEnd()
{
	QAxObject* selection = m_word->querySubObject("Selection");	
    if(NULL== selection)
	{
		return;
	}

	QVariantList params;
	params.append(6);		
	params.append(0);		
	selection->dynamicCall("EndOf(QVariant&, QVariant&)", params).toInt();
}

void WordApi::insertCellPic(int row,int column,const QString& picPath)
{
	QAxObject* selection = m_word->querySubObject("Selection");	
    if(NULL== selection)
	{
		return;
	}

	QAxObject* table = selection->querySubObject("Tables(1)");
    if(NULL== table)
	{
		return;
	}

	QAxObject* range = table->querySubObject("Cell(int, int)",row,column)->querySubObject("Range");
    if(NULL== range)
	{
		return;
	}
	range->querySubObject("InlineShapes")->dynamicCall("AddPicture(const QString&)",picPath);
}

void WordApi::setTableAutoFitBehavior(int flag)
{
	QAxObject* selection = m_word->querySubObject("Selection");	
    if(NULL== selection)
	{
		return;
	}

	QAxObject* table = selection->querySubObject("Tables(1)");
    if(NULL== table)
	{
		return;
	}

    if(0 <= flag &&flag <= 2 )
		table->dynamicCall("AutoFitBehavior(WdAutoFitBehavior)", flag);
}
void WordApi::deleteSelectColumn(int column)
{
	QAxObject* selection = m_word->querySubObject("Selection");
    if(NULL== selection)
	{
		return;
	}

	QAxObject* table = selection->querySubObject("Tables(1)");
    if(NULL== table)
	{
		return;
	}

	QAxObject* columns = table->querySubObject("Columns(int)",column);
    if(NULL== columns)
	{
		return;
	}
	columns->dynamicCall("Delete()");
}

void WordApi::setOptionCheckSpell(bool flags)
{
	QAxObject* opetions = m_word->querySubObject("Options");	
	if(!opetions)
		return;

	opetions->setProperty("CheckGrammarAsYouType",flags);
	opetions->setProperty("CheckGrammarWithSpelling",flags);
	opetions->setProperty("ContextualSpeller",flags);
	opetions->setProperty("CheckSpellingAsYouType",flags);
}

void WordApi::addTableRow(int tableIndex ,int nRow,int rowCount)
{
	QAxObject* tables=m_document->querySubObject("Tables");
    if(NULL== tables)
	{
		return;
	}

	QAxObject* table = tables->querySubObject("Item(int)",tableIndex);
    if(NULL== table)
	{
		return;
	}

	QAxObject* rows =table->querySubObject("Rows");
    if(NULL== rows)
	{
		return;
	}

	int Count =rows->dynamicCall("Count").toInt();
	if(0<nRow && nRow <= Count )
	{   
		for(int i =0; i<rowCount; ++i)
		{
			QString sPos = QString("Item(%1)").arg(nRow+i);
			QAxObject* row= rows->querySubObject(sPos.toStdString().c_str());
			QAxObject* row1= rows->querySubObject("Last");
			QAxObject* row2= rows->querySubObject("Row(int)", nRow+i);
			QAxObject* row3= rows->querySubObject("Item(int)",nRow+i);
            if (NULL != row)
			{
				QVariant param =row ->asVariant();
				QAxObject* NewRow = rows->querySubObject("Add(Variant)",param);
			}
			
		}			
	}	
}

////�������
void WordApi::insertTable(int tableIndex,int row,int column)
{
	
	QAxObject* tables=m_document->querySubObject("Tables");
    if(NULL==tables)
	{
		return;
	}

	QAxObject* table = tables->querySubObject("Item(int)",tableIndex);
    if(NULL==table)
	{
		return;
	}

	QAxObject* selection = m_word->querySubObject("Selection");
    if(NULL==selection)
	{
		return;
	}

	QAxObject* range = selection->querySubObject("Range");
    if(NULL==range)
	{
		return;
	}

	QVariantList params;
	params.append(range->asVariant());		
	params.append(row);
	params.append(column);	
	tables->querySubObject("Add(QAxObject*, int, int, QVariant&, QVariant&)", params);
	table = selection->querySubObject("Tables(int)",1);
	table->setProperty("Style","������"); 

	QAxObject* Borders = table->querySubObject("Borders");
    if(NULL==Borders)
	{
		return;
	}

	Borders->setProperty("InsideLineStyle",1);
	Borders->setProperty("OutsideLineStyle",1);
}

//���ñ���п�
void WordApi::setColumnWidth(int nTable,int column,int width)
{
	QAxObject* pTables =m_document->querySubObject("Tables");
    if(NULL== pTables)
	{
		return;
	}

	QAxObject* table=pTables->querySubObject("Item(int)",nTable);
	if(table)
	{
      table->querySubObject("Columns(int)",column)->setProperty("width",width);
	}	
}

//�ڱ���в���ͼƬ
void WordApi::insertCellPic(int nTable,int row,int column,const QString& picPath)
{
	QAxObject* pTables=m_document->querySubObject("Tables");
    if(NULL== pTables)
 	{
 		return;
 	}

	QAxObject* table=pTables->querySubObject("Item(int)",nTable);
    if(NULL== table)
	{
		return;
	}

	QAxObject* range=table->querySubObject("Cell(int,int )",row,column)->querySubObject("Range");                      
    if(NULL== range)
	{
		return;
	}
	range->querySubObject("InlineShapes")->dynamicCall("AddPicture(const QString&)",picPath);
}

//�������ݴ���
void WordApi::setCellFontBold(int nTable,int row,int column,bool isBold)
{
	QAxObject* pTables = m_document->querySubObject("Tables");
    if(NULL== pTables)
	{
		return;
	}

	QAxObject* table =pTables->querySubObject("Item(int)",nTable);
    if(NULL== table)
	{
		return;
	}
    table->querySubObject("Cell(int,int )",row,column)->querySubObject("Range")->dynamicCall("SetBold(int)",isBold);
}

//�������ִ�С
void WordApi:: setCellFontSize(int nTable,int row,int column,int size)
{
	QAxObject* pTables=m_document->querySubObject("Tables");
    if(NULL== pTables)
	{
		return;
	}

	QAxObject* table =pTables->querySubObject("Item(int)",nTable);
    if(NULL== table)
	{
		return;
	}
	table->querySubObject("Cell(int,int)",row,column)->querySubObject("Range")->querySubObject("Font")->setProperty("Size",size);                      
}

void WordApi::setVisible(bool isVisible)
{
    if(m_word != NULL)
	{
		m_word->setProperty("Visible",isVisible);
	}
}
