# SMRR - Folder for "Simulazioni Montecarlo di Rivelatori di Radiazione" course
To run any non Geant4 programm use the following command line "gdb -batch -ex "run" ./test1"

### WSL install (linux subsystem with Ubuntu distr)
Visit https://www.boot.dev/lessons/3d1c6ce3-bcc3-43a9-b2ae-809a968d524a
---

### CERN Geant4 install from source
```bash
$ git clone https://gitlab.cern.ch/geant4/geant4.git
$ mkdir geant4_install
$ mkdir geant4_build && cd root_build
$ cmake -DCMAKE_INSTALL_PREFIX=/home/ubuntu/SMRR/GEANT4/geant4_install \
      -DGEANT4_INSTALL_DATA=ON \
      -DGEANT4_USE_QT=ON \
      -DGEANT4_USE_OPENGL_X11=ON \  
      /home/ubuntu/SMRR/GEANT4/geant4
$ cmake --build . -- -j$(nproc)
$ cmake --install .
```
---
### CERN Root install from source
```bash
# The latest stable branch gets updated automatically on each release.
# You may update your local copy by issuing a `git pull` command from within `root_src/`.
$ git clone --branch latest-stable --depth=1 https://github.com/root-project/root.git root_src
$ mkdir root_build root_install && cd root_build
$ cmake -DCMAKE_INSTALL_PREFIX=../root_install ../root_src # && check cmake configuration output for warnings or errors
$ cmake --build . -- install -j4 # if you have 4 cores available for compilation
$ source ../root_install/bin/thisroot.sh # or thisroot.{fish,csh}
```
---
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
$ cmake -DCMAKE_PREFIX_PATH=/home/ricca/SMRR/GEANT4/geant4_install  /home/ricca/SMRR/GEANT4/geant4/examples/basic/B1
$ make -j$(nproc) VERBOSE=1
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
### How to execute
```bash
$ source /home/ricca/SMRR/GEANT4/geant4_install/bin/geant4.sh
$ ./example
```
You can make Geant4’s environment load automatically, so you don’t have to type "source /home/ricca/SMRR/GEANT4/geant4_install/bin/geant4.sh" every time.

1. Open .bashrc file
```bash
$ nano ~/.bashrc
```
2. Add this line at the end of the file "source /home/ricca/SMRR/GEANT4/geant4_install/bin/geant4.sh"
3. Save and exit (CTRL+O,ENTER,CTRL+X)
4. Reload it
```bash
$ source ~/.bashrc
```
5. Open new terminal and run
```bash
$ geant4-config --version
```
If it prints something sensible → ✅ done.

---

### To compile with ROOT
```bash
$ cmake -DCMAKE_PREFIX_PATH="/home/ubuntu/SMRR/GEANT4/geant4_install;/home/ubuntu/SMRR/ROOT/root_install" \
      /home/ubuntu/SMRR/GEANT4/geant4/examples/basic/B1
$ make -j$(nproc) VERBOSE=1
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
