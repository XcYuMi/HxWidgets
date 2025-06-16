#include "WidgetUtils.hpp"
#include "HxWidgetsGlobalPrivate.hpp"

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>

HX_USING_NAMESPACE;

///////////////////////////////////////////////////////// QWidget //////////////////////////////////////////////////////

void WidgetUtils::setWidth(QWidget *widget, int width) {
    widget->resize(width, widget->height());
}

void WidgetUtils::setHeight(QWidget *widget, int height) {
    widget->resize(widget->width(), height);
}

///////////////////////////////////////////////////////// COmboBox /////////////////////////////////////////////////////

void WidgetUtils::SetComboxBoxCurrentData(QComboBox *control, const QVariant &data, bool blockSignal) {
    if(blockSignal)
        control->blockSignals(true);

    const auto index = control->findData(data);
    control->setCurrentIndex(index);

    if(blockSignal)
        control->blockSignals(false);
}

///////////////////////////////////////////////////////// QSpinBox /////////////////////////////////////////////////////

void WidgetUtils::SetSpinBoxValue(QSpinBox *control, int value, bool blockSignal) {
    if(blockSignal)
        control->blockSignals(true);

    control->setValue(value);

    if(blockSignal)
        control->blockSignals(false);
}


///////////////////////////////////////////////////////// QSlider /////////////////////////////////////////////////////
void WidgetUtils::SetSliderValue(QSlider *control, int value, bool blockSignal) {
    if(blockSignal)
        control->blockSignals(true);

    control->setValue(value);

    if(blockSignal)
        control->blockSignals(false);
}
