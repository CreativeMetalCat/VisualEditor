#pragma once
#include <QVector>
#include "JSONWidgetBase.h"

class JSONWidgetManager
{
public:
	QVector<JSONWidgetBase*>Widgets = QVector< JSONWidgetBase*>();
};

