#include "HxItemViewDragDropHelper.hpp"
#include "HxWidgetsGlobalPrivate.hpp"

#include <QAbstractItemView>
#include <QApplication>
#include <QMouseEvent>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>

class HxItemViewDragDropHelper::HxItemViewDragDropHelperPrivate {
    HxItemViewDragDropHelper *q = nullptr;
public:
    void init(HxItemViewDragDropHelper *qptr) { q = qptr; }
    ViewOption &viewOption(QAbstractItemView *view);
    ViewOption viewOption(QAbstractItemView *view) const;
public:
    QHash<QAbstractItemView *, HxItemViewDragDropHelper::ViewOption> viewOptionHash;
};

HxItemViewDragDropHelper::HxItemViewDragDropHelper(QObject *parent)
    : Super{parent}
{
    d.reset(new HxItemViewDragDropHelperPrivate);
    d->init(this);
}

HxItemViewDragDropHelper::~HxItemViewDragDropHelper()
{
}

HxItemViewDragDropHelper *HxItemViewDragDropHelper::instance()
{
    QPointer<HxItemViewDragDropHelper> singleton;
    if (singleton.isNull())
    {
        singleton = new HxItemViewDragDropHelper(qApp);
    }
    return singleton;
}

void HxItemViewDragDropHelper::setViewDragDropEnabled(QAbstractItemView *view, bool enabled)
{
    if (enabled && !isViewDragDropEnabled(view))
    {
        view->setAcceptDrops(true);
        view->setDragEnabled(true);
        view->setAutoScroll(true);
        //view->setDropIndicatorShown(false);
        view->setDragDropMode(QAbstractItemView::InternalMove);
        view->installEventFilter(this);

        ViewOption option;
        d->viewOptionHash.insert(view, option);
    }
    else if (!enabled && isViewDragDropEnabled(view))
    {
        d->viewOptionHash.remove(view);
        view->removeEventFilter(this);
    }
}

void HxItemViewDragDropHelper::setDefaultDropAction(QAbstractItemView *view, Qt::DropAction action)
{
    if (!isViewDragDropEnabled(view))
        return;
    view->setDefaultDropAction(action);
}

bool HxItemViewDragDropHelper::isViewDragDropEnabled(QAbstractItemView *view) const
{
    const bool is = d->viewOptionHash.contains(view);
    return is;
}

bool HxItemViewDragDropHelper::eventFilter(QObject *watched, QEvent *ev)
{
    bool result = Super::eventFilter(watched, ev);

    // 检测Object是否是QAbstractItemView的ViewPort
    if (!watched->inherits("qt_scrollarea_viewport"))
        return result;

    const auto viewPort = qobject_cast<QWidget*>(watched);

    // 检测View是否启用拖拽
    const auto view = qobject_cast<QAbstractItemView *>(watched);
    if (!isViewDragDropEnabled(view))
        return result;

    const auto eventType = ev->type();
    switch (eventType)
    {
    case QEvent::MouseButtonPress:
        qDebug();
        break;
    case QEvent::DragEnter: {
        const auto event = static_cast<QDragEnterEvent *>(ev);
        result = dragEnterEvent(view, event);
    } break;
    case QEvent::DragLeave: {
        const auto event = static_cast<QDragLeaveEvent *>(ev);
        result = dragLeaveEvent(view, event);
    } break;
    case QEvent::DragMove: {
        const auto event = static_cast<QDragMoveEvent *>(ev);
        result = dragMoveEvent(view, event);
    } break;
    case QEvent::Drop: {
        const auto event = static_cast<QDropEvent *>(ev);
        result = dropEvent(view, event);
    } break;
    default:
        break;
    }

    return result;
}

bool HxItemViewDragDropHelper::mousePressEvent(QAbstractItemView *view, QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        auto &option = d->viewOption(view);
        option.mousePressPos = event->pos();
    }
    return false;
}

