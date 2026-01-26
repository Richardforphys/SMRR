$id: CMakeLists.txt

#-----------------------------------------------------------------
# Setup the project

cmake_minimun_required (VERSION 2.6 FATAL ERROR) # Minimum CMake version required
project(LaBr3)  # PROGECT_SOURCE_DIR = path to LaBr3

#-----------------------------------------------------------------
# Find Geant4 package, activate all available UI and Vis drivers by defalut
# You can set WITH_GEANT4_UIVIS to OFF via the command line or cmake/cmake-gui to build a batch mode only executable

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
option(G4ANALYSIS_USE "use root" ON)

if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else() 
    find_package(Geant4 REQUIRED)
endif()

set(G4NEUTRONHP_USE_ONLY_PHOTONEVAPORATION ON)

#-----------------------------------------------------------------
# Find root as a required package
find_package(ROOT REQUIRED)

#-----------------------------------------------------------------
# Find Geant4 package, activate all available UI and Vis drivers by defalut

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${GEANT4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})


#-----------------------------------------------------------------
# Locate sources and headers for this project

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#-----------------------------------------------------------------
# Add the executable, and link it to the Geant4 and Root libraries

add_executable(exampleLaBr3 LaBr3.cc ${sources} ${headers})
target_link_libraries(exampleLaBr3 ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#-----------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build the executable. This is so that we can run the executable directly
# because it relies on the scripts being in the current working directory.

set(EXAMPLELABR3_SCRIPTS 
    vis.mac
    visQT.mac
    )

foreach(_script ${EXAMPLELABR3_SCRIPTS})
    configure_file(${PROJECT_SOURCE_DIR}/${_script} 
    COPYONLY
    ) 
endforeach()

#-----------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX

install(TARGETS exampleLaBr3 DESTINATION bin)
