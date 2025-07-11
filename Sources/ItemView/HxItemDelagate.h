#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QStyledItemDelegate>
#include <QScopedPointer>

class QAbstractItemView;
class QAbstractItemModel;

class HX_WIDGETS_EXPORT HxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxItemDelegate)
public:
    explicit HxItemDelegate(QObject *parent = nullptr);
    virtual ~HxItemDelegate();
    void setView(QAbstractItemView *view);
    QAbstractItemView *view() const;
    void setPaitItemBackgroundEnabled(bool enable);
    bool isPaintItemBackgroundEnabled() const;
    bool isCheckEditorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void blockCommit(bool blocked);

public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:
    void dataAboutToBeCommitted(QAbstractItemModel *model, const QModelIndex &index, int dataRole, const QVariant &from, const QVariant &to) const;

public:
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    bool setModelData(QAbstractItemModel *model, const QModelIndex &index, int dataRole, const QVariant &from, const QVariant &to) const;
};
