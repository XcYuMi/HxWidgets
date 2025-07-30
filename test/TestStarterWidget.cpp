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

#include <QLabel>

class TestStarterWidget::TestStarterWidgetUi {
    TestStarterWidget *_this = nullptr;
public:
    void setupUi(TestStarterWidget *widget);
    void retranslateUi();
};

TestStarterWidget::~TestStarterWidget() { }

TestStarterWidget::TestStarterWidget(QWidget *parent) : Super(parent) {
    ui.reset(new TestStarterWidgetUi);
    ui->setupUi(this);
    initTestPages();
    ui->retranslateUi();
}

void TestStarterWidget::TestStarterWidgetUi::setupUi(TestStarterWidget *widget) {
    _this = widget;
    _this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    HxToolBar::FlowLayoutParams params;
    params.flowOrder = Hx::FlowLayout::FlowOrder::RowFirst;
    params.horizontalDirection = Hx::FlowLayout::HorizontalFlowDirection::LeftToRight;
    params.verticalDirection = Hx::FlowLayout::VerticalFlowDirection::TopToBottom;
    params.horizontalSpacing = 12;
    params.verticalSpacing = 6; 
    _this->setLayoutParams(params);
}

void TestStarterWidget::initTestPages() {
    const auto &ShowPage = [=](QWidget *page) {
        page->setParent(this);
        page->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
        page->setAttribute(Qt::WA_DeleteOnClose);
        page->show();
    };

    //addAction(tr("流动布局"), [=] { ShowPage(new FlowLayoutTestWidget); });
    addAction(tr("锚点布局"), [=] { ShowPage(new AnchorLayoutTestWidget); });
    addAction(tr("嵌套分割器"), [=] { ShowPage(new NestedSplitterTestWidget); });
    addAction(tr("范围滑块"), [=] { ShowPage(new RangeSliderTestWidget); });
    addAction(tr("浮点范围滑块"), [=] { ShowPage(new DoubleRangeSliderTestWidget); });
    //addAction(tr("工具条"), [=] { ShowPage(new ToolBarTestWidget); });
    addAction(tr("徽标"), [=] { ShowPage(new BadgeTestWidget); });
    addAction(tr("设置对框框搜索"), [=] { ShowPage(new SettingsDialogSearchTestWidget); });
    addAction(tr("项视图"), [=] { ShowPage(new ItemWidgetTestWidget); });
    addAction(tr("共享部件视图项代理"), [=] { ShowPage(new SharedWidgetItemDelagateTestWidget); });
    addAction(tr("测试可换行标签"), [=]{
        const auto label = new QLabel(this);
        label->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Window);
        label->setWordWrap(true);
        label->setText("ABCDEFGHIJKLMNOPQRST");
        label->setMinimumWidth(96);
        label->show();
    });
}

void TestStarterWidget::TestStarterWidgetUi::retranslateUi() {
    _this->setWindowTitle(tr("部件测试启动页"));
}
