#include "FlowLayout.hpp"

HX_USING_NAMESPACE;

class FlowLayout::FlowLayoutPrivate {
    FlowLayout *_this{nullptr};
public:
    struct LayoutCache {
        QSize sizeHint{-1,-1};
        QHash<int,int> heightForWidth;
        QHash<int,int> widthForHeight;
    public:
        bool isValid() const;
        void invalidate();
    };
public:
    FlowLayoutPrivate(FlowLayout *layout) : _this(layout) {}
    void invalidCache();
    QSize effectiveItemSize(QLayoutItem *item) const;
    QSize calculateSize(const QSize &constraint = QSize());
    void doLayout(const QRect &r) const;
    int directionSign() const;
    void applyDirectionAdjustment(const QRect &rect, QVector<QRect> &geometries) const;

public:
    QList<QLayoutItem*> items;
    int horizontalSpacing{6};
    int verticalSpacing{6};
    FlowLayout::FlowOrder flowOrder{FlowOrder::RowFirst};
    FlowLayout::HorizontalFlowDirection horizontalFlow{HorizontalFlowDirection::LeftToRight};
    FlowLayout::VerticalFlowDirection verticalFlow{VerticalFlowDirection::TopToBottom};
    QSize uniformItemSize{-1,-1};
    LayoutCache layoutCache;
    bool cacheValid{false};
};

FlowLayout::~FlowLayout() { }

FlowLayout::FlowLayout(QWidget *parent) : Super{parent} {
    d.reset(new FlowLayoutPrivate(this));
}

//////////////////////////////////////////////// QLayout::基础方法 //////////////////////////////////////////////////////
/*/
void FlowLayout::addWidget(QWidget *widget) {
    if(d->widgets.contains(widget))
        return;

    d->widgets.append(widget);
}

void FlowLayout::removeWidget(QWidget *widget) {
    if(!d->widgets.contains(widget))
        return;

    d->widgets.removeOne(widget);
} //*/

////////////////////////////////////////////// QLayout::属性设置获取 ////////////////////////////////////////////////////

void FlowLayout::setHorizontalSpacing(int spacing) {
    if(d->horizontalSpacing == spacing)
        return;

    d->horizontalSpacing = spacing;
}

void FlowLayout::setVerticalSpacing(int spacing) {
    if(d->verticalSpacing == spacing)
        return;

    d->verticalSpacing = spacing;
}

void FlowLayout::setFlowOrder(FlowOrder order) {
    if(d->flowOrder == order)
        return;

    d->flowOrder = order;
}

void FlowLayout::setHorizontalFlow(HorizontalFlowDirection direction) {
    if(d->horizontalFlow == direction)
        return;

    d->horizontalFlow = direction;
}

void FlowLayout::setVerticalFlow(VerticalFlowDirection direction) {
    if(d->verticalFlow == direction)
        return;

    d->verticalFlow = direction;
}

void FlowLayout::setUniformItemSize(const QSize &size) {
    if(d->uniformItemSize == size)
        return;

    d->uniformItemSize = size;
}

int FlowLayout::horizontalSpacing() const {
    return d->horizontalSpacing;
}

int FlowLayout::verticalSpacing() const {
    return d->verticalSpacing;
}

FlowLayout::FlowOrder FlowLayout::flowOrder() const {
    return d->flowOrder;
}

FlowLayout::HorizontalFlowDirection FlowLayout::horizontalFlow() const {
    return d->horizontalFlow;
}

FlowLayout::VerticalFlowDirection FlowLayout::verticalFlow() const {
    return d->verticalFlow;
}

QSize FlowLayout::uniformItemSize() const {
    return d->uniformItemSize;
}

////////////////////////////////////////////// QLayout::纯虚方法 ////////////////////////////////////////////////////////

void FlowLayout::addItem(QLayoutItem *item) {
    if(d->items.contains(item))
        return;
    d->items.append(item);
    d->invalidCache();
}

int FlowLayout::count() const {
    return d->items.count();
}

