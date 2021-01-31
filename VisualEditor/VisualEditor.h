#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VisualEditor.h"

class VisualEditor : public QMainWindow
{
    Q_OBJECT

public:
    VisualEditor(QWidget *parent = Q_NULLPTR);

private:
    Ui::VisualEditorClass ui;
};
