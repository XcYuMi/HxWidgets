#pragma once
#include <QtGlobal>

// 库导入导出宏
#ifdef HX_WIDGET_INSOURCE
    #define HX_WIDGETS_EXPORT
#else
    #ifdef HX_WIDGETS_LIBRARY
        #define HX_WIDGETS_EXPORT Q_DECL_EXPORT
    #else
        #define HX_WIDGETS_EXPORT Q_DECL_IMPORT
    #endif
#endif

// 命名空间
#define HX_BEGIN_NAMESPACE namespace Hx {
#define HX_END_NAMESPACE }
#define HX_USING_NAMESPACE using namespace Hx

// 智能指针
#include <QScopedPointer>
#include <QPointer>

// 类数据指针申明宏
#define HX_DECLARE_PRIVATE(T) \
    class T##Private; \
    QScopedPointer<T##Private> d; \
    friend class T##Private;

// 类界面指针申明宏
#define HX_DECLARE_UI(T) \
    class T##Ui; \
    QScopedPointer<T##Ui> ui; \
    friend class T##Ui;
