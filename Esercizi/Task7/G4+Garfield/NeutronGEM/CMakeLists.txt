#-------------------------------------------------------------------------------
# Setup the project
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(NeutronGEM)

#-------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
option(G4ANALYSIS_USE "Use ROOT for analysis" ON)

set(CMAKE_BUILD_TYPE "Debug")  # or Release

#-------------------------------------------------------------------------------
# Geant4
#-------------------------------------------------------------------------------
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})

#-------------------------------------------------------------------------------
# ROOT
#-------------------------------------------------------------------------------
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARY_DIR})

#-------------------------------------------------------------------------------
# Garfield++
#-------------------------------------------------------------------------------
if(NOT DEFINED ENV{GARFIELD_HOME})
    message(FATAL_ERROR "Please set GARFIELD_HOME environment variable")
endif()

set(GARFIELD_INCLUDE_DIR $ENV{GARFIELD_HOME}/Include)
set(GARFIELD_LIBRARY_DIR $ENV{GARFIELD_HOME}/Library)
set(GARFIELD_LIB Garfield)   # nome senza 'lib' e senza estensione

include_directories(${GARFIELD_INCLUDE_DIR})
link_directories(${GARFIELD_LIBRARY_DIR})

#-------------------------------------------------------------------------------
# Project include directories
#-------------------------------------------------------------------------------
include_directories(${PROJECT_SOURCE_DIR}/include)

#-------------------------------------------------------------------------------
# Sources and headers
#-------------------------------------------------------------------------------
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/include/*.hh)

#-------------------------------------------------------------------------------
# Executable
#-------------------------------------------------------------------------------
add_executable(NeutronGEM NeutronGEM.cc ${SOURCES} ${HEADERS})

#-------------------------------------------------------------------------------
# Link libraries
#-------------------------------------------------------------------------------
target_link_libraries(NeutronGEM
    ${Geant4_LIBRARIES}
    ${ROOT_LIBRARIES}
    ${GARFIELD_LIB}
    ${GDML_LIBRARIES}    # se usi GDML
    -lgfortran           # solo se necessario
)

#-------------------------------------------------------------------------------
# Install
#-------------------------------------------------------------------------------
install(TARGETS NeutronGEM DESTINATION bin)