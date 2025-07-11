#include "SharedWidgetItemDelagateTestWidget.hpp"
#include "WidgetsTestGlobalPrivate.hpp"

class SharedWidgetItemDelagateTestWidget::SharedWidgetItemDelagateTestWidgetUi {
    SharedWidgetItemDelagateTestWidget *_this = nullptr;
public:
    void setupUi(SharedWidgetItemDelagateTestWidget *widget);
    void retranslateUi();
public:

};

SharedWidgetItemDelagateTestWidget::~SharedWidgetItemDelagateTestWidget() {

}

SharedWidgetItemDelagateTestWidget::SharedWidgetItemDelagateTestWidget(QWidget *parent) : QWidget{parent} {
    ui.reset(new SharedWidgetItemDelagateTestWidgetUi);
    ui->setupUi(this);
    ui->retranslateUi();
}

void SharedWidgetItemDelagateTestWidget::SharedWidgetItemDelagateTestWidgetUi::setupUi(SharedWidgetItemDelagateTestWidget *widget) {
    _this = widget;

}

void SharedWidgetItemDelagateTestWidget::SharedWidgetItemDelagateTestWidgetUi::retranslateUi() {
    _this->setWindowTitle(tr("共享部件视图项代理测试"));
}
