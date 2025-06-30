#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QObject>

class QWidget;

class HxWidgetSearchHelperDef : public QObject {
    Q_OBJECT
public:
    using QObject::QObject;
public:
    using Container = QWidget*;
    using LocateFunctor = std::function<void(QWidget *widget)>;
    using WidgetFilterFunctor = std::function<bool(QWidget *widget)>;
    using WidgetTextGenerator = std::function<QString(QWidget)>;
};

class HX_WIDGETS_EXPORT HxWidgetSearchHelper : public HxWidgetSearchHelperDef
{
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxWidgetSearchHelper)
    using Super = HxWidgetSearchHelperDef;
protected:
    explicit HxWidgetSearchHelper(QObject *parent = nullptr);
public:
    ~HxWidgetSearchHelper();
    static HxWidgetSearchHelper *instance();
    void setContainerSearchable(Container container, bool searchable);
    void setLocator(Container container, const LocateFunctor &functor);
    void setFilter(Container container, const WidgetFilterFunctor &functor);
    void setTextGenerator(Container container, const WidgetTextGenerator &functor);
    void searchWidget(Container container, const QString &text);
    void searchWidget(Container container, const QRegularExpression &regexp);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    bool isContainerSearchable(QWidget *widget) const;
};

inline HxWidgetSearchHelper *widgetSearchHelper()
{
    return HxWidgetSearchHelper::instance();
}

