OBJECTS=main.o blitter.o operations.o machine.o
TARGET=../public/libminimon.wasm

LD=$(WASI)/wasm-ld
CC=$(WASI)/clang
CPP=$(WASI)/clang++

CFLAGS=--target=wasm32-unknown-unknown-wasm -O2 -Iinclude
CPPFLAGS=$(CFLAGS) -std=c++17
LDFLAGS = --allow-undefined --export-dynamic --no-entry -s

all: $(TARGET)

clean:
	rm -Rf $(TARGET) $(OBJECTS)

dasm: $(TARGET)
	wasm-dis $<

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

include/table.h: ../data/table.py ../data/s1c88.csv
	python3 ../data/table.py > $@

%.o: %.c include/table.h
	$(CC) $(CFLAGS) $< -c -o $@

%.o: %.cc include/table.h
	$(CPP) $(CPPFLAGS) $< -c -o $@

.phony: all clean disassembly
