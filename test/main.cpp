#include <QApplication>
#include "TestStarterWidget.hpp"
#include <QDir>
#include <QFile>

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
    QString qss = QStringLiteral(R"(
* {
    font: 14px Microsoft YaHei;
}
    )");

    QDir dir(":/stylesheets/");
    const auto &names = dir.entryList({"*.qss", "*.css"}, QDir::Files | QDir::NoSymLinks);
    for(const auto &name : names) {
        const auto& path = dir.path() + "/" + name;
        QFile file(path);
        if(!file.open(QFile::Text | QFile::ReadOnly)) {
            continue;
        }
        const auto &text = file.readAll();
        if(!text.isEmpty()) {
            qss += text;
            qss += "\n";
        }
        file.close();
    }

    qApp->setStyleSheet(qss);
}
