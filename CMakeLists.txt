cmake_minimum_required(VERSION 2.8.12)
project(RunnerTranslator)

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set (CMAKE_MODULE_PATH
     ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH}
)

# Find the required Libaries
find_package (Qt5 REQUIRED CONFIG COMPONENTS Widgets Core Network)
find_package (KF5 REQUIRED COMPONENTS I18n Service Runner)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

set(krunner_translator_SRCS
    translator.cpp
    translatorjob.cpp
)

# Now make sure all files get to the right place
add_library(krunner_translator MODULE ${krunner_translator_SRCS})
target_link_libraries(krunner_translator KF5::Runner Qt5::Widgets Qt5::Network
                      KF5::I18n
                      KF5::Service)
install(TARGETS krunner_translator DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-runner-translator.desktop DESTINATION ${SERVICES_INSTALL_DIR})
