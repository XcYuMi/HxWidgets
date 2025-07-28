#include "HxCustomizedWindowHelper.hpp"
#include "HxWidgetsGlobalPrivate.hpp"
#ifdef Q_OS_WINDOWS

bool HxCustomizedWindowHelper::eventFilter(QObject *object, QEvent *event) {
    const bool result = QObject::eventFilter(object, event);
    return result;
}

bool HxCustomizedWindowHelper::nativeEventFilter(const QByteArray &eventType, void *pmessage, long *result) {
    Q_UNUSED(eventType); Q_UNUSED(pmessage); Q_UNUSED(result);
    return false;
}

#endif
