#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QAbstractItemView>
#include <QPointer>
#include <QHashFunctions>

#define ITEM_SEARCH_USEORIGINALINDEX

class QAbstractItemView;

class HX_WIDGETS_EXPORT HxItemSearchHelper : public QObject
{
    Q_OBJECT
public:
    using Flag = int;                                                       // 搜索标识值类型,一般是Enum值
    using CanCountFunctor = std::function<bool(const QModelIndex &)>;       // Index是否参与计数回调函数类型
    static bool isSearchBarEnabled() { return true;  }                      // 控制界面中的搜索栏是否可见
    static bool isSearchHelperEnabled() { return true; }                    // 控制搜索功能是否启用SearchHelper
    static bool isSearchProxyModelEnabled() { return false; }               // 是否使用ProxeModel
    static bool isSearchSupported(const QList<QAbstractItemView*> &views);  // 给定的View是否支持搜索功能

public:
    explicit HxItemSearchHelper(QObject *parent = nullptr);
    ~HxItemSearchHelper();
    void setView(QAbstractItemView *view);                                  // 设置视图
    void setSearchText(const QString &text, int role = Qt::DisplayRole);    // 设置文本搜索关键字
    void setSearchText(const QString &text, QList<int> roles);              // 设置指定Role的搜索关键字
    void setNofilterFlag(Flag flag, int role = Qt::UserRole);               // 设置指定Role中不参与搜索的标识值
    void setSearchFlag(Flag flag, int role = Qt::UserRole);                 // 设置指定Role的搜索标志值
    void setTextSearchableColumns(QList<int> columns);                      // 设置参与关键字搜索的列
    QList<int> textSearchableColumns() const;                               // 获取参与关键字搜索的列
    int matchCount() const { return mMatchCount; }                          // 匹配计数
    void setCanCountFunctor(CanCountFunctor functor);                       // 设置Item是否参与计数回调函数
    void setDynamicFilterEnabled(bool enable);                              // 设置是否启用动态过滤

signals:
    void matchCountChanged(int count);

private:
    void onRowsInserted(const QModelIndex &parent, int first, int last);
    void onRowsAboutTobeRemoved(const QModelIndex &parent, int first, int last);
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);

private:
    void search();                                                                      // 搜索条件发生改变后执行搜索过滤
    void handleRows(const QModelIndex &parent, int first, int last);                    // 过滤子Item
    void handleRow(const QModelIndex &index);                                           // 过滤Item
    void handleRowRemoved(const QModelIndex &index);                                    // Item即将被移除处理函数                                 
    bool filterIntex(const QModelIndex &index);                                         // 根据匹配结果更新当前Item相关的可见状态和折叠状态
    void setMatchCount(int count);                                                      // 设置匹配计数
    QAbstractItemModel *model() const;                                                  // 模型
    
    bool shouldFilter() const;                                                          // 判断当前搜索条件是否需要执行过滤搜索
    int totalValidCount() const;                                                        // 所有需要基数的行的总数
    bool matchRow(const QModelIndex &index);                                            // 根据搜索文本或者过滤字段匹配Item
    bool canCount(const QModelIndex &index) const;                                      // 判断某个Item是否参与计数
    void showRowRecursively(const QModelIndex &index);                                  // 递归设置Item及其子孙Item的可见状态
    QModelIndexList siblingIndexes(const QModelIndex &index) const;                     // 当前Item所在行的所有兄弟Item的索引
    QModelIndex firstColumnIndex(const QModelIndex &index) const;                       // 返回当前Item所在行第一列Item的索引
    QString indexText(const QModelIndex &index) const;                                  // Item文本(可搜索的)

    bool isRowVisible(const QModelIndex &index) const;                                  // Item是否乐见
    void setRowVisible(const QModelIndex &index, bool visible, bool record = true);     // 设置可见状态
    void recordRowVisible(const QModelIndex &index, bool visible);                      // 记录可见状态
    void resetRowsVisible();                                                            // 重置可见状态

    void setIndexExpanded(const QModelIndex &index, bool expanded, bool record = true); // 设置折叠状态
    void recordRowExpand(const QModelIndex &index, bool expand);                        // 记录折叠状态
    void resetRowsExpand();                                                             // 重置折叠状态

    void setAccepted(const QModelIndex &index, bool accepted);                          // 设置Accept状态
    bool isAccepted(const QModelIndex &index) const;                                    // 是否Accepted
    void resetAcceptStates();                                                           // 清空所有Accept状态
    void clearAcceptState(const QModelIndex &index);                                    // 清除指定Index的Accept状态
    void clearAcceptStateRecursively(const QModelIndex &index);                         // 递归清除指定Index的Accept状态
    bool isHandled(const QModelIndex &index) const;                                     // 判断是否已处理(已设置Accept状态)

private:
    QAbstractItemView *mView{ nullptr };                    // 目标视图
    QHash<int, QString> mRoleTextHash;                      // Role-文本关键字映射
    QHash<int, Flag> mRoleFlagHash;                         // Role-搜索标识映射
    QHash<int, Flag> mNofilterFlagValueHash;                // Role-不参与搜索的标识值
    QList<int> mTextSearchableColumns;                      // 不参与文本搜索的列
    int mMatchCount = 0;                                    // 匹配成功项个数
    QHash<QPersistentModelIndex, bool> mAcceptStateHash;    // Accept状态Hash
    QHash<QPersistentModelIndex, bool> mExpendStateHash;    // 折叠状态Hash
    QHash<QPersistentModelIndex, bool> mVisibleStateHash;   // 可见状态Hash
    CanCountFunctor mCanCountFunctor;                       // Item是否参与计数回调函数
    bool mDynamicFilterEnabled = true;                      // 是否启用动态过滤
};

