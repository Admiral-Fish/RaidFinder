lessThan(QT_MAJOR_VERSION, 5): error("You need at least Qt 5.9 to build RaidFinder")
equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 9): error("You need at least Qt 5.9 to build RaidFinder")

QT += concurrent widgets
CONFIG += c++1z lrelease embed_translations

TARGET = RaidFinder
TEMPLATE = app
VERSION = 2.0.0

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
    i18n/RaidFinder_zh.ts \
    i18n/RaidFinder_tw.ts

FORMS += \
    Forms/Controls/IVFilter.ui \
    Forms/MainWindow.ui \
    Forms/Profile/ProfileEditor.ui \
    Forms/Profile/ProfileManager.ui \
    Forms/Tools/DenMap.ui \
    Forms/Tools/EncounterLookup.ui \
    Forms/Tools/IVCalculator.ui \
    Forms/Tools/SeedCalculator.ui \
    Forms/Util/RaidInfo12.ui \
    Forms/Util/RaidInfo35.ui

HEADERS += \
    Core/FrameFilter.hpp \
    Core/Generator/RaidGenerator.hpp \
    Core/Loader/DenLoader.hpp \
    Core/Loader/PersonalLoader.hpp \
    Core/Loader/ProfileLoader.hpp \
    Core/RNG/XoroShiro.hpp \
    Core/Results/Den.hpp \
    Core/Results/Frame.hpp \
    Core/Results/PersonalInfo.hpp \
    Core/Results/Pokemon.hpp \
    Core/Results/Profile.hpp \
    Core/Results/Raid.hpp \
    Core/Searcher/Matrix.hpp \
    Core/Searcher/SeedSearcher.hpp \
    Core/Searcher/SeedSearcher12.hpp \
    Core/Searcher/SeedSearcher35.hpp \
    Core/Util/Game.hpp \
    Core/Util/Global.hpp \
    Core/Util/IVChecker.hpp \
    Core/Util/Translator.hpp \
    Forms/Controls/CheckList.hpp \
    Forms/Controls/IVFilter.hpp \
    Forms/Controls/Label.hpp \
    Forms/Controls/TableView.hpp \
    Forms/Controls/TextBox.hpp \
    Forms/MainWindow.hpp \
    Forms/Profile/ProfileEditor.hpp \
    Forms/Profile/ProfileManager.hpp \
    Forms/Tools/DenMap.hpp \
    Forms/Tools/EncounterLookup.hpp \
    Forms/Tools/IVCalculator.hpp \
    Forms/Tools/SeedCalculator.hpp \
    Forms/Util/RaidInfo12.hpp \
    Forms/Util/RaidInfo35.hpp \
    Models/FrameModel.hpp \
    Models/ProfileModel.hpp \
    Models/TableModel.hpp

SOURCES += \
    Core/FrameFilter.cpp \
    Core/Generator/RaidGenerator.cpp \
    Core/Loader/DenLoader.cpp \
    Core/Loader/PersonalLoader.cpp \
    Core/Loader/ProfileLoader.cpp \
    Core/RNG/XoroShiro.cpp \
    Core/Results/Den.cpp \
    Core/Results/Frame.cpp \
    Core/Results/PersonalInfo.cpp \
    Core/Results/Pokemon.cpp \
    Core/Results/Profile.cpp \
    Core/Results/Raid.cpp \
    Core/Searcher/Matrix.cpp \
    Core/Searcher/SeedSearcher.cpp \
    Core/Searcher/SeedSearcher12.cpp \
    Core/Searcher/SeedSearcher35.cpp \
    Core/Util/IVChecker.cpp \
    Core/Util/Translator.cpp \
    Forms/Controls/CheckList.cpp \
    Forms/Controls/IVFilter.cpp \
    Forms/Controls/Label.cpp \
    Forms/Controls/TableView.cpp \
    Forms/Controls/TextBox.cpp \
    Forms/MainWindow.cpp \
    Forms/Profile/ProfileEditor.cpp \
    Forms/Profile/ProfileManager.cpp \
    Forms/Tools/DenMap.cpp \
    Forms/Tools/EncounterLookup.cpp \
    Forms/Tools/IVCalculator.cpp \
    Forms/Tools/SeedCalculator.cpp \
    Forms/Util/RaidInfo12.cpp \
    Forms/Util/RaidInfo35.cpp \
    Models/FrameModel.cpp \
    Models/ProfileModel.cpp \
    main.cpp
