#include "JSONWidgetBase.h"

JSONWidgetBase::JSONWidgetBase(QWidget *parent, QString name)
	: QWidget(parent), Name(name)
{
	ui.setupUi(this);

	setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
}

JSONWidgetBase::~JSONWidgetBase()
{
}

QJsonValue JSONWidgetBase::GenerateJsonValue()
{
	//because it is a base of the bases - there is nothing to return
	return QJsonObject();
}
