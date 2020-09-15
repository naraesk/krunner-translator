cmake_minimum_required(VERSION 2.8.12)
project(RunnerTranslator)

find_package(ECM 5.12.0 REQUIRED NO_MODULE)
set (CMAKE_MODULE_PATH
     ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH}
)


# Find the required Libaries
find_package (Qt5 ${QT_MIN_VERSION} REQUIRED CONFIG COMPONENTS Widgets Core Network Quick QuickWidgets)
find_package (KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS I18n Service Runner TextWidgets ConfigWidgets PlasmaQuick)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

set(krunner_translator_SRCS
    translator.cpp
        provider/baidu.cpp
        provider/youdao.cpp
        provider/GoogleTranslate.cpp provider/GoogleTranslate.h translateShellProcess.cpp translateShellProcess.h provider/Bing.cpp provider/Bing.h)

set(kcm_krunner_translator_SRCS
    config/translator_config.cpp
        api/CommandLineEngine.h)

ki18n_wrap_ui(kcm_krunner_translator_SRCS config/translator_config.ui)
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
install(FILES plasma-runner-translator.desktop config/plasma-runner-translator_config.desktop DESTINATION ${SERVICES_INSTALL_DIR})
