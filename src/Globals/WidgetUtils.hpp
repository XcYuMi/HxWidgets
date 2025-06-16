#pragma once
#include "HxWidgetsGlobal.hpp"

class QWidget;
class QComboBox;
class QSlider;
class QSpinBox;
class QCheckBox;

HX_BEGIN_NAMESPACE

class HX_WIDGETS_EXPORT WidgetUtils {
public: // QWidget
    static void setWidth(QWidget *widget, int width);
    static void setHeight(QWidget *widget, int height);

public: // QComboBox
    static void SetComboxBoxCurrentData(QComboBox *control, const QVariant &data, bool blockSignal = true);

public: // SpinBox
    static void SetSpinBoxValue(QSpinBox *control, int value, bool blockSignal = true);

public: // Slider
    static void SetSliderValue(QSlider *control, int value, bool blockSignal = true);
};

HX_END_NAMESPACE
