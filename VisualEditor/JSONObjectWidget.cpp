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
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>
#include <QJsonDocument>

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

	connect(this, &JSONWidgetBase::OnChanged, GetFileObject(), &JSONObjectWidget::OnChildChanged);
}

JSONObjectWidget::JSONObjectWidget(QJsonArray jsonArray, QWidget* parent, QString name, bool AllowNameChange,bool isArray)
	: JSONWidgetBase(parent, name), IsArray(isArray)
{

	ui = new Ui::JSONObjectWidget();
	ui->setupUi(this);

	Type = QJsonValue::Object;

	ui->groupBox->setTitle(name + QString(IsArray ? "[ ]" : ""));

	//to prevent moving or renaming of the root
	ui->groupBox->setProperty("AllowNameChange", AllowNameChange);

	//id used for better work with children
	int currentId = 0;
	for (auto it = jsonArray.begin(); it != jsonArray.end(); ++it)
	{
		if ((*it).isObject() || (*it).isArray())
		{
			//generate object
			JSONObjectWidget* object;
			//arrays are generated *slightly* differently from usual objects
			if ((*it).isArray())
			{
				object = new JSONObjectWidget((*it).toArray(), this, "arrayItem" + QString::number(currentId), true, true);
			}
			else
			{
				object = new JSONObjectWidget((*it).toObject(), this, "arrayItem" + QString::number(currentId));
			}
			//default info for every object -> mostly used for convience of layout creation
			object->Id = currentId;
			//add those object to window and array that will be recorded
			ui->verticalLayoutBox->addWidget(object);
			ChildObjects.append(object);
		}
		else
		{
			//generate properties
			JSONPropertyWidget* jsonProp = new JSONPropertyWidget(this, "arrayItem" + QString::number(currentId),(*it));

			//default info for every object -> mostly used for convience of layout creation
			jsonProp->Id = currentId;
			//add those object to window and array that will be recorded
			ui->verticalLayoutBox->addWidget(jsonProp);
			ChildObjects.append(jsonProp);
		}
		currentId++;
	}


	//setup event filters to allow for dragging, property editing to occur
	ui->groupBox->installEventFilter(this);
	ui->groupBox->setAcceptDrops(true);

	connect(this, &JSONWidgetBase::OnChanged, GetFileObject(), &JSONObjectWidget::OnChildChanged);

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
				//doing it this way would imply creating nameless objects
				array.append(ChildObjects[i]->GenerateJsonValue());
			}
		}
		//we have to convert it to value
		return QJsonValue(array);
	}
	else
	{
		QJsonObject result;
		if (!ChildObjects.empty())
		{
			for (int i = 0; i < ChildObjects.count(); i++)
			{
				//add property to the object
				//if property is an object it would simply generate same function -> basicaly recursion
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

void JSONObjectWidget::OnChildChanged(EditorActions::SEditorAction* action)
{
	emit OnChangeInChild(action);

	this->Name;
}

void JSONObjectWidget::ChangeChildId(int newId)
{
	//no point in trying to change child id if it's the only child or given id is bad
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
				ChildObjects.move(ChildObjects.indexOf(editor->WidgetToEdit, 0), newId);

				ShakeChildren();
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
	//if we have noi children - there is nothing to delete
	if (!ChildObjects.empty())
	{
		//we have to get the editor window to get widget to delete
		if (PropertyEditor* editor = qobject_cast<PropertyEditor*>(sender()->parent()))
		{
			//remove it from list so it would not be written into the file
			ChildObjects.removeOne(editor->WidgetToEdit);

			//remove it from window
			ui->verticalLayoutBox->removeWidget(editor->WidgetToEdit);

			//delete it fully to avoid bugs
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
	else if (JSONObjectWidget* parentObj = qobject_cast<JSONObjectWidget*>(parent()))
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
	//create array to return
	QVector<JSONWidgetBase*> result = QVector<JSONWidgetBase*>();
	//it doesn't matter if this object has child objects - it needs to record self
	result.append(this);
	//to avoid unnessesary looping
	if (!ChildObjects.empty())
	{		
		for (auto it = ChildObjects.begin(); it != ChildObjects.end(); ++it)
		{
			if (JSONObjectWidget* obj = qobject_cast<JSONObjectWidget*>((*it)))
			{
				//it it's object we ask it to do same thing as this object is doing
				result.append(obj->GetListOfAllJsonWidgets());
			}
			else
			{
				//if it's property - we just add it
				result.append((*it));
			}		
		}
		
	}

	return result;
}

void JSONObjectWidget::ShakeChildren()
{
	if (!ChildObjects.isEmpty())
	{
		int currentId = 0;
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

bool JSONObjectWidget::eventFilter(QObject* object, QEvent* event)
{
	if (object == ui->groupBox)
	{
		if (event->type() == QEvent::MouseButtonPress && ui->groupBox->property("AllowNameChange").toBool())
		{
			//we drag with left button
			if (static_cast<QMouseEvent*>(event)->button() == Qt::LeftButton && !VisualEditorGlobals::IsAnyObjectBeingMoved)
			{
				//record id to reduce calls
				const int id = GetFileObject()->GetListOfAllJsonWidgets().indexOf(this, 0);
				//if id was not found - no point in dragging this object
				if (id > 0)
				{
					
					QDrag* drag = new QDrag(this);
					QMimeData* mimeData = new QMimeData;

					//"veeditor/movedObject" is type existing just for this app
					mimeData->setData("veeditor/movedObject", QString::number(id).toUtf8());

					drag->setMimeData(mimeData);

					Qt::DropAction dropAction = drag->exec();

					event->accept();

					//to prevent grabbing the whole tree when dragging
					VisualEditorGlobals::IsAnyObjectBeingMoved = true;
				}
			}
		}
		else if (event->type() == QEvent::ContextMenu && !VisualEditorGlobals::IsAnyPropertyBeingEdited)
		{
			//open a property editor window
			//parent()->parent() = First parent() is the object window, second parent() is actual parent of the object window
			if (JSONObjectWidget* obj = qobject_cast<JSONObjectWidget*>(parent()->parent()))
			{
				//create new window and setup it's events
				PropertyEditor* propEdit = new PropertyEditor(this, this);

				connect(propEdit->GetIdSpinBox(), qOverload<int>(&QSpinBox::valueChanged), obj, [this](int newId) {ChangeChildId(newId); });

				connect(propEdit->GetDeleteButton(), &QPushButton::pressed, obj, [this]() { DeleteChild(); });

				connect(propEdit->GetIsArrayCheckBox(), &QCheckBox::stateChanged, this, [this](bool newState)
				{
					IsArray = newState;
					ui->groupBox->setTitle(Name + QString(IsArray ? "[ ]" : ""));
				});

				connect(propEdit->GetNameEdit(), &QLineEdit::textChanged, this, [this](QString newName)
				{
					if (newName != "")
					{
						ui->groupBox->setTitle(newName + QString(IsArray ? "[]" : ""));
						Name = newName;
					}
				});

				//the reason why it doesn't use getter function -> i got tired of creating them and they don't benefit anyone
				connect(propEdit->ui.button_createPrefab, &QPushButton::pressed, this, &JSONObjectWidget::CreatePrefab);

				propEdit->GetNameEdit()->setText(Name);

				propEdit->showNormal();

				//to avoid opening edtiors for each parent item
				VisualEditorGlobals::IsAnyPropertyBeingEdited = true;
			}
		}
		else if (event->type() == QEvent::DragEnter)
		{
			QDragEnterEvent* dragEvent = static_cast<QDragEnterEvent*>(event);
			//it it's types used by this app-> accept them
			if (dragEvent->mimeData()->hasFormat("toolbox/property")
				|| dragEvent->mimeData()->hasFormat("toolbox/object") 
				|| dragEvent->mimeData()->hasFormat("toolbox/customObjectInfo")
				|| dragEvent->mimeData()->hasFormat("veeditor/movedObject"))
			{
				dragEvent->acceptProposedAction();
				return true;
			}
			VisualEditorGlobals::IsAnyObjectBeingMoved = false;
		}
		else if (event->type() == QEvent::Drop)
		{
			QDropEvent* drop = static_cast<QDropEvent*>(event);
			//create new empty property
			if (drop->mimeData()->hasFormat("toolbox/property"))
			{
				//spawn new empty property
				JSONPropertyWidget* prop = new JSONPropertyWidget(this);
				ui->verticalLayoutBox->addWidget(prop);
				prop->Id = ChildObjects.count();
				ChildObjects.append(prop);
			}
			//create new empty object
			if (drop->mimeData()->hasFormat("toolbox/object"))
			{
				//generate empty json object to add 
				QJsonObject jsonObj = QJsonObject();
				if (drop->mimeData()->hasFormat("toolbox/customObjectInfo"))
				{
					//if it has custom data attached to it -> load it
					QJsonDocument prefabDoc = QJsonDocument::fromJson((drop->mimeData()->data("toolbox/customObjectInfo")));
					jsonObj = prefabDoc.object();
				}
				//otherwise we just fill it with empty data
				JSONObjectWidget* obj = new JSONObjectWidget(jsonObj, this);
				ui->verticalLayoutBox->addWidget(obj);

				//because we add it in the end we simply set it to (last index + 1), which would basicaly become next last index
				obj->Id = ChildObjects.count();
				ChildObjects.append(obj);
			}
			//reparent
			if (drop->mimeData()->hasFormat("veeditor/movedObject"))
			{
				//generate id to avoid unnesseary calls
				const int index = QString::fromUtf8(drop->mimeData()->data("veeditor/movedObject")).toInt();

				const auto list = GetFileObject()->GetListOfAllJsonWidgets();
				if (index > 0 && index < list.count())
				{
					auto child = GetFileObject()->GetListOfAllJsonWidgets().at(QString::fromUtf8(drop->mimeData()->data("veeditor/movedObject")).toInt());
					if (child != this)
					{
						if (JSONObjectWidget* obj = qobject_cast<JSONObjectWidget*>(child->parent()->parent()))
						{
							obj->ChildObjects.removeOne(child);
							obj->ShakeChildren();
						}
						else
						{
							qWarning() << child->parent()->parent()->objectName();
						}

						child->setParent(this);
						ChildObjects.append(child);
						ShakeChildren();
					}
				}
				else
				{
					qWarning() << "Error: Invalid index : " + QString::number(index) + " in " + objectName() + " Array size: " + QString::number(list.count());
					return true;
				}
			}
			VisualEditorGlobals::IsAnyObjectBeingMoved = false;
			return true;
		}
	}
	return false;
}

void JSONObjectWidget::CreatePrefab()
{
	//get name for new prefab
	QString prefabName = QInputDialog::getText(this, "Create Name For Prefab", "Name:");

	//we can not create files with no name
	if (prefabName != "")
	{
		//Create prefab folder if it doesn't exists
		if (!QDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/JsonVisualEditorsPrefabs/").exists())
		{
			QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/JsonVisualEditorsPrefabs/");
		}
		//.vep = Visual Editor Prefab
		QFile file(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/JsonVisualEditorsPrefabs/"+ prefabName + ".vep");

		if (!file.open(QIODevice::ReadWrite))
		{
			QMessageBox::critical(this, "Warning!", file.errorString(), QMessageBox::Ok);
			return;
		}

		//Create and save file
		QJsonObject docObj;
		docObj[Name] = (GenerateJsonValue().toObject());
		QJsonDocument prefabDocument(docObj);
		file.write(prefabDocument.toJson());

		file.close();
	}
	else
	{
		QMessageBox::critical(this, "Error!", "Empty Name is not allowed!", QMessageBox::Ok);
	}
}

JSONObjectWidget::~JSONObjectWidget()
{
	delete ui;
}