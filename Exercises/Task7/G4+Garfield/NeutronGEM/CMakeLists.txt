#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(NeutronGEM)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
option(G4ANALYSIS_USE "use ROOT" ON)
SET(CMAKE_MODULE_PATH /home/g4user/g4/geant4.10.00.p02/cmake/Modules)
set(G4NEUTRONHP_USE_ONLY_PHOTONEVAPORATION ON)
set(G4LIB_BUILD_GDML ON)
set(CMAKE_BUILD_TYPE "Debug")
 

find_package(ROOT)

if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()


add_definitions(-DG4VERBOSE -DG4UI_USE -DG4VIS_USE -DG4ANALYSIS_USE)
add_definitions(-DG4LIB_USE_GDML)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories($ENV{ROOTSYS}/include)
include_directories($ENV{GARFIELD_HOME}/Include)


link_directories($ENV{ROOTSYS}/lib)
link_directories($ENV{GARFIELD_HOME}/Library)
#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB headers $ENV{GARFIELD_HOME}/Include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(NeutronGEM NeutronGEM.cc ${sources} ${headers})

target_link_libraries(NeutronGEM ${Geant4_LIBRARIES} -lGarfield -lgfortran -lGeom -lGdml ${GDML_LIBRARIES} ${ROOT_LIBRARIES})


#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS NeutronGEM DESTINATION bin )
