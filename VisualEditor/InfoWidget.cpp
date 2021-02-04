#include "InfoWidget.h"

InfoWidget::InfoWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	this->setWindowFlag(Qt::Window);

	setWindowTitle("Info");
}

InfoWidget::~InfoWidget()
{
}
