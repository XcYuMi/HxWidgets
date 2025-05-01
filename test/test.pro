QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += HX_WIDGET_INSOURCE

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FlowLayoutTestWidget.cpp \
    main.cpp \
    TestStarterWidget.cpp

HEADERS += \
    FlowLayoutTestWidget.hpp \
    TestStarterWidget.hpp

FORMS += \
    TestStarterWidget.ui

HXWIDGETS_LIB_PATH = $$OUT_PWD/../src/debug
HXWIDGETS_SRC_PATH = $$PWD/../src

message(LibPath = $$absolute_path($$HXWIDGETS_LIB_PATH))
message(IncludePath = $$absolute_path($$HXWIDGETS_SRC_PATH))

win32:CONFIG(release, debug|release): LIBS += -L$$HXWIDGETS_LIB_PATH -lHxWidgets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$HXWIDGETS_LIB_PATH -lHxWidgetsd
include($$HXWIDGETS_SRC_PATH/HxWidgets.pri)
