#include "FlowLayoutTestWidget.hpp"
#include "WidgetsTestGlobalPrivate.hpp"
#include "FlowLayout.hpp"
#include "WidgetUtils.hpp"

using Hx::WidgetUtils;

#include <QContextMenuEvent>
#include <QMetaEnum>

#include <QGridLayout>
#include <QFormLayout>
#include <QFrame>
#include <QMenu>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QCheckBox>
#include <QComboBox>
#include <QRadioButton>

#include <QPainter>
class ColorRect : public QWidget {
    QColor mColor;
public:
    ColorRect(const QColor &color, const QSize &size, QWidget *parent = nullptr) : QWidget{parent} {
        mColor = color;
        setFixedSize(size);
    }
protected:
    void paintEvent(QPaintEvent *event) {
        QPainter painter(this);
        painter.fillRect(rect(), mColor);
    }
};

class FlowLayoutTestWidget::FlowLayoutTestWidgetUi {
    FlowLayoutTestWidget *_this{nullptr};
public:
    void setupUi(FlowLayoutTestWidget *widget);
    void retranslateUi();
    QWidget *contextMenu();
    void retranslateMenuUi();
    void initContextMenu();
public:
    QFrame *flowContainer{nullptr};
    Hx::FlowLayout *flowLayout{nullptr};
    QMenu *menu{nullptr};
    QLabel *labelFlowOrder{nullptr};
    QComboBox *selectorFlowOrder{nullptr};
    QLabel *labelHorFlow{nullptr};
    QComboBox *selectorHorFlow{nullptr};
    QLabel *labelVerFlow{nullptr};
    QComboBox *selectorVerFlow{nullptr};
    QLabel *labelHorSpacing{nullptr};
    QSpinBox *spinHorSpacing{nullptr};
    QSlider *sliderHorSpacing{nullptr};
    QLabel *labelVerSpacing{nullptr};
    QSpinBox *spinVerSpacing{nullptr};
    QSlider *sliderVerSpacing{nullptr};
};

FlowLayoutTestWidget::~FlowLayoutTestWidget() { }

FlowLayoutTestWidget::FlowLayoutTestWidget(QWidget *parent) : QWidget{parent} {
    ui.reset(new FlowLayoutTestWidgetUi);
    ui->setupUi(this);
    ui->retranslateUi();
}

void FlowLayoutTestWidget::FlowLayoutTestWidgetUi::setupUi(FlowLayoutTestWidget *widget) {
    _this = widget;

    flowContainer = new QFrame(_this);
    flowContainer->setFrameStyle(QFrame::StyledPanel);

    const bool in_layout  = false;
    if(in_layout) {
        const auto layout = new QGridLayout(_this);
        layout->addItem(new QSpacerItem(6, 6, QSizePolicy::Expanding, QSizePolicy::Expanding), 0, 0);
        layout->addWidget(flowContainer, 0 , 1);
        layout->addItem(new QSpacerItem(6, 6, QSizePolicy::Expanding, QSizePolicy::Expanding), 0, 2);
        layout->addItem(new QSpacerItem(6, 6, QSizePolicy::Expanding, QSizePolicy::Expanding), 1, 0, 3);
    } else {
        const auto layout = new QVBoxLayout(_this);
        layout->addWidget(flowContainer);
    }

    flowLayout = new Hx::FlowLayout(flowContainer);

    flowLayout->addWidget(new ColorRect(Qt::red, QSize(48, 48), flowContainer));
    flowLayout->addWidget(new ColorRect(Qt::red, QSize(48, 48), flowContainer));
    flowLayout->addWidget(new ColorRect(Qt::red, QSize(48, 48), flowContainer));
    flowLayout->addWidget(new ColorRect(Qt::red, QSize(48, 48), flowContainer));
    flowLayout->addWidget(new ColorRect(Qt::red, QSize(48, 48), flowContainer));
    flowLayout->addWidget(new ColorRect(Qt::red, QSize(48, 48), flowContainer));
    flowLayout->addWidget(new ColorRect(Qt::red, QSize(48, 48), flowContainer));
}

void FlowLayoutTestWidget::FlowLayoutTestWidgetUi::retranslateUi() {
    _this->setWindowTitle("浮动布局-测试窗口");
}

template<class T>
void InitSelectorWidthEnum(QComboBox* selector) {
    const auto& metaEnum = QMetaEnum::fromType<T>();
    for (int i = 0; i < metaEnum.keyCount(); ++i) {
        const auto value = T(metaEnum.value(i));
        const auto& name = metaEnum.key(i);
        selector->addItem(name, QVariant::fromValue<T>(value));
    }
}

