#pragma once

#include <QtWidgets/QMainWindow>
#include <QVBoxLayout>
#include "ui_VisualEditor.h"
#include "FileTab.h"


class VisualEditor : public QMainWindow
{
    Q_OBJECT

public:
    VisualEditor(QWidget *parent = Q_NULLPTR);
private:
    Ui::VisualEditorClass ui;

    QVector<FileTabWidget*> TabsWidgets = QVector<FileTabWidget*>();

    QWidget* toolBoxScrollWidget;
    QVBoxLayout* toolBoxScrollVertialLayout;
private slots:
    void SaveCurrentFile();

    void OpenNewFile();

    void OnTabClosed(int);
};
