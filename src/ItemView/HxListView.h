#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QListView>
#include <QPointer>
#include <QSortFilterProxyModel>

class HX_WIDGETS_EXPORT HxListView : public QListView
{
    Q_OBJECT
public:
    explicit HxListView(QWidget *parent = nullptr);
    virtual ~HxListView();
    void setSortFilterEnabled(bool enabled);
    QPointer<QSortFilterProxyModel> proxyModel() const;
protected:
    QModelIndex mapToSource(const QModelIndex &index) const;
    QModelIndexList mapToSource(const QModelIndexList &indexes) const;
    QModelIndex mapFromSource(const QModelIndex &index) const;
    QModelIndexList mapFromSource(const QModelIndexList &indexes) const;
};
