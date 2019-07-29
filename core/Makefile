OBJECTS=irq.o lcd.o blitter.o operations.o machine.o debug.o tim256.o rtc.o control.o timer.o
TARGET=../public/libminimon.wasm

LD=wasm-ld
CC=clang
CPP=clang++

EXPORTS = \
	--export get_machine \
	--export lcd_render \
	--export cpu_reset \
	--export cpu_advance \
	--export cpu_step \
	--export cpu_read \
	--export cpu_write

CPPFLAGS = --target=wasm32-wasm -O2 -Iinclude -std=c++17 --sysroot=./wasi-libc/sysroot
LDFLAGS = --no-entry --allow-undefined $(EXPORTS) -L ./wasi-libc/sysroot/lib/wasm32-wasi -lc

all:table $(DEPDIR) $(TARGET)

clean:
	rm -Rf $(TARGET) $(OBJECTS)

dasm: $(TARGET)
	wasm-dis $<

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

table: ../data/table.py ../data/s1c88.csv
	python3 ../data/table.py > include/table.h

%.o: %.c include/*.h
	$(CC) $(CFLAGS) $< -c -o $@

%.o: %.cc include/*.h
	$(CPP) $(CPPFLAGS) $< -c -o $@

$(DEPDIR):
	mkdir -p $(DEPDIR)

.phony: all clean disassembly wasi-libc
