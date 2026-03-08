cmake_minimum_required(VERSION 3.16)
project(RunnerTranslator)

find_package(ECM 6.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

set(QT_MIN_VERSION "6.6.0")
set(KF_MIN_VERSION "6.0.0")

# Find the required Libraries
find_package(Qt6 ${QT_MIN_VERSION} REQUIRED CONFIG COMPONENTS Widgets Core Network)
find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS I18n Runner ConfigWidgets KCMUtils)
find_package(Qt6Test REQUIRED)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

enable_testing(true)

set(krunner_translator_SRCS
        src/Translator.cpp
        src/translationEngines/impl/baidu.cpp
        src/translationEngines/impl/youdao.cpp
        src/translationEngines/impl/GoogleTranslate.cpp
        src/translationEngines/impl/Bing.cpp
        src/TranslateShellProcess.cpp
        src/language/LanguageRepository.cpp
        src/language/Language.cpp
        src/Parser.cpp
        src/TranslationQuery.cpp)

set(kcm_krunner_translator_SRCS
        src/config/translator_config.cpp
        src/language/Language.cpp
        src/language/LanguageRepository.cpp)

ki18n_wrap_ui(kcm_krunner_translator_SRCS src/config/translator_config.ui)

kcoreaddons_add_plugin(krunner_translator
        SOURCES ${krunner_translator_SRCS}
        INSTALL_NAMESPACE "kf6/krunner")
target_link_libraries(krunner_translator
        KF6::Runner
        KF6::I18n
        KF6::ConfigWidgets
        Qt6::Widgets
        Qt6::Network)

kcoreaddons_add_plugin(kcm_krunner_translator
        SOURCES ${kcm_krunner_translator_SRCS}
        INSTALL_NAMESPACE "kf6/krunner/kcms")
target_link_libraries(kcm_krunner_translator
        Qt6::Core
        Qt6::Gui
        KF6::CoreAddons
        KF6::ConfigCore
        KF6::I18n
        KF6::ConfigWidgets
        KF6::KCMUtils
        KF6::Runner)

# Testing

set(test_dependencies
        src/TranslateShellProcess.cpp
        src/TranslationQuery.cpp
        src/language/LanguageRepository.cpp
        src/language/Language.cpp
        )
add_executable(translator_test test/TranslateShellProcessTest.cpp ${test_dependencies})
add_test(NAME translator_test COMMAND translator_test)

target_link_libraries(translator_test PRIVATE Qt6::Test)
