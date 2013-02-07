OTHER_FILES += \
    README.md \
    COPYING

HEADERS += \
    mainwindow.h \
    codeeditor.h \
    tabmanager.h \
    cpplexer.h \
    editorconfigdlg.h \
    comipler.h \
    gcc.h

SOURCES += \
    mainwindow.cpp \
    main.cpp \
    codeeditor.cpp \
    tabmanager.cpp \
    cpplexer.cpp \
    editorconfigdlg.cpp \
    comipler.cpp \
    gcc.cpp

RESOURCES += \
    rc.qrc

TRANSLATIONS += zh_CN.ts

FORMS += \
    editorconfigdlg.ui
