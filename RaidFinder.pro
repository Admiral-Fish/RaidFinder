lessThan(QT_MAJOR_VERSION, 5): error("You need at least Qt 5.9 to build RaidFinder")
equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 9): error("You need at least Qt 5.9 to build RaidFinder")

QT += widgets
CONFIG += c++1z lrelease embed_translations

TARGET = RaidFinder
TEMPLATE = app
VERSION = 1.1.1

DEFINES += VERSION=\\\"$$VERSION\\\"
DEFINES += QT_DEPRECATED_WARNINGS

RC_ICONS += Images/raidfinder.ico
ICON += Images/raidfinder.icns

DEFINES += QT_DEPRECATED_WARNINGS

RESOURCES += \
    resources.qrc \
    QDarkStyleSheet/qdarkstyle/style.qrc

TRANSLATIONS += \
    i18n/RaidFinder_de.ts \
    i18n/RaidFinder_en.ts \
    i18n/RaidFinder_es.ts \
    i18n/RaidFinder_fr.ts \
    i18n/RaidFinder_it.ts \
    i18n/RaidFinder_ja.ts \
    i18n/RaidFinder_ko.ts \
    i18n/RaidFinder_zh.ts

FORMS += \
    Forms/Controls/IVFilter.ui \
    Forms/MainWindow.ui \
    Forms/ProfileEditor.ui \
    Forms/ProfileManager.ui

HEADERS += \
    Core/DenLoader.hpp \
    Core/FrameCompare.hpp \
    Core/Game.hpp \
    Core/Global.hpp \
    Core/RNG/XoroShiro.hpp \
    Core/RaidGenerator.hpp \
    Core/Results/Den.hpp \
    Core/Results/Frame.hpp \
    Core/Results/Profile.hpp \
    Core/Results/Raid.hpp \
    Core/Util/Translator.hpp \
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
    Core/DenLoader.cpp \
    Core/FrameCompare.cpp \
    Core/RNG/XoroShiro.cpp \
    Core/RaidGenerator.cpp \
    Core/Results/Den.cpp \
    Core/Results/Frame.cpp \
    Core/Results/Profile.cpp \
    Core/Results/Raid.cpp \
    Core/Util/Translator.cpp \
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
