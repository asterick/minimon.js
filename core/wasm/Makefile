SRCDIR=../src
BUILDDIR=obj

SOURCES=$(filter-out $(SRCDIR)/retroarch.cc, $(wildcard $(SRCDIR)/*.cc))
OBJECTS=$(patsubst $(SRCDIR)/%.cc,$(BUILDDIR)/%.o,$(SOURCES)) $(BUILDDIR)/main.o
TRACE_OBJECTS=$(patsubst $(SRCDIR)/%.cc,$(BUILDDIR)/%.to,$(SOURCES)) $(BUILDDIR)/main.to
TARGET=../../public/libminimon.wasm
TRACE_TARGET=../../public/libminimon.tracing.wasm

# Uses the system WASI toolchain: clang with the wasm32 backend, wasm-ld
# from lld, and the wasi-libc sysroot (apt install lld wasi-libc on
# Debian/Ubuntu; installs to /usr/include/wasm32-wasi + /usr/lib/wasm32-wasi)
LD=wasm-ld
DUMP=llvm-dwarfdump
CC=clang
WASI_SYSROOT?=/usr
WASI_LIBDIR?=$(WASI_SYSROOT)/lib/wasm32-wasi

EXPORTS = \
	--export get_machine \
	--export set_sample_rate \
	--export update_inputs \
	--export cpu_reset \
	--export cpu_advance \
	--export cpu_step \
	--export cpu_read \
	--export cpu_write \
	--export get_description

CPPFLAGS = --target=wasm32-wasi -O2 -I. -I../include -std=c++17 --sysroot=$(WASI_SYSROOT) -g -Wall
LDFLAGS = --no-entry --allow-undefined -L $(WASI_LIBDIR) -lc $(EXPORTS)

all: $(BUILDDIR) $(TARGET) $(TRACE_TARGET)

clean:
	rm -Rf $(TARGET) $(TRACE_TARGET) $(BUILDDIR)

retroarch: $(BUILDDIR)
	make -f Make.retroarch

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

$(TRACE_TARGET): $(TRACE_OBJECTS)
	$(LD) $(LDFLAGS) $(TRACE_OBJECTS) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc ../include/*.h
	$(CC) $(CPPFLAGS) $< -c -o $@

$(BUILDDIR)/%.to: $(SRCDIR)/%.cc ../include/*.h
	$(CC) -D TRACING $(CPPFLAGS) $< -c -o $@

$(BUILDDIR)/main.o: main.cc ../include/*.h
	$(CC) $(CPPFLAGS) $< -c -o $@

$(BUILDDIR)/main.to: main.cc ../include/*.h
	$(CC) -D TRACING $(CPPFLAGS) $< -c -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

.PHONY: all clean disassembly
