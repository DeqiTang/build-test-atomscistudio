cmake_minimum_required(VERSION 3.20)
project(atomscistudio LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_VERSION_MAJOR 6)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Core OpenGL Gui 3DCore 3DRender 3DInput 3DExtras Concurrent REQUIRED)
#find_package(OpenGL REQUIRED)
#find_package(GLUT REQUIRED)
#include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )


include_directories(
    ${QT_QTOPENGL_INCLUDE_DIR} 
    ${OPENGL_INCLUDE_DIR} 
)

find_package(Boost COMPONENTS program_options filesystem system  REQUIRED)
include_directories( 
    ${Boost_INCLUDE_DIR}  
)

# libssh2
set(LIBSSH2_BUILD_TOOL "autotools" CACHE STRING "Specify the build tool used for the libssh2 installed in the system")
if (LIBSSH2_BUILD_TOOL STREQUAL "cmake")
    message("The specified build tool for libssh2 is: " "${LIBSSH2_BUILD_TOOL}")
    find_package(Libssh2 REQUIRED CONFIG)
    message("${LIBSSH2_ROOT}")
    include_directories(
        ${LIBSSH2_ROOT}/include
    )
    #include_directories(
    #    ${Libssh2_INCLUDE_DIR}
    #)
    link_libraries(${Libssh2_LIBRARIES})
    message("${Libssh2_LIBRARIES}")
    link_directories(
        ${LIBSSH2_ROOT}/lib
    )
    link_libraries("-lssh2")
elseif (LIBSSH2_BUILD_TOOL STREQUAL "autotools")
    message("The specified build tool for libssh2 is: " "${LIBSSH2_BUILD_TOOL}")
    find_package(PkgConfig)
    pkg_check_modules(LIBSSH2 libssh2)
    link_directories(
        ${LIBSSH2_LIBRARY_DIRS}
    )
    message("${LIBSSH2_LIBRARIES}")
    #add_compile_options("-lssh2")
    link_libraries("-lssh2")
    link_libraries(${LIBSSH2_LIBRARIES})
endif()



find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})
link_libraries(${ARMADILLO_LIBRARIES})

include_directories("./src")

find_package(atomsciflow REQUIRED)
include_directories(${ATOMSCIFLOW_INCLUDE_DIR})
link_libraries(${ATOMSCIFLOW_LIBRARIES})


file(GLOB PROJECT_SOURCES
    ./src/main.cpp

    ./src/main/*.h
    ./src/main/*.cpp

    ./src/modeling/*.h
    ./src/modeling/*.cpp

    ./src/calc/*.h
    ./src/calc/*.cpp
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(MACOSX TRUE)
        qt_add_executable(atomscistudio
            MACOSX_BUNDLE
            ${PROJECT_SOURCES}
        )
        set_target_properties(atomscistudio PROPERTIES
            BUNDLE True
        )
    else()
        qt_add_executable(atomscistudio
            ${PROJECT_SOURCES}
        )
    endif()        
else()
    if(ANDROID)
        add_library(atomscistudio SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(atomscistudio
            ${PROJECT_SOURCES}
        )
    endif()
endif()


if (LIBSSH2_BUILD_TOOL STREQUAL "cmake")
    target_link_libraries(atomscistudio
        PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
        PRIVATE Qt${QT_VERSION_MAJOR}::OpenGL
        PRIVATE Qt${QT_VERSION_MAJOR}::Gui
        PRIVATE Qt${QT_VERSION_MAJOR}::3DCore
        PRIVATE Qt${QT_VERSION_MAJOR}::3DRender
        PRIVATE Qt${QT_VERSION_MAJOR}::3DInput
        PRIVATE Qt${QT_VERSION_MAJOR}::3DExtras
        PRIVATE Qt${QT_VERSION_MAJOR}::Concurrent
        #${OPENGL_LIBRARIES}
        #${GLUT_LIBRARY}
        Libssh2::libssh2
        #-lssh2
    )
elseif (LIBSSH2_BUILD_TOOL STREQUAL "autotools")
    target_link_libraries(atomscistudio
        PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
        PRIVATE Qt${QT_VERSION_MAJOR}::OpenGL
        PRIVATE Qt${QT_VERSION_MAJOR}::Gui
        PRIVATE Qt${QT_VERSION_MAJOR}::3DCore
        PRIVATE Qt${QT_VERSION_MAJOR}::3DRender
        PRIVATE Qt${QT_VERSION_MAJOR}::3DInput
        PRIVATE Qt${QT_VERSION_MAJOR}::3DExtras
        PRIVATE Qt${QT_VERSION_MAJOR}::Concurrent
        #${OPENGL_LIBRARIES}
        #${GLUT_LIBRARY}
        ${LIBSSH2_LIBRARIES}
        #-lssh2
    )
endif()

# ------------------------------------------------
#                   set install
# ------------------------------------------------
install(
    TARGETS atomscistudio
    EXPORT atomscistudio
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION bin # important for building on macos with -DCMAKE_MACOSX_BUNDLE=True
)

