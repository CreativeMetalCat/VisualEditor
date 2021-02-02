#pragma once

#include <QWidget>
#include <QJsonObject>
#include <QJsonValue>
#include "ui_JSONWidgetBase.h"

class JSONWidgetBase : public QWidget
{
	Q_OBJECT

public:
	JSONWidgetBase(QWidget *parent = Q_NULLPTR, QString name = "object");
	~JSONWidgetBase();

	//Name of the object that will be used in json file generation
	//*Has no effect on how editor works
	QString Name;

	virtual QJsonValue GenerateJsonValue();

protected:
	//Type of the object that will be used in json file generation
	QJsonValue::Type Type = QJsonValue::Type::Undefined;

private:
	Ui::JSONWidgetBase ui;
};