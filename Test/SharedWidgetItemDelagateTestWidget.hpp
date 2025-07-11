#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QWidget>

class SharedWidgetItemDelagateTestWidget : public QWidget {
    Q_OBJECT
    HX_DECLARE_UI(SharedWidgetItemDelagateTestWidget)
public:
    explicit SharedWidgetItemDelagateTestWidget(QWidget *parent = nullptr);
    ~SharedWidgetItemDelagateTestWidget();
};
