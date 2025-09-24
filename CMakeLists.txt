cmake_minimum_required(VERSION 3.16.0)
project(RunnerTranslator)

set(QT_MIN_VERSION 6.5.0)
set(KF6_MIN_VERSION 6.0.0)
find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH})

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Gui Widgets Network)
find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS Runner I18n CoreAddons Config ConfigWidgets KCMUtils)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

set(krunner_translator_SRCS
    src/translator.cpp
    src/provider/baidu.cpp
    src/provider/youdao.cpp
    src/provider/GoogleTranslate.cpp src/provider/GoogleTranslate.h src/translateShellProcess.cpp src/translateShellProcess.h src/provider/Bing.cpp src/provider/Bing.h
    src/LanguageRepository.cpp src/LanguageRepository.h
    src/languages.cpp src/languages.h src/SupportedLanguages.h
)
set(kcm_krunner_translator_SRCS
    src/config/translator_config.cpp
    src/api/CommandLineEngine.h src/languages.cpp src/languages.h src/LanguageRepository.cpp src/LanguageRepository.h
)

ki18n_wrap_ui(kcm_krunner_translator_SRCS src/config/translator_config.ui)
kcoreaddons_add_plugin(kcm_krunner_translator SOURCES ${kcm_krunner_translator_SRCS} INSTALL_NAMESPACE "kf6/krunner/kcms")
target_link_libraries(kcm_krunner_translator
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    KF6::Runner
    KF6::I18n
    KF6::CoreAddons
    KF6::ConfigCore
    KF6::ConfigWidgets
    KF6::KCMUtils
)

kcoreaddons_add_plugin(krunner_translator SOURCES ${krunner_translator_SRCS} INSTALL_NAMESPACE "kf6/krunner")
target_link_libraries(krunner_translator
    Qt6::Core
    Qt6::Network
    KF6::Runner
    KF6::I18n
    KF6::CoreAddons
    KF6::ConfigCore
    KF6::KCMUtils
)
