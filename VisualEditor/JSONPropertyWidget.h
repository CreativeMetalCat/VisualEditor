#pragma once

#include <QWidget>
#include "JSONWidgetBase.h"
#include "ui_JSONPropertyWidget.h"

class JSONPropertyWidget : public JSONWidgetBase
{
	Q_OBJECT

public:
	JSONPropertyWidget(QWidget *parent = Q_NULLPTR, QString name = "object",QJsonValue value = QJsonValue());
	~JSONPropertyWidget();

	virtual QJsonValue GenerateJsonValue() override;

	QJsonValue::Type type;

	//does same job as OnTypeSelectionChanged(QString selection)
	//but doesn't emit any signals
	void ChangeTypeSelection(QString type);

	virtual void ChangeName(QString name);
protected:
	virtual void contextMenuEvent(QContextMenuEvent*)override;

	virtual void mousePressEvent(QMouseEvent*)override;

	virtual void mouseReleaseEvent(QMouseEvent*)override;

	//Keep old value in memory for simplier work with editor action of chaning spin box value
	double CurrentSpinBoxValue = 0;
public:
	Ui::JSONPropertyWidget ui;
private slots:
	void OnTypeSelectionChanged(QString selection);

	/// <summary>
	/// Changes child id
	/// </summary>
	/// <param name="newId"></param>
	void OnIdSpinBoxValueChanged(int newId);

	/// <summary>
	/// Removes child told by PropertyEditor
	/// </summary>
	void OnDeleteButtonPressed() { DeleteChild(); }

	void OnTextChanged(QString text);
};
