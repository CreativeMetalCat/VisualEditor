#include "JSONObjectWidget.h"
#include "ui_JSONObjectWidget.h"
#include "JSONPropertyWidget.h"
#include <QJsonObject>
#include <QInputDialog>

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

	ui->groupBox->installEventFilter(this);
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

bool JSONObjectWidget::eventFilter(QObject* object, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonPress && object == ui->groupBox)
	{
		bool ok = false;
		QString newTitle = QInputDialog::getText(this, "Enter new name", "",QLineEdit::Normal, ui->groupBox->title(),&ok);
		if (newTitle != "" && ok)
		{
			ui->groupBox->setTitle(newTitle);
		}
		return true;
	}
	return false;
}

JSONObjectWidget::~JSONObjectWidget()
{
	delete ui;
}
