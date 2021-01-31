#pragma once

#include <QWidget>
#include "JSONWidgetBase.h"
namespace Ui { class JSONObjectWidget; };

class JSONObjectWidget : public JSONWidgetBase
{
	Q_OBJECT

public:
	JSONObjectWidget(QWidget *parent = Q_NULLPTR, QString name = "object");
	~JSONObjectWidget();

	QVector<JSONWidgetBase*>ChildObjects = QVector<JSONWidgetBase*>();

	virtual QJsonValue GenerateJsonValue() override;

private:
	Ui::JSONObjectWidget *ui;
};
