lessThan(QT_MAJOR_VERSION, 5): error("You need at least Qt 5.9 to build RaidFinder")
equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 9): error("You need at least Qt 5.9 to build RaidFinder")

QT += widgets
CONFIG += c++1z lrelease embed_translations

TARGET = RaidFinder
TEMPLATE = app
VERSION = 1.3.0

DEFINES += VERSION=\\\"$$VERSION\\\"
DEFINES += QT_DEPRECATED_WARNINGS

RC_ICONS += Resources/Images/raidfinder.ico
ICON += Resources/Images/raidfinder.icns

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
    Forms/DenMap.ui \
    Forms/IVCalculator.ui \
    Forms/MainWindow.ui \
    Forms/ProfileEditor.ui \
    Forms/ProfileManager.ui

HEADERS += \
    Core/FrameCompare.hpp \
    Core/Game.hpp \
    Core/Generator/RaidGenerator.hpp \
    Core/Global.hpp \
    Core/Loader/DenLoader.hpp \
    Core/Loader/ProfileLoader.hpp \
    Core/RNG/XoroShiro.hpp \
    Core/Results/Den.hpp \
    Core/Results/Frame.hpp \
    Core/Results/Profile.hpp \
    Core/Results/Raid.hpp \
    Core/Util/IVChecker.hpp \
    Core/Util/Personal.hpp \
    Core/Util/Translator.hpp \
    Forms/Controls/CheckList.hpp \
    Forms/Controls/IVFilter.hpp \
    Forms/Controls/Label.hpp \
    Forms/Controls/TableView.hpp \
    Forms/Controls/TextBox.hpp \
    Forms/DenMap.hpp \
    Forms/IVCalculator.hpp \
    Forms/MainWindow.hpp \
    Forms/ProfileEditor.hpp \
    Forms/ProfileManager.hpp \
    Models/FrameModel.hpp \
    Models/ProfileModel.hpp \
    Models/TableModel.hpp

SOURCES += \
    Core/FrameCompare.cpp \
    Core/Generator/RaidGenerator.cpp \
    Core/Loader/DenLoader.cpp \
    Core/Loader/ProfileLoader.cpp \
    Core/RNG/XoroShiro.cpp \
    Core/Results/Den.cpp \
    Core/Results/Frame.cpp \
    Core/Results/Profile.cpp \
    Core/Results/Raid.cpp \
    Core/Util/IVChecker.cpp \
    Core/Util/Personal.cpp \
    Core/Util/Translator.cpp \
    Forms/Controls/CheckList.cpp \
    Forms/Controls/IVFilter.cpp \
    Forms/Controls/Label.cpp \
    Forms/Controls/TableView.cpp \
    Forms/Controls/TextBox.cpp \
    Forms/DenMap.cpp \
    Forms/IVCalculator.cpp \
    Forms/MainWindow.cpp \
    Forms/ProfileEditor.cpp \
    Forms/ProfileManager.cpp \
    Models/FrameModel.cpp \
    Models/ProfileModel.cpp \
    main.cpp
