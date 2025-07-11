#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QWidget>

class RangeSliderTestWidget : public QWidget {
    Q_OBJECT
    HX_DECLARE_UI(RangeSliderTestWidget)
public:
    explicit RangeSliderTestWidget(QWidget *parent = nullptr);
    ~RangeSliderTestWidget();
};
