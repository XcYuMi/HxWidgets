QT += core gui widgets

TEMPLATE = lib
TARGET = HxWidgets

SolutionDir = $$PWD/../
message(SolutionDir = $$absolute_path($$SolutionDir))

ProjectName = $$TARGET
message(ProjectName = $${ProjectName})

win32:CONFIG(release, debug|release): Configuration = Release
else:win32:CONFIG(debug, debug|release): Configuration = Debug
message(Configuration = $$Configuration)

#获取目标架构
TARGET_ARCH = $$QMAKE_TARGET.arch
# QMAKE_TARGET.arch 未定义,则动态监测
!isEmpty(TARGET_ARCH) {
    contains(TARGET_ARCH, x86_64) { Platform = x64 }
    else { Platform = x86 }
} else {
    win32 {
        #Windows 平台动态检测
        contains(MAKEFILE_GENERATOR, "64") {
            Platform = x64
        } else {
            Platform = x86
        }
    } else:unix {
        #Unix平台使用 uname -m
        TARGET_ARCH = $$system(uname -m)
        contains(TARGET_ARCH, x86_64) { Platform = x64 }
        else { Platform = x64 }
    }
}
#允许手动覆盖(如 qmake "ARCH=x64")
!isEmpty(ARCH_OVERRIDE) {
    Platform = $$ARCH_OVERRIDE
}

message(Platform = $${Platform})

OutputDir = $$SolutionDir/Bin/$$Platform/$$Configuration/
message(OutputDir = $$absolute_path($$OutputDir))
DESTDIR = $$OutputDir

TempDir = $$SolutionDir/Temp/$$Platform/$$Configuration/$$ProjectName/
OBJECTS_DIR = $$TempDir/obj
MOC_DIR = $$TempDir/moc
RCC_DIR = $$TempDir/rcc
UI_DIR = $$TempDir/ui

isEqual(Configuration,Debug) TARGET = $${ProjectName}d

message(Target = $${TARGET})

LibraryDir = $$SolutionDir/Lib/$$Platform/$$Configuration/
message(LibraryDir = $$absolute_path($$LibraryDir))
win32 {
    !exists($$LibraryDir): QMAKE_MKDIR = $$LibraryDir
    QMAKE_IMPLIB = $$LibraryDir/$${TARGET}.lib
}

BUILD_PATH = $$OUT_PWD/../../../
message(BuildPath = $$absolute_path($$BUILD_PATH))

DEFINES += HX_WIDGETS_LIBRARY


CONFIG += c++17

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # 禁用Qt6之前弃用的所有API

include(Globals/Globals.pri)
include(Layouts/Layouts.pri)
include(Controls/Controls.pri)
include(Containers/Containers.pri)
include(WindowSystem/WindowSystem.pri)
include(Dialogs/Dialogs.pri)
include(DockSystem/DockSystem.pri)

TRANSLATIONS += \
    src_en_US.ts
