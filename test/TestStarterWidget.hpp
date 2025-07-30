#pragma once
#include "HxWidgetsGlobal.hpp"
#include "HxToolBar.hpp"

class TestStarterWidget : public HxToolBar {
    Q_OBJECT
    HX_DECLARE_UI(TestStarterWidget)
    using Super = HxToolBar;
public:
    TestStarterWidget(QWidget *parent = nullptr);
    ~TestStarterWidget();

private:
    void initTestPages();
};
