ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

AMJULIB		:=	$(AMJULIB_MAC_SCRIPT_PATH)/../../
RUNDIR		:=	../../Assets/
BUILD		:=	../../Build/Mac
TARGET		:=	$(BUILD)/macgame
LIBDIRS   	:= 	$(AMJULIB)/Releases/amjulib-v.0.5-macosx
SOURCES		:=	../../Source 
BASSPATH	:=	$(AMJULIB)/3rdPartyLibs/Bass/Bass2.4.16.7/Macosx

LIBS	:= \
	-lamju \
	$(BASSPATH)/libbass.dylib \
	$(BASSPATH)/libbassmidi.dylib \
	-framework OpenGL \
	-framework GLUT \
	-framework Carbon \
	-lcurl \
	-lstdc++ -lm 

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile
CFLAGS += -D_DEBUG -DAMJU_USE_BASS_MIDI -DAMJU_USE_BASS -Wno-deprecated-declarations

run:
	$(TARGET)

#	cp $(TARGET) $(RUNDIR); \
#	cp $(BASSPATH)/libbassmidi.dylib $(RUNDIR); \
#	cp $(BASSPATH)/libbass.dylib $(RUNDIR); \
#	cd $(RUNDIR); \
#	./macgame


runnoglue:
	cp $(TARGET) $(RUNDIR)/Mac/; \
	cp $(BASSPATH)/libbassmidi.dylib $(RUNDIR)/Mac/; \
	cp $(BASSPATH)/libbass.dylib $(RUNDIR)/Mac/; \
	cd $(RUNDIR)/Mac/; \
	./macgame


runsetupold:
	cp $(TARGET) $(RUNDIR); \
	cp $(BASSPATH)/libbassmidi.dylib $(RUNDIR); \
	cp $(BASSPATH)/libbass.dylib $(RUNDIR); 

runsetup:
	cp $(BASSPATH)/libbassmidi.dylib $(BUILD); \
	cp $(BASSPATH)/libbass.dylib $(BUILD); 


