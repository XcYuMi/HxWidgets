#pragma once
#include <QDebug>
#include <QLoggingCategory>
#pragma execution_character_set("utf-8")

Q_DECLARE_LOGGING_CATEGORY(lcWidgetsTest)

#ifdef Q_CC_MSVC
#define __TEST_FUNCTION__ __FUNCTIONW__
#else
#define __TEST_FUNCTION__ __FUNCTION__
#endif

#define TEST_DEBUG qCDebug(lcWidgetsTest).noquote() << QString("[%1][%2]").arg(__TEST_FUNCTION__).arg(__LINE__)
#define TEST_INFO qCInfo(lcWidgetsTest).noquote() << QString("[%1][%2]").arg(__TEST_FUNCTION__).arg(__LINE__)
#define TEST_WARN qCWarning(lcWidgetsTest).noquote() << QString("[%1][%2]").arg(__TEST_FUNCTION__).arg(__LINE__)
#define TEST_FATAL qCFatal(lcWidgetsTest).noquote() << QString("[%1][%2]").arg(__TEST_FUNCTION__).arg(__LINE__)
