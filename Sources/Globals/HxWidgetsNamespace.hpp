#pragma once
#include <QObject>

class Hx : public QObject {
    Q_GADGET
public:
    enum class WindowFlag {

    };
    Q_DECLARE_FLAGS(WindowFlags, WindowFlag)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Hx::WindowFlags)
