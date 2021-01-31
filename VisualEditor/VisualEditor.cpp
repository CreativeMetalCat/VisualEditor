#include "VisualEditor.h"
#include <QFile>
#include <QMessageBox>
#include <QJsonDocument>
#include "JSONObjectWidget.h"
#include "JsonEditorTreeModel.h"
#include "JSONPropertyWidget.h"
#include <QListView>

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

    if (!keys.empty())
    {
        for (auto it = keys.begin(); it != keys.end(); ++it)
        {
            if (docObject.value((*it)).isObject())
            {
                JSONObjectWidget* object = new JSONObjectWidget(docObject.value((*it)).toObject(), this, (*it));
                VerticalLayout->addWidget(object);
                
            }
            else
            {
                //generate properties
                JSONPropertyWidget* jsonProp = new JSONPropertyWidget(this, (*it), docObject.value((*it)));
                VerticalLayout->addWidget(jsonProp);
            }
        }
    }
}
