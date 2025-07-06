#include <QApplication>
#include "TestStarterWidget.hpp"
#include <QDir>
#include <QFile>
#include <QStyle>

void initStyle();

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication app(argc, argv);


    TestStarterWidget starterWidget;
    starterWidget.show();

    initStyle();

    return app.exec();
}

void initStyle() {
    QString qss = 
"*{" "\n"
"    font: 14px Microsoft YaHei;" "\n"
"}" "\n\n"
;

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

    qApp->style()->unpolish(qApp);
    qApp->setStyleSheet(qss);
    qApp->style()->polish(qApp);
}
