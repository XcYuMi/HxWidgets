#include "HxListView.h"
#include "HxWidgetsGlobalPrivate.hpp"

HxListView::HxListView(QWidget *parent)
	: QListView{ parent }
{
}

HxListView::~HxListView()
{
}

void HxListView::setSortFilterEnabled(bool enabled)
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

QPointer<QSortFilterProxyModel> HxListView::proxyModel() const
{
	return qobject_cast<QSortFilterProxyModel*>(model());
}

QModelIndex HxListView::mapToSource(const QModelIndex &index) const
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

QModelIndexList HxListView::mapToSource(const QModelIndexList &indexes) const
{
	QModelIndexList sourceIndexes;
	for (const QModelIndex &index : indexes)
	{
		const QModelIndex &sourceIndex = mapToSource(index);
		sourceIndexes.append(sourceIndex);
	}
	return sourceIndexes;
}

QModelIndex HxListView::mapFromSource(const QModelIndex &index) const
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

QModelIndexList HxListView::mapFromSource(const QModelIndexList &indexes) const
{
	QModelIndexList proxyIndexes;
	for (const QModelIndex &index : indexes)
	{
		const QModelIndex &proxyIndex = mapFromSource(index);
		proxyIndexes.append(proxyIndex);
	}
	return proxyIndexes;
}
