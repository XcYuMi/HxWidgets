#include "HxWidgetSearchHelper.hpp"
#include "HxWidgetsGlobalPrivate.hpp"

#include <QApplication>
#include <QWidget>
#include <QHash>
#include <QMultiHash>

struct ContainerData {
    QHash<QWidget*,QString> widgetTextHash;
    QMultiHash<QString,QWidget*> textWidgetsHash;
    HxWidgetSearchHelper::LocateFunctor widgetLocator;
    HxWidgetSearchHelper::WidgetFilterFunctor widgetFilter;
    HxWidgetSearchHelper::WidgetTextGenerator textGenerator;
};

class HxWidgetSearchHelper::HxWidgetSearchHelperPrivate
{
    HxWidgetSearchHelper *q = nullptr;
public:
    QHash<HxWidgetSearchHelper::Container, ContainerData> containerDataHash;
public:
    void init(HxWidgetSearchHelper *qptr) { q = qptr; }
    inline ContainerData &containerData(HxWidgetSearchHelper::Container container) {
        return containerDataHash[container];
    }
    inline ContainerData containerData(HxWidgetSearchHelper::Container container) const {
        return containerDataHash.value(container);
    }

};

HxWidgetSearchHelper::HxWidgetSearchHelper(QObject *parent)
    : Super{parent}
{
    d.reset(new HxWidgetSearchHelperPrivate);
    d->init(this);
}

HxWidgetSearchHelper::~HxWidgetSearchHelper()
{
}

HxWidgetSearchHelper *HxWidgetSearchHelper::instance()
{
    static QPointer<HxWidgetSearchHelper> singleton;
    if(singleton.isNull())
        singleton = new HxWidgetSearchHelper(qApp);
    return singleton;
}

void HxWidgetSearchHelper::setContainerSearchable(Container container, bool searchable)
{
    if(searchable && !isContainerSearchable(container))
    {
        ContainerData data;
        d->containerDataHash.insert(container, data);
        container->installEventFilter(this);
    }
    else
    {
        d->containerDataHash.remove(container);
        for(QWidget *widget : container->findChildren<QWidget*>())
            disconnect(widget, nullptr, this, nullptr);
    }
}

void HxWidgetSearchHelper::setLocator(Container container, const LocateFunctor &functor)
{
    if(isContainerSearchable(container))
    {
        auto &data = d->containerData(container);
        data.widgetLocator = functor;
    }
}

void HxWidgetSearchHelper::setFilter(Container container, const WidgetFilterFunctor &functor)
{
    if(isContainerSearchable(container))
    {
        auto &data = d->containerData(container);
        data.widgetFilter = functor;
    }
}

void HxWidgetSearchHelper::setTextGenerator(Container container, const WidgetTextGenerator &functor)
{
    if(isContainerSearchable(container))
    {
        auto &data = d->containerData(container);
        data.textGenerator = functor;
    }
}

void HxWidgetSearchHelper::searchWidget(Container container, const QString &text)
{

}

void HxWidgetSearchHelper::searchWidget(Container container, const QRegularExpression &regexp)
{

}

bool HxWidgetSearchHelper::eventFilter(QObject *watched, QEvent *event)
{
    const bool result = Super::eventFilter(watched, event);
    return result;
}

bool HxWidgetSearchHelper::isContainerSearchable(QWidget *widget) const
{
    return d->containerDataHash.contains(widget);
}


