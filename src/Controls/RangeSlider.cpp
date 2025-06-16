#include "RangeSlider.hpp"
#include "HxWidgetsGlobalPrivate.hpp"

class RangeSlider::RangeSliderPrivate {
    RangeSlider *_this{nullptr};
public:
    RangeSliderPrivate(RangeSlider *slider) { _this = slider; }
};

RangeSlider::~RangeSlider() { }

RangeSlider::RangeSlider(QWidget *parent) : QWidget{parent} {
    d.reset(new RangeSliderPrivate(this));
}
