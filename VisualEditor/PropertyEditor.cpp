#include "PropertyEditor.h"
#include "globals.h"

PropertyEditor::PropertyEditor(JSONWidgetBase* widgetToEdit, QWidget *parent)
	: QWidget(parent),WidgetToEdit(widgetToEdit)
{
	ui.setupUi(this);

	this->setWindowFlag(Qt::Window);

	this->setWindowTitle("Property Editor - " + WidgetToEdit->Name);
}

PropertyEditor::~PropertyEditor()
{
}

void PropertyEditor::closeEvent(QCloseEvent*)
{
	VisualEditorGlobals::IsAnyPropertyBeingEdited = false;
}
