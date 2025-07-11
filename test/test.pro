QT += core gui
TEMPLATE = app

TARGET = HxWidgetsTest

CONFIG += c++17
QT += widgets

#DEFINES += HX_WIDGET_INSOURCE

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(WidgetsInspector/WidgetsInspector.pri)

SOURCES += \
    AnchorLayoutTestWidget.cpp \
    BadgeTestWidget.cpp \
    DoubleRangeSliderTestWidget.cpp \
    FlowLayoutTestWidget.cpp \
    ItemWidgetTestWidget.cpp \
    NestedSplitterTestWidget.cpp \
    RangeSliderTestWidget.cpp \
    SettingsDialogSearchTestWidget.cpp \
    SharedWidgetItemDelagateTestWidget.cpp \
    ToolBarTestWidget.cpp \
    WidgetsTestGlobal.cpp \
    main.cpp \
    TestStarterWidget.cpp

HEADERS += \
    AnchorLayoutTestWidget.hpp \
    BadgeTestWidget.hpp \
    DoubleRangeSliderTestWidget.hpp \
    FlowLayoutTestWidget.hpp \
    ItemWidgetTestWidget.hpp \
    NestedSplitterTestWidget.hpp \
    RangeSliderTestWidget.hpp \
    SettingsDialogSearchTestWidget.hpp \
    SharedWidgetItemDelagateTestWidget.hpp \
    TestStarterWidget.hpp \
    ToolBarTestWidget.hpp \
    WidgetsTestGlobalPrivate.hpp

FORMS +=

HXWIDGETS_LIB_PATH = $$OUT_PWD/../Bin/x64/Debug
HXWIDGETS_SRC_PATH = $$PWD/../Src

DESTDIR = $$HXWIDGETS_LIB_PATH

message(LibPath = $$absolute_path($$HXWIDGETS_LIB_PATH))
message(IncludePath = $$absolute_path($$HXWIDGETS_SRC_PATH))

win32:CONFIG(release, debug|release): LIBS += -L$$HXWIDGETS_LIB_PATH -lHxWidgets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$HXWIDGETS_LIB_PATH -lHxWidgetsd
include($$HXWIDGETS_SRC_PATH/HxWidgets.pri)

RESOURCES += \
    resources/stylesheets.qrc
