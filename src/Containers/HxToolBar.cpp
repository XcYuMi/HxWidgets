#include "HxToolBar.hpp"
#include <QPointer>
#include <QHBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QToolButton>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

#include <variant>

bool operator ==(const HxToolBar::BoxLayoutParams& one, const HxToolBar::BoxLayoutParams& another) {
    bool equals = one.alignment == another.alignment;
    equals &= one.direction == another.direction;
    equals &= one.spacing == another.spacing;
    return equals;
}

bool operator ==(const HxToolBar::FlowLayoutParams& one, const HxToolBar::FlowLayoutParams& another) {
    bool equals = one.flowOrder == another.flowOrder;
    equals &= one.horizontalDirection == another.horizontalDirection;
    equals &= one.horizontalSpacing == another.horizontalSpacing;
    equals &= one.verticalDirection == another.verticalDirection;
    equals &= one.verticalSpacing == another.verticalSpacing;
    return equals;
}

class HxToolBar::HxToolBarPrivate
{
    HxToolBar *q = nullptr;
public:
    HxToolBarPrivate(HxToolBar *qptr);
    void init();
    void updateLayoutParams();
    void setShadowEnabled(bool enabled);
    bool isBoxLayout() const;
    bool isFlowLayout() const;
    QBoxLayout *boxLayout() const;
    Hx::FlowLayout *flowLayout() const;
    QWidget *createSeparator(const QString &name = "");
    QLabel *createSection(const QString &name = "");
public:
    QPointer<QLayout> layout;
    Qt::ToolBarAreas qreas = Qt::ToolBarArea::NoToolBarArea;
    std::variant<std::monostate, HxToolBar::BoxLayoutParams, HxToolBar::FlowLayoutParams> layoutParams;
    QMargins margins = QMargins(3, 3, 3, 3);
};

HxToolBar::HxToolBarPrivate::HxToolBarPrivate(HxToolBar *qptr) {
    q = qptr;
}

void HxToolBar::HxToolBarPrivate::init() {

}

void HxToolBar::HxToolBarPrivate::updateLayoutParams() {
    if(std::holds_alternative<HxToolBar::BoxLayoutParams>(layoutParams)) {
        const auto &params = std::get<HxToolBar::BoxLayoutParams>(layoutParams);
        if(layout == nullptr || !layout->metaObject()->inherits(&(QBoxLayout::staticMetaObject))) {
            layout = new QBoxLayout(params.direction);
            q->setLayout(nullptr);
            q->setLayout(layout);
        }
        const auto boxLayout = qobject_cast<QBoxLayout*>(layout);
        boxLayout->setDirection(params.direction);
        boxLayout->setAlignment(params.alignment);
        boxLayout->setSpacing(params.spacing);
        boxLayout->setContentsMargins(margins);
    } else if(std::holds_alternative<HxToolBar::FlowLayoutParams>(layoutParams)) {
        const auto &params = std::get<HxToolBar::FlowLayoutParams>(layoutParams);
        if(layout == nullptr || !layout->metaObject()->inherits(&(Hx::FlowLayout::staticMetaObject))) {
            layout = new Hx::FlowLayout;
            q->setLayout(nullptr);
            q->setLayout(layout);
        }
        const auto flowLayout = qobject_cast<Hx::FlowLayout*>(layout);
        flowLayout->setFlowOrder(params.flowOrder);
        flowLayout->setHorizontalFlow(params.horizontalDirection);
        flowLayout->setVerticalFlow(params.verticalDirection);
        flowLayout->setHorizontalSpacing(params.horizontalSpacing);
        flowLayout->setVerticalSpacing(params.verticalSpacing);
        flowLayout->setContentsMargins(margins);
    }
}

bool HxToolBar::HxToolBarPrivate::isBoxLayout() const {
    bool is = std::holds_alternative<HxToolBar::BoxLayoutParams>(layoutParams);
    return is;
}

bool HxToolBar::HxToolBarPrivate::isFlowLayout() const {
    bool is = std::holds_alternative<HxToolBar::FlowLayoutParams>(layoutParams);
    return is;
}

QBoxLayout *HxToolBar::HxToolBarPrivate::boxLayout() const {
    const auto boxLayout = qobject_cast<QBoxLayout*>(q->layout());
    return boxLayout;
}

Hx::FlowLayout *HxToolBar::HxToolBarPrivate::flowLayout() const {
    const auto flowLayout = qobject_cast<Hx::FlowLayout*>(layout.data());
    return flowLayout;
}

QWidget *HxToolBar::HxToolBarPrivate::createSeparator(const QString &name) {
    const auto separator = new QFrame(q);
    separator->setFrameShape(QFrame::NoFrame);
    separator->setMidLineWidth(0);
    separator->setLineWidth(0);
    separator->setObjectName(name.isEmpty() ? "ToolBarSeparator" : name);
    if(q->orientation() == Qt::Horizontal) {
        separator->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    } else {
        separator->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    }
    return separator;
}

QLabel *HxToolBar::HxToolBarPrivate::createSection(const QString &name) {
    const auto section = new QLabel(q);
    section->setObjectName(name.isEmpty() ? "ToolBarSection" : name);
    return section;
}

HxToolBar::HxToolBar(QWidget *parent) : QFrame{parent} {
    d.reset(new HxToolBarPrivate(this));
    /*/
    BoxLayoutParams params;
    setLayoutParams(params); //*/
}

HxToolBar::~HxToolBar() {

}

void HxToolBar::addWidget(QWidget *widget, int stretch) {
    if(d->isBoxLayout()) {
        const auto layout = d->boxLayout();
        layout->addWidget(widget, stretch);
    } else if(d->isFlowLayout()) {
        const auto layout = d->flowLayout();
        layout->addWidget(widget);
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "layout is null!");
    }
}

