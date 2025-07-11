#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QWidget>

class NestedSplitterTestWidget : public QWidget {
    Q_OBJECT
    HX_DECLARE_UI(NestedSplitterTestWidget)
public:
    explicit NestedSplitterTestWidget(QWidget *parent = nullptr);
    ~NestedSplitterTestWidget();
};
