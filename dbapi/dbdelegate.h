#ifndef DBDELEGATE_H
#define DBDELEGATE_H

/**
 * 自定义委托全家桶 作者:feiyangqingyun(QQ:517216493) 2019-10-25
 * 1:可设置多种委托类型,例如复选框/文本框/下拉框/日期框/微调框/进度条等
 * 2:可设置是否密文显示,一般用于文本框
 * 3:可设置是否允许编辑,一般用于下拉框
 * 4:可设置是否禁用,一般用来禁用某列
 * 5:可设置数据集合,比如下拉框数据集合
 * 6:提供值变化信号,比方说下拉框值改动触发
 * 7:可设置数据校验自动产生不同的图标
 * 8:支持设置校验列/校验规则/校验值/校验成功图标/校验失败图标/图标大小
 * 9:可设置校验数据产生不同的背景颜色和文字颜色
 * 10:校验规则支持 == > >= < <= != contains,非常丰富
 * 11:复选框自动居中而不是左侧,切换选中状态发送对应的信号
 * 12:可设置颜色委托,自动根据颜色值绘制背景颜色,自动设置最佳文本颜色
 * 13:可设置按钮委托,自动根据值生成多个按钮,按钮按下发送对应的信号
 * 14:当设置了委托列时自动绘制选中背景色和文字颜色
 * 15:根据不同的委托类型绘制,可以依葫芦画瓢自行增加自己的委托
 */

#include <QStyledItemDelegate>
#include <QPixmap>

#ifdef quc
#if (QT_VERSION < QT_VERSION_CHECK(5,7,0))
#include <QtDesigner/QDesignerExportWidget>
#else
#include <QtUiPlugin/QDesignerExportWidget>
#endif

class QDESIGNER_WIDGET_EXPORT DbDelegate : public QStyledItemDelegate
#else
class DbDelegate : public QStyledItemDelegate
#endif

{
    Q_OBJECT

public:
    explicit DbDelegate(QObject *parent = 0);

protected:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawBg(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &) const;
    void drawCheckBox(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawPushButton(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawProgressBar(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawColor(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawLineEdit(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawPixmap(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawText(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void checkData(const QString &data, bool *exist) const;

private:
    bool delegatePwd;           //是否密码显示
    bool delegateEdit;          //可否允许编辑
    bool delegateEnable;        //是否可用,这样可以用来禁用列

    int delegateColumn;         //当前委托对应的列
    QString delegateType;       //委托类型
    QStringList delegateValue;  //委托数据集合

    int checkColumn;            //校验数据列
    QString checkType;          //校验规则,例如>=
    QString checkValue;         //检验值
    QPixmap checkOkImage;       //校验成功图标
    QPixmap checkNoImage;       //校验失败图标
    int checkImageWidth;        //图片宽度
    int checkImageHeight;       //图片高度

    bool checkText;             //是否校验数据显示不同的颜色
    QColor textColor;           //文本颜色
    QColor selectBgColor;       //选中背景颜色
    QColor checkTextColor;      //校验文字颜色
    QColor checkBgColor;        //校验背景颜色

    QPoint mousePoint;          //鼠标按下处坐标
    bool mousePressed;          //鼠标是否按下

signals:
    //值改变信号
    void valueChanged(const QString &value);
    //按钮单击信号
    void buttonClicked(int btnIndex, const QModelIndex &index);

public slots:
    //设置密码显示+允许编辑+可用
    void setDelegatePwd(bool delegatePwd);
    void setDelegateEdit(bool delegateEdit);
    void setDelegateEnable(bool delegateEnable);

    //设置委托列+委托类型+参数
    void setDelegateColumn(int delegateColumn);
    void setDelegateType(const QString &delegateType);
    void setDelegateValue(const QStringList &delegateValue);

    //设置校验数据列+校验规则 == > >= < <= != contains +校验值+校验图标+图标宽高
    void setCheckColumn(int checkColumn);
    void setCheckType(const QString &checkType);
    void setCheckValue(const QString &checkValue);
    void setCheckOkImage(const QPixmap &checkOkImage);
    void setCheckNoImage(const QPixmap &checkNoImage);
    void setCheckImageWidth(int checkImageWidth);
    void setCheckImageHeight(int checkImageHeight);

    //设置是否校验文本+文字颜色+选中背景颜色+检验文本颜色+校验背景颜色
    void setCheckText(bool checkText);
    void setTextColor(const QColor &textColor);
    void setSelectBgColor(const QColor &selectBgColor);
    void setCheckTextColor(const QColor &checkTextColor);
    void setCheckBgColor(const QColor &checkBgColor);
};

#endif // DBDELEGATE_H
