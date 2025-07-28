#include "HxCustomizedWindowHelper.hpp"
#include "HxWidgetsGlobalPrivate.hpp"

#include <QApplication>
#include <QWidget>
#include <QWindow>

#define ASSERT_WINDOW(object) Q_ASSERT_X(HxCustomizedWindowHelper::isTopLevelNativeWindow(object), __FUNCTION__, "Object is not a toplevel native window!")
#define ASSERT_WINDOW_CUSTOMIZED(window) Q_ASSERT_X(isWindowCustomized(window), __FUNCTION__, "Window is not customized!")
#define ASSERT_CUSTOMIZED_WINDOW(object) ASSERT_WINDOW(object); ASSERT_WINDOW_CUSTOMIZED(object)

class HxCustomizedWindowHelper::HxCustomizedWindowHelperPrivate {
    HxCustomizedWindowHelper *q = nullptr;
public:
    HxCustomizedWindowHelperPrivate(HxCustomizedWindowHelper *qptr) { q = qptr; }
public:
    QHash<QObject*, HxCustomizedWindowHelper::WindowParam> widowParamHash;
};

HxCustomizedWindowHelper::~HxCustomizedWindowHelper() {

}

HxCustomizedWindowHelper::HxCustomizedWindowHelper(QObject *parent) : QObject{parent} {
    d.reset(new HxCustomizedWindowHelperPrivate(this));
}

HxCustomizedWindowHelper *HxCustomizedWindowHelper::instance() {
    static QPointer<HxCustomizedWindowHelper> singleton;
    if(singleton.isNull()) {
        singleton = new HxCustomizedWindowHelper(qApp);
    }
    return singleton;
}

bool HxCustomizedWindowHelper::isTopLevelNativeWindow(QObject* object) {
    if (object->isWidgetType()) {
        const auto widget = qobject_cast<QWidget*>(object);
        bool is = widget->isTopLevel();
        return is;
    } else if (object->isWidgetType()) {
        const auto window = qobject_cast<QWindow*>(object);
        bool is = window->isTopLevel();
        return is;
    } else {
        return false;
    }
}

void HxCustomizedWindowHelper::setWindowCustomized(QObject *window, bool enable) {
    ASSERT_WINDOW(window);
    if (isWindowCustomized(window) && enable) {
        return;
    }

    d->widowParamHash.insert(window, WindowParam());
}

bool HxCustomizedWindowHelper::isWindowCustomized(QObject *window) const {
    ASSERT_WINDOW(window);
    const bool is = d->widowParamHash.contains(window);
    return is;
}

HxCustomizedWindowHelper::WindowParam& HxCustomizedWindowHelper::windowParam(QObject* window) {
    ASSERT_CUSTOMIZED_WINDOW(window);
    return d->widowParamHash[window];
}

HxCustomizedWindowHelper::WindowParam HxCustomizedWindowHelper::windowParam(QObject* window) const {
    ASSERT_CUSTOMIZED_WINDOW(window);
    const auto &param = d->widowParamHash.value(window);
    return param;
}

void HxCustomizedWindowHelper::setWindowAttribute(QObject *window, const QString &name, const QVariant &value) {
    ASSERT_CUSTOMIZED_WINDOW(window);
    auto& param = windowParam(window);
    param.attributHash.insert(name, value);
}

QVariant HxCustomizedWindowHelper::windowAttribute(QObject *window, const QString &name) const {
    ASSERT_CUSTOMIZED_WINDOW(window);
    const auto &param = windowParam(window);
    return param.attributHash.value(name);
}

void HxCustomizedWindowHelper::setWindowFlag(QObject *window, Hx::WindowFlag flag, bool enable) {
    ASSERT_CUSTOMIZED_WINDOW(window);
    auto& param = windowParam(window);
    param.flags.testFlag(flag);
}

void HxCustomizedWindowHelper::setWindowFlags(QObject *window, Hx::WindowFlags flags) {
    ASSERT_CUSTOMIZED_WINDOW(window);
    auto& param = windowParam(window);
    param.flags = flags;
}

Hx::WindowFlags HxCustomizedWindowHelper::windowFlags(QObject *window) const {
    ASSERT_CUSTOMIZED_WINDOW(window);
    const auto &param = windowParam(window);
    return param.flags;
}

bool HxCustomizedWindowHelper::testWindowFlag(QObject *window, Hx::WindowFlag flag) const {
    ASSERT_CUSTOMIZED_WINDOW(window);
    const auto &param = windowParam(window);
    const auto &enabled = param.flags.testFlag(flag);
    return enabled;
}
