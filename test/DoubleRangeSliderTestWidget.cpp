#include "DoubleRangeSliderTestWidget.hpp"
#include "WidgetsTestGlobalPrivate.hpp"

class DoubleRangeSliderTestWidget::DoubleRangeSliderTestWidgetUi {
    DoubleRangeSliderTestWidget *_this{nullptr};
public:
    void setupUi(DoubleRangeSliderTestWidget *widget);
    void retranslateUi();
};

DoubleRangeSliderTestWidget::~DoubleRangeSliderTestWidget() { }

DoubleRangeSliderTestWidget::DoubleRangeSliderTestWidget(QWidget *parent) : QWidget{parent} {
    ui.reset(new DoubleRangeSliderTestWidgetUi);
    ui->setupUi(this);
    ui->retranslateUi();
}

void DoubleRangeSliderTestWidget::DoubleRangeSliderTestWidgetUi::setupUi(DoubleRangeSliderTestWidget *widget) {
    _this = widget;
    _this->setWindowFlags(Qt::Widget | Qt::WindowCloseButtonHint);
    _this->setMinimumSize(QSize(16, 9) * 15);
}

void DoubleRangeSliderTestWidget::DoubleRangeSliderTestWidgetUi::retranslateUi() {
    _this->setWindowTitle("浮点数范围滑块测试窗口");
}
