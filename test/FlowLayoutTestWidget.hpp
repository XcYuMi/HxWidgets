#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QWidget>

class FlowLayoutTestWidget : public QWidget {
    Q_OBJECT
    HX_DECLARE_UI(FlowLayoutTestWidget)
public:
    explicit FlowLayoutTestWidget(QWidget *parent = nullptr);
    ~FlowLayoutTestWidget();
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};

