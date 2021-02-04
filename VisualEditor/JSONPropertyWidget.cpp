#include "JSONPropertyWidget.h"
#include "PropertyEditor.h"
#include "JSONObjectWidget.h"
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

JSONPropertyWidget::JSONPropertyWidget(QWidget *parent, QString name, QJsonValue value)
	: JSONWidgetBase(parent,name)
{
	ui.setupUi(this);

	ui.typeBox->addItem("Null");
    ui.typeBox->addItem("Boolean");
    ui.typeBox->addItem("Number");
    ui.typeBox->addItem("Text");

	connect(ui.typeBox, &QComboBox::currentTextChanged, this, &JSONPropertyWidget::OnTypeSelectionChanged);
	connect(ui.nameEdit, &QLineEdit::textChanged, this, &JSONPropertyWidget::OnTextChanged);

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

void JSONPropertyWidget::mousePressEvent(QMouseEvent*event)
{
	//start drag if we touched this with mouse
	if (!VisualEditorGlobals::IsAnyObjectBeingMoved)
	{
		//by design every property is child of at least one object -> root object
		//but the first parent() would return groupBox
		if (JSONObjectWidget* parentObj = qobject_cast<JSONObjectWidget*>(parent()->parent()))
		{
			//record id to reduce calls
			const int id = parentObj->GetFileObject()->GetListOfAllJsonWidgets().indexOf(this, 0);
			//if id was not found - no point in dragging this object
			if (id > 0)
			{

				QDrag* drag = new QDrag(this);
				QMimeData* mimeData = new QMimeData;

				//"veeditor/movedObject" is type existing just for this app
				mimeData->setData("veeditor/movedObject", QString::number(id).toUtf8());

				drag->setMimeData(mimeData);

				Qt::DropAction dropAction = drag->exec();

				//technically this should be preventing mouse grabbing the whole tree, but for whatever reason even keeps going deeper into the tree
				event->accept();

				//to prevent grabbing the whole tree when dragging
				VisualEditorGlobals::IsAnyObjectBeingMoved = true;
			}
		}
	}
}

void JSONPropertyWidget::mouseReleaseEvent(QMouseEvent*)
{
	VisualEditorGlobals::IsAnyObjectBeingMoved = false;
}

void JSONPropertyWidget::OnIdSpinBoxValueChanged(int newId)
{
	 ChangeChildId(newId);
}

void JSONPropertyWidget::OnTextChanged(QString text)
{
	if (text != "")
	{
		Name = text;
	}
}


JSONPropertyWidget::~JSONPropertyWidget()
{
}
