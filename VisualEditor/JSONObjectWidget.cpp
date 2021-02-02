#include "JSONObjectWidget.h"
#include "ui_JSONObjectWidget.h"
#include "JSONPropertyWidget.h"
#include <QJsonObject>
#include <QInputDialog>
#include <QMouseEvent>
#include < QDragEnterEvent>
#include <QMimeData>
#include "PropertyEditor.h"

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
		int currentId = 0;
		for (auto it = keys.begin(); it != keys.end(); ++it)
		{
			if (jsonObject.value((*it)).isObject())
			{
				//generate object
				JSONObjectWidget* object = new JSONObjectWidget(jsonObject.value((*it)).toObject(), this, (*it));
				object->Id = currentId;
				ui->verticalLayoutBox->addWidget(object);
				ChildObjects.append(object);
			}
			else
			{
				//generate properties
				JSONPropertyWidget* jsonProp = new JSONPropertyWidget(this, (*it), jsonObject.value((*it)));
				jsonProp->Id = currentId;
				ui->verticalLayoutBox->addWidget(jsonProp);
				ChildObjects.append(jsonProp);
			}
			currentId++;
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
	jsonProperty->Id = ChildObjects.count();
	ChildObjects.append(jsonProperty);
}

void JSONObjectWidget::AddNewProperty(QString name, QJsonValue value)
{
	JSONPropertyWidget* jsonProperty = new JSONPropertyWidget(this, name, value);
	ui->verticalLayoutBox->addWidget(jsonProperty);
	jsonProperty->Id = ChildObjects.count();
	ChildObjects.append(jsonProperty);
}

void JSONObjectWidget::ChangeChildId(int newId)
{
	if (ChildObjects.count() > 1 && newId > 0)
	{
		if(PropertyEditor*editor = qobject_cast<PropertyEditor*>(sender()->parent()))
		{
			//we need to find id of widget that is currently occuping that place
			//id of the current object
			//move them and then update id of every other object(by doing same this as was done during creation)

			//check if widget is part of current widgets
			if (ChildObjects.contains(editor->WidgetToEdit))
			{
				int currentId = 0;
				ChildObjects.move(ChildObjects.indexOf(editor->WidgetToEdit, 0), newId);
				for (auto it = ChildObjects.begin(); it != ChildObjects.end(); ++it)
				{				
					(*it)->Id = currentId;
					currentId++;
					//readd widget to reset positions
					ui->verticalLayoutBox->removeWidget(*it);	
					ui->verticalLayoutBox->addWidget(*it);
				}
			}
		}
		else
		{
			qWarning() << sender()->parent()->objectName();
		}
	}
}

void JSONObjectWidget::DeleteChild()
{
	if (!ChildObjects.empty())
	{
		if (PropertyEditor* editor = qobject_cast<PropertyEditor*>(sender()->parent()))
		{
			ChildObjects.removeOne(editor->WidgetToEdit);

			ui->verticalLayoutBox->removeWidget(editor->WidgetToEdit);

			delete editor->WidgetToEdit;

			VisualEditorGlobals::IsAnyPropertyBeingEdited = false;
		}
	}
}

bool JSONObjectWidget::eventFilter(QObject* object, QEvent* event)
{
	if (object == ui->groupBox )
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
					Name = newTitle;
				}
				mouseEvent->accept();
				return true;
			}
		}
		else if (event->type() == QEvent::ContextMenu && !VisualEditorGlobals::IsAnyPropertyBeingEdited)
		{
			//open a property editor window
			//parent()->parent() = First parent() is the object window, second parent() is actual parent of the object window
			if (JSONObjectWidget* obj = qobject_cast<JSONObjectWidget*>(parent()->parent()))
			{
				PropertyEditor* propEdit = new PropertyEditor(this, this);				

				connect(propEdit->GetIdSpinBox(), qOverload<int>(&QSpinBox::valueChanged), obj, &JSONObjectWidget::ChangeChildId);

				connect(propEdit->GetDeleteButton(), &QPushButton::pressed, obj, &JSONObjectWidget::DeleteChild);

				propEdit->showNormal();

				VisualEditorGlobals::IsAnyPropertyBeingEdited = true;
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
				//spawn new empty property
				JSONPropertyWidget* prop = new JSONPropertyWidget(this);
				ui->verticalLayoutBox->addWidget(prop);
				prop->Id = ChildObjects.count();
				ChildObjects.append(prop);
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

					obj->Id = ChildObjects.count();
					ChildObjects.append(obj);
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
