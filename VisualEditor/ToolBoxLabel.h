#pragma once
#include <QLabel>



class ToolBoxLabel :
    public QLabel
{
    Q_OBJECT

public:
    
    enum class Type :uint
    {
        Property,
        Object,
        CustomObject
    };

    ToolBoxLabel(QString jsonPrefabString = "\"prefab\":{}", QWidget* parent = Q_NULLPTR,Type type = Type::Property);
    ToolBoxLabel(QString text, QString jsonPrefabString = "\"prefab\":{}", QWidget* parent = Q_NULLPTR, Type type = Type::Property);
    ~ToolBoxLabel();

    Type ItemType = Type::Property;

    bool IsPrefab = false;

    QString JsonPrefabString;
protected:
    virtual void mousePressEvent(QMouseEvent* event)override;

    virtual void mouseReleaseEvent(QMouseEvent* event)override;
};

