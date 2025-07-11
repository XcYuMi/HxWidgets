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
#include <QKeyEvent>

class ListWidget;

class ItemWidgetTestWidget::ItemWidgetTestWidgetUi {
    ItemWidgetTestWidget *_this = nullptr;
public:
    void setupUi(ItemWidgetTestWidget *widget);
    void retranslateUi();
    void initListWidget();
    void initTreeWidget();
    void initTableWidget();
public:
    QTabWidget *tabItemWidget = nullptr;
    ListWidget *listWidget = nullptr;
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

void ItemWidgetTestWidget::ItemWidgetTestWidgetUi::retranslateUi()
{
    _this->setWindowTitle("ItemWidgets测试");
}

void ItemWidgetTestWidget::ItemWidgetTestWidgetUi::initListWidget() {
    listWidget = new ListWidget(_this);
    tabItemWidget->addTab(listWidget, "ListWidget");

    listContentsFrame = new QFrame(_this);
    tabObjectForItemWidget->addTab(listContentsFrame, "ListObjects");
    listContentsLayout = new Hx::FlowLayout(listContentsFrame);

    for(int i = 0; i < 5; ++i) {
        const auto label = new QLabel("ABCD");
        label->setObjectName(QString("Label %1").arg(i));
        listContentsLayout->addWidget(label);
    }

    listWidget->setRootWidget(listContentsFrame);
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
    ui.reset(new ItemWidgetUi);
    ui->setupUi(this);
}

void ItemWidget::ItemWidgetUi::setupUi(ItemWidget *widget)
{
    _this = widget;

    const auto layout = new QHBoxLayout(_this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    labelName = new QLabel(_this);
    layout->addWidget(labelName, 1);

    editPath = new QLineEdit(_this);
    layout->addWidget(editPath);

    selector = new QComboBox(_this);
    layout->addWidget(selector);

    button = new QPushButton(_this);
    layout->addWidget(button);
}

class ListWidget::ListWidgetPrivate {
public:
    QWidget *rootWidget = nullptr;
    QHash<QLabel*, QListWidgetItem*> labelItemHash;
    QHash<QListWidgetItem*, bool> itemInteractiveHash;
};

ListWidget::~ListWidget() {

}

ListWidget::ListWidget(QWidget *parent) : Super{parent}
{
    d.reset(new ListWidgetPrivate);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void ListWidget::setRootWidget(QWidget *widget)
{
    if(d->rootWidget == widget) {
        return;
    }

    if(d->rootWidget) {

    }

    d->rootWidget = widget;

    const auto &labels = d->rootWidget->findChildren<QLabel*>();
    for(const auto label : labels) {
        addLabelToView(label);
    }
}

void ListWidget::addLabelToView(QLabel *label)
{
    if(d->labelItemHash.contains(label)) {
        return;
    }

    const auto item = new QListWidgetItem;
    addItem(item);
    d->labelItemHash.insert(label, item);
    item->setText(label->objectName());
    const auto widget = new ItemWidget(this);
    setItemWidget(item, widget);
}

void ListWidget::keyPressEvent(QKeyEvent* event) {
    Super::keyPressEvent(event);
    if (state() == QAbstractItemView::NoState) {
        Qt::Key key = Qt::Key(event->key());
        if (key == Qt::Key_Control || key == Qt::Key_Shift) {
            setItemWidgetsInteractive(false);
        }
    }
}

void ListWidget::keyReleaseEvent(QKeyEvent* event) {
    Super::keyReleaseEvent(event);
    if (state() == QAbstractItemView::NoState) {
        Qt::Key key = Qt::Key(event->key());
        if (key == Qt::Key_Control || key == Qt::Key_Shift) {
            setItemWidgetsInteractive(true);
        }
    }
}

void ListWidget::setItemWidgetsInteractive(bool active) {
    if (!active) {
        for (QListWidgetItem* item : d->labelItemHash.values()) {
            QWidget *widget = itemWidget(item);
            if(widget == nullptr) {
                continue;
            }
            d->itemInteractiveHash.insert(item, widget->testAttribute(Qt::WA_TransparentForMouseEvents));
            widget->setAttribute(Qt::WA_TransparentForMouseEvents);
        }
    } else {
        for (QListWidgetItem* item : d->labelItemHash.values()) {
            if (!d->itemInteractiveHash.contains(item)) {
                continue;
            }
            QWidget *widget = itemWidget(item);
            if(widget == nullptr) {
                continue;
            }
            widget->setAttribute(Qt::WA_TransparentForMouseEvents, d->itemInteractiveHash.value(item));
        }
        d->itemInteractiveHash.clear();
    }
}
