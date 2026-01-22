$id: CMakeLists.txt

#-------------------------------------------------------------------------------------------------------------------
# Setup the project

cmake_minimum_required(VERSION 2.6 FATAL ERROR)
project(LaBr3) # questa è ${PROJECT_SOURCE_DIR}

#-------------------------------------------------------------------------------------------------------------------
# Find geant4 package, activate all available UI and Vis drivers by default.
# You can set  WITH_GEANT4_UIVIS to OFF via the command line or cmake/cmake-gui to build a batch mode only executable.

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
option(G4ANALYSIS_USE "use root" ON)

if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()

set(G4NEUTRONHP_USE_ONLY_PHOTONEVAPORATION ON)

# Find ROOT as required package
find_package(ROOT REQUIRED)
#-------------------------------------------------------------------------------------------------------------------

# Sto includendo le directories di cui il compilatore deve tener conto quando runno il make file
# PROJECT_SOURCE_DIR, dove si trova il progetto
# GEANT4_INCLUDE_DIR, dove si trova installazione di G4
# ROOT_INCLUDE_DIR, dove si trova ROOT

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${GEANT4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})

#-------------------------------------------------------------------------------------------------------------------
# Locate sources and headers for this project

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#-------------------------------------------------------------------------------------------------------------------
# Add the executable and link it to the Geant4 and ROOT libraries

add_executable(exampleLaBr3 LaBr3.cc ${sources} ${headers})
target_link_libraries(exampleLaBr3 ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#-------------------------------------------------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we build LaBr3. This is so that we can run 
# the exacutable directly because it relies on the scripts being in the current working directory.

set(EXAMPLELABR3_SCRIPTS
    vis.mac
    visQT.mac
        )

foreach(_script ${EXAMPLELABR3_SCRIPTS})
    configure_file(
        ${PROFECT_SOURCE_DIR}/${_script}
        COPYONLY
    )
endforeach()

#-------------------------------------------------------------------------------------------------------------------
# install the eecutable to the CMAKE_INSTALL_PREFIX directory
install(TARGETS exampleLaBr3 DESTINATION)