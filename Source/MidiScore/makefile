all:midiscore_exe

clean:
	rm midiscore source/*.o midifile/src/*.o test/*.o

run:
	./midiscore


CC=clang++

WARNINGS=-Wall

INCLUDES=\
  -I source -I midifile/include

CFLAGS=-c $(WARNINGS) $(INCLUDES) -std=c++20 -g -O0

LDFLAGS=

SOURCES=$(wildcard source/*.cpp) $(wildcard midifile/src/*.cpp)

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=midiscore

midiscore_exe: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

