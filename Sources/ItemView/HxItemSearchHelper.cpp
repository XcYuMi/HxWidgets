#include "HxItemSearchHelper.h"
#include "HxWidgetsGlobalPrivate.hpp"

#include <QListView>
#include <QTreeView>
#include <QTableView>
#include <QListWidget>
#include <QTreeWidget>
#include <QTableWidget>

HxItemSearchHelper::~HxItemSearchHelper()
{ 
}

HxItemSearchHelper::HxItemSearchHelper(QObject *parent) : QObject { parent }
{ 
}

bool HxItemSearchHelper::isSearchSupported(const QList<QAbstractItemView*> &views)
{
    if (!HxItemSearchHelper::isSearchBarEnabled())
        return false;
    else
    {
        bool supportProxy = false;
        for (const auto view : views)
        {
            supportProxy |= view->inherits("HTreeView");
            supportProxy |= view->inherits("HListView");
            if (!supportProxy)
                break;
        }
        
        return supportProxy || HxItemSearchHelper::isSearchHelperEnabled();
    }
}

void HxItemSearchHelper::setView(QAbstractItemView *view)
{
    mView = view;

    const auto model = mView->model();

    connect(model, &QAbstractItemModel::rowsInserted, this, &HxItemSearchHelper::onRowsInserted, Qt::UniqueConnection);
    connect(model, &QAbstractItemModel::rowsRemoved, this, &HxItemSearchHelper::onRowsAboutTobeRemoved, Qt::UniqueConnection);
    connect(model, &QAbstractItemModel::dataChanged, this, &HxItemSearchHelper::onDataChanged, Qt::UniqueConnection);
    handleRows(QModelIndex(), 0, mView->model()->rowCount() - 1);
    mMatchCount = totalValidCount();
}

void HxItemSearchHelper::setSearchText(const QString &text, int role)
{
    bool need_refresh = false;
    if (mRoleTextHash.contains(role))
    {
        const auto &text_old = mRoleTextHash.value(role);
        if (text_old != text) 
        {
            mRoleTextHash.insert(role, text);
            need_refresh = true;
        }
    } 
    else {
        mRoleTextHash.insert(role, text);
        need_refresh = true;
    }

    if (need_refresh) 
        search();
}

void HxItemSearchHelper::setSearchText(const QString &text, QList<int> roles)
{
    bool need_refresh = false;
    for (const auto role : roles)
    {
        if (mRoleTextHash.contains(role)) 
        {
            const auto &text_old = mRoleTextHash.value(role);
            if (text_old != text) 
            {
                mRoleTextHash.insert(role, text);
                need_refresh = true;
            }
        }
        else 
        {
            mRoleTextHash.insert(role, text);
            need_refresh = true;
        }
    }
    
    if (need_refresh) 
        search();
}

void HxItemSearchHelper::setNofilterFlag(Flag flag, int role)
{
    mNofilterFlagValueHash.insert(role, flag);
}

void HxItemSearchHelper::setSearchFlag(Flag flag, int role)
{
    bool need_refresh = false;
    if (mRoleFlagHash.contains(role)) 
    {
        const auto &flag_old = mRoleFlagHash.value(role);
        if (flag_old != flag) 
        {
            mRoleFlagHash.insert(role, flag);
            need_refresh = true;
        }
    }
    else 
    {
        mRoleFlagHash.insert(role, flag);
        need_refresh = true;
    }

    if (need_refresh) 
        search();
}

void HxItemSearchHelper::setTextSearchableColumns(QList<int> columns)
{
    if(mTextSearchableColumns == columns) 
        return;

    mTextSearchableColumns = columns;
}

QList<int> HxItemSearchHelper::textSearchableColumns() const
{
    if(mTextSearchableColumns.isEmpty())
    {
        QList<int> columns;
        for(int i = 0; i < model()->columnCount(); ++i) 
            columns.append(i);
        return columns;
    } 
    else 
        return mTextSearchableColumns;
}

void HxItemSearchHelper::setCanCountFunctor(CanCountFunctor functor)
{
    mCanCountFunctor = functor;
}

void HxItemSearchHelper::setDynamicFilterEnabled(bool enable)
{
    mDynamicFilterEnabled = enable;
}

void HxItemSearchHelper::search()
{
    if (const auto need_filter = shouldFilter()) 
    {
        resetAcceptStates();    // 重置Accept状态

        mMatchCount = 0;
        handleRows(QModelIndex(), 0, mView->model()->rowCount() - 1);
    } 
    else 
    {
        resetRowsVisible();                 // 重置行可見性
        resetRowsExpand();                  // 重置行展開狀態
        resetAcceptStates();                // 重置处理状态
        setMatchCount(totalValidCount());   // 搜索结果数为总数
    }
}

