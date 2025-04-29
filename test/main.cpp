#include "TestStarterWidget.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestStarterWidget w;
    w.show();
    return a.exec();
}
