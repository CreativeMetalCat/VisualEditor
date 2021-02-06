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

void JSONWidgetBase::mouseReleaseEvent(QMouseEvent*)
{
	VisualEditorGlobals::IsAnyObjectBeingMoved = false;
}

EditorActions::SPropertyValueChangeAction::SPropertyValueChangeAction(JSONWidgetBase* actionSource, QString changedValue, QString oldValue, QJsonValue::Type valueType)
	:ChangedValue(changedValue), OldValue(oldValue), ValueType(valueType)
{
	ActionSource = actionSource;
	ActionType = Type::ValueChange;
}

EditorActions::SPropertyTypeSelectionChangeAction::SPropertyTypeSelectionChangeAction(JSONWidgetBase* actionSource, QString changedValue, QString oldValue)
	:ChangedValue(changedValue), OldValue(oldValue)
{
	ActionSource = actionSource;
	ActionType = Type::TypeChange;
}

EditorActions::SWidgetIdChangeAction::SWidgetIdChangeAction(JSONWidgetBase* actionSource, JSONWidgetBase* movedChild, int oldId, int newId)
	:MovedChild(movedChild),OldId(oldId),NewId(newId)
{
	ActionSource = actionSource;
	ActionType = Type::IdInParentChange;
}

EditorActions::SNameChangeAction::SNameChangeAction(JSONWidgetBase* actionSource, QString changedValue, QString oldValue)
	:ChangedValue(changedValue), OldValue(oldValue)
{
	ActionSource = actionSource;
	ActionType = Type::NameChange;
}

EditorActions::STreeRemovalAction::STreeRemovalAction(JSONWidgetBase* actionSource, QJsonValue treeValue, QString objectName, bool isArray, bool isProperty)
	:TreeValue(treeValue),ObjectName(objectName),IsArray(isArray), IsProperty(isProperty)
{
	ActionSource = actionSource;
	ActionType = Type::TreeRemoved;
}

EditorActions::STreeAditionAction::STreeAditionAction(JSONWidgetBase* actionSource, JSONWidgetBase* parent)
	:Parent(parent)
{
	ActionSource = actionSource;
	ActionType = Type::TreeAdded;
}
