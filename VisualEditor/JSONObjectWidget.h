#pragma once

#include <QWidget>
#include "JSONWidgetBase.h"
namespace Ui { class JSONObjectWidget; };

class JSONObjectWidget : public JSONWidgetBase
{
	Q_OBJECT

public:
	JSONObjectWidget(QJsonObject,QWidget *parent = Q_NULLPTR, QString name = "object",bool AllowNameChange = true);
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

private:
	Ui::JSONObjectWidget *ui;

	bool eventFilter(QObject* object, QEvent* event);
};
