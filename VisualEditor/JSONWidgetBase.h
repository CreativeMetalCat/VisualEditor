#pragma once

#include <QWidget>
#include <QJsonObject>
#include <QJsonValue>
#include "ui_JSONWidgetBase.h"
#include "globals.h"

class JSONWidgetBase;

namespace EditorActions
{
	class SEditorAction
	{
	public:
		enum class Type
		{
			//for undefined actions
			None,
			ValueChange,
			NameChange,
			IdInParentChange,
			TypeChange,
			TreeAdded,
			TreeRemoved
		};
		Type ActionType = Type::None;

		//widget in which action has happened
		JSONWidgetBase* ActionSoure;

		SEditorAction() {}

		virtual void Nothing() {}
	};
}


class JSONWidgetBase : public QWidget
{
	Q_OBJECT

public:
	JSONWidgetBase(QWidget *parent = Q_NULLPTR, QString name = "object");
	~JSONWidgetBase();

	//Name of the object that will be used in json file generation
	//*Has no effect on how editor works
	QString Name;

	/// <summary>
	/// generate json value for writing into file
	/// </summary>
	/// <param name="isInArray">objects in array can not have names</param>
	/// <returns>value to write</returns>
	virtual QJsonValue GenerateJsonValue();

	//index of the item in parent's array
	int Id = -1;

	
	virtual void mouseReleaseEvent(QMouseEvent*)override;

	/// <summary>
	/// Changes name without emiting any signals, used by undo system
	/// </summary>
	/// <param name="name"></param>
	virtual void ChangeName_NoSignal(QString name) { Name = name; }
signals:
	void OnChanged(EditorActions::SEditorAction*);
public:
	//Type of the object that will be used in json file generation
	QJsonValue::Type Type = QJsonValue::Type::Undefined;

public slots:
	virtual void ChangeChildId(int newId,JSONWidgetBase*objToEdit){}

	virtual void ChangeChildId(int newId) {}

	virtual void ChangeChildId_NoSignal(int newId, JSONWidgetBase* objToEdit) {}

	virtual void DeleteChild() {}

	virtual void DeleteChild_NoSignal() {}

private:
	Ui::JSONWidgetBase ui;
};

namespace EditorActions
{

	class SPropertyValueChangeAction : public SEditorAction
	{
	public:
		QString ChangedValue = "Null";

		QString OldValue = "Null";

		QJsonValue::Type ValueType;

		SPropertyValueChangeAction(JSONWidgetBase* actionSoure, QString changedValue, QString oldValue, QJsonValue::Type valueType);
	};

	class SPropertyTypeSelectionChangeAction :public SEditorAction
	{
	public:
		QString ChangedValue = "Null";

		QString OldValue = "Null";

		SPropertyTypeSelectionChangeAction(JSONWidgetBase* actionSoure, QString changedValue, QString oldValue);
	};

	class SWidgetIdChangeAction :public SEditorAction
	{
	public:
		int OldId = -1;

		int NewId = -1;

		JSONWidgetBase* MovedChild = Q_NULLPTR;

		SWidgetIdChangeAction(JSONWidgetBase* actionSoure, JSONWidgetBase* movedChild, int oldId, int newId);
	};

	class SNameChangeAction :public SEditorAction
	{
	public:

		QString ChangedValue = "Null";

		QString OldValue = "Null";

		SNameChangeAction(JSONWidgetBase* actionSoure, QString changedValue, QString oldValue);
	};

	class STreeRemovalAction :public SEditorAction
	{
	public:
		QString ObjectName;
		QJsonValue TreeValue;

		bool IsArray = false;

		bool IsProperty = false;

		STreeRemovalAction(JSONWidgetBase* actionSoure, QJsonValue treeValue, QString objectName, bool IsArray = false, bool IsProperty = false);
	};
}

typedef EditorActions::SEditorAction::Type EditorActionType;
