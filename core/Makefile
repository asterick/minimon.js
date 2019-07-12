OBJECTS=main.o blitter.o operations.o machine.o
TARGET=../public/libminimon.wasm

LD=wasm-ld
CC=clang
CPP=clang++

CPPFLAGS = --target=wasm32-wasm -O2 -Iinclude -std=c++17
LDFLAGS = --allow-undefined --export-dynamic --no-entry

all: $(DEPDIR) $(TARGET)

clean:
	rm -Rf $(TARGET) $(OBJECTS)

dasm: $(TARGET)
	wasm-dis $<

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

include/table.h: ../data/table.py ../data/s1c88.csv
	python3 ../data/table.py > $@

%.o: %.c include/*.h
	$(CC) $(CFLAGS) $< -c -o $@

%.o: %.cc include/*.h
	$(CPP) $(CPPFLAGS) $< -c -o $@

$(DEPDIR):
	mkdir -p $(DEPDIR)

.phony: all clean disassembly wasi-libc
