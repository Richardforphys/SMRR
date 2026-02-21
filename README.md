# SMRR - Folder for "Simulazioni Montecarlo di Rivelatori di Radiazione" course
Lezioni --> Tutto ciò che non è legato all'esame \
   ├── TOF (Legato a lezioni su CAD) \
   ├── LaBr3 (Costruzione in classe rivelatore al bromuro di lantanio) \
**Esercizi --> Esercizi di Geant4 da svolgere per l'esame**\
   ├── Task1 \
   ├── Task2 \
   ├── Task3 \
   ├── Task4 \
   ├── Task6 \
   ├── Task7 (Facoltativo, da non valutare)\
**Garfield_tasks --> Esercizi di G++ da svolgere per l'esame** \
   ├── GEM_THICK \
   ├── Rx_Interaction 

## WSL install (linux subsystem with Ubuntu distr)
Visit https://www.boot.dev/lessons/3d1c6ce3-bcc3-43a9-b2ae-809a968d524a

## CERN Geant4 install from source
```bash
# Make sure xerces is installed
$ sudo apt-update
$ sudo apt install libxerces-c-dev
$ git clone https://gitlab.cern.ch/geant4/geant4.git
$ mkdir geant4_install
$ mkdir geant4_build && cd root_build
$ cmake \
  -DCMAKE_INSTALL_PREFIX=/home/ubuntu/SMRR/geant4_install \
  -DGEANT4_USE_GDML=ON \
  -DGEANT4_INSTALL_DATA=ON \
  -DGEANT4_USE_QT=ON \
  -DGEANT4_USE_OPENGL_X11=ON \
  /home/ubuntu/SMRR/geant4
$ cmake --build . -- -j$(nproc)
$ cmake --install .
```
---
## CERN CLHEP install from source
```bash
# The latest stable branch gets updated automatically on each release.
# You may update your local copy by issuing a `git pull` command from within `root_src/`.
$ git clone https://github.com/drbenmorgan/CLHEP.git
$ mkdir CLHEP_build CLHEP_install && cd CLHEP_build
$ cmake -DCMAKE_INSTALL_PREFIX=/home/ubuntu/SMRR/CLHEP_install /home/ubuntu/SMRR/CLHEP
$ make
$ make test
$ make install
```
---
## CERN Root install from source
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
## CERN Garfield++ install from source
```bash
# First install GSL
$ sudo apt update
$ sudo apt install libgsl-dev
# Verify installation
$ gsl-config --version
# Modify basrc file
$ nano ~/.bashrc
# Insert this at the end of the file
$ export GARFIELD_HOME=/home/mydir/garfield
$ git clone https://gitlab.cern.ch/garfield/garfieldpp.git $GARFIELD_HOME
$ cd $GARFIELD_HOME
$ mkdir build && cd build
$ cmake $GARFIELD_HOME
$ make
$ make install
# Modify basrc file again, inserting the following line at the eof
$ source $GARFIELD_HOME/install/share/Garfield/setupGarfield.sh
```
---
### How to build a GEANT4 programme
Follow this --> https://geant4.web.cern.ch/documentation/pipelines/master/bfad_html/ForApplicationDevelopers/GettingStarted/makeFile.html

```bash
$ mkdir example_build
$ cd example_build
$ cmake -DCMAKE_PREFIX_PATH=/home/ubuntu/SMRR/geant4_install  /home/ubuntu/SMRR/Exercises/task1/task1b
$ make -j$(nproc) VERBOSE=1
```

To solve ENSDATA.dat not found:

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

### To compile with ROOT and CLHEP
```bash
$ cmake -DCMAKE_PREFIX_PATH="/home/ubuntu/SMRR/geant4_install;/home/ubuntu/SMRR/ROOT/root_install;/home/ubuntu/SMRR/CLHEP_install" /home/ubuntu/SMRR/Exercises/task2/task2a
$ make -j$(nproc) VERBOSE=1
```