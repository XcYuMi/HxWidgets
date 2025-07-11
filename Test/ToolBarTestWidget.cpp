#include "ToolBarTestWidget.hpp"
#include "WidgetsTestGlobalPrivate.hpp"
#include "HxToolBar.hpp"
#include "WidgetUtils.hpp"

#include <QStyle>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QToolBar>
#include <QCheckBox>
#include <QToolButton>
#include <QAction>
#include <QInputDialog>
#include <QComboBox>
#include <QSlider>

enum ToolBarLayoutMode {
    BoxLayout,
    FlowLayout
};

class ToolBarTestWidget::ToolBarTestWidgetUi {
    ToolBarTestWidget *_this = nullptr;
public:
    void setupUi(ToolBarTestWidget *widget);
    void retranslateUi();
public:
    HxToolBar *toolBar = nullptr;
    QToolBar *editorBar = nullptr;
    QToolButton *buttonExpand = nullptr;
public:
    QAction *actionAddAction = nullptr;
    QAction *actionAddSeparator = nullptr;
    QAction *actionAddStretch = nullptr;
    QAction *actionAddSpacing = nullptr;
    QComboBox *comboboxLayoutMode = nullptr;
};

ToolBarTestWidget::~ToolBarTestWidget() {

}

ToolBarTestWidget::ToolBarTestWidget(QWidget *parent) : QWidget{parent} {
    ui.reset(new ToolBarTestWidgetUi);
    ui->setupUi(this);
    ui->retranslateUi();
}

void ToolBarTestWidget::ToolBarTestWidgetUi::setupUi(ToolBarTestWidget *widget)
{
    _this = widget;
    _this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
    const auto rootLayout = new QVBoxLayout(_this);
    rootLayout->setContentsMargins(6,6,6,6);
    rootLayout->setSpacing(8);

    toolBar = new HxToolBar(_this);
    rootLayout->addWidget(toolBar);

    rootLayout->addStretch();

    const auto editorLayout = new QHBoxLayout;
    rootLayout->addLayout(editorLayout);
    editorLayout->setContentsMargins(0,0,0,0);
    editorLayout->setSpacing(6);
    editorLayout->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    QIcon iconExpand;
    iconExpand.addPixmap(_this->style()->standardPixmap(QStyle::SP_ArrowLeft), QIcon::Normal, QIcon::On);
    iconExpand.addPixmap(_this->style()->standardPixmap(QStyle::SP_ArrowRight), QIcon::Normal, QIcon::Off);
    buttonExpand = new QToolButton(_this);
    buttonExpand->setCheckable(true);
    buttonExpand->setChecked(true);
    buttonExpand->setIcon(iconExpand);
    editorLayout->addWidget(buttonExpand, 0);

    if(editorBar = new QToolBar(_this)) {
        editorLayout->addWidget(editorBar);

        comboboxLayoutMode = new QComboBox(editorBar);
        editorBar->addWidget(comboboxLayoutMode);
        comboboxLayoutMode->addItem("BoxLayout",  ToolBarLayoutMode::BoxLayout);
        comboboxLayoutMode->addItem("FlowLayout", ToolBarLayoutMode::FlowLayout);

        editorBar->addSeparator();

        QWidget *frameBoxProperty = nullptr;
        if(const auto frame = new QFrame) {
            frameBoxProperty = frame;
            const auto layout = new QFormLayout(frame);
            const auto selectorDirection = new QComboBox(frame);
            selectorDirection->addItem("LeftToRight", QBoxLayout::LeftToRight);
            selectorDirection->addItem("RightToLeft", QBoxLayout::RightToLeft);
            selectorDirection->addItem("TopToBottom", QBoxLayout::TopToBottom);
            selectorDirection->addItem("BottomToTop", QBoxLayout::BottomToTop);
            layout->addRow("方向", selectorDirection);
            const auto slider = new QSlider(frame);
            slider->setRange(0, 100);
            layout->addRow("间距", slider);
        }

        //actionAddAction = editorBar->addAction("", _this, &ToolBarTestWidget::onAddActionTriggered);
        actionAddAction = HxWidgetUtils::addAction(_this, QString(), _this, &ToolBarTestWidget::onAddActionTriggered);
        actionAddAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
        //editorBar->addAction(actionAddAction);
        actionAddSeparator = editorBar->addAction("", _this, &ToolBarTestWidget::onAddSeparatorTriggered);
        actionAddStretch = editorBar->addAction("", _this, &ToolBarTestWidget::onAddStretchTriggered);
        actionAddSpacing = editorBar->addAction("", _this, &ToolBarTestWidget::onAddSpacingTriggered);   
    }

    connect(buttonExpand, &QToolButton::clicked, [=] {
        editorBar->setVisible(buttonExpand->isChecked());
    });
}

void ToolBarTestWidget::ToolBarTestWidgetUi::retranslateUi()
{
    _this->setWindowTitle("ToolBar测试");
    buttonExpand->setToolTip(buttonExpand->isChecked() ? "折叠" : "展开");
    // 工具
    actionAddAction->setText("添加动作");
    actionAddSeparator->setText("添加分隔符");
    actionAddStretch->setText("添加弹簧");
    actionAddSpacing->setText("添加间距");
}

void ToolBarTestWidget::onAddActionTriggered() {
    const auto &text = QInputDialog::getText(this, tr("添加动作"), tr("文本"));
    if(text.isEmpty())
        return ;

    ui->toolBar->addAction(text);
}

void ToolBarTestWidget::onAddSeparatorTriggered() {
    ui->toolBar->addSeparator();
}

void ToolBarTestWidget::onAddStretchTriggered()
{
    ui->toolBar->addStretch(0);
}

void ToolBarTestWidget::onAddSpacingTriggered()
{
    ui->toolBar->addSpacing(12);
}
