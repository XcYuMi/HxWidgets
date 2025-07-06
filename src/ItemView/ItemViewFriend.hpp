#pragma once
#include <QAbstractItemView>

class HxItemView : public QObject {
    Q_GADGET
public:
    enum class State {
        NoState,
        DraggingState,
        DragSelectingState,
        EditingState,
        ExpandingState,
        CollapsingState,
        AnimatingState
    };
    Q_ENUM(State)

    enum class DropIndicatorPosition {
        Nowhere = 0,    // 忽略
        OnItem,         // Item应被放到 目标Item中
        AboveItem,      // Item应被放到 目标Item上方
        BelowItem,      // Item应被放到 目标Item下方
        OnViewport,     // Item应被放到 视图空白处(取决于Model行为)
    };
    Q_ENUM(DropIndicatorPosition)

    enum CursorAction {
        MoveUp,
        MoveDown,
        MoveLeft,
        MoveRight,
        MoveHome,
        MoveEnd,
        MovePageUp,
        MovePageDown,
        MoveNext,
        MovePrevious
    };
    Q_ENUM(CursorAction)
};

class ItemViewFriend : public QAbstractItemView
{
public:
    static ItemViewFriend *makeFriend(QAbstractItemView *view);
public:
    using QAbstractItemView::selectedIndexes;
    using QAbstractItemView::viewportEvent;
    HxItemView::State state() const;
    void setState(HxItemView::State state);
    void moveViewCursor(CursorAction action, Qt::KeyboardModifiers modifiers);
};
