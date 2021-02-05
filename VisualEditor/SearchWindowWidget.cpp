#include "SearchWindowWidget.h"
#include "JSONPropertyWidget.h"

SearchWindowWidget::SearchWindowWidget(QVector<JSONWidgetBase*> widgets, QWidget *parent)
	: QWidget(parent), Widgets(widgets)
{
	ui.setupUi(this);

	setWindowTitle("Search Window");
	
	ui.comboBox_SearchType->addItem("Both");
	ui.comboBox_SearchType->addItem("Names");
	ui.comboBox_SearchType->addItem("Values");

	setWindowFlag(Qt::Window);

	connect(ui.pushButton, &QPushButton::pressed, this, [this]()
	{
		int ind = Find(ui.lineEdit->text(), ESearchType(ui.comboBox_SearchType->currentIndex()), CurrentSearchId);
		CurrentSearchId++;
		emit OnSeacrhComplete(ind != -1, ind);
	});

	connect(ui.lineEdit, &QLineEdit::textChanged, this, [this](QString) {CurrentSearchId = 0; });
}

SearchWindowWidget::~SearchWindowWidget()
{
}

int SearchWindowWidget::Find(QString whatToFind, ESearchType type,int startId)
{
	if (!Widgets.empty() && Widgets.count() > startId)
	{
		//depending on the type search is done differentry

		//search in name does not require any casting because all of widgets have names
		if (type == ESearchType::NameOnly)
		{
			for (auto it = Widgets.begin() + startId; it != Widgets.end(); ++it)
			{
				if ((*it)->Name.contains(whatToFind))
				{
					return it - Widgets.begin();
				}
			}
		}
		//search in value requires for objects to be cast to properties, because only they can have values
		else if (type == ESearchType::ValueOnly)
		{
			for (auto it = Widgets.begin() + startId; it != Widgets.end(); ++it)
			{
				if (JSONPropertyWidget* prop = qobject_cast<JSONPropertyWidget*>(*it))
				{
					QString value = "Null";
					//generate values based on the type
					//because each value uses their own type of editing field(for user's comfort) switch has to be used
					switch (prop->type)
					{
					case  QJsonValue::String:
						value = prop->ui.valueEdit->text();
						break;
					case  QJsonValue::Bool:
						value = prop->ui.boolValue->isChecked() ? "True" : "False";
						break;
					case  QJsonValue::Double:
						value = QString::number(prop->ui.doubleSpinBox->value());
						break;
					default:
						break;
					}

					if (value.contains(whatToFind))
					{
						return it - Widgets.begin();
					}
				}
			}
		}
		//seach in both would mean checking name first and then value(least efficient one)
		else
		{
			for (auto it = Widgets.begin() + startId; it != Widgets.end(); ++it)
			{
				if ((*it)->Name.contains(whatToFind))
				{
					return it - Widgets.begin();
				}

				if (JSONPropertyWidget* prop = qobject_cast<JSONPropertyWidget*>(*it))
				{
					QString value = "Null";
					//generate values based on the type
					//because each value uses their own type of editing field(for user's comfort) switch has to be used
					switch (prop->type)
					{
					case  QJsonValue::String:
						value = prop->ui.valueEdit->text();
						break;
					case  QJsonValue::Bool:
						value = prop->ui.boolValue->isChecked() ? "True" : "False";
						break;
					case  QJsonValue::Double:
						value = QString::number(prop->ui.doubleSpinBox->value());
						break;
					default:
						break;
					}

					if (value.contains(whatToFind))
					{
						return it - Widgets.begin();
					}
				}
			}
		}
	}
	return -1;
}
