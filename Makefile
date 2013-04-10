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

$(ZIPNAME): $(CPPFILES) $(HFILES) $(REPORT)
	zip $(ZIPNAME) $(CPPFILES) $(HFILES) $(REPORT)

clean:
	-rm *.o

proper: clean
	-rm $(TARGET)
