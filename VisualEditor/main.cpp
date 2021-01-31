#include "VisualEditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VisualEditor w;
    w.show();
    return a.exec();
}
