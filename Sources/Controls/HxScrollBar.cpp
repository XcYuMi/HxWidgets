#include "HxScrollBar.hpp"
#include "HxWidgetsGlobalPrivate.hpp"

class HxScrollBar::HxScrollBarPrivate {
    HxScrollBar *q = nullptr;
public:
    HxScrollBarPrivate(HxScrollBar *qptr) { q = qptr;}
public:
};

HxScrollBar::~HxScrollBar() {
}

HxScrollBar::HxScrollBar(QWidget *parent) : Super{parent} {
    d.reset(new HxScrollBarPrivate(this));
}
