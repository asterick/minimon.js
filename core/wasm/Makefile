SRCDIR=../src
BUILDDIR=obj

SOURCES=$(filter-out $(SRCDIR)/retroarch.cc, $(wildcard $(SRCDIR)/*.cc))
OBJECTS=$(patsubst $(SRCDIR)/%.cc,$(BUILDDIR)/%.o,$(SOURCES)) $(BUILDDIR)/main.o
TARGET=../../public/libminimon.wasm

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
	--export get_description \
	--export get_bios_trace \
	--export get_cartridge_trace

# The tracing core is the only build: classification maps live in wasm
# memory (see main.cc) so the per-access trace hook never crosses into JS
CPPFLAGS = --target=wasm32-wasi -O2 -D TRACING -I. -I../include -std=c++17 --sysroot=$(WASI_SYSROOT) -g -Wall
LDFLAGS = --no-entry --allow-undefined -L $(WASI_LIBDIR) -lc $(EXPORTS)

all: $(BUILDDIR) $(TARGET)

clean:
	rm -Rf $(TARGET) $(BUILDDIR)

retroarch: $(BUILDDIR)
	make -f Make.retroarch

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc ../include/*.h
	$(CC) $(CPPFLAGS) $< -c -o $@

$(BUILDDIR)/main.o: main.cc ../include/*.h
	$(CC) $(CPPFLAGS) $< -c -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

.PHONY: all clean disassembly