void HxItemSearchHelper::handleRows(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    if (!shouldFilter()) 
    {
        setMatchCount(totalValidCount());
        return;
    }

    for (int row = first; row <= last; ++row) 
    {
        const auto &index = model()->index(row, 0, parent);
        handleRow(index);
    }
}

void HxItemSearchHelper::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    if(mDynamicFilterEnabled)
        handleRows(parent, first, last);
}

void HxItemSearchHelper::handleRow(const QModelIndex &index)
{
    const auto &text = indexText(index);
    HX_DEBUG << text;
    const bool match = filterIntex(index);
    for (int row = 0; row < model()->rowCount(index); ++row) 
    {
        const auto &childIndex = model()->index(row, 0, index);
        handleRow(childIndex);
    }
}

void HxItemSearchHelper::onRowsAboutTobeRemoved(const QModelIndex &parent, int first, int last)
{
    if (!shouldFilter()) 
    {
        setMatchCount(totalValidCount());
        return;
    }

    // 刷新计数
    for (int row = first; row <= last; ++row) 
    {
        const auto &index = model()->index(row, 0, parent);
        handleRowRemoved(index);
    }

    // 刷新所有祖先的可见性和折叠状态
    // TODO: 需要优化,主处理当前祖孙链的状态,优化Accept状态与MatchCount的更新
    resetAcceptStates(); 
    mMatchCount = 0;
    handleRows(QModelIndex(), 0, mView->model()->rowCount() - 1);
}

void HxItemSearchHelper::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    if (!mDynamicFilterEnabled)
        return;

    if (!textSearchableColumns().contains(topLeft.column()) && !textSearchableColumns().contains(bottomRight.column()))
        return;

    bool contains_role = false;
    for (int role : roles) 
        if (mRoleTextHash.contains(role))
        {
            contains_role = true;
            break;
        }
    if (contains_role)
    {
        if (!shouldFilter())
            return;
        clearAcceptState(topLeft);
        handleRows(topLeft.parent(), bottomRight.row(), bottomRight.row());
    }
        
}

void HxItemSearchHelper::handleRowRemoved(const QModelIndex &index)
{
    if (isHandled(index))
        return;

    const auto accept = isAccepted(index);
    if (accept && canCount(index)) 
        setMatchCount(mMatchCount - 1);

    clearAcceptState(index);
}

bool HxItemSearchHelper::filterIntex(const QModelIndex &index)
{
    if (!index.isValid()) 
        return true;

    const bool handled = isHandled(index);
    if (handled)
    {
        const auto &text = indexText(index);
        const bool accepted = isAccepted(index);
        HX_DEBUG << "Already Handled" << text << "Hash = " << QPersistentModelIndex(index);
        return accepted;
    }
        
    bool match = matchRow(index);
    setAccepted(index, match);
    setRowVisible(index, match);

    if (match) 
    {
        if (canCount(index)) 
            setMatchCount(mMatchCount + 1);
        
        // 显示并展开本项的所有祖先项
        QModelIndex parent = index.parent();
        while (parent.isValid())
        {
            setAccepted(parent, true);
            setRowVisible(parent, true);
            setIndexExpanded(parent, true);
            parent = parent.parent();
        }

        // 递归显示本项的所有子孙项
        showRowRecursively(index);
    }
    
    return match;
}

bool HxItemSearchHelper::canCount(const QModelIndex &index) const
{
    if (mCanCountFunctor)
        return mCanCountFunctor(index);

    return true;
}

void HxItemSearchHelper::showRowRecursively(const QModelIndex &index)
{
    // 显示本项
    setAccepted(index, true);
    setRowVisible(index, true);

    // 显示子项
    const auto model = mView->model();
    for (int i = 0; i < model->rowCount(index); ++i) {
        const auto &childIndex = model->index(i, 0, index);
        showRowRecursively(childIndex);
    }
}

QModelIndexList HxItemSearchHelper::siblingIndexes(const QModelIndex &index) const
{
    QModelIndexList indexes;
    if (model())
    {
        for (int column = 0; column < model()->columnCount(index.parent()); ++column)
        {
            const auto &indx = model()->index(index.row(), column, index.parent());
            if (indx.isValid())
                indexes.append(index);
        }
    }
    return indexes;
}

QModelIndex HxItemSearchHelper::firstColumnIndex(const QModelIndex &index) const
{
    if (model())
        return model()->index(index.row(), 0, index.parent());
    else
        return QModelIndex();
}

