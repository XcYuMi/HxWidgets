#include "WidgetUtils.hpp"
#include "HxWidgetsGlobalPrivate.hpp"

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>

///////////////////////////////////////////////////////// QWidget //////////////////////////////////////////////////////

void HxWidgetUtils::setWidth(QWidget *widget, int width) {
    widget->resize(width, widget->height());
}

void HxWidgetUtils::setHeight(QWidget *widget, int height) {
    widget->resize(widget->width(), height);
}

///////////////////////////////////////////////////////// QWidget::addAction //////////////////////////////////////////////////////

QAction *HxWidgetUtils::addAction(QWidget *widget, const QString &text) {
    const auto action = new QAction(widget);
    action->setText(text);
    widget->addAction(action);
    return action;
}

QAction *HxWidgetUtils::addAction(QWidget *widget, const QIcon &icon, const QString &text) {
    const auto action = new QAction(widget);
    action->setIcon(icon);
    action->setText(text);
    widget->addAction(action);
    return action;
}

///////////////////////////////////////////////////////// COmboBox /////////////////////////////////////////////////////

void HxWidgetUtils::SetComboxBoxCurrentData(QComboBox *control, const QVariant &data, bool blockSignal) {
    if(blockSignal)
        control->blockSignals(true);

    const auto index = control->findData(data);
    control->setCurrentIndex(index);

    if(blockSignal)
        control->blockSignals(false);
}

///////////////////////////////////////////////////////// QSpinBox /////////////////////////////////////////////////////

void HxWidgetUtils::SetSpinBoxValue(QSpinBox *control, int value, bool blockSignal) {
    if(blockSignal)
        control->blockSignals(true);

    control->setValue(value);

    if(blockSignal)
        control->blockSignals(false);
}


///////////////////////////////////////////////////////// QSlider /////////////////////////////////////////////////////
void HxWidgetUtils::SetSliderValue(QSlider *control, int value, bool blockSignal) {
    if(blockSignal)
        control->blockSignals(true);

    control->setValue(value);

    if(blockSignal)
        control->blockSignals(false);
}
