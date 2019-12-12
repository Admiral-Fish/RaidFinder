lessThan(QT_MAJOR_VERSION, 5): error("You need at least Qt 5.9 to build RaidFinder")
equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 9): error("You need at least Qt 5.9 to build RaidFinder")

QT += widgets
CONFIG += c++1z lrelease embed_translations

TARGET = RaidFinder
TEMPLATE = app
VERSION = 1.0.0
#GIT_VERSION = $$system(git rev-parse HEAD)
#GIT_BRANCH = $$system(git rev-parse --abbrev-ref HEAD)

#DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"
#DEFINES += GIT_BRANCH=\\\"$$GIT_BRANCH\\\"
DEFINES += VERSION=\\\"$$VERSION\\\"
DEFINES += QT_DEPRECATED_WARNINGS

#RC_ICONS += Images/raidfinder.ico
#ICON += Images/raidfinder.icns

DEFINES += QT_DEPRECATED_WARNINGS

RESOURCES += \
    QDarkStyleSheet/qdarkstyle/style.qrc

TRANSLATIONS += \
    i18n/RaidFinder_de.ts \
    i18n/RaidFinder_en.ts \
    i18n/RaidFinder_es.ts \
    i18n/RaidFinder_fr.ts \
    i18n/RaidFinder_it.ts \
    i18n/RaidFinder_ja.ts \
    i18n/RaidFinder_ko.ts \
    i18n/RaidFinder_zh_Hans_CN.ts

FORMS += \
    Forms/Controls/IVFilter.ui \
    Forms/MainWindow.ui \
    Forms/ProfileEditor.ui \
    Forms/ProfileManager.ui

HEADERS += \
    Core/Frame.hpp \
    Core/FrameCompare.hpp \
    Core/Game.hpp \
    Core/Global.hpp \
    Core/Nature.hpp \
    Core/Power.hpp \
    Core/Profile.hpp \
    Core/RaidGenerator.hpp \
    Core/XoroShiro.hpp \
    Forms/Controls/CheckList.hpp \
    Forms/Controls/IVFilter.hpp \
    Forms/Controls/Label.hpp \
    Forms/Controls/TableView.hpp \
    Forms/Controls/TextBox.hpp \
    Forms/MainWindow.hpp \
    Forms/ProfileEditor.hpp \
    Forms/ProfileManager.hpp \
    Models/FrameModel.hpp \
    Models/ProfileModel.hpp \
    Models/TableModel.hpp

SOURCES += \
    Core/Frame.cpp \
    Core/FrameCompare.cpp \
    Core/Nature.cpp \
    Core/Power.cpp \
    Core/Profile.cpp \
    Core/RaidGenerator.cpp \
    Core/XoroShiro.cpp \
    Forms/Controls/CheckList.cpp \
    Forms/Controls/IVFilter.cpp \
    Forms/Controls/Label.cpp \
    Forms/Controls/TableView.cpp \
    Forms/Controls/TextBox.cpp \
    Forms/MainWindow.cpp \
    Forms/ProfileEditor.cpp \
    Forms/ProfileManager.cpp \
    Models/FrameModel.cpp \
    Models/ProfileModel.cpp \
    main.cpp


