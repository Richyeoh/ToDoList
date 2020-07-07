#include "todomodel.h"
#include "todolist.h"

ToDoModel::ToDoModel(QObject *parent)
    : QAbstractListModel(parent),
      mList(nullptr)
{
}

int ToDoModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;

    // FIXME: Implement me!
    return mList->items().size();
}

QVariant ToDoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()|| !mList)
        return QVariant();

    const ToDoItem item = mList->items().at(index.row());
    switch (role) {
    case DoneRole:
        return QVariant(item.done);
    case DescriptionRole:
        return  QVariant(item.description);
    }
    return QVariant();
}

bool ToDoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    ToDoItem item = mList->items().at(index.row());
    switch (role) {
    case DoneRole:
        item.done = value.toBool();
        break;
    case DescriptionRole:
        item.description = value.toString();
        break;
    }

    if (mList->setItemAt(index.row(),item)) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ToDoModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> ToDoModel::roleNames() const
{
    QHash<int,QByteArray> names;
    names[DoneRole] = "done";
    names[DescriptionRole] = "description";
    return names;
}

ToDoList *ToDoModel::list() const
{
    return mList;
}

void ToDoModel::setList(ToDoList *list)
{
    beginResetModel();
    if(mList){
        mList->disconnect();
    }
    mList = list;
    if(mList){
        connect(mList,&ToDoList::preItemAppend,this,[=](){
            const int index = mList->items().size();
            beginInsertRows(QModelIndex(),index,index);
        });

        connect(mList,&ToDoList::postItemAppend,this,[=](){
            endInsertRows();
        });

        connect(mList,&ToDoList::preItemRemoved,this,[=](int index){
            beginRemoveRows(QModelIndex(),index,index);
        });

        connect(mList,&ToDoList::postItemRemoved,this,[=](){
            endRemoveRows();
        });
    }
    endResetModel();
}
