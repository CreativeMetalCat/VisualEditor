#include "JSONPropertyWidget.h"
#include "PropertyEditor.h"

JSONPropertyWidget::JSONPropertyWidget(QWidget *parent, QString name, QJsonValue value)
	: JSONWidgetBase(parent,name)
{
	ui.setupUi(this);

	ui.typeBox->addItem("Null");
    ui.typeBox->addItem("Boolean");
    ui.typeBox->addItem("Number");
    ui.typeBox->addItem("Text");

	connect(ui.typeBox, &QComboBox::currentTextChanged, this, &JSONPropertyWidget::OnTypeSelectionChanged);

	if (value != QJsonValue())
	{
		switch (value.type())
		{
		case  QJsonValue::String:
			ui.typeBox->setCurrentIndex(0);
			ui.stackedWidget->setCurrentIndex(0);
			ui.valueEdit->setText(value.toString());
			break;
		case  QJsonValue::Bool:
			ui.typeBox->setCurrentIndex(1);
			ui.stackedWidget->setCurrentIndex(1);
			ui.boolValue->setChecked(value.toBool());
			break;
		case  QJsonValue::Double:
			ui.typeBox->setCurrentIndex(2);
			ui.stackedWidget->setCurrentIndex(2);
			ui.doubleSpinBox->setValue(value.toDouble());
			break;
		}
		ui.nameEdit->setText(name);
	}
	else
	{
		ui.typeBox->setCurrentIndex(3);
		ui.stackedWidget->setCurrentIndex(3);
	}
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

void JSONPropertyWidget::contextMenuEvent(QContextMenuEvent*event)
{
	if (!VisualEditorGlobals::IsAnyPropertyBeingEdited)
	{
		//open a property editor window
		//parent()->parent() = First parent() is the object window, second parent() is actual parent of the object window
		if (JSONWidgetBase* obj = qobject_cast<JSONWidgetBase*>(parent()->parent()))
		{
			PropertyEditor* propEdit = new PropertyEditor(this, this);

			connect(propEdit->GetIdSpinBox(), qOverload<int>(&QSpinBox::valueChanged), obj, &JSONWidgetBase::ChangeChildId);

			connect(propEdit->GetDeleteButton(), &QPushButton::pressed, obj, &JSONWidgetBase::DeleteChild);

			propEdit->showNormal();

			VisualEditorGlobals::IsAnyPropertyBeingEdited = true;
		}
	}
}

void JSONPropertyWidget::OnIdSpinBoxValueChanged(int newId)
{
	 ChangeChildId(newId);
}


JSONPropertyWidget::~JSONPropertyWidget()
{
}
