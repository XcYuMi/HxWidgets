#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QWidget>

class DoubleRangerSlider : public QWidget {
    Q_OBJECT
    HX_DECLARE_PRIVATE(DoubleRangerSlider)
public:
    explicit DoubleRangerSlider(QWidget *parent = nullptr);
    virtual ~DoubleRangerSlider();
};
