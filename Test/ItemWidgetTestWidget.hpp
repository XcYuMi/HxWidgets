#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QListWidget>
#include <QFrame>

class ItemWidgetTestWidget : public QWidget {
    Q_OBJECT
    HX_DECLARE_UI(ItemWidgetTestWidget)
public:
    explicit ItemWidgetTestWidget(QWidget *parent = nullptr);
    ~ItemWidgetTestWidget();
};

class QLabel;
class ListWidget : public QListWidget {
    Q_OBJECT
    HX_DECLARE_PRIVATE(ListWidget)
    using Super = QListWidget;
public:
    explicit ListWidget(QWidget *parent = nullptr);
    ~ListWidget();
    void setRootWidget(QWidget *widget);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    void loadLabels();
    void addLabelToView(QLabel *label);
    void setItemWidgetsInteractive(bool active);
};

class ItemWidget : public QFrame {
    Q_OBJECT
    using Super = QFrame;
    HX_DECLARE_UI(ItemWidget)
public:
    explicit ItemWidget(QWidget *parent = nullptr);
    ~ItemWidget();
};
