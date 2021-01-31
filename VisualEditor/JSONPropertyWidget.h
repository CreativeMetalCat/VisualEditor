#pragma once

#include <QWidget>
#include "JSONWidgetBase.h"
#include "ui_JSONPropertyWidget.h"

class JSONPropertyWidget : public JSONWidgetBase
{
	Q_OBJECT

public:
	JSONPropertyWidget(QWidget *parent = Q_NULLPTR, QString name = "object");
	~JSONPropertyWidget();

	virtual QJsonValue GenerateJsonValue() override;
private:
	Ui::JSONPropertyWidget ui;

private slots:
	void OnTypeSelectionChanged(QString selection);
};
