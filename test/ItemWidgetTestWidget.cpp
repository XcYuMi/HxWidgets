#include "ItemWidgetTestWidget.hpp"

#include <QVBoxLayout>
#include <QTabWidget>
#include <QListWidget>
#include <QTreeWidget>
#include <QTableWidget>

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include "FlowLayout.hpp"

class ItemWidgetTestWidget::ItemWidgetTestWidgetUi {
    ItemWidgetTestWidget *_this = nullptr;
public:
    void setupUi(ItemWidgetTestWidget *widget);
    void initListWidget();
    void initTreeWidget();
    void initTableWidget();
public:
    QTabWidget *tabItemWidget = nullptr;
    QListWidget *listWidget = nullptr;
    QTreeWidget *treeWidget = nullptr;
    QTableWidget *tableWidget = nullptr;
    QTabWidget *tabObjectForItemWidget;
    QFrame *listContentsFrame = nullptr;
    Hx::FlowLayout *listContentsLayout = nullptr;
};

ItemWidgetTestWidget::~ItemWidgetTestWidget() {

}

ItemWidgetTestWidget::ItemWidgetTestWidget(QWidget *parent) : QWidget{parent} {
    ui.reset(new ItemWidgetTestWidgetUi);
    ui->setupUi(this);
}

void ItemWidgetTestWidget::ItemWidgetTestWidgetUi::setupUi(ItemWidgetTestWidget *widget)
{
    _this = widget;

    const auto layout = new QVBoxLayout(_this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    tabItemWidget = new QTabWidget(_this);
    layout->addWidget(tabItemWidget);

    tabObjectForItemWidget = new QTabWidget(_this);
    layout->addWidget(tabObjectForItemWidget);

    initListWidget();
}

void ItemWidgetTestWidget::ItemWidgetTestWidgetUi::initListWidget() {
    listWidget = new QListWidget(_this);
    tabItemWidget->addTab("ListWidget", listWidget);

    listContentsFrame = new QFrame(_this);
    tabObjectForItemWidget->addTab("ListObjects", listContentsFrame);
    listContentsLayout = new Hx::FlowLayout(listContentsFrame);

    for(int i = 0; i < 5; ++i) {
        const auto label = new QLabel("ABCD");
        listContentsLayout->addWidget(label);
    }
}

class ItemWidget::ItemWidgetUi {
    ItemWidget *_this = nullptr;
public:
    void setupUi(ItemWidget *widget);
public:
    QLabel *labelName = nullptr;
    QLineEdit *editPath = nullptr;
    QComboBox *selector = nullptr;
    QPushButton *button = nullptr;
};

ItemWidget::~ItemWidget() {

}

ItemWidget::ItemWidget(QWidget *parent) : Super{parent}{

}

void ItemWidget::ItemWidgetUi::setupUi(ItemWidget *widget)
{
    _this = widget;

    const auto layout = new QHBoxLayout(_this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    labelName = new QLabel(_this);
    layout->addWidget(labelName);

    editPath = new QLineEdit(_this);
    layout->addWidget(editPath);

    selector = new QComboBox(_this);
    layout->addWidget(selector);

    button = new QPushButton(_this);
    layout->addWidget(button);
}

ListWidget::ListWidget(QWidget *parent) : Super{parent}
{

}

ListWidget::~ListWidget()
{

}

void ListWidget::setRootWidget(QWidget *widget)
{
    if(mRootWidget == widget) {
        return;
    }

    if(mRootWidget) {

    }

    mRootWidget = widget;

    const auto &labels = mRootWidget->findChildren<QLabel*>();
    for(const auto label : labels) {
        addLabelToView(label);
    }
}

void ListWidget::addLabelToView(QLabel *label)
{
    if(mLabelItemHash.contains(label)) {
        return;
    }

    add
}
