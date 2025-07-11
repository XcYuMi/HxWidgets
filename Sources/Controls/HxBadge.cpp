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
    d.reset(new HxBadgePrivate);
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


HxTextBadge *HxBadge::addTextBadge(QWidget *widget, Position position, const QPoint &offset) {
    const auto badge = new HxTextBadge;
    badge->setPosition(position);
    badge->setOffset(offset);
    bageManager()->addBadge(widget, badge);
    return badge;
}

HxIconBadge *HxBadge::addIconBadge(QWidget *widget, Position position, const QPoint &offset)
{
    const auto badge = new HxIconBadge;
    badge->setPosition(position);
    badge->setOffset(offset);
    bageManager()->addBadge(widget, badge);
    return badge;
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

void HxBagdeManager::addBadge(QWidget *widget, HxBadge *badge) {
    if(!d->widgetBadgeHash.contains(widget)){
        widget->installEventFilter(d->widgetBadgeTracker);
    }

    d->widgetBadgeHash.insert(widget, badge);
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
    Q_UNUSED(event);
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
    auto parentWidget = widget->parentWidget();
    if(parentWidget == nullptr) {
        parentWidget = widget;
    }

    badge->setParent(parentWidget);
    badge->adjustSize();

    const auto &widgetGeometry = widget->geometry();
    const auto &badgeRect = badge->geometry();

    const auto &offset = badge->offset();
    const auto position = badge->position();
    const auto anchor = badge->anchor();
    const auto &rect = calculateBadgeGeometry(widgetGeometry, badgeRect, offset, position, anchor);

    badge->move(rect.topLeft());
    badge->raise();
    badge->show();
}

bool HxWidgetBadgeTrack::eventFilter(QObject *watched, QEvent *event)
{
    const bool result = Super::eventFilter(watched, event);
    return result;
}

QRect HxBadgeTargetTrack::calculateBadgeGeometry(const QRect &targetRect,
                                                 const QRect &badgeGeometry,
                                                 const QPoint &offset,
                                                 HxBadge::Position position,
                                                 HxBadge::Position anchor)
{
    const auto &rct = targetRect;
    using Position = HxBadge::Position;

    auto rect = badgeGeometry;
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
        break;
    case Position::Left:
        point.rx() = rct.left();
        point.ry() = rct.center().y();
        break;
    case Position::Center:
        point = rct.center();
        break;
    case Position::Right:
        point.rx() = rct.right();
        point.ry() = rct.center().y();
        break;
    case Position::BottomLeft:
        point = rct.bottomLeft();
        break;
    case Position::Bottom:
        point.rx() = rct.center().x();
        point.ry() = rct.bottom();
    case Position::BottomRight:
        point = rct.bottomRight();
    default:
        break;
    }

    point += offset;

    switch (anchor) {
    case Position::TopLeft:
        rect.moveTopLeft(point);
        break;
    case Position::Top:
        rect.moveCenter(point);
        rect.moveTop(point.y());
        break;
    case Position::TopRight:
        rect.moveTopRight(point);
        break;
    case Position::Left:
        rect.moveCenter(point);
        rect.moveLeft(point.x());
        break;
    case Position::Center:
        rect.moveCenter(point);
        break;
    case Position::Right:
        rect.moveCenter(point);
        rect.moveRight(point.x());
        break;
    case Position::BottomLeft:
        rect.moveBottomLeft(point);
        break;
    case Position::Bottom:
        rect.moveCenter(point);
        rect.moveBottom(point.y());
        break;
    case Position::BottomRight:
        rect.moveBottomRight(point);
        break;
    default:
        break;
    }

    return rect;
}
