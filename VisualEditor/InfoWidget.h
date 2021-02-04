#pragma once

#include <QWidget>
#include "ui_InfoWidget.h"

class InfoWidget : public QWidget
{
	Q_OBJECT

public:
	InfoWidget(QWidget *parent = Q_NULLPTR);
	~InfoWidget();

private:
	Ui::InfoWidget ui;
};
