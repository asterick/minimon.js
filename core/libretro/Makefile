SRCDIR 			:= ../src
BUILDDIR 		:= obj
SOURCES 		:= $(filter-out $(SRCDIR)/bridge.cc, $(wildcard $(SRCDIR)/*.cc))
OBJECTS 		:= $(patsubst $(SRCDIR)/%.cc,$(BUILDDIR)/%.o,$(SOURCES)) $(BUILDDIR)/libretro.o
TARGET_NAME 	:= $(BUILDDIR)/minimon

STATIC_LINKING 	:= 0
AR             	:= ar
LIBM    		:= -lm
CFLAGS			:= -I.. -I../include -pedantic

ifeq ($(platform),)
	platform = unix
	ifeq ($(shell uname -a),)
		platform = win
	else ifneq ($(findstring MINGW,$(shell uname -a)),)
		platform = win
	else ifneq ($(findstring Darwin,$(shell uname -a)),)
		platform = osx
	else ifneq ($(findstring win,$(shell uname -a)),)
		platform = win
	endif
endif

# system platform
system_platform = unix
ifeq ($(shell uname -a),)
	EXE_EXT = .exe
	system_platform = win
else ifneq ($(findstring Darwin,$(shell uname -a)),)
	system_platform = osx
	arch = intel
	ifeq ($(shell uname -p),powerpc)
		arch = ppc
	endif
else ifneq ($(findstring MINGW,$(shell uname -a)),)
	system_platform = win
endif

ifeq ($(ARCHFLAGS),)
	ifeq ($(archs),ppc)
		ARCHFLAGS = -arch ppc -arch ppc64
	else
		ARCHFLAGS = -arch i386 -arch x86_64
	endif
endif

ifeq ($(platform), osx)
	ifndef ($(NOUNIVERSAL))
		CFLAGS += $(ARCHFLAGS)
		LFLAGS += $(ARCHFLAGS)
	endif
	CFLAGS += -Wno-gnu
endif

ifeq ($(STATIC_LINKING), 1)
	EXT := a
endif

ifeq ($(platform), unix)
	EXT ?= so
	TARGET := $(TARGET_NAME)_libretro.$(EXT)
	FPIC := -fPIC
	SHARED := -shared -Wl,--version-script=link.T -Wl,--no-undefined
else ifeq ($(platform), linux-portable)
	TARGET := $(TARGET_NAME)_libretro.$(EXT)
	FPIC := -fPIC -nostdlib
	SHARED := -shared -Wl,--version-script=link.T
	LIBM :=
else ifneq (,$(findstring osx,$(platform)))
	TARGET := $(TARGET_NAME)_libretro.dylib
	FPIC := -fPIC
	SHARED := -dynamiclib
else ifneq (,$(findstring ios,$(platform)))
	TARGET := $(TARGET_NAME)_libretro_ios.dylib
	FPIC := -fPIC
	SHARED := -dynamiclib

	ifeq ($(IOSSDK),)
		IOSSDK := $(shell xcodebuild -version -sdk iphoneos Path)
	endif

	DEFINES := -DIOS
	CC = clang++ -arch armv7 -isysroot $(IOSSDK)
	ifeq ($(platform),ios9)
		CC     += -miphoneos-version-min=8.0
		CFLAGS += -miphoneos-version-min=8.0
	else
		CC     += -miphoneos-version-min=5.0
		CFLAGS += -miphoneos-version-min=5.0
	endif
else ifneq (,$(findstring qnx,$(platform)))
	TARGET := $(TARGET_NAME)_libretro_qnx.so
	FPIC := -fPIC
	SHARED := -shared -Wl,--version-script=link.T -Wl,--no-undefined
else ifeq ($(platform), emscripten)
	TARGET := $(TARGET_NAME)_libretro_emscripten.bc
	FPIC := -fPIC
	SHARED := -shared -Wl,--version-script=link.T -Wl,--no-undefined
else ifeq ($(platform), vita)
	TARGET := $(TARGET_NAME)_vita.a
	CC = arm-vita-eabi-g++
	AR = arm-vita-eabi-ar
	CFLAGS += -Wl,-q -Wall -O3
	STATIC_LINKING = 1
else
	CC = g++
	TARGET := $(TARGET_NAME)_libretro.dll
	SHARED := -shared -static-libgcc -static-libstdc++ -s -Wl,--version-script=link.T -Wl,--no-undefined
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -O0 -g
else
	CFLAGS += -O3
endif

ifneq (,$(findstring qnx,$(platform)))
	CFLAGS += -Wc,-std=c++17
else
	CFLAGS += -std=c++17
endif

LDFLAGS += $(LIBM)

all: $(TARGET)

$(TARGET): $(OBJECTS)
ifeq ($(STATIC_LINKING), 1)
	$(AR) rcs $@ $(OBJECTS)
else
	$(CC) $(FPIC) $(SHARED) $(INCLUDES) -o $@ $(OBJECTS) $(LDFLAGS)
endif

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/libretro.o: libretro.cc $(BUILDDIR)
	$(CC) $(CFLAGS) $(FPIC) -c -o $@ $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc $(BUILDDIR)
	$(CC) $(CFLAGS) $(FPIC) -c -o $@ $<

test: $(TARGET)
	~/Desktop/RetroArch.app/Contents/MacOS/RetroArch --verbose -L $< ../../roms/Pokemon\ Race\ Mini\ \(J\).min

clean:
	rm -Rf $(BUILDDIR)

.PHONY: clean all
