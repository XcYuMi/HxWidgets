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
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    void loadLabels();
    void addLabelToView(QLabel *label);

private:
    QWidget *mRootWidget = nullptr;
    QHash<QLabel*, QListWidgetItem*> mLabelItemHash;
};

class ItemWidget : public QFrame {
    Q_OBJECT
    using Super = QFrame;
    HX_DECLARE_UI(ItemWidget)
public:
    explicit ItemWidget(QWidget *parent = nullptr);
    ~ItemWidget();
};
