MAINFILE = main
SRC = $(shell pwd)

# Note: for now biber is not needed, so it has been disabled.
# If it is needed please replace the following lines:
# - pdf1: pdflatex bib pdflatex
# - docker run --mount src=$(SRC),target=/usr/src/tex,type=bind dxjoke/tectonic-docker /bin/sh -c "tectonic --keep-intermediates --reruns 0 ${MAINFILE}.tex; biber ${MAINFILE}; tectonic ${MAINFILE}.tex"

# ***********************************************************
# ****** MAIN

.PHONY: all
all:  # all do nothing by default

.PHONY: pdf pdf1 pdf2
pdf: pdf1  # default
pdf1: pdflatex pdflatex
pdf2: pdfdocker

# ***********************************************************
# ****** BUILD RECIPES

# build using docker
pdfdocker:
	docker pull dxjoke/tectonic-docker
	docker run --mount src=$(SRC),target=/usr/src/tex,type=bind dxjoke/tectonic-docker /bin/sh -c "tectonic --keep-intermediates --reruns 0 ${MAINFILE}.tex; tectonic ${MAINFILE}.tex"

# build using pdflatex
pdflatex: 
	pdflatex -shell-escape -interaction=nonstopmode $(MAINFILE)
bib:
	biber $(MAINFILE)

# read
read:
	evince $(MAINFILE).pdf &

.PHONY: pdfdocker pdflatex bib read

# ***********************************************************
# ****** CLEAN RECIPES
.PHONY: clean
clean:
	-rm -f *.aux
	-rm -f *.log
	-rm -f *.toc
	-rm -f *.bbl
	-rm -f *.blg
	-rm -f *.out
	-rm -f make/bib

.PHONY: cleanall
cleanall: clean
	-rm -f *.pdf
	-rm -f *.ps
	-rm -f *.dvi
	-rm -rf ./make
	-rm -f *.mtc*
	-rm -f *.maf
	-rm -f *.listing
	-rm -f *.loa
	-rm -f *.lof
	-rm -f *.lot
	-rm -f *.bcf
	-rm -f *.fls
	-rm -f *.pyg
	-rm -f *.run.xml