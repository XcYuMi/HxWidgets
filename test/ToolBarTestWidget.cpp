#include "ToolBarTestWidget.hpp"
#include "WidgetsTestGlobalPrivate.hpp"
#include "HxToolBar.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QToolBar>
#include <QAction>

class ToolBarTestWidget::ToolBarTestWidgetUi {
    ToolBarTestWidget *_this = nullptr;
public:
    void setupUi(ToolBarTestWidget *widget);
    void retranslateUi();
public:
    HxToolBar *toolBar = nullptr;
    QToolBar *toolPopup = nullptr;
    QPushButton *buttonExpand = nullptr;
public:
    QPushButton *buttonAddAction = nullptr;
    QPushButton *buttonAddSeparator = nullptr;
    QPushButton *buttonAddStretch = nullptr;
    QPushButton *buttonAddSpacing = nullptr;
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
    const auto rootLayout = new QVBoxLayout(_this);
    rootLayout->setContentsMargins(0,0,0,0);
    rootLayout->setSpacing(8);

    toolBar = new HxToolBar(_this);
    rootLayout->addWidget(toolBar);

    rootLayout->addStretch();

    buttonExpand = new QPushButton(_this);
    rootLayout->addWidget(buttonExpand, 0, Qt::AlignVCenter | Qt::AlignRight);

    if(toolPopup = new QToolBar(_this)) {
        toolPopup->setWindowFlag(Qt::Popup);
        toolPopup->setAttribute(Qt::WA_DeleteOnClose, false);
        buttonAddAction = new QPushButton(toolPopup);
        toolPopup->addWidget(buttonAddAction);
    }

    connect(buttonExpand, &QPushButton::clicked, [=] {
        toolPopup->move(QCursor::pos());
        toolPopup->show();
    });
}

void ToolBarTestWidget::ToolBarTestWidgetUi::retranslateUi()
{
    _this->setWindowTitle("ToolBar测试");
    buttonExpand->setText("工具");
    // 工具
    buttonAddAction->setText("添加动作");
}
