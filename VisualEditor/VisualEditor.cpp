#include "VisualEditor.h"
#include <QFile>
#include <QMessageBox>
#include <QJsonDocument>
#include "JSONObjectWidget.h"
#include "JSONPropertyWidget.h"
#include <QListView>
#include "ToolBoxLabel.h"

VisualEditor::VisualEditor(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    scrollWidget = new QWidget(this);
    VerticalLayout = new QVBoxLayout(scrollWidget);
    ui.scrollArea->setWidget(scrollWidget);

    QFile file("json/test.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Warinig!", "Failed to open file!", QMessageBox::Ok, QMessageBox::Ok);
    }
    QString val;
    val = file.readAll();
    file.close();

    //load document and create JSON object to work with
    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject docObject = document.object();

    QStringList keys = docObject.keys();

    fileObject = new JSONObjectWidget(docObject, this, "file",false);
    VerticalLayout->addWidget(fileObject);

    toolBoxScrollWidget = new QWidget(this);
    toolBoxScrollVertialLayout = new QVBoxLayout(toolBoxScrollWidget);
    ui.ToolBox->setWidget(toolBoxScrollWidget);

    //add default buttons to tool box
    ToolBoxLabel* propertyButton = new ToolBoxLabel("Property", this, ToolBoxLabel::Type::Property);
    toolBoxScrollVertialLayout->addWidget(propertyButton);

    ToolBoxLabel* objectButton = new ToolBoxLabel("JsonObject", this, ToolBoxLabel::Type::Object);
    toolBoxScrollVertialLayout->addWidget(objectButton);

    connect(ui.actionSave_File, &QAction::triggered, this, &VisualEditor::SaveCurrentFile);
}

void VisualEditor::SaveCurrentFile()
{
    QFile file("json/test.json");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QMessageBox::warning(this, "Warinig!", "Failed to open file!", QMessageBox::Ok, QMessageBox::Ok);
    }

    //Create and save file
    QJsonDocument boardDocument(fileObject->GenerateJsonValue().toObject());
    file.write(boardDocument.toJson());

}