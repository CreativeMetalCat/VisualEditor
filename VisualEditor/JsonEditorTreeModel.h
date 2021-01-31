#pragma once
#include <QAbstractItemModel>
#include "JSONObjectWidget.h"

class JsonEditorTreeModel : public QAbstractItemModel
{
public:
	// Inherited via QAbstractItemModel
	virtual Q_INVOKABLE QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	virtual Q_INVOKABLE QModelIndex parent(const QModelIndex& child) const override;
	virtual Q_INVOKABLE int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual Q_INVOKABLE int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual Q_INVOKABLE QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	JSONObjectWidget* root;
};

