# $Id: GNUmakefile 42 2010-01-15 15:48:21Z vnivanch $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := task4c
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/architecture.gmk

#Add ROOT options for compilation
#CPPFLAGS += `root-config --cflags`
#LDFLAGS  += `root-config --libs`

include $(G4INSTALL)/config/binmake.gmk