void HxToolBar::removeWidget(QWidget *widget) {
    if(d->isBoxLayout()) {
        const auto layout = d->boxLayout();
        layout->removeWidget(widget);
    } else if(d->isFlowLayout()) {
        const auto layout = d->flowLayout();
        layout->removeWidget(widget);
    } else {
        
    }
}

void HxToolBar::addToolButton(QToolButton *button) {
    addWidget(button);
}

QToolButton *HxToolBar::addToolButton(const QString &text) {
    QToolButton *button = new QToolButton(this);
    button->setText(text);
    return button;
}

QToolButton *HxToolBar::addToolButton(const QIcon &icon, const QString &text) {
    QToolButton *button = new QToolButton(this);
    button->setIcon(icon);
    button->setText(text);
    return button;
}

QToolButton *HxToolBar::addToolButton(const QString &name, const QString &tip) {
    QToolButton *button = new QToolButton(this);
    button->setObjectName(name);
    button->setToolTip(tip);
    return button;
}

void HxToolBar::addAction(QAction *action) {
    if(action->isSeparator()) { // 分隔符或者栏目
        const auto &text = action->text();
        if(text.isEmpty()) { // 文本为空是分隔符
            const auto separator = d->createSeparator();
            addWidget(separator);
            const auto &UpdateProperties = [=] {
                separator->setVisible(action->isVisible());
            };
            UpdateProperties();
            connect(action, &QAction::changed, UpdateProperties);
            connect(action, &QAction::destroyed, this, [=] {
                removeWidget(separator);
            });

        } else { // 文本不为空是栏目
            const auto section = d->createSection();
            addWidget(section);
            const auto &UpdateProperties = [=] {
                section->setVisible(action->isVisible());
            };
            UpdateProperties();
            connect(action, &QAction::changed, UpdateProperties);
            connect(action, &QAction::destroyed, this, [=] {
                removeWidget(section);
            });
        }
    } else { // Action类型添加ToolButton
        const auto button = new QToolButton(this);
        addWidget(button);

        const auto UpdateProperties = [=] {
            button->setText(action->text());
            button->setIcon(action->icon());
            button->setCheckable(action->isCheckable());
            button->setChecked(action->isChecked());
            button->setToolTip(action->toolTip());
            button->setStatusTip(action->statusTip());
            button->setEnabled(action->isEnabled());
            button->setVisible(action->isVisible());
        };
        UpdateProperties();
        connect(action, &QAction::changed, UpdateProperties);
        connect(action, &QAction::destroyed, this, [=] {
            removeWidget(button);
        });
        connect(button, &QToolButton::clicked, action, &QAction::trigger);
    }
}

QAction *HxToolBar::addAction(const QString &text) {
    QAction *action = new QAction(this);
    action->setText(text);
    addAction(action);
    return action;
}

QAction *HxToolBar::addAction(const QIcon &icon, const QString &text) {
    QAction *action = new QAction(this);
    action->setIcon(icon);
    action->setText(text);
    addAction(action);
    return action;
}

void HxToolBar::addStretch(int stretch, const QString &name) {
    if(d->isFlowLayout()) {
        return;
    }

    const auto layout = d->boxLayout();
    if(name.isEmpty()) {
        layout->addStretch(stretch);
    } else {

    }
}

void HxToolBar::addSpacing(int spacing, const QString &name) {
    if(d->isFlowLayout()) {
        return;
    }

    const auto layout = d->boxLayout();
    if(name.isEmpty()) {
        layout->addSpacing(spacing);
    } else {

    }
}

void HxToolBar::addSeparator(const QString &name) {
    const auto separator = d->createSeparator(name);
    addWidget(separator, 0);
}

Qt::Orientation HxToolBar::orientation() const {
    if(const auto layout = d->boxLayout()) {
        const auto direction = layout->direction();
        switch(direction) {
        case QBoxLayout::LeftToRight:
        case QBoxLayout::RightToLeft:
            return Qt::Horizontal;
            break;
        case QBoxLayout::TopToBottom:
        case QBoxLayout::BottomToTop:
            return Qt::Vertical;
        default:
            Q_UNREACHABLE();
        }
    } else if(const auto layout = d->flowLayout()) {
        const auto order = layout->flowOrder();
        if(order == Hx::FlowLayout::FlowOrder::RowFirst) {
            return Qt::Horizontal;
        } else {
            return Qt::Vertical;
        }
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "layout is null!");
        return Qt::Horizontal;
    }
}

QMargins HxToolBar::margins() const {
    return d->margins;
}

void HxToolBar::setMargins(const QMargins& margins) {
    if (d->margins == margins)
        return;
    d->margins;
    d->updateLayoutParams();
}

void HxToolBar::setMargins(int left, int top, int right, int bottom) {
    setMargins(QMargins{ left, top, right, bottom });
}

void HxToolBar::setLayoutParams(const BoxLayoutParams &params) {
    if(!std::holds_alternative<BoxLayoutParams>(d->layoutParams)) {
        d->layoutParams = params;
        d->updateLayoutParams();
        return;
    }

    if(std::get<BoxLayoutParams>(d->layoutParams) == params) {
        return;
    }

    d->layoutParams = params;
    d->updateLayoutParams();
}

void HxToolBar::setLayoutParams(const HxToolBar::FlowLayoutParams &params) {
    if(!std::holds_alternative<FlowLayoutParams>(d->layoutParams)) {
        d->layoutParams = params;
        d->updateLayoutParams();
        return;
    }

    if(std::get<FlowLayoutParams>(d->layoutParams) == params) {
        return;
    }

    d->layoutParams = params;
    d->updateLayoutParams();
}

void HxToolBar::changeEvent(QEvent *event)
{
    Q_UNUSED(event);
}
