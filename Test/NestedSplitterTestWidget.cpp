#include "NestedSplitterTestWidget.hpp"
#include "WidgetsTestGlobalPrivate.hpp"

class NestedSplitterTestWidget::NestedSplitterTestWidgetUi {
    NestedSplitterTestWidget *_this{nullptr};
public:
    void setupUi(NestedSplitterTestWidget *widget);
    void retreanslateUi();
};

NestedSplitterTestWidget::~NestedSplitterTestWidget() { }

NestedSplitterTestWidget::NestedSplitterTestWidget(QWidget *parent) : QWidget{parent} {
    ui.reset(new NestedSplitterTestWidgetUi);
    ui->setupUi(this);
    ui->retreanslateUi();
}

void NestedSplitterTestWidget::NestedSplitterTestWidgetUi::setupUi(NestedSplitterTestWidget *widget) {
    _this = widget;
    _this->setWindowFlags(Qt::Widget | Qt::WindowCloseButtonHint);
    _this->setMinimumSize(QSize(16, 9) * 15);
}

void NestedSplitterTestWidget::NestedSplitterTestWidgetUi::retreanslateUi() {
    _this->setWindowTitle("嵌套式分割器测试窗口");
}
