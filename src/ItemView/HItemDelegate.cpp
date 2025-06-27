#include "HxItemDelagate.h"
#include "HxWidgetsGlobalPrivate.hpp"

#include <QApplication>
#include <QAbstractItemView>
#include <QItemEditorFactory>
#include <QMouseEvent>

class HxItemDelegate::HxItemDelegatePrivate {
public:
	QAbstractItemView *view = nullptr;
	bool paintItemBackgroundEnabled = true;
	bool commitBlocking = false;
};

HxItemDelegate::HxItemDelegate(QObject *parent)
	: QStyledItemDelegate{parent} 
{ 
    d.reset(new HxItemDelegatePrivate);
}

HxItemDelegate::~HxItemDelegate()
{

}

void HxItemDelegate::setView(QAbstractItemView *view)
{
	d->view = view;
}

QAbstractItemView *HxItemDelegate::view() const
{
	return d->view;
}

void HxItemDelegate::setPaitItemBackgroundEnabled(bool enable)
{
	if (d->paintItemBackgroundEnabled == enable)
		return;

	d->paintItemBackgroundEnabled = enable;
	if (view())
		view()->viewport()->update();
}

bool HxItemDelegate::isPaintItemBackgroundEnabled() const
{
	return d->paintItemBackgroundEnabled;
}

void HxItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const  {
	// 创建新选项对象，避免修改原对象
	QStyleOptionViewItem opt = option;
	initStyleOption(&opt, index);

	if (!isPaintItemBackgroundEnabled())
	{
		// 关键步骤：移除背景和选中状态绘制
		opt.backgroundBrush = Qt::NoBrush;								// 清除背景画刷
		opt.features.setFlag(QStyleOptionViewItem::Alternate, false);	// 清除
		opt.state.setFlag(QStyle::State_Selected, false);				// 清除选中状态标志
		opt.state.setFlag(QStyle::State_MouseOver, false);				// 清除Hover状态标志
	}
	
	// 绘制内容（文本/图标等）
	const QWidget *widget = option.widget;
	QStyle *style = widget ? widget->style() : QApplication::style();
	style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
}

bool HxItemDelegate::isCheckEditorEvent(QEvent *ev, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	const int eventType = ev->type();
	// 确保Item是Checkbale的
	const auto flags = model->flags(index);
	if (!flags.testFlag(Qt::ItemIsEnabled))
		return false;
	if (!flags.testFlag(Qt::ItemIsUserCheckable))
		return false;
	if (!option.state.testFlag(QStyle::State_Enabled))
		return false;

	// 确保Item有CheckState
	QVariant value = index.data(Qt::CheckStateRole);
	if (!value.isValid())
		return false;

	const QWidget *widget = option.widget;
	QStyle *style = widget ? widget->style() : QApplication::style();

	// 确保我们有正确的事件类型
	if (eventType == QEvent::MouseButtonRelease || eventType == QEvent::MouseButtonDblClick || eventType == QEvent::MouseButtonPress)
	{
		QStyleOptionViewItem viewOpt(option);
		initStyleOption(&viewOpt, index);
		QRect checkRect = style->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &viewOpt, widget);
		QMouseEvent *event = static_cast<QMouseEvent *>(ev);
		if (event->button() != Qt::LeftButton || !checkRect.contains(event->pos()))
			return false;

		if (eventType == QEvent::MouseButtonPress || eventType == QEvent::MouseButtonDblClick)
			return true;
	}
	else if (eventType == QEvent::KeyPress)
	{
		const auto event = static_cast<QKeyEvent *>(ev);
		if (event->key() != Qt::Key_Space && event->key() != Qt::Key_Select)
			return false;
	}
	else {
		return false;
	}

	return true;
}

void HxItemDelegate::blockCommit(bool blocked)
{
	d->commitBlocking = blocked;
}

void HxItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
#ifdef QT_NO_PROPERTIES
	Q_UNUSED(model);
	Q_UNUSED(editor);
	Q_UNUSED(index);
#else
	Q_ASSERT(model);
	Q_ASSERT(editor);
	const auto &data_from = model->data(index, Qt::EditRole);
	QByteArray property_name = editor->metaObject()->userProperty().name();
	if (property_name.isEmpty())
		property_name = itemEditorFactory()->valuePropertyName(model->data(index, Qt::EditRole).userType());
	if (!property_name.isEmpty()) {
		const auto &data_to = editor->property(property_name);
		//model->setData(index, editor->property(property_name), Qt::EditRole);
		setModelData(model, index, Qt::EditRole, data_from, data_to);
	}
#endif
}

bool HxItemDelegate::editorEvent(QEvent *ev, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	Q_ASSERT(ev);
	Q_ASSERT(model);

	const int eventType = ev->type();
	// 确保Item是Checkbale的
	Qt::ItemFlags flags = model->flags(index);
	if (!(flags & Qt::ItemIsUserCheckable) || !(option.state & QStyle::State_Enabled) || !(flags & Qt::ItemIsEnabled))
		return false;

	// 确保Item有CheckState
	QVariant value = index.data(Qt::CheckStateRole);
	if (!value.isValid())
		return false;

	const QWidget *widget = option.widget;
	QStyle *style = widget ? widget->style() : QApplication::style();

	// 确保我们有正确的事件类型
	if (eventType == QEvent::MouseButtonRelease || eventType == QEvent::MouseButtonDblClick || eventType == QEvent::MouseButtonPress)
	{
		QStyleOptionViewItem viewOpt(option);
		initStyleOption(&viewOpt, index);
		QRect checkRect = style->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &viewOpt, widget);
		QMouseEvent *event = static_cast<QMouseEvent *>(ev);
		if (event->button() != Qt::LeftButton || !checkRect.contains(event->pos()))
			return false;

		if (eventType == QEvent::MouseButtonPress || eventType == QEvent::MouseButtonDblClick)
			return true;
	}
	else if (eventType == QEvent::KeyPress)
	{
		const auto event = static_cast<QKeyEvent *>(ev);
		if (event->key() != Qt::Key_Space && event->key() != Qt::Key_Select)
			return false;
	}
	else {
		return false;
	}

	// 更新CheckState值
	Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
	if (flags & Qt::ItemIsUserTristate)
		state = ((Qt::CheckState)((state + 1) % 3));
	else
		state = (state == Qt::Checked) ? Qt::Unchecked : Qt::Checked;

	// 设置CheckState
	// return model->setData(index, state, Qt::CheckStateRole); // 原来的设置
	return setModelData(model, index, Qt::CheckStateRole, (state == Qt::Checked) ? Qt::Unchecked : Qt::Checked, state);
}

bool HxItemDelegate::setModelData(QAbstractItemModel *model, const QModelIndex &index, int dataRole, const QVariant &from, const QVariant &to) const
{
	emit dataAboutToBeCommitted(model, index, dataRole, from, to);
	if (d->commitBlocking)
		return false;
	return model->setData(index, to, dataRole);
}
