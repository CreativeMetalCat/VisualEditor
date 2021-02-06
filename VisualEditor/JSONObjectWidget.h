#pragma once

#include <QWidget>
#include "JSONWidgetBase.h"
namespace Ui { class JSONObjectWidget; };

class JSONObjectWidget : public JSONWidgetBase
{
	Q_OBJECT

public:
	JSONObjectWidget(QJsonObject, QWidget* parent = Q_NULLPTR, QString name = "object", bool AllowNameChange = true, bool isArray = false);
	JSONObjectWidget(QJsonArray, QWidget* parent = Q_NULLPTR, QString name = "object", bool AllowNameChange = true,bool isArray = false);
	~JSONObjectWidget();

	QVector<JSONWidgetBase*>ChildObjects = QVector<JSONWidgetBase*>();

	virtual QJsonValue GenerateJsonValue() override;

	/// <summary>
	/// Creates a new property with everything set to default
	/// </summary>
	void AddNewProperty();

	/// <summary>
	///  Creates a new property with data from value
	/// </summary>
	/// <param name="value">data to load from</param>
	void AddNewProperty(QString name,QJsonValue value);

	void OnChildChanged(EditorActions::SEditorAction* action);
signals:
	void OnChangeInChild(EditorActions::SEditorAction*);
public:
	/// <summary>
	/// Changes child id
	/// </summary>
	/// <param name="newId"></param>
	virtual void ChangeChildId(int newId, JSONWidgetBase* objToEdit)override;

	/// <summary>
	/// Changes child id
	/// </summary>
	/// <param name="newId"></param>
	virtual void ChangeChildId_NoSignal(int newId, JSONWidgetBase* objToEdit)override;

	virtual void ChangeName_NoSignal(QString)override;

	/// <summary>
	/// Changes child id
	/// </summary>
	/// <param name="newId"></param>
	virtual void ChangeChildId(int newId)override;

	/// <summary>
	/// Removes child told by PropertyEditor
	/// </summary>
	virtual void DeleteChild()override;

	bool IsArray = false;
	/// <summary>
	/// Returns the "main" object - the file object that contains the whole thing
	/// </summary>
	/// <param name="start"></param>
	/// <returns></returns>
	JSONObjectWidget* GetFileObject(JSONObjectWidget*start = Q_NULLPTR);

	/// <summary>
	/// Returns list of all of the children(and their children) in one list
	/// This function is VERY inefficient, so try not to call it often
	/// </summary>
	/// <returns>list of all of the children(and their children) in one list</returns>
	QVector<JSONWidgetBase*> GetListOfAllJsonWidgets();

	/// <summary>
	/// "Shakes" chilren to correctly display items after adding/removing a child
	/// </summary>
	void ShakeChildren();

private slots:

	/// <summary>
	/// Creates new prefab file in Documents/Prefabs/
	/// </summary>
	void CreatePrefab();
private:
	Ui::JSONObjectWidget *ui;

	virtual bool eventFilter(QObject* object, QEvent* event)override;
};
