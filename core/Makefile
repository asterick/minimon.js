OBJECTS=irq.o lcd.o blitter.o operations.o machine.o debug.o tim256.o rtc.o control.o timers.o input.o eeprom.o gpio.o
TARGET=../public/libminimon.wasm

LD=$(shell which wasm-ld-8)
LD := $(if $(LD), $(LD), "wasm-ld")
CC=clang-8

EXPORTS = \
	--export get_machine \
	--export lcd_render \
	--export update_inputs \
	--export cpu_reset \
	--export cpu_advance \
	--export cpu_step \
	--export cpu_read \
	--export cpu_write

CFLAGS = --target=wasm32-wasm -O2 -Iinclude -std=c99 --sysroot=./wasi-libc/sysroot
CPPFLAGS = --target=wasm32-wasm -O2 -Iinclude -std=c++17 --sysroot=./wasi-libc/sysroot
LDFLAGS = --no-entry --allow-undefined $(EXPORTS) -L ./wasi-libc/sysroot/lib/wasm32-wasi -lc

all: $(DEPDIR) $(TARGET)

clean:
	rm -Rf $(TARGET) $(OBJECTS) include/table.h

dasm: $(TARGET)
	wasm-dis $<

$(TARGET): include/table.h $(OBJECTS)
	echo $(LD)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

include/table.h: ../tools/table.py ../tools/s1c88.csv
	python3 ../tools/table.py > include/table.h

%.o: %.c include/table.h include/*.h
	$(CC) $(CFLAGS) $< -c -o $@

%.o: %.cc include/table.h include/*.h
	$(CC) $(CPPFLAGS) $< -c -o $@

$(DEPDIR):
	mkdir -p $(DEPDIR)

.phony: all clean disassembly wasi-libc
