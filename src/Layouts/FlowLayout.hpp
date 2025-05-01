#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QLayout>

HX_BEGIN_NAMESPACE

class HX_WIDGETS_EXPORT FlowLayout : public QLayout
{
    Q_OBJECT
    HX_DECLARE_PRIVATE(FlowLayout)
    using Super = QLayout;

public:
    enum FlowOrder {
        RowFirst,
        ColumnFirst
    };
    Q_ENUM(FlowOrder)

    enum HorizontalFlowDirection {
        LeftToRight,
        RightToLeft,
    };
    Q_ENUM(HorizontalFlowDirection)

    enum VerticalFlowDirection {
        TopToBottom,
        BottomToTop,
    };
    Q_ENUM(VerticalFlowDirection)

public:
    explicit FlowLayout(QWidget *parent = nullptr);
    virtual ~FlowLayout();
    //void addWidget(QWidget *widget);
    //void removeWidget(QWidget *widget);

public:
    void setHorizontalSpacing(int spacing);
    void setVerticalSpacing(int spacing);
    void setFlowOrder(FlowOrder order);
    void setHorizontalFlow(HorizontalFlowDirection direction);
    void setVerticalFlow(VerticalFlowDirection direction);
    void setUniformItemSize(const QSize &size);

public:
    int horizontalSpacing() const;
    int verticalSpacing() const;
    FlowOrder flowOrder() const;
    HorizontalFlowDirection horizontalFlow() const;
    VerticalFlowDirection verticalFlow() const;
    QSize uniformItemSize() const;

public:
    void addItem(QLayoutItem *item) override;
    int count() const override;
    QLayoutItem *itemAt(int index) const override;
    QLayoutItem *takeAt(int index) override;

public:
    QSize sizeHint() const override;
    void setGeometry(const QRect &rect) override;

public:
    bool hasHeightForWidth() const override;
    int heightForWidth(int width) const override;
};

HX_END_NAMESPACE
