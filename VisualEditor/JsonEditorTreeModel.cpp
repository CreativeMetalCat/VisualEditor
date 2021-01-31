#include "JsonEditorTreeModel.h"

Q_INVOKABLE QModelIndex JsonEditorTreeModel::index(int row, int column, const QModelIndex& parent ) const
{
    return Q_INVOKABLE  this->createIndex(row, column);
}

Q_INVOKABLE QModelIndex JsonEditorTreeModel::parent(const QModelIndex& child) const
{
    return Q_INVOKABLE QModelIndex();
}

Q_INVOKABLE int JsonEditorTreeModel::rowCount(const QModelIndex& parent ) const
{
    return Q_INVOKABLE int();
}

Q_INVOKABLE int JsonEditorTreeModel::columnCount(const QModelIndex& parent ) const
{
    return Q_INVOKABLE int();
}

Q_INVOKABLE QVariant JsonEditorTreeModel::data(const QModelIndex& index, int role) const
{
   return Q_INVOKABLE QVariant();
}
