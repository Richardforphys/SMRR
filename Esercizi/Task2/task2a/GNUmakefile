# $Id: GNUmakefile 285 2010-02-14 23:33:24Z selles $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := task2a
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/architecture.gmk

#Add ROOT options for compilation
CPPFLAGS += `root-config --cflags`
LDFLAGS  += `root-config --libs`

include $(G4INSTALL)/config/binmake.gmk

