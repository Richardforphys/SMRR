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
### How to build
Follow this --> https://geant4.web.cern.ch/documentation/pipelines/master/bfad_html/ForApplicationDevelopers/GettingStarted/makeFile.html

```bash
$ mkdir example_build
$ cd example_build
$ cmake -DCMAKE_PREFIX_PATH=/home/you/geant4-install /path/to/example
$ make -j4 VERBOSE=1
```
\\
---
### LaBr3
Folder for Bromuro di Lantanio 