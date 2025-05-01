#include <QApplication>
#include "TestStarterWidget.hpp"

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication app(argc, argv);

    TestStarterWidget starterWidget;
    starterWidget.show();

    return app.exec();
}
