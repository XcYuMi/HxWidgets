#include "HxTableView.h"
#include "HxWidgetsGlobalPrivate.hpp"
#include "HxItemDelagate.h"
#include "HxItemViewPaintHelper.h"

#include <QPainter>
#include <QMouseEvent>

class HxTableView::HxTableViewPrivate
{
public:
	bool exposed = false;
	bool blockSelectionOnCheckToggler = false;
	bool paintItemBackgroundEnabled = false;
};

HxTableView::HxTableView(QWidget *parent)
	: QTableView{ parent }
{
	d.reset(new HxTableViewPrivate);
}

HxTableView::~HxTableView()
{
}

void HxTableView::setSelectionOnCheckTogglerBlocked(bool blocked)
{
	d->blockSelectionOnCheckToggler = blocked;
}

bool HxTableView::isSelectionOnCheckTogglerBlocked() const
{
	return d->blockSelectionOnCheckToggler;
}

void HxTableView::setPaintItemBackgroundEnabled(bool enabled)
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

bool HxTableView::isPaintItemBackgroundEnabled() const
{
	return d->paintItemBackgroundEnabled;
}

void HxTableView::mousePressEvent(QMouseEvent *event)
{
	// 如果未设置CheckBox鼠标单击阻止调用默认事件函数
	if (!isSelectionOnCheckTogglerBlocked())
		return QTableView::mousePressEvent(event);
	
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
		return QTableView::mousePressEvent(event);

	const auto mode = selectionMode();
	setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
	QTableView::mousePressEvent(event);
	setSelectionMode(mode);
}

void HxTableView::mouseMoveEvent(QMouseEvent *event)
{
	QTableView::mouseMoveEvent(event);
}

void HxTableView::mouseReleaseEvent(QMouseEvent *event)
{
	// 如果未设置CheckBox鼠标单击阻止调用默认事件函数
	if (!isSelectionOnCheckTogglerBlocked())
		return QTableView::mouseReleaseEvent(event);

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
		return QTableView::mouseReleaseEvent(event);

	const auto mode = selectionMode();
	setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
	QTableView::mouseReleaseEvent(event);
	setSelectionMode(mode);
}

void HxTableView::showEvent(QShowEvent *event)
{
	QTableView::showEvent(event);
	if (d->exposed)
		return;

	d->exposed = true;
	connect(selectionModel(), &QItemSelectionModel::selectionChanged, this, [=] {
		int i = 0;
	});
}

void HxTableView::paintEvent(QPaintEvent *event)
{
	if (isPaintItemBackgroundEnabled())
	{
		QPainter painter(viewport());
		HxItemViewPaintHelper::instance()->paintEvent(this, &painter, event);
		painter.end();
	}
	QTableView::paintEvent(event);
}

void HxTableView::initViewItemStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
	const auto &flags = model()->flags(index);
	auto &state = option->state;
	auto &palette = option->palette;

	// Rect 
	if (index.isValid())
	{
		option->rect = visualRect(index);
	}

	// Select״̬
	if (const auto selectionModel = this->selectionModel())
	{
		bool selected = selectionModel->isSelected(index);
		state.setFlag(QStyle::State_Selected, selected);
	}

	// Endable״̬
	state.setFlag(QStyle::State_Enabled, true);
	if (!flags.testFlag(Qt::ItemIsEnabled))
		state.setFlag(QStyle::State_Enabled, false);

	// Focus״̬
	if (currentIndex() == index)
	{
		const bool focus = (hasFocus() || viewport()->hasFocus()) && currentIndex().isValid();
		if (focus)
			state.setFlag(QStyle::State_HasFocus, true);
	}
}
