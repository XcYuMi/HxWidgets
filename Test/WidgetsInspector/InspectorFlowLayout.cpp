#include "InspectorFlowLayout.h"

#include <QStyle>
#include <QWidget>

int MyFlowLayout::doLayout(const QRect &rect, bool testOnly) const {
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;
    //! [9]

    //! [10]
    for (QLayoutItem *item : std::as_const(p.itemList)) {
        const QWidget *wid = item->widget();
        int spaceX = horizontalSpacing();
        if (spaceX == -1)
            spaceX = wid->style()->layoutSpacing(
                QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
        int spaceY = verticalSpacing();
        if (spaceY == -1)
            spaceY = wid->style()->layoutSpacing(
                QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
        //! [10]
        //! [11]
        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if (!testOnly)
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - rect.y() + bottom;
}

int MyFlowLayout::smartSpacing(QStyle::PixelMetric pm) const {
    QObject *parent = this->parent();
    if (!parent) {
        return -1;
    } else if (parent->isWidgetType()) {
        QWidget *pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, nullptr, pw);
    } else {
        return static_cast<QLayout *>(parent)->spacing();
    }
}

void MyFlowLayout::addItem(QLayoutItem *item) {
    p.itemList.append(item);
}

int MyFlowLayout::heightForWidth(int width) const {
    int height = doLayout(QRect(0, 0, width, 0), true);
    return height;
}

int MyFlowLayout::count() const {
    return p.itemList.count();
}

QLayoutItem* MyFlowLayout::itemAt(int index) const {
    if (count() <= index) {
        return nullptr;
    }
    return p.itemList.at(index); 
}

QSize MyFlowLayout::minimumSize() const {
    QSize size;
    for (const QLayoutItem *item : std::as_const(p.itemList)) {
        size = size.expandedTo(item->minimumSize());
    }

    const QMargins margins = contentsMargins();
    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
    return size;
}

void MyFlowLayout::setGeometry(const QRect &rect) {
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QLayoutItem * MyFlowLayout::takeAt(int index) {
    if (index >= 0 && index < p.itemList.size()) {
        return p.itemList.takeAt(index);
    }
    return nullptr;
}

int MyFlowLayout::horizontalSpacing() const {
    if (p.horSpacing >= 0) {
        return p.horSpacing;
    } else {
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

void MyFlowLayout::setHorizontalSpacing(int spacing) {
    if(p.horSpacing == spacing) {
        return ;
    }

    p.horSpacing = spacing;
}

int MyFlowLayout::verticalSpacing() const {
    if (p.verSpacing >= 0) {
        return p.verSpacing;
    } else {
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}

void MyFlowLayout::setVerticalSpacing(int spacing) {
    if(p.verSpacing == spacing) {
        return ;
    }

    p.verSpacing = spacing;
}
