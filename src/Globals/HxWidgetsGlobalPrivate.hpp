#pragma once
#include "HxWidgetsGlobal.hpp"

#pragma execution_character_set("utf-8")
#include <QDebug>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(lcHxWidgets)

#ifdef Q_CC_MSVC
#define __HX_FUNCTION__ __FUNCTIONW__
#else
#define __HX_FUNCTION__ __FUNCTION__
#endif

#define HX_DEBUG qCDebug(lcHxWidgets).noquote() << QString("[%1][%2]").arg(__HX_FUNCTION__).arg(__LINE__)
#define HX_INFO qCInfo(lcHxWidgets).noquote() << QString("[%1][%2]").arg(__HX_FUNCTION__).arg(__LINE__)
#define HX_WARN qCWarning(lcHxWidgets).noquote() << QString("[%1][%2]").arg(__HX_FUNCTION__).arg(__LINE__)
#define HX_FATAL qCFatal(lcHxWidgets).noquote() << QString("[%1][%2]").arg(__HX_FUNCTION__).arg(__LINE__)
