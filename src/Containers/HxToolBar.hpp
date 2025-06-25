#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QFrame>
#include <QIcon>
#include <QBoxLayout>
#include "FlowLayout.hpp"

class QToolButton;
class QAction;

class HX_WIDGETS_EXPORT HxToolBar : public QFrame {
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxToolBar)
public:
    explicit HxToolBar(QWidget *parent = nullptr);
    virtual ~HxToolBar();

    struct ActionParams {
        QString text;
        QIcon icon;
        QString toolTip;
        QString name;
        QString statusTip;
        bool checkable = false;
        bool checked = false;
    };

    enum class LayoutType {
        Box,
        FlowLayout
    };

    struct BoxLayoutParams {
        QBoxLayout::Direction direction = QBoxLayout::LeftToRight;
        Qt::Alignment alignment = Qt::AlignCenter | Qt::AlignLeft;
        int spacing = 3;
    };

    struct FlowLayoutParams {
        Hx::FlowLayout::FlowOrder flowOrder = Hx::FlowLayout::FlowOrder::RowFirst;
        Hx::FlowLayout::HorizontalFlowDirection horizontalDirection = Hx::FlowLayout::HorizontalFlowDirection::LeftToRight;
        Hx::FlowLayout::VerticalFlowDirection verticalDirection = Hx::FlowLayout::VerticalFlowDirection::TopToBottom;
        int horizontalSpacing = 3;
        int verticalSpacing = 3;
    };

public:
    void addWidget(QWidget *widget, int stretch = 0);
    void removeWidget(QWidget *widget);
    void addToolButton(QToolButton *button);
    QToolButton *addToolButton(const ActionParams &params);
    void addAction(QAction *action);
    QAction *addAction(const ActionParams &params);
    void addStretch(int stretch, const QString &name = "");
    void addSpacing(int spacing, const QString &name = "");
    void addSeparator(const QString &name = "");
    Qt::Orientation orientation() const;

public:
    QMargins margins() const;
    void setMargins(const QMargins &margins);
    void setMargins(int left, int top, int right, int bottom);
    void setLayoutParams(const BoxLayoutParams &params);
    void setLayoutParams(const FlowLayoutParams &params);

public:
    Qt::ToolBarAreas areas() const;
    void setSreas(const Qt::ToolBarAreas areas);

protected:
    void changeEvent(QEvent *event);
};