QString HxItemSearchHelper::indexText(const QModelIndex &index) const
{
    const auto role = mRoleTextHash.keys().isEmpty() ? Qt::DisplayRole : mRoleTextHash.keys().first();
    const auto &text = mView->model()->data(index, role).toString();
    return text;
}

void HxItemSearchHelper::recordRowVisible(const QModelIndex &index, bool visible)
{
    if (mVisibleStateHash.contains(QPersistentModelIndex(index)))
        return;
    const auto &text = indexText(index);
    HX_DEBUG << QString("%1-%2").arg(text).arg(visible);
    mVisibleStateHash.insert(QPersistentModelIndex(index), visible);
}

void HxItemSearchHelper::resetRowsVisible()
{
    for (const auto &index : mVisibleStateHash.keys())
    {
        const bool visible = mVisibleStateHash.value(index);
        const auto &text = indexText(index);
        int row = index.row();
        setRowVisible(index, visible, false);
    }
    mVisibleStateHash.clear();
}

void HxItemSearchHelper::resetRowsExpand()
{
    for (const auto &index : mExpendStateHash.keys())
    {
        const bool expanded = mExpendStateHash.value(index);
        setIndexExpanded(index, expanded, false);
    }
        
    mExpendStateHash.clear();
}

void HxItemSearchHelper::setAccepted(const QModelIndex &index, bool accepted)
{
    mAcceptStateHash.insert(QPersistentModelIndex(index), accepted);
}

void HxItemSearchHelper::resetAcceptStates()
{
    mAcceptStateHash.clear();
}

void HxItemSearchHelper::clearAcceptState(const QModelIndex &index)
{
    mAcceptStateHash.remove(QPersistentModelIndex(index));
}

void HxItemSearchHelper::clearAcceptStateRecursively(const QModelIndex &index)
{
    clearAcceptState(index);
    if (model()->hasChildren(index))
    {
        for (int i = 0; i < model()->rowCount(); ++i)
        {
            const auto &childIndex = model()->index(0, 0, index);
            clearAcceptState(childIndex);
        }
    }
}

bool HxItemSearchHelper::isHandled(const QModelIndex &index) const
{
    return false;
    //return mAcceptStateHash.contains(QPersistentModelIndex(index));
}

bool HxItemSearchHelper::isAccepted(const QModelIndex &index) const
{
    return mAcceptStateHash.value(index, false);
}

void HxItemSearchHelper::setMatchCount(int count)
{
    if (mMatchCount == count)
        return;
    mMatchCount = count;
    emit matchCountChanged(count);
}

QAbstractItemModel *HxItemSearchHelper::model() const
{
    return mView->model();
}

bool HxItemSearchHelper::isRowVisible(const QModelIndex &index) const
{
    if (mView->inherits("QListView"))
    {
        const auto concrete_view = qobject_cast<QListView *>(mView);
        const bool visible = !concrete_view->isRowHidden(index.row());
        return visible;
    }

    else if (mView->inherits("QTreeView"))
    {
        const auto concrete_view = qobject_cast<QTreeView *>(mView);
        const bool visible = !concrete_view->isRowHidden(index.row(), index.parent());
        return visible;
    }

    else if (mView->inherits("QTableView"))
    {
        const auto concrete_view = qobject_cast<QTableView *>(mView);
        const bool visible = !concrete_view->isRowHidden(index.row());
        return visible;
    }
    const auto &className = mView->metaObject()->className();
    Q_ASSERT_X(false, __FUNCTION__, QString("Unexpected View Type []!").arg(className).toUtf8()); 
    return false;
}

void HxItemSearchHelper::setRowVisible(const QModelIndex &index, bool visible, bool record)
{
    if (mView->inherits("QListView")) 
    {
        const auto concrete_view = qobject_cast<QListView*>(mView);
        const auto old_visible = !concrete_view->isRowHidden(index.row());
        if (record)
            recordRowVisible(index, old_visible);
        
        concrete_view->setRowHidden(index.row(), !visible);
        const auto &text = indexText(index);
        HX_DEBUG << QString("%1-%2").arg(text).arg(visible);
    }

    else if (mView->inherits("QTreeView")) 
    {
        const auto concrete_view = qobject_cast<QTreeView *>(mView);
        const auto old_visible = !concrete_view->isRowHidden(index.row(), index.parent());
        if (record)
            recordRowVisible(index, old_visible);
        
        concrete_view->setRowHidden(index.row(), index.parent(), !visible);
        const auto &text = indexText(index);
        HX_DEBUG << QString("%1-%2").arg(text).arg(visible);
    }

    else if (mView->inherits("QTableView")) 
    {
        const auto concrete_view = qobject_cast<QTableView *>(mView);
        const auto old_visible = !concrete_view->isRowHidden(index.row());
        if (record)
            recordRowVisible(index, old_visible);
        
        concrete_view->setRowHidden(index.row(), !visible);
        const auto &text = indexText(index);
        HX_DEBUG << QString("%1-%2").arg(text).arg(visible);
    }
    else
    {
        const auto &className = mView->metaObject()->className();
        Q_ASSERT_X(false, __FUNCTION__, QString("Unexpected View Type []!").arg(className).toUtf8());
    }  
}

