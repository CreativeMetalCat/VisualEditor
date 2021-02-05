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
			TypeChange
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


signals:
	void OnChanged(EditorActions::SEditorAction*);
protected:
	//Type of the object that will be used in json file generation
	QJsonValue::Type Type = QJsonValue::Type::Undefined;

public slots:
	virtual void ChangeChildId(int newId){}

	virtual void DeleteChild() {}

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

		SPropertyValueChangeAction(JSONWidgetBase* actionSoure, QString changedValue, QString oldValue);
	};

	class SPropertyTypeSelectionChangeAction :public SEditorAction
	{
	public:
		QString ChangedValue = "Null";

		QString OldValue = "Null";

		SPropertyTypeSelectionChangeAction(JSONWidgetBase* actionSoure, QString changedValue, QString oldValue);
	};
}
