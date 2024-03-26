# QT Documentation suggest 3.16
cmake_minimum_required(VERSION 3.16)

project(OpenGL_0 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-g)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)

if (COMMAND qt_standard_project_setup)
    qt_standard_project_setup()
else()
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
endif()

set(CMAKE_AUTORCC ON)

qt_add_executable(OpenGL_0 WIN32 MACOSX_BUNDLE
    mainwindow.cpp mainwindow.h
    mainview.cpp mainview.h
    userinput.cpp
    main.cpp
    resources.qrc
    vertex.h
    utility.cpp
    model.h model.cpp
    mainwindow.ui
    camera.h camera.cpp
    keyboardstatus.h keyboardstatus.cpp
    vectormath.h vectormath.cpp
    scene.h scene.cpp
    sceneobject.h sceneobject.cpp
    portalobject.h portalobject.cpp
    sceneobjectmanipulation.cpp
    texturedobject.h texturedobject.cpp
    ShaderType.h

)

target_include_directories(OpenGL_0 PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(OpenGL_0 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
)

# This is used for interoperability, do not remove even on linux;
# On linux, result is an executable;
# On Windows, result is a Win32 executable, instead of console executable, command prompt window is not created;
# On MacOS, result is executable bundle;
set_target_properties(OpenGL_0 PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
