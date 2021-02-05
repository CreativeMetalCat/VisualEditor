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
#include "InfoWidget.h"
#include "SearchWindowWidget.h"
#include <QScrollBar>

//Points to User/Documents/JsonVisualEditorsPrefabs
//Should be considered: this could be a variable that can be changed in settings in the future
#define PREFAB_DIRECTORY QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/JsonVisualEditorsPrefabs/"

#define DOCUMENTS_DIR QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)

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

    connect(ui.tabWidget, &QTabWidget::tabCloseRequested, this, &VisualEditor::OnTabClosed);

    connect(ui.actionSave_File, &QAction::triggered, this, &VisualEditor::SaveCurrentFile);

    connect(ui.actionOpen_2, &QAction::triggered, this, &VisualEditor::OpenNewFile);

    connect(ui.actionNew_File, &QAction::triggered, this,[this]()
    {
        //create tab with empty name
        FileTabWidget* newTab = new FileTabWidget("new file", this);
        ui.tabWidget->addTab(newTab, newTab->FilePath);
    });

    connect(ui.actionInfo, &QAction::triggered, this, [this]()
    {
        InfoWidget* info = new InfoWidget(this);
        info->showNormal();
    });

    connect(ui.actionSave_as, &QAction::triggered, this, &VisualEditor::SaveCurrentFileAs);

    connect(ui.actionSearch, &QAction::triggered, this, [this]()
    {
        //check if any search window is open
        if (!IsSearchWindowOpen)
        {
            IsSearchWindowOpen = true;

            if (FileTabWidget* tab = qobject_cast<FileTabWidget*>(ui.tabWidget->currentWidget()))
            {
                auto list = tab->fileObject->GetListOfAllJsonWidgets();
                //spawn widget
                SearchWindowWidget* searchWidget = new SearchWindowWidget(list, this);
                searchWidget->showNormal();

                //connect to close event to reset values
                connect(searchWidget, &QWidget::close, this, [this]()
                {
                    IsSearchWindowOpen = false;
                });

                connect(searchWidget, &SearchWindowWidget::OnSeacrhComplete, this, [this,tab,list](bool success, int id)
                {
                    if (success)
                    {
                        tab->ui.scrollArea->verticalScrollBar()->setValue(list.at(id)->pos().y());

                        tab->ui.scrollArea->horizontalScrollBar()->setValue(list.at(id)->pos().x());
                    }
                    else
                    {
                        QMessageBox::warning(this, "Didn't find!", "Nothing matching the input data was found!");
                    }
                });
            }
        }
    });
}

void VisualEditor::SaveCurrentFile()
{
    if (FileTabWidget* tab = qobject_cast<FileTabWidget*>(ui.tabWidget->currentWidget()))
    {
        bool isNewFile = tab->FilePath == "new file";
        if(isNewFile)
        { 
            tab->FilePath = QFileDialog::getSaveFileName
            (this, "Save file as",
                DOCUMENTS_DIR, 
                ("Json Files (*.json *.bowerrc *jscsrc *webmanifest *js.map *css.map *ts.map *har *jslintrc *jsonId);;Any (*.*)")
            );
            
        }
        QFile file(tab->FilePath);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QMessageBox::warning(this, "Warinig!", "Failed to open file!", QMessageBox::Ok, QMessageBox::Ok);
        }

        //Create and save file
        QJsonDocument boardDocument(tab->fileObject->GenerateJsonValue().toObject());
        file.write(boardDocument.toJson());   
        file.close();

        if (isNewFile)
        {
            FileTabWidget* newTab = new FileTabWidget(tab->FilePath, this);
            ui.tabWidget->addTab(newTab, newTab->FilePath);
        }

        delete tab;
    }
}

void VisualEditor::SaveCurrentFileAs()
{
    if (FileTabWidget* tab = qobject_cast<FileTabWidget*>(ui.tabWidget->currentWidget()))
    {
        tab->FilePath = QFileDialog::getSaveFileName
        (this, "Save file as",
            DOCUMENTS_DIR,
            ("Json Files (*.json *.bowerrc *jscsrc *webmanifest *js.map *css.map *ts.map *har *jslintrc *jsonId);;Any (*.*)")
        );

        QFile file(tab->FilePath);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QMessageBox::warning(this, "Warinig!", "Failed to open file!", QMessageBox::Ok, QMessageBox::Ok);
        }

        //Create and save file
        QJsonDocument boardDocument(tab->fileObject->GenerateJsonValue().toObject());
        file.write(boardDocument.toJson());
        file.close();

        FileTabWidget* newTab = new FileTabWidget(tab->FilePath, this);
        ui.tabWidget->addTab(newTab, newTab->FilePath);


        delete tab;
    }
}

void VisualEditor::OpenNewFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select JSON file", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), ("Json Files (*.json *.bowerrc *jscsrc *webmanifest *js.map *css.map *ts.map *har *jslintrc *jsonId);;Any (*.*)"));
    FileTabWidget* newTab = new FileTabWidget(fileName, this);
    ui.tabWidget->addTab(newTab, newTab->FilePath);
}

void VisualEditor::OnTabClosed(int tabId)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save", "Do you wish to save changes?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (reply == QMessageBox::Yes)
    {
        ui.tabWidget->setCurrentIndex(tabId);
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

            delete tab;
        }
        return;
    }
    if (reply == QMessageBox::No)
    {
        ui.tabWidget->setCurrentIndex(tabId);
        if (FileTabWidget* tab = qobject_cast<FileTabWidget*>(ui.tabWidget->currentWidget()))
        {
            delete tab;
        }
        return;
    }
    if (reply == QMessageBox::Cancel)
    {
        //do nothing and tab won't be closed
    }
}

void VisualEditor::PerformSearch()
{

}
