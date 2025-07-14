#include "TestStarterWidget.hpp"
#include "WidgetsTestGlobalPrivate.hpp"
#include "HxToolBar.hpp"

#include <QVBoxLayout>
#include "FlowLayoutTestWidget.hpp"
#include "AnchorLayoutTestWidget.hpp"
#include "NestedSplitterTestWidget.hpp"
#include "RangeSliderTestWidget.hpp"
#include "DoubleRangeSliderTestWidget.hpp"
#include "ToolBarTestWidget.hpp"
#include "BadgeTestWidget.hpp"
#include "SettingsDialogSearchTestWidget.hpp"
#include "ItemWidgetTestWidget.hpp"
#include "SharedWidgetItemDelagateTestWidget.hpp"

class TestStarterWidget::TestStarterWidgetUi {
    TestStarterWidget *_this = nullptr;
public:
    void setupUi(TestStarterWidget *widget);
    void retranslateUi();
public:
    HxToolBar *toolBar = nullptr;
};

TestStarterWidget::~TestStarterWidget() { }

TestStarterWidget::TestStarterWidget(QWidget *parent) : QWidget(parent) {
    ui.reset(new TestStarterWidgetUi);
    ui->setupUi(this);
    initTestPages();
    ui->retranslateUi();
}

void TestStarterWidget::TestStarterWidgetUi::setupUi(TestStarterWidget *widget) {
    _this = widget;
    _this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    const auto layout = new QVBoxLayout(_this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    toolBar = new HxToolBar(_this);
    layout->addWidget(toolBar);
    
    HxToolBar::FlowLayoutParams params;
    params.flowOrder = Hx::FlowLayout::FlowOrder::RowFirst;
    params.horizontalDirection = Hx::FlowLayout::HorizontalFlowDirection::LeftToRight;
    params.verticalDirection = Hx::FlowLayout::VerticalFlowDirection::TopToBottom;
    params.horizontalSpacing = 12;
    params.verticalSpacing = 6; 
    toolBar->setLayoutParams(params); 
}

void TestStarterWidget::initTestPages() {
    const auto &ShowPage = [=](QWidget *page) {
        page->setParent(this);
        page->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
        page->setAttribute(Qt::WA_DeleteOnClose);
        page->show();
    };

    ui->toolBar->addAction(tr("流动布局"), [=] { ShowPage(new FlowLayoutTestWidget); });
    ui->toolBar->addAction(tr("锚点布局"), [=] { ShowPage(new AnchorLayoutTestWidget); });
    ui->toolBar->addAction(tr("嵌套分割器"), [=] { ShowPage(new NestedSplitterTestWidget); });
    ui->toolBar->addAction(tr("范围滑块"), [=] { ShowPage(new RangeSliderTestWidget); });
    ui->toolBar->addAction(tr("浮点范围滑块"), [=] { ShowPage(new DoubleRangeSliderTestWidget); });
    ui->toolBar->addAction(tr("工具条"), [=] { ShowPage(new ToolBarTestWidget); });
    ui->toolBar->addAction(tr("徽标"), [=] { ShowPage(new BadgeTestWidget); });
    ui->toolBar->addAction(tr("设置对框框搜索"), [=] { ShowPage(new SettingsDialogSearchTestWidget); });
    ui->toolBar->addAction(tr("项视图"), [=] { ShowPage(new ItemWidgetTestWidget); });
    ui->toolBar->addAction(tr("共享部件视图项代理"), [=] { ShowPage(new SharedWidgetItemDelagateTestWidget); });
}

void TestStarterWidget::TestStarterWidgetUi::retranslateUi() {
    _this->setWindowTitle(tr("部件测试启动页"));
}
