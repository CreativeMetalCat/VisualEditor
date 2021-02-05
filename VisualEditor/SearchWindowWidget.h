#pragma once

#include <QWidget>
#include "ui_SearchWindowWidget.h"
#include "JSONWidgetBase.h"

enum class ESearchType :uint
{
	All,
	NameOnly,
	//this would skip objects
	ValueOnly
};

class SearchWindowWidget : public QWidget
{
	Q_OBJECT

public:
	SearchWindowWidget(QVector<JSONWidgetBase*>,QWidget *parent = Q_NULLPTR);
	~SearchWindowWidget();


	/// <summary>
	/// Find first object that fits
	/// </summary>
	/// <param name="whatToFind">-> what to use for search</param>
	/// <param name="type"></param>
	/// <param name="startId"></param>
	/// <returns>Returns id in main list or -1 if nothing was found</returns>
	int Find(QString whatToFind, ESearchType type = ESearchType::All, int startId = 0);

	int CurrentSearchId = 0;
signals:
	void OnSeacrhComplete(bool successful, int treeId);
public:
	Ui::SearchWindowWidget ui;
private:
	QVector<JSONWidgetBase*> Widgets;

};
