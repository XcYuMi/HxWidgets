#include "DoubleRangerSlider.hpp"
#include "HxWidgetsGlobalPrivate.hpp"

class DoubleRangerSlider::DoubleRangerSliderPrivate {
    DoubleRangerSlider *_this{nullptr};
public:
    DoubleRangerSliderPrivate(DoubleRangerSlider *slider) { _this = slider; }
};

DoubleRangerSlider::~DoubleRangerSlider() { }

DoubleRangerSlider::DoubleRangerSlider(QWidget *parent) : QWidget{parent} {
    d.reset(new DoubleRangerSliderPrivate(this));
}


