  # $Id: CMakeLists.txt 68058 2013-03-13 14:47:43Z gcosmo $

  #----------------------------------------------------------------------------
  # Setup the project
  cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
  # Impone che la versione di CMake sia pari o superiore a 2.6
  # altrimenti CMake termina la compilazione con errore FATAL_ERROR.
  # Serve a garantire che tutte le istruzioni qui presenti siano supportate
  project(task2a)
  # Definisce il nome del progetto come "task1b"
  # Questo permette di inizializzare in automatico variabili successivamente utili
  # come PROJECT_NAME, PROJECT_SOURCE_DIR, PROJECT_BINARY_DIR

  option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
  if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
  else()
    find_package(Geant4 REQUIRED)
  endif()
  find_package(ROOT REQUIRED)
  find_package(CLHEP REQUIRED)

  include_directories(/usr/include)
  include_directories(${ROOT_INCLUDE_DIRS})
  # Definisce un'opzione configurabile dall'utente con valore di default "ON".
  # Se il valore della variabile "WITH_GEANT4_UIVIS" è ON, il comando garantisce la possibilità
  # di utilizzare la User Interface e i Visual Attributes, caricando i driver appositi.
  # Viene inoltre richiesto di utilizzare un'installazione valida di Geant4.
  # Altrimenti si utilizza esclusivamente il core di Geant4, utile per simulazioni in modalità batch.

  #----------------------------------------------------------------------------
  include(${Geant4_USE_FILE})
  # Include un CMake file fornito da Geant4, il quale
  #   1. Aggiunge le directory di include
  #   2. Definisce le macro di compilazione
  # Include directories
  include_directories(
      ${PROJECT_SOURCE_DIR}/include
      ${Geant4_INCLUDE_DIR}
      )
  # Aggiunge la directory ${PROJECT_SOURCE_DIR}/include al path degli header.

  #----------------------------------------------------------------------------
  file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
  # Aggiunge tutti i file di formato .cc che si trovano all'interno
  # della directory ${PROJECT_SOURCE_DIR}/src.
  file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
  # Aggiunge tutti i file di formato .hh che si trovano all'interno
  # della directory ${PROJECT_SOURCE_DIR}/include.

  #----------------------------------------------------------------------------
  add_executable(exampletask2a task2a.cc ${sources} ${headers})
  # Crea un eseguibile di nome "exampletask1b" usando come sorgenti
  # task1b.cc e tutti i file .cc e .hh precedentemente aggiunti.

  target_link_libraries(exampletask2a
    PRIVATE
        ${Geant4_LIBRARIES}
        ${ROOT_LIBRARIES}
        CLHEP::CLHEP
  )
  #Collega l'eseguibile alle librerie di Geant4

  #----------------------------------------------------------------------------
  set(EXAMPLEtask2a_SCRIPTS
    vis.mac
    )
  # Stila un elenco di file macro di Geant4, necessari per la visualizzazione dei
  # risultati delle simulaizoni e per il run delle stesse.

  foreach(_script ${EXAMPLEtask2a_SCRIPTS})
    configure_file(
      ${PROJECT_SOURCE_DIR}/${_script}
      ${PROJECT_BINARY_DIR}/${_script}
      COPYONLY
      )
  endforeach()
  # Copia ogni script all'interno di ${EXAMPLEtask1b_SCRIPTS}, all'interno della 
  # directory di build dalla directory sorgente.

  #----------------------------------------------------------------------------
  add_custom_target(task2a DEPENDS exampletask2a)

  #----------------------------------------------------------------------------
  # Installa l'eseguibile all'interno della cartella 'bin' situata all'interno 
  # del path CMAKE_INSTALL_PREFIX
  install(TARGETS exampletask2a DESTINATION bin)