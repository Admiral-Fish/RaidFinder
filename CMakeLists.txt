cmake_minimum_required(VERSION 3.16.0)

project(RaidFinder VERSION 2.7.1 LANGUAGES CXX)
add_compile_definitions(VERSION="2.7.1")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include_directories(Externals/nlohmann)

include(EmbedTranslations)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core LinguistTools Network Widgets REQUIRED)

set(PROJECT_TS
    i18n/RaidFinder_de.ts
    i18n/RaidFinder_es.ts
    i18n/RaidFinder_fr.ts
    i18n/RaidFinder_it.ts
    i18n/RaidFinder_ja.ts
    i18n/RaidFinder_ko.ts
    i18n/RaidFinder_pt.ts
    i18n/RaidFinder_tw.ts
    i18n/RaidFinder_zh.ts
)
	
set(PROJECT_QRC
    Externals/QDarkStyleSheet/qdarkstyle/dark/style.qrc
    resources.qrc
)

set(PROJECT_UI
    Forms/Controls/IVFilter.ui
    Forms/MainWindow.ui
    Forms/Profile/ProfileEditor.ui
    Forms/Profile/ProfileManager.ui
    Forms/Tools/DenMap.ui
    Forms/Tools/EncounterLookup.ui
    Forms/Tools/IVCalculator.ui
    Forms/Tools/Settings.ui
    Forms/Tools/Bots.ui
)

set(PROJECT_SOURCES
    Core/Generator/RaidGenerator.cpp
    Core/Loader/DenLoader.cpp
    Core/Loader/PersonalLoader.cpp
    Core/Loader/ProfileLoader.cpp
    Core/Results/Den.cpp
    Core/Results/PersonalInfo.cpp
    Core/Results/Profile.cpp
    Core/Results/Raid.cpp
    Core/StateFilter.cpp
    Core/Util/IVChecker.cpp
    Core/Util/Nature.cpp
    Core/Util/Translator.cpp
    Core/Bots/BotCore.cpp
    Core/Bots/SWSHBot.cpp
    Core/Bots/Workers/BotWorker.cpp
    Core/Bots/RaidBot.cpp
    Forms/Controls/CheckList.cpp
    Forms/Controls/IVFilter.cpp
    Forms/Controls/Label.cpp
    Forms/Controls/TableView.cpp
    Forms/Controls/TextBox.cpp
    Forms/MainWindow.cpp
    Forms/Profile/ProfileEditor.cpp
    Forms/Profile/ProfileManager.cpp
    Forms/Tools/DenMap.cpp
    Forms/Tools/EncounterLookup.cpp
    Forms/Tools/IVCalculator.cpp
    Forms/Tools/Settings.cpp
    Forms/Tools/Bots.cpp
    Models/ProfileModel.cpp
    Models/StateModel.cpp
    main.cpp
)

set(FLATBUFFERS_SRC_DIR
    flatbuffers/)

qt5_add_translation(PROJECT_QM ${PROJECT_TS} OPTIONS -nounfinished -removeidentical)
embed_translations(PROJECT_I18N ${PROJECT_QM})

add_subdirectory(${FLATBUFFERS_SRC_DIR}
                 ${CMAKE_CURRENT_BINARY_DIR}/flatbuffers-build
                 EXCLUDE_FROM_ALL)

if (WIN32)
    add_executable(RaidFinder
        WIN32
        ${PROJECT_I18N}
        ${PROJECT_QRC}
        ${PROJECT_UI}
        ${PROJECT_SOURCES}
        appicon.rc
    )
elseif (APPLE)
    set(MACOSX_BUNDLE_ICON_FILE raidfinder.icns)
    set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Images/raidfinder.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    add_executable(RaidFinder
        MACOSX_BUNDLE
        ${PROJECT_I18N}
        ${PROJECT_QRC}
        ${PROJECT_UI}
        ${PROJECT_SOURCES}
        ${APP_ICON_MACOSX}
    )
else ()
    add_executable(RaidFinder
        ${PROJECT_I18N}
        ${PROJECT_QRC}
        ${PROJECT_UI}
        ${PROJECT_SOURCES}
    )
endif ()

target_link_libraries(RaidFinder PRIVATE Qt5::Core Qt5::Network Qt5::Widgets flatbuffers)
