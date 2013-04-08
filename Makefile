CXX=mpic++
CXXFLAGS=-O3 -Wall #-pedantic -std=gnu++98
TARGET=bitonicdriver
CPPFILES=main.cpp \
         bitonic_sort.cpp
OBJFILES=${CPPFILES:.cpp=.o}
HFILES=bitonic_sort.h

.phony:clean proper

all:default

default: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CXX) $(CXXFLAGS) $(OBJFILES) -o $@

bitonic.o: bitonic_sort.cpp bitonic_sort.h
	$(CXX) $(CXXFLAGS) bitonic_sort.cpp -c -o $@

main.o: main.cpp bitonic_sort.h
	$(CXX) $(CXXFLAGS) main.cpp -c -o $@

clean:
	-rm *.o

proper: clean
	-rm $(TARGET)
