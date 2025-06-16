#include "HxToolBar.hpp"
#include <QPointer>
#include <QHBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QGraphicsDropShadowEffect>

#include <variant>

bool operator ==(const HxToolBar::BoxLayoutParams& one, const HxToolBar::BoxLayoutParams& another)
{
    bool equals = one.alignment == another.alignment;
    equals &= one.direction == another.direction;
    equals &= one.spacing == another.spacing;
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
public:
    QBoxLayout *layout = nullptr;
    Qt::ToolBarAreas qreas = Qt::ToolBarArea::NoToolBarArea;
    std::variant<std::monostate, HxToolBar::BoxLayoutParams, HxToolBar::FlowLayoutParams> layoutParams;
};

HxToolBar::HxToolBarPrivate::HxToolBarPrivate(HxToolBar *qptr) {
    q = qptr;
}

void HxToolBar::HxToolBarPrivate::init() {
    layout = new QBoxLayout(QBoxLayout::LeftToRight);
    q->setLayout(layout);
}

void HxToolBar::HxToolBarPrivate::updateLayoutParams() {
}

HxToolBar::HxToolBar(QWidget *parent)
    : QFrame{parent}
{}

HxToolBar::~HxToolBar()
{

}

void HxToolBar::setLayoutParams(const BoxLayoutParams &params)
{
    if(!std::holds_alternative<BoxLayoutParams>(d->layoutParams))
    {
        d->layoutParams = params;
        d->updateLayoutParams();
        return;
    }

    if(std::get<BoxLayoutParams>(d->layoutParams) == params)
        return;

    d->layoutParams = params;
    d->updateLayoutParams();



}

void HxToolBar::setLayoutParams(const HxToolBar::FlowLayoutParams &params)
{

}

void HxToolBar::changeEvent(QEvent *event)
{

}
