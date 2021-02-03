#include "PropertyEditor.h"
#include "globals.h"
#include "JSONPropertyWidget.h"

PropertyEditor::PropertyEditor(JSONWidgetBase* widgetToEdit, QWidget *parent)
	: QWidget(parent),WidgetToEdit(widgetToEdit)
{
	ui.setupUi(this);

	this->setWindowFlag(Qt::Window);
	if (WidgetToEdit)
	{
		this->setWindowTitle("Property Editor - " + WidgetToEdit->Name);

		ui.spinBox->setValue(WidgetToEdit->Id);
		
		if (qobject_cast<JSONPropertyWidget*>(WidgetToEdit))
		{
			ui.isArrayCheckBox->setEnabled(false);

			ui.lineEdit->setVisible(false);
			ui.label->setVisible(false);
		}
		else
		{
			ui.isArrayCheckBox->setEnabled(true);
		}
	}
	else
	{
		close();
	}
}

PropertyEditor::~PropertyEditor()
{
}

void PropertyEditor::closeEvent(QCloseEvent*)
{
	VisualEditorGlobals::IsAnyPropertyBeingEdited = false;
}

QSpinBox* PropertyEditor::GetIdSpinBox()
{
	return ui.spinBox;
}

QPushButton* PropertyEditor::GetDeleteButton()
{
	return ui.pushButton;
}

QCheckBox* PropertyEditor::GetIsArrayCheckBox()
{
	return ui.isArrayCheckBox;
}

QLineEdit* PropertyEditor::GetNameEdit()
{
	return ui.lineEdit;
}
