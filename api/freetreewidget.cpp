#include "freetreewidget.h"

void freeTreeWidget(QTreeWidgetItem* root)
{
    if(nullptr == root)
        return;

    //子节点数
    int child_count=root->childCount();
    QTreeWidgetItem* child_item;

    for (int i = 0; i < child_count; i++)
    {
        child_item = root->child(0);
        delete child_item;
    }

    delete root;
}

