#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QScrollBar>

class HX_WIDGETS_EXPORT HxScrollBar : public QScrollBar {
    Q_OBJECT
    using Super = QScrollBar;
    HX_DECLARE_PRIVATE(HxScrollBar)
public:
    explicit HxScrollBar(QWidget *parent = nullptr);
    virtual ~HxScrollBar();
};
