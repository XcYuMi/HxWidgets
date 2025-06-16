#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QWidget>

class RangeSlider : public QWidget {
    Q_OBJECT
    HX_DECLARE_PRIVATE(RangeSlider)
public:
    explicit RangeSlider(QWidget *parent = nullptr);
    virtual ~RangeSlider();
};