QLayoutItem *FlowLayout::itemAt(int index) const {
    if (index >= d->items.count())
        return nullptr;

    return d->items.at(index);
}

QLayoutItem *FlowLayout::takeAt(int index) {
    const auto item = d->items.at(index);
    removeItem(item);
    return item;
}

QSize FlowLayout::sizeHint() const {
    return d->calculateSize();
}

void FlowLayout::setGeometry(const QRect &rect) {
    if(geometry() == rect && d->cacheValid)
        return;
    Super::setGeometry(rect);
    d->doLayout(rect); // 实际布局
    d->calculateSize(); // 预填充缓存
}

////////////////////////////////////////////// QLayout::重写方法 ////////////////////////////////////////////////////////

bool FlowLayout::hasHeightForWidth() const {
    return d->flowOrder == FlowOrder::RowFirst;
}

int FlowLayout::heightForWidth(int width) const {
    if(d->flowOrder != FlowOrder::RowFirst)
        return -1;
    if(!d->layoutCache.heightForWidth.contains(width))
    {
        const auto &size = d->calculateSize(QSize(width, -1));
        d->layoutCache.heightForWidth.insert(width, size.height());
    }
    return d->layoutCache.heightForWidth[width];
}

////////////////////////////////////////////// QLayout::私有方法 ////////////////////////////////////////////////////////

void FlowLayout::FlowLayoutPrivate::invalidCache() {
    cacheValid = false;
    layoutCache.sizeHint = QSize();
    layoutCache.heightForWidth.clear();
    layoutCache.widthForHeight.clear();
    _this->update();
}

QSize FlowLayout::FlowLayoutPrivate::effectiveItemSize(QLayoutItem *item) const {
    if(uniformItemSize.isValid())
        return uniformItemSize;
    return item->sizeHint().expandedTo(QSize(1,1));
}

QSize FlowLayout::FlowLayoutPrivate::calculateSize(const QSize &constraint) {
    if (items.isEmpty())
        return QSize();

    // 如果存在缓存且无约束，直接返回
    if (cacheValid && constraint.isEmpty())
        return layoutCache.sizeHint;

    const bool isRowFirst = (flowOrder == FlowOrder::RowFirst);
    const int hSign = (horizontalFlow == HorizontalFlowDirection::LeftToRight) ? 1 : -1;
    const int vSign = (verticalFlow == VerticalFlowDirection::TopToBottom) ? 1 : -1;

    int x = 0, y = 0;
    int totalWidth = 0, totalHeight = 0;
    int lineHeight = 0, columnWidth = 0;

    // 根据约束调整初始值
    int maxWidth = constraint.width();  // 可能为-1（无约束）
    int maxHeight = constraint.height();

    // 临时存储用于计算总尺寸
    QVector<QSize> itemSizes;
    for (QLayoutItem *item : items) {
        itemSizes.append(effectiveItemSize(item));
    }

    for (const QSize &itemSize : itemSizes) {
        if (isRowFirst) {
            // 行主序的尺寸计算
            bool needWrap = (maxWidth > 0) &&
                            (x + hSign * (itemSize.width() + horizontalSpacing) > maxWidth);

            if (needWrap) {
                x = 0;
                y += vSign * (lineHeight + verticalSpacing);
                lineHeight = 0;
            }

            lineHeight = qMax(lineHeight, itemSize.height());
            x += hSign * (itemSize.width() + horizontalSpacing);
            totalWidth = qMax(totalWidth, x);
            totalHeight = y + vSign * lineHeight;
        } else {
            // 列主序的尺寸计算
            bool needNewColumn = (maxHeight > 0) &&
                                 (y + vSign * (itemSize.height() + verticalSpacing) > maxHeight);

            if (needNewColumn) {
                y = 0;
                x += hSign * (columnWidth + horizontalSpacing);
                columnWidth = 0;
            }

            columnWidth = qMax(columnWidth, itemSize.width());
            y += vSign * (itemSize.height() + verticalSpacing);
            totalHeight = qMax(totalHeight, y);
            totalWidth = x + hSign * columnWidth;
        }
    }

    // 计算实际尺寸（考虑方向符号）
    totalWidth = qAbs(totalWidth) - horizontalSpacing;
    totalHeight = qAbs(totalHeight) - verticalSpacing;

    return QSize(totalWidth, totalHeight);
}

