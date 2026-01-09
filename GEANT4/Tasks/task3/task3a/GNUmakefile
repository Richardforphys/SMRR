# $Id: GNUmakefile 42 2010-01-15 15:48:21Z vnivanch $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := task3a
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib bin

ifdef G4ANALYSIS_USE_ROOT
  CPPFLAGS += -DG4ANALYSIS_USE_ROOT
endif

include $(G4INSTALL)/config/architecture.gmk

#Add ROOT options for compilation
ifdef G4ANALYSIS_USE_ROOT
  CPPFLAGS += `root-config --cflags`
  LDFLAGS  += `root-config --libs`
endif

include $(G4INSTALL)/config/binmake.gmk

