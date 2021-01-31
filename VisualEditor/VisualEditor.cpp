#include "VisualEditor.h"
#include "JSONPropertyWidget.h"

VisualEditor::VisualEditor(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    JSONPropertyWidget* test = new JSONPropertyWidget(this);
    ui.verticalLayout->addWidget(test);
}
