#include "JSONObjectWidget.h"
#include "ui_JSONObjectWidget.h"
#include "JSONPropertyWidget.h"
#include <QJsonObject>
#include <QInputDialog>
#include <QMouseEvent>
#include < QDragEnterEvent>
#include <QMimeData>
#include "PropertyEditor.h"
#include <QJsonArray>
#include <QDrag>

JSONObjectWidget::JSONObjectWidget(QJsonObject jsonObject, QWidget *parent, QString name, bool AllowNameChange,bool isArray)
	: JSONWidgetBase(parent,name),IsArray(isArray)
{
	ui = new Ui::JSONObjectWidget();
	ui->setupUi(this);

	Type = QJsonValue::Object;

	QStringList keys = jsonObject.keys();

	ui->groupBox->setTitle(name + QString(IsArray ? "[ ]" : ""));
	
	ui->groupBox->setProperty("AllowNameChange", AllowNameChange);

	if (!keys.empty())
	{
		int currentId = 0;
		for (auto it = keys.begin(); it != keys.end(); ++it)
		{
			if (jsonObject.value((*it)).isObject() || jsonObject.value((*it)).isArray())
			{
				//generate object
				JSONObjectWidget* object;
				if (jsonObject.value((*it)).isArray())
				{
					object = new JSONObjectWidget(jsonObject.value((*it)).toArray(), this, (*it), true, true);
				}
				else
				{
					object = new JSONObjectWidget(jsonObject.value((*it)).toObject(), this, (*it));
				}
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

JSONObjectWidget::JSONObjectWidget(QJsonArray jsonArray, QWidget* parent, QString name, bool AllowNameChange,bool isArray)
	: JSONWidgetBase(parent, name), IsArray(isArray)
{

	ui = new Ui::JSONObjectWidget();
	ui->setupUi(this);

	Type = QJsonValue::Object;

	ui->groupBox->setTitle(name + QString(IsArray ? "[ ]" : ""));

	ui->groupBox->setProperty("AllowNameChange", AllowNameChange);

	int currentId = 0;
	for (auto it = jsonArray.begin(); it != jsonArray.end(); ++it)
	{
		if ((*it).isObject() || (*it).isArray())
		{
			//generate object
			JSONObjectWidget* object;
			if ((*it).isArray())
			{
				object = new JSONObjectWidget((*it).toArray(), this, "arrayItem" + QString(currentId), true, true);
			}
			else
			{
				object = new JSONObjectWidget((*it).toObject(), this, "arrayItem" + QString(currentId));
			}
			object->Id = currentId;
			ui->verticalLayoutBox->addWidget(object);
			ChildObjects.append(object);
		}
		else
		{
			//generate properties
			JSONPropertyWidget* jsonProp = new JSONPropertyWidget(this, "arrayItem" + QString(currentId),(*it));
			jsonProp->Id = currentId;
			ui->verticalLayoutBox->addWidget(jsonProp);
			ChildObjects.append(jsonProp);
		}
		currentId++;
	}



	ui->groupBox->installEventFilter(this);
	ui->groupBox->setAcceptDrops(true);
}

QJsonValue JSONObjectWidget::GenerateJsonValue()
{
	if (IsArray)
	{
		//objects in array can not have names
		QJsonArray array;
		if (!ChildObjects.empty())
		{
			for (int i = 0; i < ChildObjects.count(); i++)
			{
				array.append(ChildObjects[i]->GenerateJsonValue());
			}
		}
		return QJsonValue(array);
	}
	else
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

JSONObjectWidget* JSONObjectWidget::GetFileObject(JSONObjectWidget* start)
{
	//parent is still JSON object so we ask it to get parent for us
	//first parent will always be groupBox and parent of that is the actual object
	if (JSONObjectWidget * parentObj = qobject_cast<JSONObjectWidget*>((start? start->parent(): parent())->parent()))
	{
		return  parentObj->GetFileObject();
	}
	//if parent is not a json object we found the "file" core
	else
	{
		return this;
	}
}

QVector<JSONWidgetBase*> JSONObjectWidget::GetListOfAllJsonWidgets()
{
	if (!ChildObjects.empty())
	{
		QVector<JSONWidgetBase*> result = QVector<JSONWidgetBase*>();
		result.append(this);
		for (auto it = ChildObjects.begin(); it != ChildObjects.end(); ++it)
		{
			if (JSONObjectWidget* obj = qobject_cast<JSONObjectWidget*>((*it)))
			{
				result.append(obj->GetListOfAllJsonWidgets());
			}
			else
			{
				result.append((*it));
			}		
		}
		return result;
	}
	return QVector<JSONWidgetBase*>();
}

bool JSONObjectWidget::eventFilter(QObject* object, QEvent* event)
{
	if (object == ui->groupBox )
	{
		if (event->type() == QEvent::MouseButtonPress && ui->groupBox->property("AllowNameChange").toBool())
		{
			QDrag* drag = new QDrag(this);
			QMimeData* mimeData = new QMimeData;

			mimeData->setData("veeditor/movedObject", QString::number(GetFileObject()->GetListOfAllJsonWidgets().indexOf(this, 0)).toUtf8());

			drag->setMimeData(mimeData);

			Qt::DropAction dropAction = drag->exec();

			event->accept();
		}
		else if (event->type() == QEvent::ContextMenu && !VisualEditorGlobals::IsAnyPropertyBeingEdited)
		{
			//open a property editor window
			//parent()->parent() = First parent() is the object window, second parent() is actual parent of the object window
			if (JSONObjectWidget* obj = qobject_cast<JSONObjectWidget*>(parent()->parent()))
			{
				PropertyEditor* propEdit = new PropertyEditor(this, this);		

				connect(propEdit->GetIdSpinBox(), qOverload<int>(&QSpinBox::valueChanged), obj, &JSONWidgetBase::ChangeChildId);

				connect(propEdit->GetDeleteButton(), &QPushButton::pressed, obj, &JSONWidgetBase::DeleteChild);

				connect(propEdit->GetIsArrayCheckBox(), &QCheckBox::stateChanged, this, &JSONObjectWidget::OnIsArrayChanged);

				connect(propEdit->GetNameEdit(), &QLineEdit::textChanged, this, &JSONObjectWidget::OnNameChanged);

				propEdit->GetNameEdit()->setText(Name);

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
			if (dragEvent->mimeData()->hasFormat("veeditor/movedObject"))
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
			if (drop->mimeData()->hasFormat("veeditor/movedObject"))
			{
				qWarning() << QString::fromUtf8(drop->mimeData()->data("veeditor/movedObject")).toInt();

				auto child  = GetFileObject()->GetListOfAllJsonWidgets().at(QString::fromUtf8(drop->mimeData()->data("veeditor/movedObject")).toInt());
				if (JSONObjectWidget* obj = qobject_cast<JSONObjectWidget*>(child->parent()))
				{
					obj->ChildObjects.removeOne(obj);
				}

				child->setParent(this);
				ChildObjects.append(child);
			}
			return true;
		}
	}
	return false;
}

void JSONObjectWidget::OnIsArrayChanged(bool newState)
{
	IsArray = newState;
	ui->groupBox->setTitle(Name + QString(IsArray ? "[ ]" : ""));
}

void JSONObjectWidget::OnNameChanged(QString newName)
{
	if (newName != "" )
	{
		ui->groupBox->setTitle(newName + QString(IsArray ? "[]" : ""));
		Name = newName;
	}
}

JSONObjectWidget::~JSONObjectWidget()
{
	delete ui;
}
