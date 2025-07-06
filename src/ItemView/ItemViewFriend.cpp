#include "ItemViewFriend.hpp"

ItemViewFriend *ItemViewFriend::makeFriend(QAbstractItemView *view)
{
    Q_ASSERT_X((sizeof(ItemViewFriend) == sizeof(QAbstractItemView)), __FUNCTION__, "Class layout mismatch!");
    const auto view_friend = static_cast<ItemViewFriend*>(view);
    return view_friend;
}

HxItemView::State ItemViewFriend::state() const
{
    const auto origin_state = QAbstractItemView::state();
    const auto state = static_cast<HxItemView::State>(origin_state);
    return state;
}

void ItemViewFriend::setState(HxItemView::State state)
{
    const auto origin_state = static_cast<QAbstractItemView::State>(state);
    QAbstractItemView::setState(origin_state);
}

void ItemViewFriend::moveViewCursor(CursorAction action, Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(action); Q_UNUSED(modifiers);
}
