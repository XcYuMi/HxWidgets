#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QWidget>

class DoubleRangeSliderTestWidget : public QWidget {
    Q_OBJECT
    HX_DECLARE_UI(DoubleRangeSliderTestWidget)
public:
    explicit DoubleRangeSliderTestWidget(QWidget *parent = nullptr);
    ~DoubleRangeSliderTestWidget();
};

