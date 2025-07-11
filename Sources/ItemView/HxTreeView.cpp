#include "HxTreeView.h"
#include "HxWidgetsGlobalPrivate.hpp"
#include "HxItemDelagate.h"
#include "HxItemViewPaintHelper.h"

#include <QPainter>
#include <QMouseEvent>

class HxTreeView::HxTreeViewPrivate
{
public:
	bool exposed = false;
	bool blockSelectionOnCheckToggler = false;
	bool paintItemBackgroundEnabled = false;
};

HxTreeView::HxTreeView(QWidget *parent)
	: QTreeView{ parent }
{
    d.reset(new HxTreeViewPrivate);
}

HxTreeView::~HxTreeView()
{
}

void HxTreeView::setSortFilterEnabled(bool enabled)
{
	if (enabled)
	{
		if (!proxyModel())
		{
			const auto proxy = new QSortFilterProxyModel(this);
			proxy->setSourceModel(model());
			setModel(proxy);
		}
	}
	else
	{
		if (proxyModel())
		{
			setModel(proxyModel()->sourceModel());
		}
	}
}

QPointer<QSortFilterProxyModel> HxTreeView::proxyModel() const
{
	return qobject_cast<QSortFilterProxyModel*>(model());
}

void HxTreeView::setSelectionOnCheckTogglerBlocked(bool blocked)
{
	d->blockSelectionOnCheckToggler = blocked;
}

bool HxTreeView::isSelectionOnCheckTogglerBlocked() const
{
	return d->blockSelectionOnCheckToggler;
}

void HxTreeView::setPaintItemBackgroundEnabled(bool enabled)
{
	if (d->paintItemBackgroundEnabled == enabled)
		return;

	d->paintItemBackgroundEnabled = enabled;

    HxItemViewPaintHelper::PaintOptions options;
	options.OptimizeHoverPaint = false;
	options.DelegateBackgroundPaint = true;
    HxItemViewPaintHelper::instance()->addView(this, options);

	update();
}

bool HxTreeView::isPaintItemBackgroundEnabled() const
{
	return d->paintItemBackgroundEnabled;
}

QModelIndex HxTreeView::mapToSource(const QModelIndex &index) const
{
	if (!proxyModel())
		return index;
	else
	{
		if (index.model() == proxyModel())
			return proxyModel()->mapToSource(index);
		else
			return index;
	}		
}

QModelIndexList HxTreeView::mapToSource(const QModelIndexList &indexes) const
{
	QModelIndexList sourceIndexes;
	for (const QModelIndex &index : indexes)
	{
		const QModelIndex &sourceIndex = mapToSource(index);
		sourceIndexes.append(sourceIndex);
	}
	return sourceIndexes;
}

QModelIndex HxTreeView::mapFromSource(const QModelIndex &index) const
{
	if (!proxyModel())
		return index;
	else
	{
		if (index.model() == proxyModel())
			return index;
		else
			return proxyModel()->mapFromSource(index);
	}
}

QModelIndexList HxTreeView::mapFromSource(const QModelIndexList &indexes) const
{
	QModelIndexList proxyIndexes;
	for (const QModelIndex &index : indexes)
	{
		const QModelIndex &proxyIndex = mapFromSource(index);
		proxyIndexes.append(proxyIndex);
	}
	return proxyIndexes;
}

void HxTreeView::initViewItemStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
	const auto &flags = model()->flags(index);
	auto &state = option->state;
	auto &palette = option->palette;
    Q_UNUSED(palette);

	// Item矩形
	if (index.isValid())
	{
		option->rect = visualRect(index);
	}

	// 选中状态
	if (const auto selectionModel = this->selectionModel())
	{
		bool selected = selectionModel->isSelected(index);
		state.setFlag(QStyle::State_Selected, selected);
	}

	// 可用状态
	state.setFlag(QStyle::State_Enabled, true);
	if (!flags.testFlag(Qt::ItemIsEnabled))
		state.setFlag(QStyle::State_Enabled, false);

	// 焦点状态
	if (currentIndex() == index)
	{
		const bool focus = (hasFocus() || viewport()->hasFocus()) && currentIndex().isValid();
		if (focus)
			state.setFlag(QStyle::State_HasFocus, true);
	}
}

void HxTreeView::paintEvent(QPaintEvent *event)
{
	if (isPaintItemBackgroundEnabled())
	{
		QPainter painter(viewport());
        HxItemViewPaintHelper::instance()->paintEvent(this, &painter, event);
		painter.end();
	}
	Super::paintEvent(event);
}

void HxTreeView::mousePressEvent(QMouseEvent *event)
{
	// 如果未设置CheckBox鼠标单击阻止调用默认事件函数
	if (!isSelectionOnCheckTogglerBlocked())
		return Super::mousePressEvent(event);

	// 获取是否满足阻止条件(事件发生在CheckBox上)
	bool needBlockSelection = false;
	QPersistentModelIndex index = indexAt(event->pos());
    if (const auto delegate = qobject_cast<HxItemDelegate *>(itemDelegate(index)))
	{
		QStyleOptionViewItem option;
		initViewItemStyleOption(&option, index);
		needBlockSelection = delegate->isCheckEditorEvent(event, model(), option, index);
	}

	// 如果不满足阻止条件调用默认事件函数
	if (!needBlockSelection)
		return Super::mousePressEvent(event);

	const auto mode = selectionMode();
	setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
	Super::mousePressEvent(event);
	setSelectionMode(mode);
}

void HxTreeView::mouseReleaseEvent(QMouseEvent *event)
{
	// 如果未设置CheckBox鼠标单击阻止调用默认事件函数
	if (!isSelectionOnCheckTogglerBlocked())
		return Super::mouseReleaseEvent(event);

	// 获取是否满足阻止条件(事件发生在CheckBox上)
	bool needBlockSelection = false;
	QPersistentModelIndex index = indexAt(event->pos());
    if (const auto delegate = qobject_cast<HxItemDelegate *>(itemDelegate(index)))
	{
		QStyleOptionViewItem option;
		initViewItemStyleOption(&option, index);
		needBlockSelection = delegate->isCheckEditorEvent(event, model(), option, index);
	}

	// 如果不满足阻止条件调用默认事件函数
	if (!needBlockSelection)
		return Super::mouseReleaseEvent(event);

	const auto mode = selectionMode();
	setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
	Super::mouseReleaseEvent(event);
	setSelectionMode(mode);
}
