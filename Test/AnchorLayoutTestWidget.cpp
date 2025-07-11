#include "AnchorLayoutTestWidget.hpp"
#include "WidgetsTestGlobalPrivate.hpp"

class AnchorLayoutTestWidget::AnchorLayoutTestWidgetUi {
    AnchorLayoutTestWidget *_this{nullptr};
public:
    void setupUi(AnchorLayoutTestWidget *widget);
    void retranslateUi();
};

AnchorLayoutTestWidget::~AnchorLayoutTestWidget() { }

AnchorLayoutTestWidget::AnchorLayoutTestWidget(QWidget *parent) : QWidget{parent} {
    ui.reset(new AnchorLayoutTestWidgetUi);
    ui->setupUi(this);
    ui->retranslateUi();
}

void AnchorLayoutTestWidget::AnchorLayoutTestWidgetUi::setupUi(AnchorLayoutTestWidget *widget) {
    _this = widget;
    _this->setWindowFlags(Qt::Widget | Qt::WindowCloseButtonHint);
    _this->setMinimumSize(QSize(16, 9) * 15);
}

void AnchorLayoutTestWidget::AnchorLayoutTestWidgetUi::retranslateUi() {
    _this->setWindowTitle("锚布局测试窗口");
}
