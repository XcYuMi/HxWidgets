#include <QApplication>
#include "TestStarterWidget.hpp"

void initStyle();

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication app(argc, argv);
    initStyle();

    TestStarterWidget starterWidget;
    starterWidget.show();

    return app.exec();
}

void initStyle() {
    const auto &qss = QStringLiteral(R"(
* {
    font: 14px Microsoft YaHei;
}
    )");
    qApp->setStyleSheet(qss);
}
