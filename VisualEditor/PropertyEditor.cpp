#include "PropertyEditor.h"
#include "globals.h"

PropertyEditor::PropertyEditor(JSONWidgetBase* widgetToEdit, QWidget *parent)
	: QWidget(parent),WidgetToEdit(widgetToEdit)
{
	ui.setupUi(this);

	this->setWindowFlag(Qt::Window);
	if (WidgetToEdit)
	{
		this->setWindowTitle("Property Editor - " + WidgetToEdit->Name);

		ui.spinBox->setValue(WidgetToEdit->Id);
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
