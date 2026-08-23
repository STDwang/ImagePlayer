cmake_minimum_required(VERSION 3.16)

project(ImagePlayer
    VERSION 1.0.0
    DESCRIPTION "Qt Quick image viewer"
    LANGUAGES CXX
)

# Project options
option(IMAGEPLAYER_ENABLE_DEPLOY
    "Create a deploy target that copies the Qt runtime next to the executable"
    ON
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Dependencies
find_package(Qt5 5.9 REQUIRED COMPONENTS
    Core Gui PrintSupport Qml Quick Svg Widgets
)

# The configured Qt 5.9.7 msvc2015 kit is 32-bit. Stop during configuration
# with a useful message instead of producing opaque x86/x64 linker errors.
if(WIN32
   AND CMAKE_SIZEOF_VOID_P EQUAL 8
   AND CMAKE_PREFIX_PATH MATCHES "[/\\\\]msvc2015[/\\\\]?$"
)
    message(FATAL_ERROR
        "The selected compiler Kit is amd64, but Qt at '${CMAKE_PREFIX_PATH}' "
        "is 32-bit. In the VS Code status bar, select an MSVC x86 Kit, then "
        "run 'CMake: Delete Cache and Reconfigure'."
    )
endif()

# Source discovery. CONFIGURE_DEPENDS makes CMake regenerate the build system
# when code files are added to or removed from src/ and t2d/.
file(GLOB_RECURSE PROJECT_CODE CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/t2d/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/t2d/*.h"
)

list(APPEND PROJECT_CODE "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

set(APP_RESOURCES qml.qrc t2d/t2d_res.qrc)
if(WIN32)
    list(APPEND APP_RESOURCES logo.rc)
endif()

# Application target
add_executable(ImagePlayer WIN32
    ${PROJECT_CODE}
    ${APP_RESOURCES}
)

target_include_directories(ImagePlayer PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/t2d"
)

target_compile_definitions(ImagePlayer PRIVATE QT_DEPRECATED_WARNINGS)

if(MSVC)
    target_compile_options(ImagePlayer PRIVATE /utf-8)
endif()

target_link_libraries(ImagePlayer PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::PrintSupport
    Qt5::Qml
    Qt5::Quick
    Qt5::Svg
    Qt5::Widgets
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
    ${PROJECT_CODE}
    ${APP_RESOURCES}
)

# Windows deployment. Building ImagePlayer only compiles the application;
# building the separate deploy target copies Qt DLLs, plugins and QML modules.
if(WIN32 AND IMAGEPLAYER_ENABLE_DEPLOY AND TARGET Qt5::qmake)
    get_target_property(QT_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE
        NAMES windeployqt
        HINTS "${QT_BIN_DIR}"
        NO_DEFAULT_PATH
    )

    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_target(deploy
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                --qmldir "${CMAKE_CURRENT_SOURCE_DIR}"
                "$<TARGET_FILE:ImagePlayer>"
            DEPENDS ImagePlayer
            COMMENT "Deploying Qt runtime dependencies"
            VERBATIM
        )
    else()
        message(WARNING "windeployqt was not found; deploy target is unavailable")
    endif()
endif()
