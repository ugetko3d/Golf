#include "Golf.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Golf w;
    w.show();
    return a.exec();
}
