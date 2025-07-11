#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QFrame>
#include <QAction>
#include <QToolButton>
#include <QIcon>

#include <QBoxLayout>
#include "FlowLayout.hpp"

class QToolButton;
class QAction;

class HX_WIDGETS_EXPORT HxToolBar : public QFrame {
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxToolBar)
public:
    explicit HxToolBar(QWidget *parent = nullptr);
    virtual ~HxToolBar();

    enum class LayoutType {
        Box,
        FlowLayout
    };

    struct BoxLayoutParams {
        QBoxLayout::Direction direction = QBoxLayout::LeftToRight;
        Qt::Alignment alignment = Qt::AlignCenter | Qt::AlignLeft;
        int spacing = 3;
    };

    struct FlowLayoutParams {
        Hx::FlowLayout::FlowOrder flowOrder = Hx::FlowLayout::FlowOrder::RowFirst;
        Hx::FlowLayout::HorizontalFlowDirection horizontalDirection = Hx::FlowLayout::HorizontalFlowDirection::LeftToRight;
        Hx::FlowLayout::VerticalFlowDirection verticalDirection = Hx::FlowLayout::VerticalFlowDirection::TopToBottom;
        int horizontalSpacing = 3;
        int verticalSpacing = 3;
    };

public:
    void addWidget(QWidget *widget, int stretch = 0);
    void removeWidget(QWidget *widget);
    void addStretch(int stretch, const QString &name = "");
    void addSpacing(int spacing, const QString &name = "");
    void addSeparator(const QString &name = "");
    Qt::Orientation orientation() const;

public:
    void addAction(QAction *action);                                        // 添加动作
    QAction *addAction(const QString &text);                                // 指定文本添加动作
    QAction *addAction(const QIcon &icon, const QString &text);             // 指定图标和文本添加动作
    void addToolButton(QToolButton *button);                                // 添加按钮
    QToolButton *addToolButton(const QString &text);                        // 指定文本添加按钮
    QToolButton *addToolButton(const QIcon &icon, const QString &text);     // 指定图标和文本添加按钮
    QToolButton *addToolButton(const QString &name, const QString &tip);    // 指定对象名和工具提示添加按钮

public:
    QMargins margins() const;
    void setMargins(const QMargins &margins);
    void setMargins(int left, int top, int right, int bottom);
    void setLayoutParams(const BoxLayoutParams &params);
    void setLayoutParams(const FlowLayoutParams &params);

public:
    Qt::ToolBarAreas areas() const;
    void setSreas(const Qt::ToolBarAreas areas);

protected:
    void changeEvent(QEvent *event);

public:
    // 指定文本添加动作，连接到一个函数指针(有上下文)
    template<class Object, typename Functor>
    inline typename std::enable_if<!std::is_same<const char*, Functor>::value && QtPrivate::IsPointerToTypeDerivedFromQObject<Object*>::Value, QAction *>::type
    addAction(const QString &text, const Object *object, Functor slot) {
        QAction *action = addAction(text);
        connect(action, &QAction::triggered, object, std::move(slot));
        return action;
    }
    // 指定文本添加动作，连接到一个函数指针(无上下文)
    template <typename Functor>
    inline QAction *addAction(const QString &text, Functor slot) {
        QAction *action = addAction(text);
        connect(action, &QAction::triggered, slot);
        return action;
    }
    // 指定图标和文本添加动作，连接到一个函数指针(有上下文)
    template<class Object, typename Functor>
    static inline typename std::enable_if<!std::is_same<const char*, Functor>::value && QtPrivate::IsPointerToTypeDerivedFromQObject<Object*>::Value, QAction *>::type
    addAction(const QIcon &actionIcon, const QString &text, const Object *object, Functor slot) {
        QAction *action = addAction(actionIcon, text);
        connect(action, &QAction::triggered, object, std::move(slot));
        return action;
    }
    // 指定图标和文本添加动作，连接到一个函数指针(无上下文)
    template <typename Func1>
    static inline QAction *addAction(const QIcon &actionIcon, const QString &text, Func1 slot) {
        QAction *action = addAction(actionIcon, text);
        connect(action, &QAction::triggered, slot);
        return action;
    }

public:
    // 指定文本添加按钮，连接到一个函数指针(有上下文)
    template<class Object, typename Functor>
    inline typename std::enable_if<!std::is_same<const char*, Functor>::value && QtPrivate::IsPointerToTypeDerivedFromQObject<Object*>::Value, QToolButton*>::type
    addToolButton(const QString &text, const Object *object, Functor slot) {
        QToolButton *button = addToolButton(text);
        connect(button, &QToolButton::clicked, object, std::move(slot));
        return button;
    }
    // 指定文本添加动作，连接到一个函数指针(无上下文)
    template <typename Functor>
    inline QToolButton *addToolButton(const QString &text, Functor slot) {
        QToolButton *button = addToolButton(text);
        connect(button, &QToolButton::clicked, slot);
        return button;
    }
    // 指定图标和文本添加按钮，连接到一个函数指针(有上下文)
    template<class Object, typename Functor>
    inline typename std::enable_if<!std::is_same<const char*, Functor>::value && QtPrivate::IsPointerToTypeDerivedFromQObject<Object*>::Value, QToolButton*>::type
    addToolButton(const QIcon &icon, const QString &text, const Object *object, Functor slot) {
        QToolButton *button = addToolButton(icon,text);
        connect(button, &QToolButton::clicked, object, std::move(slot));
        return button;
    }
    // 指定文本添加动作，连接到一个函数指针(无上下文)
    template <typename Functor>
    inline QToolButton *addToolButton(const QIcon &icon, const QString &text, Functor slot) {
        QToolButton *button = addToolButton(icon, text);
        connect(button, &QToolButton::clicked, slot);
        return button;
    }
    // 指定对象名和提示添加按钮，连接到一个函数指针(有上下文)
    template<class Object, typename Functor>
    inline typename std::enable_if<!std::is_same<const char*, Functor>::value && QtPrivate::IsPointerToTypeDerivedFromQObject<Object*>::Value, QToolButton*>::type
    addToolButton(const QString &name, const QString &tip, const Object *object, Functor slot) {
        QToolButton *button = addToolButton(name, tip);
        connect(button, &QToolButton::clicked, object, std::move(slot));
        return button;
    }
    // 指定文本添加动作，连接到一个函数指针(无上下文)
    template <typename Functor>
    inline QToolButton *addToolButton(const QString &name, const QString &tip, Functor slot) {
        QToolButton *button = addToolButton(name, tip);
        connect(button, &QToolButton::clicked, slot);
        return button;
    }
};
