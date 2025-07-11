#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QWidget>

class BadgeTestWidget : public QWidget {
    Q_OBJECT
    HX_DECLARE_UI(BadgeTestWidget)
public:
    explicit BadgeTestWidget(QWidget *parent = nullptr);
    ~BadgeTestWidget();
};
