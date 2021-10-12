QT += core gui network KWidgetsAddons #KF5Package
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

SOURCES += \
    main.cpp \
    search.cpp

HEADERS += \
    search.h

FORMS += \
    search.ui

RESOURCES += \
    assets/resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
