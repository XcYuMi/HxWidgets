#include "BadgeTestWidget.hpp"
#include "WidgetsTestGlobalPrivate.hpp"
#include "HxBadge.hpp"

#include <QPainter>
#include <QPainterPath>

#include <QHBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QSlider>
#include <QSpinBox>
#include <QProgressBar>
#include <QMenu>
#include <QTabBar>
#include <QToolBar>

class BadgeTestWidget::BadgeTestWidgetUi {
    BadgeTestWidget *_this = nullptr;
public:
    void setupUi(BadgeTestWidget *widget);
    void initWidgetPage();
    void initItemViewPage(){}
    void initMenuPage(){}
    void initTabBarPage(){}
    void initGraphicsViewPage(){}
    void retranslateUi();
public:
    QTabWidget *tabWidget = nullptr;
    // Widget
    QGroupBox *groupBoxWidthBadge = nullptr;
    QPushButton *buttonWidthBadge = nullptr;
    QLabel *labelWidthBadge = nullptr;
};

static QPicture gRedPointPicture;

BadgeTestWidget::~BadgeTestWidget() {

}

BadgeTestWidget::BadgeTestWidget(QWidget *parent) : QWidget{parent} {
    ui.reset(new BadgeTestWidgetUi);
    ui->setupUi(this);
    ui->retranslateUi();
}

void BadgeTestWidget::BadgeTestWidgetUi::setupUi(BadgeTestWidget *widget) {
    _this = widget;

    const auto layout = new QVBoxLayout(_this);
    layout->setContentsMargins(6,6,6,6);
    layout->setSpacing(0);

    tabWidget = new QTabWidget(_this);
    layout->addWidget(tabWidget);

    if(gRedPointPicture.isNull()) {
        QRect rect(0,0,100,100);
        gRedPointPicture.setBoundingRect(rect);

        QPainterPath path;
        path.addRoundedRect(rect, 50, 50, Qt::SizeMode::RelativeSize);

        QPainter painter;
        painter.begin(&gRedPointPicture);
        painter.setRenderHints(QPainter::Antialiasing);
        painter.fillPath(path, Qt::blue);
    }


    initWidgetPage();
    initItemViewPage();
    initMenuPage();
    initTabBarPage();
    initGraphicsViewPage();
}

void BadgeTestWidget::BadgeTestWidgetUi::retranslateUi() {
    _this->setWindowTitle("徽标测试窗口");
    groupBoxWidthBadge->setTitle("部件徽标组");
    buttonWidthBadge->setText("按钮徽标");
    labelWidthBadge->setText("标签徽标");
}

void BadgeTestWidget::BadgeTestWidgetUi::initWidgetPage() {
    const auto page = new QWidget(tabWidget);
    tabWidget->addTab(page, "Widget徽标");

    const auto layout = new QVBoxLayout(page);

    if(const auto groupBox = new QGroupBox) {
        groupBoxWidthBadge = groupBox;
        layout->addWidget(groupBox);

        const auto boxLayout = new QHBoxLayout(groupBox);

        if(const auto button = new QPushButton) {
            buttonWidthBadge = button;
            boxLayout->addWidget(button);

            const auto topRightBage = HxBadge::addIconBadge(button, HxBadge::Position::TopRight, QPoint(0,0));
            topRightBage->setPicture(gRedPointPicture);
        }
        if(const auto label = new QLabel) {
            labelWidthBadge = label;
            boxLayout->addWidget(label);
        }

        boxLayout->addStretch();
    }

    layout->addStretch();
}