QWidget *FlowLayoutTestWidget::FlowLayoutTestWidgetUi::contextMenu()
{
    if (menu == nullptr) {
        menu = new QMenu(_this);
        menu->setWindowFlag(Qt::Popup);
        const auto layout = new QFormLayout(menu);

        labelFlowOrder = new QLabel(menu);
        selectorFlowOrder = new QComboBox(menu);
        InitSelectorWidthEnum<Hx::FlowLayout::FlowOrder>(selectorFlowOrder);
        layout->addRow(labelFlowOrder, selectorFlowOrder);
        connect(selectorFlowOrder, qOverload<int>(&QComboBox::currentIndexChanged), flowLayout, [=] {
            const auto value = selectorFlowOrder->currentData().value<Hx::FlowLayout::FlowOrder>();
            flowLayout->setFlowOrder(value);
        });

        labelHorFlow = new QLabel(menu);
        selectorHorFlow = new QComboBox(menu);
        InitSelectorWidthEnum<Hx::FlowLayout::HorizontalFlowDirection>(selectorHorFlow);
        layout->addRow(labelHorFlow, selectorHorFlow);
        connect(selectorHorFlow, qOverload<int>(&QComboBox::currentIndexChanged), flowLayout, [=] {
            const auto value = selectorHorFlow->currentData()
                                   .value<Hx::FlowLayout::HorizontalFlowDirection>();
            flowLayout->setHorizontalFlow(value);
        });

        labelVerFlow = new QLabel(menu);
        selectorVerFlow = new QComboBox(menu);
        InitSelectorWidthEnum<Hx::FlowLayout::VerticalFlowDirection>(selectorVerFlow);
        layout->addRow(labelVerFlow, selectorVerFlow);
        connect(selectorVerFlow, qOverload<int>(&QComboBox::currentIndexChanged), flowLayout, [=] {
            const auto value = selectorVerFlow->currentData()
                                   .value<Hx::FlowLayout::VerticalFlowDirection>();
            flowLayout->setVerticalFlow(value);
        });

        labelHorSpacing = new QLabel(menu);
        spinHorSpacing = new QSpinBox(menu);
        spinHorSpacing->setRange(0, 100);
        sliderHorSpacing = new QSlider(Qt::Horizontal, menu);
        sliderHorSpacing->setRange(0, 100);
        const auto layoutHorSpacing = new QHBoxLayout;
        layoutHorSpacing->addWidget(spinHorSpacing);
        layoutHorSpacing->addWidget(sliderHorSpacing);
        layout->addRow(labelHorSpacing, layoutHorSpacing);
        connect(sliderHorSpacing, &QSlider::valueChanged, flowLayout, [=](int value) {
            flowLayout->setHorizontalSpacing(value);
            WidgetUtils::SetSpinBoxValue(spinHorSpacing, value);
        });
        connect(spinHorSpacing, qOverload<int>(&QSpinBox::valueChanged), sliderHorSpacing, [=](int value) {
            WidgetUtils::SetSliderValue(sliderHorSpacing, value, true);
        });

        labelVerSpacing = new QLabel(menu);
        spinVerSpacing = new QSpinBox(menu);
        spinVerSpacing->setRange(0, 100);
        sliderVerSpacing = new QSlider(Qt::Horizontal, menu);
        sliderVerSpacing->setRange(0, 100);
        const auto layoutVerSpacing = new QHBoxLayout;
        layoutVerSpacing->addWidget(spinVerSpacing);
        layoutVerSpacing->addWidget(sliderVerSpacing);
        layout->addRow(labelVerSpacing, layoutVerSpacing);
        connect(sliderVerSpacing, &QSlider::valueChanged, flowLayout, [=](int value) {
            flowLayout->setVerticalSpacing(value);
            WidgetUtils::SetSpinBoxValue(spinVerSpacing, value);
        });
        connect(spinVerSpacing, qOverload<int>(&QSpinBox::valueChanged), sliderVerSpacing, [=](int value) {
            WidgetUtils::SetSliderValue(sliderVerSpacing, value, true);
        });
    }

    initContextMenu();
    retranslateMenuUi();

    return menu;
}

void FlowLayoutTestWidget::FlowLayoutTestWidgetUi::retranslateMenuUi() {
    if (menu == nullptr)
        return;

    labelFlowOrder->setText("流动顺序");
    labelHorFlow->setText("水平流动方向");
    labelVerFlow->setText("竖直流动方向");
    labelHorSpacing->setText("水平间距");
    labelVerSpacing->setText("竖直间距");
}

void FlowLayoutTestWidget::FlowLayoutTestWidgetUi::initContextMenu()
{
    WidgetUtils::SetComboxBoxCurrentData(selectorFlowOrder, QVariant::fromValue<Hx::FlowLayout::FlowOrder>(flowLayout->flowOrder()));
    WidgetUtils::SetComboxBoxCurrentData(selectorHorFlow, QVariant::fromValue<Hx::FlowLayout::HorizontalFlowDirection>(flowLayout->horizontalFlow()));
    WidgetUtils::SetComboxBoxCurrentData(selectorVerFlow, QVariant::fromValue<Hx::FlowLayout::VerticalFlowDirection>(flowLayout->verticalFlow()));

    WidgetUtils::SetSpinBoxValue(spinHorSpacing, flowLayout->horizontalSpacing());
    WidgetUtils::SetSliderValue(sliderHorSpacing, flowLayout->horizontalSpacing());

    WidgetUtils::SetSpinBoxValue(spinVerSpacing, flowLayout->verticalSpacing());
    WidgetUtils::SetSliderValue(sliderVerSpacing, flowLayout->verticalSpacing());
}

void FlowLayoutTestWidget::contextMenuEvent(QContextMenuEvent *event) {
    const auto menu = ui->contextMenu();
    menu->move(event->globalPos());
    menu->show();
}

