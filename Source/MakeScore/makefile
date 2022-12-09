all:makescore_exe

clean:
	rm makescore *.o

run:
	./makescore


CC=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++

WARNINGS=-Wall

INCLUDES=\
  -I..

CFLAGS=-c $(WARNINGS) $(INCLUDES) -std=c++11 -g -O0

LDFLAGS=

SOURCES=\
Attachment.cpp \
Bar.cpp \
Beam.cpp \
Clef.cpp \
Consts.cpp \
Flag.cpp \
Glyph.cpp \
KeySig.cpp \
MakeScore.cpp \
Pitch.cpp \
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

