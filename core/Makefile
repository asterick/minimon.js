SRCDIR=src
BUILDDIR=obj
SRCEXT=cc
OBJEXT=o

SOURCES=$(wildcard $(SRCDIR)/*.cc)
OBJECTS=$(patsubst $(SRCDIR)/%.cc,$(BUILDDIR)/%.o,$(SOURCES))
TRACE_OBJECTS=$(OBJECTS:.o=.to)
TARGET=libminimon.wasm
TRACE_TARGET=libminimon.tracing.wasm

LD=wasm-ld
DUMP=llvm-dwarfdump
CC=clang

EXPORTS = \
	--export get_machine \
	--export update_inputs \
	--export cpu_reset \
	--export cpu_advance \
	--export cpu_step \
	--export cpu_read \
	--export cpu_write \
	--export get_description

CPPFLAGS = --target=wasm32-wasm -O2 -Iinclude -std=c++17 --sysroot=./wasi-libc/sysroot -g
LDFLAGS = --no-entry --allow-undefined $(EXPORTS) -L ./wasi-libc/sysroot/lib/wasm32-wasi -lc

all: $(BUILDDIR) wasi-libc $(TARGET) $(TRACE_TARGET)
	cp $(TARGET) $(TRACE_TARGET) ../public

clean:
	rm -Rf $(TARGET) $(TRACE_TARGET) $(OBJECTS) $(TRACE_OBJECTS) include/table.h

include/table.h: ../tools/table.py ../tools/s1c88.csv
	python3 ../tools/table.py > include/table.h

wasi-libc:
	# make -C wasi-libc

$(TARGET): include/table.h wasi-libc $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

$(TRACE_TARGET): include/table.h wasi-libc $(TRACE_OBJECTS)
	$(LD) $(LDFLAGS) $(TRACE_OBJECTS) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc include/table.h include/*.h
	$(CC) $(CPPFLAGS) $< -c -o $@

$(BUILDDIR)/%.to: $(SRCDIR)/%.cc include/table.h include/*.h
	$(CC) -D TRACING $(CPPFLAGS) $< -c -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

.PHONY: all clean disassembly wasi-libc
