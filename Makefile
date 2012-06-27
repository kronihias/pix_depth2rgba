# change to your local directories!
PD_APP_DIR = /Applications/Pd-extended.app/Contents/Resources
GEM_DIR = /Users/matthias/Gem-0.93.3

CPPFLAGS = -I$(GEM_DIR)/src -I$(PD_APP_DIR)/include

#linux doesnt work yet
UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
 CPPFLAGS += -I
 CXXFLAGS = -g -O2 -fPIC -freg-struct-return -Os -falign-loops=32 -falign-functions=32 -falign-jumps=32 -funroll-loops -ffast-math -mmmx
 LDFLAGS = -shared -rdynamic
 LIBS = 
 EXTENSION = pd_linux
 USER_EXTERNALS=$(HOME)/pd-externals
endif
ifeq ($(UNAME),Darwin)
 CPPFLAGS += 
 CXXFLAGS = -arch i386 -g -fast -msse3
 LDFLAGS = -arch i386 -bundle -bundle_loader $(PD_APP_DIR)/bin/pd -undefined dynamic_lookup -mmacosx-version-min=10.6
 LIBS = -lm
 EXTENSION = pd_darwin
 USER_EXTERNALS=$(HOME)/Library/Pd
endif

.SUFFIXES = $(EXTENSION)

SOURCES = pix_depth2rgba

all:
	g++ $(CPPFLAGS) $(CXXFLAGS) -o $(SOURCES).o -c $(SOURCES).cpp
	g++ -o $(SOURCES).$(EXTENSION) $(LDFLAGS) $(SOURCES).o $(LIBS)
	rm -fr ./*.o

clean:
	rm -f $(SOURCES)*.o
	rm -f $(SOURCES)*.$(EXTENSION)

distro: clean all
	rm *.o
