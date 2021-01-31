#include "JSONPropertyWidget.h"

JSONPropertyWidget::JSONPropertyWidget(QWidget *parent, QString name)
	: JSONWidgetBase(parent,name)
{
	ui.setupUi(this);

	ui.typeBox->addItem("Null");
    ui.typeBox->addItem("Boolean");
    ui.typeBox->addItem("Number");
    ui.typeBox->addItem("Text");

	connect(ui.typeBox, &QComboBox::currentTextChanged, this, &JSONPropertyWidget::OnTypeSelectionChanged);
}

void JSONPropertyWidget::OnTypeSelectionChanged(QString selection)
{
	if (selection == "Null")
	{
		ui.stackedWidget->setCurrentIndex(3);
	}
	if (selection == "Boolean")
	{
		ui.stackedWidget->setCurrentIndex(1);
	}
	if (selection == "Number")
	{
		ui.stackedWidget->setCurrentIndex(2);
	}
	if (selection == "Text")
	{
		ui.stackedWidget->setCurrentIndex(0);
	}
}

QJsonValue JSONPropertyWidget::GenerateJsonValue()
{
	
	switch (ui.stackedWidget->currentIndex())
	{
	case 0:
		return QJsonValue(ui.valueEdit->text());
	case 1:
		return QJsonValue(ui.boolValue->isChecked());
	case 2:
		return QJsonValue(ui.doubleSpinBox->value());
	case 3:
		return QJsonValue();
	default:
		return QJsonValue();
	}
}


JSONPropertyWidget::~JSONPropertyWidget()
{
}
