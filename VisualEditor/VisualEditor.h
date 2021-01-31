#pragma once

#include <QtWidgets/QMainWindow>
#include <QVBoxLayout>
#include "JsonEditorTreeModel.h"
#include "ui_VisualEditor.h"

class VisualEditor : public QMainWindow
{
    Q_OBJECT

public:
    VisualEditor(QWidget *parent = Q_NULLPTR);

private:
    Ui::VisualEditorClass ui;

    QVBoxLayout* VerticalLayout;
    QWidget* scrollWidget;
};
