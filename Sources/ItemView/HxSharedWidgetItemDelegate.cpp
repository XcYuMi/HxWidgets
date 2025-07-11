#include "HxSharedWidgetItemDelegate.hpp"
#include "HxWidgetsGlobalPrivate.hpp"

class HxSharedItemWidget::HxSharedItemWidgetPrivate {
public:
    HxSharedItemWidget::Type type = HxSharedItemWidget::Type::Unknown;
};

HxSharedItemWidget::~HxSharedItemWidget() {

}

HxSharedItemWidget::HxSharedItemWidget(QWidget *parent) : Super{parent} {
    d.reset(new HxSharedItemWidgetPrivate);
}

HxSharedItemWidget::Type HxSharedItemWidget::type() const {
    return d->type;
}

void HxSharedItemWidget::setType(Type type) {
    if(d->type == type) {
        return;
    }
    d->type = type;
}

class HxSharedWidgetItemDelegate::HxSharedWidgetItemDelegatePrivate {
    HxSharedWidgetItemDelegate *q = nullptr;
public:
    void init(HxSharedWidgetItemDelegate *qptr) { q = qptr; }

public:
    QPointer<QAbstractItemView> view;
};

HxSharedWidgetItemDelegate::~HxSharedWidgetItemDelegate() {

}

void HxSharedWidgetItemDelegate::setView(QAbstractItemView *view) {
    Q_ASSERT_X(d->view.isNull(), __FUNCTION__, "Attempt to change another view!");
    d->view = view;
}

QPointer<QAbstractItemView> HxSharedWidgetItemDelegate::view() const {
    return d->view;
}

HxSharedItemWidget *HxSharedWidgetItemDelegate::createWidget(QWidget *parent, int itemType) const
{
    return nullptr;
}

int HxSharedWidgetItemDelegate::itemType(const QModelIndex &index) const {
    return int(ItemType::PlainText);
}

HxSharedWidgetItemDelegate::HxSharedWidgetItemDelegate(QObject *parent) : Super{parent} {
    d.reset(new HxSharedWidgetItemDelegatePrivate);
    d->init(this);
}

void HxSharedWidgetItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Super::paint(painter, option, index);
}

QWidget *HxSharedWidgetItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return Super::createEditor(parent, option, index);
}

void HxSharedWidgetItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    Super::setEditorData(editor, index);
}

void HxSharedWidgetItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    Super::setModelData(editor, model, index);
}

void HxSharedWidgetItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Super::updateEditorGeometry(editor, option, index);
}

bool HxSharedWidgetItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    bool result = Super::editorEvent(event, model, option, index);
    return result;
}

bool HxSharedWidgetItemDelegate::eventFilter(QObject *object, QEvent *event) {
    bool result = Super::eventFilter(object,event);
    return result;
}

HxSharedItemWidget *HxSharedWidgetItemDelegate::itemWidget(HxSharedItemWidget::Type widgetType,
                                                           const QModelIndex &index) const
{
    return nullptr;
}
