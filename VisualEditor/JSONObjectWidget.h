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

public:
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

private slots:
	/// <summary>
	/// Changes child id
	/// </summary>
	/// <param name="newId"></param>
	void OnIdSpinBoxValueChanged(int newId) { ChangeChildId(newId); }

	/// <summary>
	/// Removes child told by PropertyEditor
	/// </summary>
	void OnDeleteButtonPressed() { DeleteChild(); }

	void OnIsArrayChanged(bool newState);

	void OnNameChanged(QString newName);
private:
	Ui::JSONObjectWidget *ui;

	virtual bool eventFilter(QObject* object, QEvent* event)override;
};
