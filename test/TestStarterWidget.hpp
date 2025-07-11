#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QWidget>

class TestStarterWidget : public QWidget {
    Q_OBJECT
    HX_DECLARE_UI(TestStarterWidget)
public:
    TestStarterWidget(QWidget *parent = nullptr);
    ~TestStarterWidget();

private:
    void initTestPages();
};
