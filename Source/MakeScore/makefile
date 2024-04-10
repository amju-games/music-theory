all:makescore_exe

clean:
	rm makescore *.o

run:
	./makescore


CC=clang++

WARNINGS=-Wall

INCLUDES=\
  -I..

CFLAGS=-c $(WARNINGS) $(INCLUDES) -std=c++20 -g -O0

LDFLAGS=

SOURCES=\
Attachment.cpp \
Bar.cpp \
Beam.cpp \
BeamGroup.cpp \
Clef.cpp \
Consts.cpp \
Curve.cpp \
Flag.cpp \
Glyph.cpp \
Hairpin.cpp \
KeySig.cpp \
Main.cpp \
MakeScore.cpp \
NoteGlyph.cpp \
Pitch.cpp \
Quad.cpp \
RestGlyph.cpp \
Stave.cpp \
Switch.cpp \
Tie.cpp \
TimeSig.cpp \
TimeValue.cpp \
Utils.cpp 


OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=makescore

makescore_exe: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

