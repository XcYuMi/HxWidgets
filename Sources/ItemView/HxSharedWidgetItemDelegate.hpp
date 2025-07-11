#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QFrame>
#include <QStyledItemDelegate>
#include <QAbstractItemView>

// 共享项部件基类
class HxSharedItemWidget : public QFrame {
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxSharedItemWidget)
    using Super = QFrame;
    friend class HxSharedWidgetItemDelegate;
public:
    enum class Type {
        Unknown,
        Renderer,
        Indicator,
        Editor,
    };
    Q_ENUM(Type)
public:
    explicit HxSharedItemWidget(QWidget *parent = nullptr);
    virtual ~HxSharedItemWidget();
    Type type() const;

protected:
    void setType(Type type);
};

// 共享部件项代理
class HxSharedWidgetItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxSharedWidgetItemDelegate)
    using Super = QStyledItemDelegate;

public:
    enum class ItemType {
        None = 0,
        PlainText,
        User = 1000,
    };

public:
    explicit HxSharedWidgetItemDelegate(QObject *parent = nullptr);
    virtual ~HxSharedWidgetItemDelegate();
    void setView(QAbstractItemView *view);
    QPointer<QAbstractItemView> view() const;

// 保护的虚函数
protected:
    virtual HxSharedItemWidget *createWidget(QWidget *parent, int itemType) const;
    virtual int itemType(const QModelIndex &index) const;

// 重载的公共函数
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

// 重写的保护局函数
protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    HxSharedItemWidget *itemWidget(HxSharedItemWidget::Type widgetType,const QModelIndex &index) const;
};
