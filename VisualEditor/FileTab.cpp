 #include "FileTab.h"
#include <QFile>
#include <QFile>
#include <QMessageBox>
#include <QJsonDocument>

FileTabWidget::FileTabWidget(QString filePath, QWidget *parent)
	: QWidget(parent),FilePath(filePath)
{
	ui.setupUi(this);

    QWidget* result = new QWidget(this);

    scrollWidget = new QWidget(this);
    VerticalLayout = new QVBoxLayout(scrollWidget);
    ui.scrollArea->setWidget(scrollWidget);

    QFile file(filePath);
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

    fileObject = new JSONObjectWidget(docObject, this, "file", false);
    VerticalLayout->addWidget(fileObject);
}

FileTabWidget::~FileTabWidget()
{
}
