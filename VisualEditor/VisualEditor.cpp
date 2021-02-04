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
#include <QStringList>

//Points to User/Documents/JsonVisualEditorsPrefabs
//Should be considered: this could be a variable that can be changed in settings in the future
#define PREFAB_DIRECTORY QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/JsonVisualEditorsPrefabs/"

VisualEditor::VisualEditor(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    toolBoxScrollWidget = new QWidget(this);
    toolBoxScrollVertialLayout = new QVBoxLayout(toolBoxScrollWidget);
    ui.ToolBox->setWidget(toolBoxScrollWidget);

    //add default buttons to tool box
    ToolBoxLabel* propertyButton = new ToolBoxLabel("Property","{}", this, ToolBoxLabel::Type::Property);
    toolBoxScrollVertialLayout->addWidget(propertyButton);

    ToolBoxLabel* objectButton = new ToolBoxLabel("JsonObject","{}", this, ToolBoxLabel::Type::Object);
    toolBoxScrollVertialLayout->addWidget(objectButton);
  
    if (QDir(PREFAB_DIRECTORY).exists())
    {
        QDir prefabDirectory(PREFAB_DIRECTORY);

        QStringList files = prefabDirectory.entryList(QStringList() << "*.vep" << "*.VEP", QDir::Files);

        if (!files.empty())
        {
            //iterate throgh the array and load any prefab that can be found
            for (auto it = files.begin(); it != files.end(); ++it)
            {
                QFile file(PREFAB_DIRECTORY  + *it);
                if (file.open(QFile::ReadOnly | QFile::Text))
                {
                    //read file and immediately close it to avoid any issues
                    QString val = file.readAll();
                    file.close();

                    ToolBoxLabel* prefabButton = new ToolBoxLabel(*it, val, this, ToolBoxLabel::Type::CustomObject);
                    toolBoxScrollVertialLayout->addWidget(prefabButton);
                }
                else
                {
                    qWarning() << "Failed to open prefab!: " + (*it);
                }
            }
        }
    }

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
