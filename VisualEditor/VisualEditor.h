#pragma once

#include <QtWidgets/QMainWindow>
#include <QVBoxLayout>
#include "JSONObjectWidget.h"
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

    QWidget* toolBoxScrollWidget;
    QVBoxLayout* toolBoxScrollVertialLayout;

    JSONObjectWidget* fileObject;
};
