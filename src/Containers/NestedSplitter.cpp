#include "NestedSplitter.hpp"
#include "HxWidgetsGlobalPrivate.hpp"
#include "WidgetUtils.hpp"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

HX_USING_NAMESPACE;

class NestedSplitter::NestedSplitterPrivate {
public:
    QWidget* container = nullptr;
    QList<QWidget*> orderedWidgets; // 添加顺序记录
    QHash<QWidget*, QPair<QWidget*, Qt::Orientation>> relations;
    QHash<QPair<QWidget*, QWidget*>, SplitterHandler*> handlers;
};

NestedSplitter::~NestedSplitter() {
    qDeleteAll(d->handlers.values());
    d->handlers.clear();

    qDeleteAll(d->orderedWidgets);
    d->orderedWidgets.clear();

    d->relations.clear();
}

NestedSplitter::NestedSplitter(QObject* parent) : QObject(parent) {
    d.reset(new NestedSplitterPrivate);
}

void NestedSplitter::setContainer(QWidget* container) {
    if (d->container) {
        d->container->removeEventFilter(this);
        d->container->deleteLater();
    }

    d->container = container;
    if (d->container) {
        d->container->installEventFilter(this);
        d->container->setMouseTracking(true);
    }
}

void NestedSplitter::addWidget(QWidget* widget, Qt::Orientation orientation) {
    if (!d->container || d->container->children().contains(widget))
        return;

    if (d->relations.isEmpty()) {
        widget->setParent(d->container);
        widget->show();
        widget->setGeometry(d->container->rect());
        d->relations[widget] = {nullptr, Qt::Horizontal};
        d->orderedWidgets.append(widget); // 记录顺序
        return;
    }
    splitWidget(d->orderedWidgets.last(), widget, orientation);
}

void NestedSplitter::splitWidget(QWidget* existing, QWidget* newcomer, Qt::Orientation orientation) {
    if (!d->relations.contains(existing))
        return;

    newcomer->setParent(d->container);
    SplitterHandler* handler = createHandler();
    d->handlers.insert({existing, newcomer}, handler);
    d->relations[newcomer] = {existing, orientation};
    d->orderedWidgets.append(newcomer); // 记录新部件
    updateLayout();
}

QJsonObject NestedSplitter::saveState() const {
    QJsonObject state;
    QJsonArray widgets;
    for (auto it = d->relations.constBegin(); it != d->relations.constEnd(); ++it) {
        QJsonObject item;
        item["id"] = it.key()->objectName();
        item["parent"] = it.value().first ? it.value().first->objectName() : "";
        item["orientation"] = it.value().second;
        widgets.append(item);
    }
    state["widgets"] = widgets;
    return state;
}

void NestedSplitter::restoreState(const QJsonObject& state) {
    // 实现反序列化逻辑
}

bool NestedSplitter::eventFilter(QObject* watched, QEvent* event) {
    if (watched == d->container && event->type() == QEvent::Resize) {
        updateLayout();
    }
    return QObject::eventFilter(watched, event);
}

SplitterHandler* NestedSplitter::createHandler() {
    auto handler = new SplitterHandler(d->container);
    connect(handler, &SplitterHandler::geometryChanged,
            this, &NestedSplitter::updateLayout);
    return handler;
}

void NestedSplitter::updateLayout() {
    if (!d->container)
        return;

    QWidget* root = findRootWidget();
    if (root)
        calculateGeometry(root, d->container->rect());

    for (auto it = d->handlers.begin(); it != d->handlers.end(); ++it) {
        auto [first, second] = it.key();
        Qt::Orientation orient = d->relations[second].second;
        it.value()->setGeometry(calculateHandlerGeometry(first, second, orient));
    }
}

void NestedSplitter::calculateGeometry(QWidget* widget, const QRect& available) {
    widget->setGeometry(available);

    for (auto it = d->relations.begin(); it != d->relations.end(); ++it) {
        if (it.value().first == widget) {
            QRect childRect;
            SplitterHandler* handler = d->handlers.value({widget, it.key()});

            if (it.value().second == Qt::Horizontal) {
                childRect = QRect(available.right() - handler->width(), available.top(),
                                  available.width() - handler->width(), available.height());
                WidgetUtils::setWidth(widget,available.width() - childRect.width() - handler->width());
            } else {
                childRect = QRect(available.left(), available.bottom() - handler->height(),
                                  available.width(), available.height() - handler->height());
                WidgetUtils::setHeight(widget, available.height() - childRect.height() - handler->height());
            }
            calculateGeometry(it.key(), childRect);
        }
    }
}

QWidget* NestedSplitter::findRootWidget() const {
    for (auto it = d->relations.begin(); it != d->relations.end(); ++it) {
        if (!it.value().first) return it.key();
    }
    return nullptr;
}

QRect NestedSplitter::calculateHandlerGeometry(QWidget* first, QWidget* second,
                                               Qt::Orientation orientation) const {
    const int handleSize = 5;
    if (orientation == Qt::Horizontal) {
        return QRect(first->geometry().right(), first->y(),
                     handleSize, first->height());
    }
    return QRect(first->x(), first->geometry().bottom(),
                 first->width(), handleSize);
}

class SplitterHandler::SplitterHandlerPrivate {
public:
    QPoint dragStartPos;
};

SplitterHandler::~SplitterHandler() { }

SplitterHandler::SplitterHandler(QWidget* parent) : QWidget(parent) {
    d.reset(new SplitterHandlerPrivate);

    setMouseTracking(true);
    setCursor(Qt::SizeAllCursor);
}

void SplitterHandler::mousePressEvent(QMouseEvent* event) {
    d->dragStartPos = event->pos();
    QWidget::mousePressEvent(event);
}

void SplitterHandler::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        move(mapToParent(event->pos() - d->dragStartPos));
        emit geometryChanged();
    }
}

void SplitterHandler::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.fillRect(rect(), QColor(160, 160, 160, 128));
}
