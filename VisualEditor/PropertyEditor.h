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

	QSpinBox* GetIdSpinBox();

	QPushButton* GetDeleteButton();

	QCheckBox* GetIsArrayCheckBox();

	QLineEdit* GetNameEdit();
	
	JSONWidgetBase* WidgetToEdit = nullptr;
public:
	Ui::PropertyEditor ui;
	
};
