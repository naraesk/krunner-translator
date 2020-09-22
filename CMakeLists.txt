cmake_minimum_required(VERSION 2.8.12)
project(RunnerTranslator)

find_package(ECM 5.12.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH
        ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH}
        )


# Find the required Libaries
find_package(Qt5 ${QT_MIN_VERSION} REQUIRED CONFIG COMPONENTS Widgets Core Network Quick QuickWidgets)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS I18n Service Runner TextWidgets ConfigWidgets PlasmaQuick)
find_package(Qt5Test REQUIRED)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

enable_testing(true)

set(krunner_translator_SRCS
        src/Translator.cpp
        src/translationEngines/impl/baidu.cpp
        src/translationEngines/impl/youdao.cpp
        src/translationEngines/impl/GoogleTranslate.cpp src/translationEngines/impl/GoogleTranslate.h src/TranslateShellProcess.cpp src/TranslateShellProcess.h src/translationEngines/impl/Bing.cpp src/translationEngines/impl/Bing.h
        src/language/LanguageRepository.cpp src/language/LanguageRepository.h
        src/language/Language.cpp src/language/Language.h src/language/SupportedLanguages.h)

set(kcm_krunner_translator_SRCS
        src/config/translator_config.cpp
        src/translationEngines/api/CommandLineEngine.h src/language/Language.cpp src/language/Language.h src/language/LanguageRepository.cpp src/language/LanguageRepository.h)

ki18n_wrap_ui(kcm_krunner_translator_SRCS src/config/translator_config.ui)
add_library(kcm_krunner_translator MODULE ${kcm_krunner_translator_SRCS})
target_link_libraries(kcm_krunner_translator
        Qt5::Core
        Qt5::Gui
        KF5::CoreAddons
        KF5::ConfigCore
        KF5::I18n
        KF5::ConfigWidgets
        KF5::Runner
        )

# Now make sure all files get to the right place
add_library(krunner_translator MODULE ${krunner_translator_SRCS})
target_link_libraries(krunner_translator KF5::Runner Qt5::Widgets Qt5::Network
        KF5::I18n
        KF5::Service
        KF5::ConfigWidgets
        KF5::Plasma)

add_dependencies(krunner_translator kcm_krunner_translator)

install(TARGETS krunner_translator kcm_krunner_translator DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-runner-translator.desktop src/config/plasma-runner-translator_config.desktop DESTINATION ${SERVICES_INSTALL_DIR})

# Testing

set(test_dependencies
        src/TranslateShellProcess.cpp
        src/language/LanguageRepository.cpp
        src/language/Language.cpp
        )
add_executable(translator_test test/TranslateShellProcessTest.cpp ${test_dependencies} )
add_test(NAME translator_test COMMAND translator_test )

target_link_libraries(translator_test PRIVATE Qt5::Test)
