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

EditorActions::SPropertyValueChangeAction::SPropertyValueChangeAction(JSONWidgetBase* actionSoure, QString changedValue, QString oldValue, QJsonValue::Type valueType)
	:ChangedValue(changedValue), OldValue(oldValue), ValueType(valueType)
{
	ActionSoure = actionSoure;
	ActionType = Type::ValueChange;
}

EditorActions::SPropertyTypeSelectionChangeAction::SPropertyTypeSelectionChangeAction(JSONWidgetBase* actionSoure, QString changedValue, QString oldValue)
	:ChangedValue(changedValue), OldValue(oldValue)
{
	ActionSoure = actionSoure;
	ActionType = Type::TypeChange;
}

EditorActions::SWidgetIdChangeAction::SWidgetIdChangeAction(JSONWidgetBase* actionSoure, JSONWidgetBase* movedChild, int oldId, int newId)
	:MovedChild(movedChild),OldId(oldId),NewId(newId)
{
	ActionSoure = actionSoure;
	ActionType = Type::IdInParentChange;
}

EditorActions::SNameChangeAction::SNameChangeAction(JSONWidgetBase* actionSoure, QString changedValue, QString oldValue)
	:ChangedValue(changedValue), OldValue(oldValue)
{
	ActionSoure = actionSoure;
	ActionType = Type::NameChange;
}

EditorActions::STreeRemovalAction::STreeRemovalAction(JSONWidgetBase* actionSoure, QJsonValue treeValue, QString objectName, bool isArray)
	:TreeValue(treeValue),ObjectName(objectName),IsArray(isArray)
{
	ActionSoure = actionSoure;
	ActionType = Type::TreeRemoved;
}
