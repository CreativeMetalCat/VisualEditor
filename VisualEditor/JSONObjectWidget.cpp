#include "JSONObjectWidget.h"
#include "ui_JSONObjectWidget.h"
#include "JSONPropertyWidget.h"
#include <QJsonObject>
#include <QInputDialog>
#include <QMouseEvent>
#include < QDragEnterEvent>
#include <QMimeData>

JSONObjectWidget::JSONObjectWidget(QJsonObject jsonObject, QWidget *parent, QString name, bool AllowNameChange)
	: JSONWidgetBase(parent,name)
{
	ui = new Ui::JSONObjectWidget();
	ui->setupUi(this);

	Type = QJsonValue::Object;

	QStringList keys = jsonObject.keys();

	ui->groupBox->setTitle(name);
	
	ui->groupBox->setProperty("AllowNameChange", AllowNameChange);

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
	ui->groupBox->setAcceptDrops(true);
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
	if (object == ui->groupBox)
	{
		if (event->type() == QEvent::MouseButtonPress && ui->groupBox->property("AllowNameChange").toBool())
		{
			QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
			if (mouseEvent->button() == Qt::LeftButton)
			{
				bool ok = false;
				QString newTitle = QInputDialog::getText(this, "Enter new name", "", QLineEdit::Normal, ui->groupBox->title(), &ok);
				if (newTitle != "" && ok)
				{
					ui->groupBox->setTitle(newTitle);
				}
				return true;
			}
			else if (mouseEvent->button() == Qt::RightButton)
			{
				//spawn a tree view with buttons to select which object to reparent to
			}
		}
		else if(event->type() == QEvent::DragEnter)
		{
			
			QDragEnterEvent* dragEvent = static_cast<QDragEnterEvent*>(event);
			if (dragEvent->mimeData()->hasFormat("toolbox/property"))
			{
				dragEvent->acceptProposedAction();
				return true;
			}
			if (dragEvent->mimeData()->hasFormat("toolbox/object"))
			{
				dragEvent->acceptProposedAction();
				return true;
			}
			
		}
		else if (event->type() == QEvent::Drop)
		{
			QDropEvent* drop = static_cast<QDropEvent*>(event);
			if (drop->mimeData()->hasFormat("toolbox/property"))
			{
				return true;
				//spawn new empty property
				JSONPropertyWidget* prop = new JSONPropertyWidget(this);
				ui->verticalLayoutBox->addWidget(prop);
			}
			if (drop->mimeData()->hasFormat("toolbox/object"))
			{
				if (drop->mimeData()->hasFormat("toolbox/customObjectInfo"))
				{
					qWarning() << QString::fromUtf8(drop->mimeData()->data("toolbox/customObjectInfo"));
				}
				else
				{
					//spawn new empty object
					JSONObjectWidget* obj = new JSONObjectWidget(QJsonObject(), this);
					ui->verticalLayoutBox->addWidget(obj);
				}
			}
			return true;
		}
	}
	return false;
}

JSONObjectWidget::~JSONObjectWidget()
{
	delete ui;
}
