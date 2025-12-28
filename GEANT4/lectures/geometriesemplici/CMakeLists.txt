cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(LaBr3)

# Options
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)

# ROOT setup
set(ROOT_DIR "/home/ubuntu/SMRR/ROOT/root_install")
find_package(ROOT REQUIRED COMPONENTS Core RIO)

# Geant4 setup
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${Geant4_INCLUDE_DIR}
    ${ROOT_INCLUDE_DIRS}
)

# Sources and headers
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Executable
add_executable(exampleLaBr3 LaBr3.cc ${sources} ${headers})

# Link libraries
target_link_libraries(exampleLaBr3 ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

# Copy scripts
set(EXAMPLELABR3_SCRIPTS vis.mac)
foreach(_script ${EXAMPLELABR3_SCRIPTS})
    configure_file(${PROJECT_SOURCE_DIR}/${_script} ${PROJECT_BINARY_DIR}/${_script} COPYONLY)
endforeach()

# Install target
install(TARGETS exampleLaBr3 DESTINATION bin)