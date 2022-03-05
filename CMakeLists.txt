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
link_libraries(${Boost_LIBRARIES})

find_package(PkgConfig)
set(YAML_CPP_PKGCONFIG_DIR "UNSET" CACHE STRING "Specify the dir path which contains the yaml-cpp.pc file")
if (NOT ${YAML_CPP_PKGCONFIG_DIR} STREQUAL "UNSET")
    if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
        set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH};${YAML_CPP_PKGCONFIG_DIR}") # separated by semicolon
    else()
        set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${YAML_CPP_PKGCONFIG_DIR}") # separated by colon
    endif()
    message(STATUS "PKG_CONFIG_PATH: $ENV{PKG_CONFIG_PATH}")
endif()    
pkg_check_modules(YAML_CPP yaml-cpp)
include_directories(${YAML_CPP_INCLUDE_DIRS})
link_directories(${YAML_CPP_LIBRARY_DIRS})
link_libraries(${YAML_CPP_LIBRARIES})
message("YAML_CPP_INCLUDE_DIRS: ${YAML_CPP_INCLUDE_DIRS}")
message("YAML_CPP_LIBRARY_DIRS: ${YAML_CPP_LIBRARY_DIRS}")
message("YAML_CPP_LIBRARIES: ${YAML_CPP_LIBRARIES}")

find_package(OpenSSL REQUIRED)
if (OPENSSL_FOUND)
    message("Found OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
    message("Found OpenSSL SSL library: ${OPENSSL_SSL_LIBRARY}")
    message("Found OpenSSL Crypto library: ${OPENSSL_CRYPTO_LIBRARY}")
    link_libraries(OpenSSL::SSL)
    link_libraries(OpenSSL::Crypto)
endif()

# libssh2
find_package(Libssh2 CONFIG)
if (Libssh2_FOUND)
    set(LIBSSH2_FOUND_LIBS Libssh2::libssh2)
else ()
    find_package(PkgConfig)
    pkg_check_modules(LIBSSH2 libssh2)
    link_directories(${LIBSSH2_LIBRARY_DIRS})
    message("Found libssh2 with PkgConfig")
    message("Found LIBSSH2_LIBRARY_DIRS: ${LIBSSH2_LIBRARY_DIRS}")
    message("Found LIBSSH2_LIBRARIES: ${LIBSSH2_LIBRARIES}")
    set(LIBSSH2_FOUND_LIBS ${LIBSSH2_LIBRARIES})
endif()

if (NOT DEFINED LIBSSH2_FOUND_LIBS)
    message(STATUS "libssh2 not found, neither with cmake, nor PkgConfig")
    message(FATAL_ERROR "Stop cmake due to failure in finding libssh2") # FATAL_ERROR will stop cmake
endif()

find_package(OpenMP)
if (OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
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

    ./src/config/*.h
    ./src/config/*.cpp
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    #if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
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
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    ${LIBSSH2_FOUND_LIBS}
)

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