void FlowLayout::FlowLayoutPrivate::doLayout(const QRect &r) const {
    if (items.isEmpty())
        return;

    const bool isRowFirst = (flowOrder == FlowOrder::RowFirst);
    const int hSign = (horizontalFlow == HorizontalFlowDirection::LeftToRight) ? 1 : -1; // 水平方向符号 (1 或 -1)
    const int vSign = (verticalFlow == VerticalFlowDirection::TopToBottom) ? 1 : -1; // 垂直方向符号 (1 或 -1)

    //const auto &margins = _this->contentsMargins();
    const auto &rect = _this->contentsRect();

    int x = rect.x();    // 当前行/列的起始坐标
    int y = rect.y();
    int lineHeight = 0;  // 行主序时记录行高
    int columnWidth = 0; // 列主序时记录列宽

    QVector<QRect> itemGeometries;

    // 遍历所有子项，计算每个项的位置
    for (QLayoutItem *item : items) {
        QSize itemSize = effectiveItemSize(item);

        if (isRowFirst) {
            // 行主序布局：优先水平排列，超出宽度则换行
            int nextX = x + hSign * (itemSize.width() + 0);
            bool exceedBoundary = (hSign > 0) ? (nextX > rect.right()) : (nextX < rect.left());

            if (exceedBoundary) {
                // 换行：重置x，y按垂直方向移动
                x = rect.x();
                y += vSign * (lineHeight + verticalSpacing);
                lineHeight = 0;
            }

            itemGeometries.append(QRect(x, y, itemSize.width(), itemSize.height()));
            lineHeight = qMax(lineHeight, itemSize.height());
            x += hSign * (itemSize.width() + horizontalSpacing);
        } else {
            // 列主序布局：优先垂直排列，超出高度则换列
            int nextY = y + vSign * (itemSize.height() + 0);
            bool exceedBoundary = (vSign > 0) ? (nextY > rect.bottom()) : (nextY < rect.top());

            if (exceedBoundary) {
                // 换列：重置y，x按水平方向移动
                y = rect.y();
                x += hSign * (columnWidth + horizontalSpacing);
                columnWidth = 0;
            }

            itemGeometries.append(QRect(x, y, itemSize.width(), itemSize.height()));
            columnWidth = qMax(columnWidth, itemSize.width());
            y += vSign * (itemSize.height() + verticalSpacing);
        }
    }

    // 应用逆向流动方向调整
    applyDirectionAdjustment(rect, itemGeometries);

    // 设置子项的实际几何位置
    for (int i = 0; i < items.size(); ++i) {
        items[i]->setGeometry(itemGeometries[i]);
    }
}

void FlowLayout::FlowLayoutPrivate::applyDirectionAdjustment(const QRect &rect, QVector<QRect> &geometries) const {
    if(horizontalFlow == HorizontalFlowDirection::LeftToRight && verticalFlow == VerticalFlowDirection::TopToBottom)
        return;

    // 计算布局总范围
    QRect totalBounding;
    for (const QRect &geo : geometries) {
        totalBounding = totalBounding.united(geo);
    }

    // 水平逆向（从右到左）
    if (horizontalFlow == HorizontalFlowDirection::RightToLeft) {
        const int rightEdge = rect.right();
        for (QRect &geo : geometries) {
            int itemRight = geo.right() - totalBounding.left();
            geo.moveRight(rightEdge - (geo.right() - totalBounding.left()));
        }
    }

    // 垂直逆向（从下到上）
    if (verticalFlow == VerticalFlowDirection::BottomToTop) {
        const int bottomEdge = rect.bottom();
        for (QRect &geo : geometries) {
            int itemBottom = geo.bottom() - totalBounding.top();
            geo.moveBottom(bottomEdge - (geo.bottom() - totalBounding.top()));
        }
    }
}
