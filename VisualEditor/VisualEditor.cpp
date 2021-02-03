#include "VisualEditor.h"
#include <QFile>
#include <QMessageBox>
#include <QJsonDocument>
#include "JSONObjectWidget.h"
#include "JSONPropertyWidget.h"
#include <QListView>
#include <QFileDialog>
#include <QStandardPaths>
#include "ToolBoxLabel.h"

VisualEditor::VisualEditor(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    toolBoxScrollWidget = new QWidget(this);
    toolBoxScrollVertialLayout = new QVBoxLayout(toolBoxScrollWidget);
    ui.ToolBox->setWidget(toolBoxScrollWidget);

    //add default buttons to tool box
    ToolBoxLabel* propertyButton = new ToolBoxLabel("Property", this, ToolBoxLabel::Type::Property);
    toolBoxScrollVertialLayout->addWidget(propertyButton);

    ToolBoxLabel* objectButton = new ToolBoxLabel("JsonObject", this, ToolBoxLabel::Type::Object);
    toolBoxScrollVertialLayout->addWidget(objectButton);

    connect(ui.actionSave_File, &QAction::triggered, this, &VisualEditor::SaveCurrentFile);

    connect(ui.actionOpen_2, &QAction::triggered, this, &VisualEditor::OpenNewFile);
}

void VisualEditor::SaveCurrentFile()
{
    if (FileTabWidget* tab = qobject_cast<FileTabWidget*>(ui.tabWidget->currentWidget()))
    {
        QFile file(tab->FilePath);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QMessageBox::warning(this, "Warinig!", "Failed to open file!", QMessageBox::Ok, QMessageBox::Ok);
        }

        //Create and save file
        QJsonDocument boardDocument(tab->fileObject->GenerateJsonValue().toObject());
        file.write(boardDocument.toJson());
    }
}

void VisualEditor::OpenNewFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select JSON file", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), ("Json Files (*.json *.bowerrc *jscsrc *webmanifest *js.map *css.map *ts.map *har *jslintrc *jsonId);;Any (*.*)"));
    FileTabWidget* newTab = new FileTabWidget(fileName, this);
    ui.tabWidget->addTab(newTab, newTab->FilePath);
}
