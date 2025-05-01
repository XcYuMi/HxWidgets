#pragma once
#include "HxWidgetsGlobal.hpp"

#include <QWidget>

HX_BEGIN_NAMESPACE

namespace WidgetUtils {

inline void setWidth(QWidget *widget, int width) {
    widget->resize(width, widget->height());
}

inline void setHeight(QWidget *widget, int height) {
    widget->resize(widget->width(), height);
}

}

HX_END_NAMESPACE
