#$id: CMakeLists.txt

#------------------------------------------------------------------------------------------------
# Set up the project

cmake_minimum_required(VERSION 2.6 FATAL ERROR) #compatibilità dela versione di CMake

project(LaBr3) # nome del progetto, *1
#------------------------------------------------------------------------------------------------
#Fid Genat4 package; activate all available UI and VIS drivers by default

#You can set WITH_GEANT4_UIVIS to OFF via command line or cmake/cmake-gui to build a batch mode only executable
#if WITH_GEANT4_UIVIS  is ON then once the simulatio is executed it will visualizze all partcle tracks, detector and so on
option(WITH_GEANT4_UIVIS "Build example with GEant4 UI and Vis drivers ON")
option(G4ANALYSES_USE "use_root" ON) #Use ROOT for analyses purpouses
if (WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all visall)
else()
    find_package(Geant4 REQUIRED) # Geant4 package is the miimum required for CMake to make the executable
endif()

set(G4NEUTRONHP_USE_ONLY_PHOTONEVAPORATION ON) #settig the evironment variable "G4NEUTRONHP_USE_ONLY_PHOTONEVAPORATION" to ON

#Find ROOT as required package
set(ROOT_DIR "/home/ubuntu/SMRR/ROOT/root_install") #telling cmake where to find root
find_package(ROOT REQUIRED)

#------------------------------------------------------------------------------------------------
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})

#------------------------------------------------------------------------------------------------
#Locate sources and headers for this project
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)  #definito qui *1, cerca tutti i file all'interno della project_source_dir che sono nella cartella src e di formato .cc
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh) #cerca tutti i file all'interno della project_source_dir che sono nella cartella include e di formato .hh

#------------------------------------------------------------------------------------------------
# Add executable and link it to the Geant4 ad ROOT libs
# "exampleLaBr3" è il ome che avrà l'eseguibile una volta lanciato il comando "make"
add_executable(exampleLaBr3 LaBr3.cc ${sources} ${headers})
target_link_libraries(exampleLaBr3 ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

