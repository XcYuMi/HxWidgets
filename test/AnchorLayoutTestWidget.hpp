#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QWidget>

class AnchorLayoutTestWidget : public QWidget {
    Q_OBJECT
    HX_DECLARE_UI(AnchorLayoutTestWidget)
public:
    explicit AnchorLayoutTestWidget(QWidget *parent = nullptr);
    ~AnchorLayoutTestWidget();
};
