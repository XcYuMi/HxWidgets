#pragma once
#include "HxWidgetsGlobal.hpp"
#include "HxWidgetsNamespace.hpp"
#include <QObject>
#include <QAbstractNativeEventFilter>

class HX_WIDGETS_EXPORT HxCustomizedWindowHelper : public QObject, public QAbstractNativeEventFilter {
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxCustomizedWindowHelper)
protected:
    explicit HxCustomizedWindowHelper(QObject *parent = nullptr);
public:
    static HxCustomizedWindowHelper *instance();
    ~HxCustomizedWindowHelper();
    void setWindowCustomized(QObject *window, bool enable);
    bool isWindowCustomized(QObject *window) const;
    void setWindowAttribute(QObject *window, const QString &name, const QVariant &value);
    QVariant windowAttribute(QObject *window, const QString &name) const;
    void setWindowFlag(QObject *window, Hx::WindowFlag flag, bool enable);
    void setWindowFlags(QObject *window, Hx::WindowFlags flags);
    Hx::WindowFlags windowFlags(QObject *window) const;
    bool testWindowFlag(QObject *window, Hx::WindowFlag flag) const;

protected:
    bool eventFilter(QObject *object, QEvent *event) override;
    bool nativeEventFilter(const QByteArray &eventType, void *pmessage, long *result) override;

public:
    static bool isTopLevelNativeWindow(QObject* window);

private:
    struct WindowParam {
        QHash<QString,QVariant> attributHash;
        Hx::WindowFlags flags;
    };
    WindowParam &windowParam(QObject *window);
    WindowParam windowParam(QObject *window) const;
};

inline HxCustomizedWindowHelper *windowHelper() {
    return HxCustomizedWindowHelper::instance();
}
