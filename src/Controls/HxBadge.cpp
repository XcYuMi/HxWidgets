#include "HxBadge.hpp"
#include "HxWidgetsGlobalPrivate.hpp"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>

/// HxBadget ///

class HxBadge::HxBadgePrivate {
public:
    HxBadge::Position position = HxBadge::Position::TopRight;
    HxBadge::Position anchor = HxBadge::Position::Center;
    QPoint offset = QPoint(0,0);
};

HxBadge::~HxBadge() {

}

HxBadge::HxBadge(QWidget *parent) : QFrame{parent} {
    setFrameShape(QFrame::NoFrame);
}


void HxBadge::setPosition(Position position){
    if(d->position == position) {
        return;
    }

    d->position = position;
    emit positionChanged(d->position);
}

void HxBadge::setAnchor(Position anchor) {
    if(d->anchor == anchor) {
        return;
    }

    d->anchor = anchor;
    emit anchorChanged(d->anchor);
}

void HxBadge::setOffset(const QPoint &offset) {
    if(d->offset == offset) {
        return;
    }

    d->offset = offset;
    emit offsetChanged(d->offset);
}

HxBadge::Position HxBadge::position() const {
    return d->position;
}

HxBadge::Position HxBadge::anchor() const {
    return d->anchor;
}

QPoint HxBadge::offset() const {
    return d->offset;
}


HxTextBadge *HxBadge::addTextBadge(QWidget *widget, BadgePosition position, const QPoint &offset) {

}

/// BadgeManager ///

class HxBagdeManager::HxBagdeManagerPrivate {
    HxBagdeManager *_this{nullptr};
public:

    void init(HxBagdeManager *manager);
public:
    HxWidgetBadgeTrack *widgetBadgeTracker = nullptr;
    QMultiHash<QWidget*, HxBadge*> widgetBadgeHash;
};

void HxBagdeManager::HxBagdeManagerPrivate::init(HxBagdeManager *manager) {
    _this = manager;
}

HxBagdeManager::~HxBagdeManager() {

}

HxBagdeManager::HxBagdeManager(QObject *parent) : QObject{parent} {
    d.reset(new HxBagdeManagerPrivate);
    d->init(this);

    d->widgetBadgeTracker = new HxWidgetBadgeTrack(this);
}

HxBagdeManager *HxBagdeManager::instance() {
    static QPointer<HxBagdeManager> singleton;
    if(singleton.isNull()) {
        singleton = new HxBagdeManager(qApp);
    }
    return singleton;
}

HxBadge *HxBagdeManager::addBadge(QWidget *widget, HxBadge *badge) {
    if(!d->widgetBadgeHash.contains(widget)){
        widget->installEventFilter(d->widgetBadgeTracker);
    }

    d->widgetBadgeHash.insert(widget, data);
    d->widgetBadgeTracker->updateBadge(widget, badge);
}



/// TextBadge ///

class HxTextBadge::HxTextBadgePrivate {
    HxTextBadge *_this = nullptr;
public:
    void init(HxTextBadge *badge);
public:
    QString text;
    QLabel *label = nullptr;
};

void HxTextBadge::HxTextBadgePrivate::init(HxTextBadge *badge) {
    _this = badge;
}

HxTextBadge::~HxTextBadge() {

}

HxTextBadge::HxTextBadge() : HxBadge{nullptr} {
    d.reset(new HxTextBadgePrivate);
    d->init(this);

    d->label = new QLabel(this);
    d->label->setObjectName("TextBadgeLabel");

    const auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    layout->addWidget(d->label);
}

void HxTextBadge::setText(const QString &text) {
    if(d->text == text)
        return;

    d->text = text;
    d->label->setText(text);
}

QString HxTextBadge::text() const {
    return d->text;
}

/// IconBadge ///

class HxIconBadge::HxIconBadgePrivate {
    HxIconBadge *_this = nullptr;
public:
    void init(HxIconBadge *badge);
public:
    QIcon icon;
    QPicture picture;
    QSize iconSize = QSize(16,16);
};

void HxIconBadge::HxIconBadgePrivate::init(HxIconBadge *badge) {
    _this = badge;
}

HxIconBadge::~HxIconBadge() {

}

HxIconBadge::HxIconBadge() : HxBadge{nullptr} {
    d.reset(new HxIconBadgePrivate);
    d->init(this);
}

void HxIconBadge::setIcon(const QIcon &icon) {
    d->icon =icon;
    update();
}

void HxIconBadge::setIconSize(const QSize &size) {
    if(d->iconSize == size)
        return;

    d->iconSize = size;
    setFixedSize(d->iconSize);
}

void HxIconBadge::setPicture(const QPicture &picture) {
    d->picture = picture;
    update();
}

QIcon HxIconBadge::icon() const {
    return d->icon;
}

QSize HxIconBadge::iconSize() const {
    return d->iconSize;
}

void HxIconBadge::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if(!d->icon.isNull()) {
        const auto &pixmap = d->icon.pixmap(size());
        painter.drawPixmap(rect(), pixmap);
    } else if(!d->picture.isNull()) {
        painter.setBrush(Qt::red);
        d->picture.setBoundingRect(rect());
        d->picture.play(&painter);
    }
}

void HxWidgetBadgeTrack::updateBadge(QWidget *widget, HxBadge *badge) {
    auto parentWidget = widget->parent();
    if(parentWidget == nullptr) {
        parentWidget = widget;
    }

    badge->setParent(parentWidget);

    badge->adjustSize();

    const auto &rct = widget->geometry();

    auto rect = badge->geometry();

    const auto &offset = badge->offset();
    using Position = HxBadge::Position;
    const auto position = badge->position();
    const auto anchor = badge->anchor();
    QPoint point;
    switch(position) {
    case Position::Top:
        point.rx() = rct.center().x();
        point.ry() = rct.top();
        break;
    case Position::TopLeft:
        point = rct.topLeft();
        break;
    case Position::TopRight:
        point = rct.topRight();
    case Position::Left:

    }

    badge->raise();

}
