#pragma once

#include <QWidget>
#include "ui_FileTab.h"
#include "JSONObjectWidget.h"

class FileTabWidget : public QWidget
{
	Q_OBJECT

public:
	FileTabWidget(QString filePath = "json/file.json",QWidget *parent = Q_NULLPTR);
	~FileTabWidget();


	QString FilePath;
	Ui::FileTab ui;

private slots:

private:
	

	QVBoxLayout* VerticalLayout;
	QWidget* scrollWidget;
public:
	JSONObjectWidget* fileObject;
};
