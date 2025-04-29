QT += core gui widgets

TEMPLATE = lib

win32 {
    CONFIG(debug, debug|release) {
        TARGET= HxWidgetsd
    } else {
        TARGET= HxWidgets
    }
}

DEFINES += HX_WIDGETS_LIBRARY


CONFIG += c++17

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # 禁用Qt6之前弃用的所有API

include(Globals/Globals.pri)
include(NativeWindow/NativeWindow.pri)
include(Dialogs/Dialogs.pri)
include(Controls/Controls.pri)
include(Layouts/Layouts.pri)
include(Containers/Containers.pri)
include(DockSystem/DockSystem.pri)

TRANSLATIONS += \
    src_en_US.ts
