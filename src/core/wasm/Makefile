SRCDIR=../src
BUILDDIR=obj

SOURCES=$(filter-out $(SRCDIR)/retroarch.cc, $(wildcard $(SRCDIR)/*.cc))
OBJECTS=$(patsubst $(SRCDIR)/%.cc,$(BUILDDIR)/%.o,$(SOURCES)) $(BUILDDIR)/main.o
TARGET=../../static/libminimon.wasm

LD=$(if $(shell which wasm-ld),wasm-ld,wasm-ld-11)
DUMP=llvm-dwarfdump
CC=clang

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

CPPFLAGS = --target=wasm32-wasm -O2 -I. -I../include -std=c++17 --sysroot=./wasi-libc/sysroot -g -Wall
LDFLAGS = --no-entry --allow-undefined -L ./wasi-libc/sysroot/lib/wasm32-wasi -lc $(EXPORTS)

all: $(BUILDDIR) $(TARGET)

clean:
	rm -Rf $(TARGET) $(BUILDDIR)

wasi-libc:
	make -C wasi-libc

$(TARGET): $(OBJECTS) wasi-libc
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc ../include/*.h
	$(CC) -D TRACING $(CPPFLAGS) $< -c -o $@

$(BUILDDIR)/main.o: main.cc ../include/*.h
	$(CC) -D TRACING $(CPPFLAGS) $< -c -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

.PHONY: all clean disassembly wasi-libc
