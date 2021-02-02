#pragma once

#include <QWidget>
#include "ui_PropertyEditor.h"
#include "JSONWidgetBase.h"


class PropertyEditor : public QWidget
{
	Q_OBJECT

public:
	PropertyEditor(JSONWidgetBase* widgetToEdit,QWidget *parent = Q_NULLPTR);
	~PropertyEditor();

	virtual void closeEvent(QCloseEvent*)override;
private:
	Ui::PropertyEditor ui;
	JSONWidgetBase* WidgetToEdit = nullptr;
};
