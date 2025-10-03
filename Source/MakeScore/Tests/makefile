all:makescore_exe

clean:
	rm testrunner.exe *.o

run:
	./testrunner.exe


CC=clang++

WARNINGS=-Wall

INCLUDES=\
  -I..

CFLAGS=-c $(WARNINGS) $(INCLUDES) -std=c++20 -g -O0

LDFLAGS=

SOURCES=$(filter-out ../Main.cpp, $(wildcard ../*.cpp) $(wildcard *.cpp))

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=testrunner.exe

makescore_exe: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