void HxItemSearchHelper::setIndexExpanded(const QModelIndex &index, bool expanded, bool record)
{
    if (!model()->hasChildren(index))
        return;

    if (mView->inherits("QTreeView"))
    {
        const auto concrete_view = qobject_cast<QTreeView *>(mView);
        const auto old_expanded = concrete_view->isExpanded(index);
        if (record)
            recordRowExpand(index, old_expanded);

        concrete_view->setExpanded(index, expanded);
    }  
}

void HxItemSearchHelper::recordRowExpand(const QModelIndex &index, bool expand)
{
    if (!mExpendStateHash.contains(index))
        mExpendStateHash.insert(index, expand);
}

bool HxItemSearchHelper::shouldFilter() const
{
    bool need_filter = false;
    for (const auto &search_text: mRoleTextHash.values()) 
        need_filter|= !search_text.simplified().isEmpty();

    for (const auto role : mRoleFlagHash.keys())
    {
        const auto search_flag = mRoleFlagHash.value(role);
        if (mNofilterFlagValueHash.contains(role)) {
            if (mNofilterFlagValueHash.value(role) == search_flag) 
                continue;
        }
        const auto nofilter_flag = mNofilterFlagValueHash.value(role);
        need_filter |= search_flag != nofilter_flag;
    }
    return need_filter;
}

int HxItemSearchHelper::totalValidCount() const
{
    std::function<QModelIndexList(const QModelIndex&)> GetAllIndexes = [&](const QModelIndex &index) 
    {
        QModelIndexList indexList;
        if (index.isValid()) 
        {    
            indexList.append(index);
            if (model()->hasChildren(index)) 
            {
                for (int i = 0; i < model()->rowCount(index); ++i) 
                {
                    const auto &childIndex = model()->index(i, 0, index);
                    indexList.append(GetAllIndexes(childIndex));
                }
            }         
        }
        return indexList;
    };

    QModelIndexList all_indexes;
    for (int i = 0; i < model()->rowCount(); ++i) 
    {
        const auto index = model()->index(i, 0, QModelIndex());
        all_indexes += GetAllIndexes(index);
    }

    for (const auto &index : all_indexes) 
    {
        if (!canCount(index)) 
            all_indexes.removeOne(index);
    }

    return all_indexes.count();
}

bool HxItemSearchHelper::matchRow(const QModelIndex &index)
{
    // 文本匹配规则: 只要任意一列的任意Role的文本匹配，本行文本就匹配成功
    bool text_matched = true;
    for (const auto role : mRoleTextHash.keys())
    {
        const auto &search_text = mRoleTextHash.value(role);
        const auto &sibling_indexes = siblingIndexes(index);
        for(const auto &index : sibling_indexes)
        {
            if(!textSearchableColumns().contains(index.column())) 
                continue;
            
            const auto &text = mView->model()->data(index, role).toString();
            text_matched = text.toLower().contains(search_text.toLower());
            if(text_matched) 
                break;
        }
        if (text_matched) 
            break;
    }

    // 标记匹配规则：本行中任意一列匹配Role则本行匹配该Role, 本行匹配所有Role才算匹配成功;
    bool flag_matched = true;
    for (const auto role : mRoleFlagHash.keys()) 
    {
        const auto search_flag = mRoleFlagHash.value(role);
        const auto &sibling_indexes = siblingIndexes(index);
        bool flag_role_match = true;
        if (mNofilterFlagValueHash.contains(role) && mNofilterFlagValueHash.value(role) == search_flag)
        {
            // Flag为NoFilter时匹配成功
        } 
        else 
        {
            for (const auto &index : sibling_indexes) 
            {
                const auto flag = mView->model()->data(index, role).toInt();
                flag_role_match &= bool(flag == search_flag);
                if (flag_role_match) 
                    break; 
            }
        }
        flag_matched &= flag_role_match;
    }

    const auto matched = text_matched && flag_matched;
    return matched;
}

