#include "JSONObjectWidget.h"
#include "ui_JSONObjectWidget.h"
#include "JSONPropertyWidget.h"
#include <QJsonObject>

JSONObjectWidget::JSONObjectWidget(QJsonObject jsonObject, QWidget *parent, QString name )
	: JSONWidgetBase(parent,name)
{
	ui = new Ui::JSONObjectWidget();
	ui->setupUi(this);

	Type = QJsonValue::Object;

	QStringList keys = jsonObject.keys();

	ui->groupBox->setTitle(name);

	if (!keys.empty())
	{
		for (auto it = keys.begin(); it != keys.end(); ++it)
		{
			if (jsonObject.value((*it)).isObject())
			{
				//generate object
				JSONObjectWidget* object = new JSONObjectWidget(jsonObject.value((*it)).toObject(), this, (*it));
				ui->verticalLayoutBox->addWidget(object);
			}
			else
			{
				//generate properties
				JSONPropertyWidget* jsonProp = new JSONPropertyWidget(this, (*it), jsonObject.value((*it)));
				ui->verticalLayoutBox->addWidget(jsonProp);
			}
		}
	}
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

void JSONObjectWidget::AddNewProperty()
{
	JSONPropertyWidget* jsonProperty = new JSONPropertyWidget(this);
	ui->verticalLayoutBox->addWidget(jsonProperty);
}

void JSONObjectWidget::AddNewProperty(QString name, QJsonValue value)
{
	JSONPropertyWidget* jsonProperty = new JSONPropertyWidget(this, name, value);
	ui->verticalLayoutBox->addWidget(jsonProperty);
}
