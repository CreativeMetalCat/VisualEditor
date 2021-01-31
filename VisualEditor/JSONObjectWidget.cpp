#include "JSONObjectWidget.h"
#include "ui_JSONObjectWidget.h"
#include <QJsonObject>

JSONObjectWidget::JSONObjectWidget(QWidget *parent, QString name )
	: JSONWidgetBase(parent,name)
{
	ui = new Ui::JSONObjectWidget();
	ui->setupUi(this);

	Type = QJsonValue::Object;
}

JSONObjectWidget::~JSONObjectWidget()
{
	delete ui;
}

QJsonValue JSONObjectWidget::GenerateJsonValue()
{
	QJsonObject result;

	if (!ChildObjects.empty())
	{
		for (int i = 0; i < ChildObjects.count(); i++)
		{
			result[ChildObjects[i]->Name] = ChildObjects[i]->GenerateJsonValue();
		}
	}

	return result;
}
