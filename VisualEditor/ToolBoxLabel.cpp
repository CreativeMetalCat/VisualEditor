#include "ToolBoxLabel.h"
#include <QDrag>
#include <QMimeData>

ToolBoxLabel::ToolBoxLabel(QString jsonPrefabString,QWidget* parent, Type type) :QLabel("None", parent), ItemType(type),JsonPrefabString(jsonPrefabString) {}

ToolBoxLabel::ToolBoxLabel(QString text, QString jsonPrefabString, QWidget* parent, Type type) : QLabel(text, parent),  JsonPrefabString(jsonPrefabString), ItemType(type) {}

void ToolBoxLabel::mousePressEvent(QMouseEvent* event)
{
	//start drap and drop

    QDrag* drag = new QDrag(this);
    QMimeData* mimeData = new QMimeData;

    switch (ItemType)
    {
    case Type::Property:
        //we simply tell that this is property
        //because everything else will be created in the widget itself
        mimeData->setData("toolbox/property", ("Add new " + text()).toUtf8());
        break;
    case Type::Object:
        //we simply tell that this is property
        //because everything else will be created in the widget itself
        mimeData->setData("toolbox/object", ("Add new " + text()).toUtf8());
        break;
    case Type::CustomObject:
        //we still tell that this is an object
        mimeData->setData("toolbox/object", ("Add new " + text()).toUtf8());
        //but we also tell that this is a custom object
        //and write the customObject data
        mimeData->setData("toolbox/customObjectInfo", QString(JsonPrefabString).toUtf8());
        break;
    default:
        break;
    }
    
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec();
}

void ToolBoxLabel::mouseReleaseEvent(QMouseEvent* event)
{
}

ToolBoxLabel::~ToolBoxLabel()
{
}
