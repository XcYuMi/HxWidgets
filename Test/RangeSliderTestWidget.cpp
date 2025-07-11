#include "RangeSliderTestWidget.hpp"
#include "WidgetsTestGlobalPrivate.hpp"

class RangeSliderTestWidget::RangeSliderTestWidgetUi {
    RangeSliderTestWidget *_this{nullptr};
public:
    void setupUi(RangeSliderTestWidget *widget);
    void retranslateUi();
};

RangeSliderTestWidget::~RangeSliderTestWidget() { }

RangeSliderTestWidget::RangeSliderTestWidget(QWidget *parent) : QWidget{parent} {
    ui.reset(new RangeSliderTestWidgetUi);
    ui->setupUi(this);
    ui->retranslateUi();
}

void RangeSliderTestWidget::RangeSliderTestWidgetUi::setupUi(RangeSliderTestWidget *widget) {
    _this = widget;
    _this->setWindowFlags(Qt::Widget | Qt::WindowCloseButtonHint);
    _this->setMinimumSize(QSize(16, 9) * 15);
}

void RangeSliderTestWidget::RangeSliderTestWidgetUi::retranslateUi() {
    _this->setWindowTitle("范围滑块测试窗口");
}
