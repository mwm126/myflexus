# DO-NOT-REMOVE begin-copyright-block 
#
# Redistributions of any form whatsoever must retain and/or include the
# following acknowledgment, notices and disclaimer:
#
# This product includes software developed by Carnegie Mellon University.
#
# Copyright 2012 by Mohammad Alisafaee, Eric Chung, Michael Ferdman, Brian 
# Gold, Jangwoo Kim, Pejman Lotfi-Kamran, Onur Kocberber, Djordje Jevdjic, 
# Jared Smolens, Stephen Somogyi, Evangelos Vlachos, Stavros Volos, Jason 
# Zebchuk, Babak Falsafi, Nikos Hardavellas and Tom Wenisch for the SimFlex 
# Project, Computer Architecture Lab at Carnegie Mellon, Carnegie Mellon University.
#
# For more information, see the SimFlex project website at:
#   http://www.ece.cmu.edu/~simflex
#
# You may not use the name "Carnegie Mellon University" or derivations
# thereof to endorse or promote products derived from this software.
#
# If you modify the software you must place a notice on or within any
# modified version provided or made available to any third party stating
# that you have modified the software.  The notice shall include at least
# your name, address, phone number, email address and the date and purpose
# of the modification.
#
# THE SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY WARRANTY OF ANY KIND, EITHER
# EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO ANY WARRANTY
# THAT THE SOFTWARE WILL CONFORM TO SPECIFICATIONS OR BE ERROR-FREE AND ANY
# IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
# TITLE, OR NON-INFRINGEMENT.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
# BE LIABLE FOR ANY DAMAGES, INCLUDING BUT NOT LIMITED TO DIRECT, INDIRECT,
# SPECIAL OR CONSEQUENTIAL DAMAGES, ARISING OUT OF, RESULTING FROM, OR IN
# ANY WAY CONNECTED WITH THIS SOFTWARE (WHETHER OR NOT BASED UPON WARRANTY,
# CONTRACT, TORT OR OTHERWISE).
#
# DO-NOT-REMOVE end-copyright-block   

PROGNAME = filter

CC = gcc
EXT = .c

#add the required paths, e.g.
LIBDIRS = ../netpbm/lib
INCDIRS = ../netpbm/lib ../netpbm
CFLAGS = -g3 -O3 -Wall
CPPFLAGS = -D_GNU_SOURCE
LDFLAGS = ../netpbm/lib/libnetpbm.a
LIBS = pthread

##########################################################

CXX = $(CC)
CXXFLAGS = $(CFLAGS)

LDFLAGS += $(addprefix -L,$(LIBDIRS)) 
CPPFLAGS += $(addprefix -I,$(INCDIRS))

.PHONY: all strip clean depends docs pdf epsfigs

sources := $(subst ./,,$(shell find . -name \*$(EXT)))
deps := $(addprefix .deps/,$(sources:$(EXT)=.d))
docsources := $(wildcard *$(TEX))
figs := $(patsubst %.fig,%.eps,$(wildcard *.fig))
objects := $(sources:$(EXT)=.o)

all: $(deps) $(PROGNAME) docs

$(PROGNAME): $(objects)
	@echo "Building executable ($@)"
	$(CC) -o $@ $(filter-out %.d,$^) $(LDFLAGS) $(addprefix -l,$(LIBS))
	@size $@
	@echo "Done"

strip: $(PROGNAME)
	@echo "Before:"
	@size $<
	@strip $<
	@echo "After:"
	@size $<
	@echo "Done"

%.o: %$(EXT)
	$(CXX) -o $@ -c $(CPPFLAGS) $(CXXFLAGS) $<

docs: $(patsubst %.tex,%.dvi,$(docsources))

pdf: $(patsubst %.tex,%.pdf,$(docsources))

epsfigs: $(figs)
	@echo "Rebuilt EPS files from FIGs."

%.dvi: %.tex $(wildcard *.bib) $(wildcard *.eps) epsfigs
	latex $(patsubst %.tex,%,$<)
	bibtex $(patsubst %.tex,%,$<)
	latex $(patsubst %.tex,%,$<)
	latex $(patsubst %.tex,%,$<)

%.ps: %.dvi
	dvips -Ppdf -tletter $<

%.pdf: %.ps
	ps2pdf $<

%.eps: %.fig
	fig2dev -L eps $^ $@

clean:
	rm -f $(PROGNAME) $(PROGNAME).core core
	rm -f $(objects)
	csh -c "rm -f $(patsubst %.tex,%,$(docsources)).{dvi,ps,pdf,aux,log,bbl,blg}"
	rm -f $(patsubst %.fig,%.eps,$(figs))
	@echo "Done"

redep:
	@echo "Removing dependency files"
	@rm -fr .deps

veryclean: clean redep

backup: veryclean
	@echo "Creating backup..."
	@mkdir -p ../backups
	@cp -r . "../backups/$(PROGNAME).`date`"

file = $(patsubst .deps/%.d,%,$@)
$(deps):
	@echo "Generating dependencies ($@)"
	@mkdir -p $(shell dirname $@)
	@sh -ec "$(CC) -MM $(CPPFLAGS) $(file)$(EXT) | sed '1s|^.*:|$@ $(file).o:|g' > $@"

-include $(deps) /dev/null
