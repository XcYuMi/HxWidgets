#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QObject>
#include <QPersistentModelIndex>

class QAbstractItemView;
class QMimeData;
class QMouseEvent;
class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;

class HX_WIDGETS_EXPORT HxItemViewDragDropHelper : public QObject
{
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxItemViewDragDropHelper)
    using Super = QObject;

protected:
    explicit HxItemViewDragDropHelper(QObject *parent = nullptr);
public:
    ~HxItemViewDragDropHelper();
    static HxItemViewDragDropHelper *instance();
public:
    enum class DropIndicatorPosition {
        Nowhere = 0,    // 忽略
        OnItem,         // Item应被放到 目标Item中
        AboveItem,      // Item应被放到 目标Item上方
        BelowItem,      // Item应被放到 目标Item下方
        OnViewport,     // Item应被放到 视图空白处(取决于Model行为)
    };

    using ViewAcceptDropFunctor = std::function<bool(const QMimeData *, const QModelIndex &, const DropIndicatorPosition &)>;
    using ViewDropMimeDataFunctor = std::function<void(const QModelIndex &parentIndex, int index, const QMimeData *data)>;

    struct ViewOption {
        QPoint mousePressPos;
        QPersistentModelIndex dropTargetIndex;
        DropIndicatorPosition dropPosition;
        ViewAcceptDropFunctor acceptDropFunctor;
        ViewDropMimeDataFunctor dropMimeDataFunctor;
    };

public:
    void setViewDragDropEnabled(QAbstractItemView *view, bool enabled);
    void setDefaultDropAction(QAbstractItemView *view, Qt::DropAction action);
    bool isViewDragDropEnabled(QAbstractItemView *view) const;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    bool mousePressEvent(QAbstractItemView *view, QMouseEvent *event);
    bool mouseMoveEvent(QAbstractItemView *view, QMouseEvent *event);
    bool dragEnterEvent(QAbstractItemView *view, QDragEnterEvent *event);
    bool dragMoveEvent(QAbstractItemView *view, QDragMoveEvent *event);
    bool dragLeaveEvent(QAbstractItemView *view, QDragLeaveEvent *event);
    bool dropEvent(QAbstractItemView *view, QDropEvent *event);

private:
    bool viewAcceptDrop(QAbstractItemView *view, const QMimeData *mimeData, const QModelIndex &targetIndex, const DropIndicatorPosition &dropPosition) const;
    void viewDropMimeData(QAbstractItemView *view, const QModelIndex &parentIndex, int index, const QMimeData *data);
    QPixmap viewDragPixmap(QAbstractItemView *view) const;

private:
    void calaulateDropParams(QAbstractItemView *view, const QMimeData *mimeData, const QPoint &pos);
    QAbstractItemView *viewFromViewport(QWidget *widget) const;
};

inline HxItemViewDragDropHelper *dragDropHelper()
{
    return HxItemViewDragDropHelper::instance();
}
