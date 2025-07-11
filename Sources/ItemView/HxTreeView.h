#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QTreeView>
#include <QScopedPointer>
#include <QPointer>
#include <QSortFilterProxyModel>

class QSortFilterProxyModel;

class HX_WIDGETS_EXPORT HxTreeView : public QTreeView
{
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxTreeView)
    using Super = QTreeView;

public:
    explicit HxTreeView(QWidget *parent = nullptr);
    virtual ~HxTreeView();
    void setSortFilterEnabled(bool enabled);
    QPointer<QSortFilterProxyModel> proxyModel() const;
    void setSelectionOnCheckTogglerBlocked(bool blocked);
    bool isSelectionOnCheckTogglerBlocked() const;
    void setPaintItemBackgroundEnabled(bool enabled);
    bool isPaintItemBackgroundEnabled() const;

protected:
    QModelIndex mapToSource(const QModelIndex &index) const;
    QModelIndexList mapToSource(const QModelIndexList &indexes) const;
    QModelIndex mapFromSource(const QModelIndex &index) const;
    QModelIndexList mapFromSource(const QModelIndexList &indexes) const;
    void initViewItemStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};
