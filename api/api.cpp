#include "api.h"
#include "quiwidget.h"

API::API(QObject *parent) : QObject(parent)
{

}

void API::reboot()
{
    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    qApp->closeAllWindows();
}

void API::initTableView(QTableView *tableView)
{
    //选中整行,每次只允许选中一行,鼠标按下或者双击即可执行
    initTableView2(tableView);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::DoubleClicked);
}

void API::initTableView2(QTableView *tableView)
{
    tableView->setAlternatingRowColors(false);
    tableView->verticalHeader()->setVisible(false);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->horizontalHeader()->setMaximumHeight(App::RowHeight);
    tableView->verticalHeader()->setDefaultSectionSize(App::RowHeight);
}

QString API::getSaveFileNames(const QString &defaultName, const QString &filter)
{
    QString name = QString("%1/%2").arg(App::SelectDirName).arg(defaultName);
    QString file = QFileDialog::getSaveFileName(0, "保存", name, filter);

    //自动保存最后一次选择的文件夹路径
    if (file.length() > 0) {
        QFileInfo f(file);
        App::SelectDirName = f.absolutePath();
        App::writeConfig();
    }

    return file;
}

QString API::getFileName(const QString &filter, const QString &defaultDir, const QString &fileName)
{
    QString file;
    QFileDialog dialog;
    dialog.setFixedSize(900, 600);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setWindowTitle("保存文件");
    dialog.setLabelText(QFileDialog::Accept, "保存(&S)");
    dialog.setLabelText(QFileDialog::Reject, "取消(&C)");
    dialog.selectFile(fileName);
    dialog.setNameFilter(filter);
    dialog.setDirectory(defaultDir);

    if (dialog.exec() == 1) {
        file = dialog.selectedFiles().value(0);
        file = QFileInfo(file).suffix().isEmpty() ? "" : file;
    }

    return file;
}

QString API::getDefaultDir()
{
    return QUIHelper::appPath();
}
