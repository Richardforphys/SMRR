# SMRR - Folder for "Simulazioni Montecarlo di Rivelatori di Radiazione" course
To run any programm use the following command line "gdb -batch -ex "run" ./test1"

### Folder Structure
TaskNmr
* TaskNmr --> prima sottocartella, contiene i file principali
    * include --> contains all .hh files
    * src     --> contains all .cc files
    * CMakeList.txt --> il programma CMake serve per fare creare l'eseguibile del programma.\
    Per dirgli come farlo serve questo file di testo, che si ottiene tramite il build.
* TaskNmr_build --> cartella necessaria per creare l'eseguibile 
\\
---
### How to build a GEANT4 programme
Follow this --> https://geant4.web.cern.ch/documentation/pipelines/master/bfad_html/ForApplicationDevelopers/GettingStarted/makeFile.html

```bash
$ mkdir example_build
$ cd example_build
$ cmake -DCMAKE_PREFIX_PATH=/home/you/geant4-install /path/to/example
$ make -j4 VERBOSE=1
```
Nel caso in cui in fase di compilazione dovesse apparire il fatal error "ENSDATA.dat not found", 
Comandi per risolvere ENSDATA.dat not found:

```bash
$ export G4ENSDFSTATEDATA=/home/ubuntu/SMRR/GEANT4/geant4_install/share/Geant4/data/G4ENSDFSTATE3.0
$ export G4PHOTONEvaporationDATA=/home/ubuntu/SMRR/GEANT4/geant4_install/share/Geant4/data/PhotonEvaporation6.1.2
$ export G4RADIOACTIVEDATA=/home/ubuntu/SMRR/GEANT4/geant4_install/share/Geant4/data/RadioactiveDecay6.1.2
$ export G4NEUTRONHPDATA=/home/ubuntu/SMRR/GEANT4/geant4_install/share/Geant4/data/G4NDL4.7.1
$ export G4SAIDXSDATA=/home/ubuntu/SMRR/GEANT4/geant4_install/share/Geant4/data/G4SAIDDATA2.0
$ export G4LEDATA=/home/ubuntu/SMRR/GEANT4/geant4_install/share/Geant4/data/G4EMLOW8.8
```
---

### To compile with ROOT
```bash
$ cmake -DCMAKE_PREFIX_PATH="/home/ubuntu/SMRR/GEANT4/geant4_install;/home/ubuntu/SMRR/ROOT/root_install" \
      /home/ubuntu/SMRR/GEANT4/LABR3/LaBr3
$ make
```

---
### CMakeLists.txt minimal content
```bash
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
```