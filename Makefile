###################################################################################
## This file is part of bitonic-sort
##
## bitonic-sort is free software: you can redistribute it and#or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
##  bitonic-sort is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with bitonic-sort.  If not, see <http:##www.gnu.org#licenses#>.
##
##  Copyright 2013 Aaron Wilhelm
###################################################################################

CXX=mpic++
CXXFLAGS=-O3 -Wall #-pedantic -std=gnu++98
TARGET=bitonicdriver
CPPFILES=main.cpp \
         bitonic_sort.cpp
OBJFILES=${CPPFILES:.cpp=.o}
HFILES=bitonic_sort.h
ZIPNAME=Wilhelm_Aaron_CS387_Hmwk2_Bitonic_sort.zip
REPORT=report/Wilhelm_Aaron_CS387_Hmwk2_Bitonic_sort.pdf

.phony:clean proper

all:default

default: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CXX) $(CXXFLAGS) $(OBJFILES) -o $@

bitonic.o: bitonic_sort.cpp bitonic_sort.h
	$(CXX) $(CXXFLAGS) bitonic_sort.cpp -c -o $@

main.o: main.cpp bitonic_sort.h
	$(CXX) $(CXXFLAGS) main.cpp -c -o $@

results/results.txt: results/parse_results.sh
	cd results && bash -c "./parse_results.sh > results.txt"

$(REPORT): report/report.tex results/results.txt
	cd report && pdflatex report.tex
	cd report && mv report.pdf ../$(REPORT)

$(ZIPNAME): $(CPPFILES) $(HFILES) $(REPORT) Makefile
	zip $(ZIPNAME) $(CPPFILES) $(HFILES) $(REPORT) Makefile

clean:
	-rm *.o

proper: clean
	-rm $(TARGET)
