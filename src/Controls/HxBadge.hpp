#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QFrame>
#include <QIcon>
#include <QPicture>
#include <QModelIndex>

class QMenu;
class QToolBar;
class QTabBar;
class QAbstractItemView;
class QGraphicsView;
class QGraphicsScene;
class QGraphicsItem;

class HxBadge;
class HxTextBadge;
class HxIconBadge;

/// Badge
class HX_WIDGETS_EXPORT HxBadge : public QFrame {
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxBadge)

public:
    enum Position {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        Left,
        Top,
        Bottom,
        Right,
        Center,
    };
    Q_ENUM(Position)

public:
    explicit HxBadge(QWidget *parent = nullptr);
    virtual ~HxBadge();
    void setPosition(Position position);
    void setAnchor(Position anchor);
    void setOffset(const QPoint &offset);
    Position position() const;
    Position anchor() const;
    QPoint offset() const;

signals:
    void positionChanged(Position position);
    void anchorChanged(Position anchor);
    void offsetChanged(const QPoint &offset);

public:
    // 向Widget目标添加徽标
    static HxTextBadge *addTextBadge(QWidget *widget, Position position, const QPoint &offset);
    static HxIconBadge *addIconBadge(QWidget *widget, Position position, const QPoint &offset);
    // 向ItemView目标添加徽标
    static HxTextBadge *addTextBadge(QAbstractItemView *view, const QModelIndex &index, Position position, const QPoint &offset);
    static HxIconBadge *addIconBadge(QAbstractItemView *view, const QModelIndex &index, Position position, const QPoint &offset);
    // 向GraphicsView添加徽标
    static HxTextBadge *addTextBadge(QGraphicsView *view, QGraphicsItem *item, Position position, const QPoint &offset);
    static HxIconBadge *addIconBadge(QGraphicsView *view, QGraphicsItem *item, Position position, const QPoint &offset);
    // 向MenuItem目标添加徽标
    static HxTextBadge *addTextBadge(QMenu *menu, QAction *action, Position position, const QPoint &offset);
    static HxIconBadge *addIconBadge(QMenu *menu, QAction *action, Position position, const QPoint &offset);
    // 向TabBarItem目标添加徽标
    static HxTextBadge *addTextBadge(QTabBar *tabBar, int tabIndex, Position position, const QPoint &offset);
    static HxIconBadge *addIconBadge(QTabBar *tabBar, int tabIndex, Position position, const QPoint &offset);
    // 移除徽标
    static bool removeBadge(HxBadge *badge);                                        // 移除指定徽标
    static bool removeBadges();                                                     // 移除所有徽标
    static bool removeBadges(QWidget *widget);                                      // 移除Widget上添加的所有徽标
    static bool removeBadges(QAbstractItemView *view, const QModelIndex &index);    // 移除视图向上添加的所有徽标
    static bool removeBadges(QGraphicsView *view, QGraphicsItem *item);             // 移除图形视图中图形项上添加的所有徽标
    static bool removeBadges(QMenu *menu, QAction *action);                         // 移除菜单项上添加的所有徽标
    static bool removeBadges(QTabBar *tabBar, int tabIndex);                        // 移除TabBarItem上添加的所有徽标
    static bool removeBadgesInItemView(QAbstractItemView *view);                    // 移除视图中的所有徽标
    static bool removeBadgesInItemView(QGraphicsView *view);                        // 移除图形视图中的所有图标
    static bool removeBadgesInMenu(QMenu *menu);                                    // 移除菜单中的所有徽标
    static bool removeBadgesInTabBar(QTabBar *tabBar);                              // 移除TabBar中的所有徽标
};

/// BadgeManager
class HxBagdeManager : public QObject {
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxBagdeManager)
protected:
    explicit HxBagdeManager(QObject *parent = nullptr);
public:
    ~HxBagdeManager();
    static HxBagdeManager *instance();
    HxBadge *addBadge(QWidget *widget, HxBadge *badge);
protected:

};

static HxBagdeManager *bageManager() {
    return HxBagdeManager::instance();
}

class HxBadgeTargetTrack : public QObject {
    Q_OBJECT
public:
    explicit HxBadgeTargetTrack(QObject *parent = nullptr) : QObject{parent} {}
    virtual ~HxBadgeTargetTrack() {}
};

class HxWidgetBadgeTrack : public HxBadgeTargetTrack {
    Q_OBJECT
public:
    explicit HxWidgetBadgeTrack(QObject *parent = nullptr);
    ~HxWidgetBadgeTrack();
    void updateBadge(QWidget *widget, HxBadge *badge);
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

class HxViewItemBadgeTracker : public HxBadgeTargetTrack {
    Q_OBJECT
public:
    explicit HxViewItemBadgeTracker(QObject *parent = nullptr);
    ~HxViewItemBadgeTracker();
    void addViewItem(QAbstractItemView *view, const QModelIndex &index);
    void removeViewItem(QAbstractItemView *view, const QModelIndex &index);
    void removeView(QAbstractItemView *view);
};

class HxTabBarItemBadgeTracker : public HxBadgeTargetTrack {
    Q_OBJECT
public:
    explicit HxTabBarItemBadgeTracker(QObject *parent = nullptr);
    ~HxTabBarItemBadgeTracker();
};

class HxMenuItemBadgeTracker : public HxBadgeTargetTrack {
    Q_OBJECT
public:
    explicit HxMenuItemBadgeTracker(QObject *parent = nullptr);
    ~HxMenuItemBadgeTracker();
};

class HxGraphicsViewItemBadgeTracker : public HxBadgeTargetTrack {
    Q_OBJECT
public:
    explicit HxGraphicsViewItemBadgeTracker(QObject *parent = nullptr);
    ~HxGraphicsViewItemBadgeTracker();
};



/// TextBadge
class HX_WIDGETS_EXPORT HxTextBadge : public HxBadge {
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxTextBadge)
public:
    HxTextBadge();
    ~HxTextBadge();
    void setText(const QString &text);
    QString text() const;
};

/// IconBadge
class HX_WIDGETS_EXPORT HxIconBadge : public HxBadge {
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxIconBadge)
public:
    HxIconBadge();
    ~HxIconBadge();
    void setIcon(const QIcon &icon);
    void setIconSize(const QSize &size);
    void setPicture(const QPicture &picture);

public:
    QIcon icon() const;
    QSize iconSize() const;

protected:
    void paintEvent(QPaintEvent *event) override;
};
