ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

AMJULIB		:=	$(AMJULIB_MAC_SCRIPT_PATH)/../../
RUNDIR		:=	../../Assets/
BUILD		:=	../../Build/Mac
TARGET		:=	$(BUILD)/macgame
TEST_TARGET	:=	$(BUILD)/macgame_test
LIBDIRS   	:= 	$(AMJULIB)/Releases/amjulib-v.0.5-macosx
SOURCES		:=	../../Source ../../Source/Animals
BASSPATH	:=	$(AMJULIB)/3rdPartyLibs/Bass/2.4.18.3/bass24-osx
BASSMIDIPATH	:=	$(AMJULIB)/3rdPartyLibs/Bass/BassMidi/2.4.15.3/bassmidi24-osx


LIBS	:= \
	-lamju \
	$(BASSPATH)/libbass.dylib \
	$(BASSMIDIPATH)/libbassmidi.dylib \
	-framework OpenGL \
	-framework GLUT \
	-framework Carbon \
	-lcurl \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile
CFLAGS += -D_DEBUG -DAMJU_USE_BASS_MIDI -DAMJU_USE_BASS -Wno-deprecated-declarations

# Add path to exe to search path for dylibs.
LDFLAGS += -Wl,-rpath,@executable_path/

run:
	$(TEST_TARGET) && $(TARGET)

# Use runsetup after a make clean or first ever build


runnoglue:
	cp $(TARGET) $(RUNDIR)/Mac/; \
	cp $(BASSMIDIPATH)/libbassmidi.dylib $(RUNDIR)/Mac/; \
	cp $(BASSPATH)/libbass.dylib $(RUNDIR)/Mac/; \
	cd $(RUNDIR)/Mac/; \
	./macgame


# Copy dylibs; remove "quarantine" security flag added by Macos on download.
runsetup:
	cp $(BASSMIDIPATH)/libbassmidi.dylib $(BUILD); \
	cp $(BASSPATH)/libbass.dylib $(BUILD); \
	xattr -d com.apple.quarantine $(BUILD)/libbass.dylib; \
	xattr -d com.apple.quarantine $(BUILD)/libbassmidi.dylib

viewscore:
	$(TARGET) --viewscore

