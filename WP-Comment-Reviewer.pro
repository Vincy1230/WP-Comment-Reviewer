QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about.cpp \
    editor.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    replier.cpp

HEADERS += \
    about.h \
    editor.h \
    login.h \
    mainwindow.h \
    replier.h

FORMS += \
    about.ui \
    editor.ui \
    login.ui \
    mainwindow.ui \
    replier.ui

TRANSLATIONS = \
        i18n/th_TH.ts \
        i18n/zh_CN.ts \
        i18n/zh_TW.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
