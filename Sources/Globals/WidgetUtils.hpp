#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QObject>
#include <QAction>
#include <QMetaType>

class QWidget;
class QComboBox;
class QSlider;
class QSpinBox;
class QCheckBox;
class QAction;

class HX_WIDGETS_EXPORT HxWidgetUtils : public QObject
{
    Q_OBJECT

protected:
    HxWidgetUtils(QObject *parent = nullptr) : QObject{parent} {}
    ~HxWidgetUtils() {}

public: // QWidget
    static void setWidth(QWidget *widget, int width);
    static void setHeight(QWidget *widget, int height);

public: // AddAction
    static QAction *addAction(QWidget *widget, const QString &text);
    static QAction *addAction(QWidget *widget, const QIcon &icon, const QString &text);
    //static QAction *addAction(QWidget *widget, const QString &text, const QObject *receiver, const char* member);
    //static QAction *addAction(QWidget *widget, const QIcon &icon, const QString &text, const QObject *receiver, const char* member);

    // addAction(QString): Connect to a QObject slot / functor or function pointer (with context)
    template<class Obj, typename Func1>
    static inline typename std::enable_if<!std::is_same<const char*, Func1>::value
                                       && QtPrivate::IsPointerToTypeDerivedFromQObject<Obj*>::Value, QAction *>::type
    addAction(QWidget *widget, const QString &text, const Obj *object, Func1 slot)
    {
        QAction *result = addAction(widget, text);
        connect(result, &QAction::triggered, object, std::move(slot));
        return result;
    }
    // addAction(QString): Connect to a functor or function pointer (without context)
    template <typename Func1>
    static inline QAction *addAction(QWidget *widget, const QString &text, Func1 slot)
    {
        QAction *result = addAction(widget, text);
        connect(result, &QAction::triggered, slot);
        return result;
    }
    // addAction(QString): Connect to a QObject slot / functor or function pointer (with context)
    template<class Obj, typename Func1>
    static inline typename std::enable_if<!std::is_same<const char*, Func1>::value
                                       && QtPrivate::IsPointerToTypeDerivedFromQObject<Obj*>::Value, QAction *>::type
    addAction(QWidget *widget, const QIcon &actionIcon, const QString &text, const Obj *object, Func1 slot)
    {
        QAction *result = addAction(widget, actionIcon, text);
        connect(result, &QAction::triggered, object, std::move(slot));
        return result;
    }
    // addAction(QIcon, QString): Connect to a functor or function pointer (without context)
    template <typename Func1>
    static inline QAction *addAction(QWidget *widget, const QIcon &actionIcon, const QString &text, Func1 slot)
    {
        QAction *result = addAction(widget, actionIcon, text);
        connect(result, &QAction::triggered, slot);
        return result;
    }

public: // QComboBox
    static void SetComboxBoxCurrentData(QComboBox *control, const QVariant &data, bool blockSignal = true);

public: // SpinBox
    static void SetSpinBoxValue(QSpinBox *control, int value, bool blockSignal = true);

public: // Slider
    static void SetSliderValue(QSlider *control, int value, bool blockSignal = true);
};
