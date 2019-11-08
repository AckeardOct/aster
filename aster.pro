TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = $$_PRO_FILE_PWD_/bin

CONFIG += conan_basic_setup
CONAN_PATH = $$_PRO_FILE_PWD_/build/conan
include($$CONAN_PATH/conanbuildinfo.pri)

SOURCES += \
        src/common/game_window.cpp \
        src/main.cpp

HEADERS += \
    src/common/game_window.h \
    src/common/logger.h
