all:makescore_exe

clean:
	rm testrunner.exe *.o ../sourse/*.o

run:
	./testrunner.exe


CC=clang++

WARNINGS=-Wall

INCLUDES=\
  -I../source -I../midifile/include

CFLAGS=-c $(WARNINGS) $(INCLUDES) -std=c++20 -g -O0

LDFLAGS=

SOURCES=$(filter-out ../source/main.cpp, $(wildcard ../midifile/src/*.cpp) $(wildcard ../source/*.cpp) $(wildcard *.cpp))

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=testrunner.exe

makescore_exe: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