bool HxItemViewDragDropHelper::mouseMoveEvent(QAbstractItemView *view, QMouseEvent *event)
{
    return false;
}

bool HxItemViewDragDropHelper::dragEnterEvent(QAbstractItemView *view, QDragEnterEvent *event)
{
    event->acceptProposedAction();
    return false;
}

bool HxItemViewDragDropHelper::dragMoveEvent(QAbstractItemView *view, QDragMoveEvent *event)
{
    const auto &pos = event->pos();
    const auto &index = view->indexAt(pos);
    QModelIndexList indexesNeedUpdate;

    auto &option = d->viewOption(view);

    if (option.dropTargetIndex != index)
    {
        if (option.dropTargetIndex.isValid())
            indexesNeedUpdate.append(option.dropTargetIndex);
        option.dropTargetIndex = index;
    }

    if (option.dropTargetIndex.isValid())
    {
        const auto *mimeData = event->mimeData();
        calaulateDropParams(view, mimeData, pos);
        if(option.dropTargetIndex.isValid())
            indexesNeedUpdate.append(option.dropTargetIndex);
    }

    for (const auto &index : indexesNeedUpdate)
    {
        view->update(index);
    }
    view->viewport()->update();

    return true;
}

bool HxItemViewDragDropHelper::dragLeaveEvent(QAbstractItemView *view, QDragLeaveEvent *event)
{
    auto &option = d->viewOption(view);
    if (option.dropTargetIndex.isValid())
    {
        // 清空拖拽标记
        option.dropTargetIndex = QModelIndex();
        option.dropPosition = DropIndicatorPosition::BelowItem;
    }
    return false;
}

bool HxItemViewDragDropHelper::dropEvent(QAbstractItemView *view, QDropEvent *event)
{
    return true;
}

bool HxItemViewDragDropHelper::viewAcceptDrop(QAbstractItemView *view, const QMimeData *mimeData, const QModelIndex &targetIndex, const DropIndicatorPosition &dropPosition) const
{
    bool accepted = true;
    const auto &option = d->viewOption(view);
    if (option.acceptDropFunctor)
        accepted = option.acceptDropFunctor(mimeData, targetIndex, dropPosition);
    else
        accepted = true;
    return accepted;
}

QPixmap HxItemViewDragDropHelper::viewDragPixmap(QAbstractItemView *view) const
{
    return QPixmap();
}

void HxItemViewDragDropHelper::calaulateDropParams(QAbstractItemView *view, const QMimeData *mimeData, const QPoint &pos)
{
    auto &option = d->viewOption(view);
    const auto &index = view->indexAt(pos);

    if (!index.isValid())
        option.dropPosition = DropIndicatorPosition::BelowItem;
    else
    {
        option.dropPosition = DropIndicatorPosition::Nowhere;
        const auto &rect = view->visualRect(index);
        const int y = pos.y(), top = rect.top(), bottom = rect.bottom(), height = rect.height();

        if(y < top + height / 3)
        {
            if (viewAcceptDrop(view, mimeData, index, DropIndicatorPosition::AboveItem))
                option.dropPosition = DropIndicatorPosition::AboveItem;
        }
        else if (y < bottom - height / 3)
        {
            if (viewAcceptDrop(view, mimeData, index, DropIndicatorPosition::BelowItem))
                option.dropPosition = DropIndicatorPosition::BelowItem;
        }
        else
        {
            if (viewAcceptDrop(view, mimeData, index, DropIndicatorPosition::OnItem))
                option.dropPosition = DropIndicatorPosition::OnItem;
        }
    }

}

HxItemViewDragDropHelper::ViewOption &HxItemViewDragDropHelper::HxItemViewDragDropHelperPrivate::viewOption(QAbstractItemView *view)
{
    return viewOptionHash[view];
}

HxItemViewDragDropHelper::ViewOption HxItemViewDragDropHelper::HxItemViewDragDropHelperPrivate::viewOption(QAbstractItemView *view) const
{
    return viewOptionHash[view];
}
