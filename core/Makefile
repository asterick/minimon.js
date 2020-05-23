OBJECTS=bridge.o irq.o lcd.o blitter.o operations.o machine.o debug.o tim256.o rtc.o control.o timers.o input.o eeprom.o gpio.o audio.o
TARGET=libminimon.wasm

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

all: $(DEPDIR) $(TARGET)
	cp libminimon* ../public

clean:
	rm -Rf $(TARGET) $(OBJECTS) include/table.h

dasm: $(TARGET)
	wasm-dis $<

include/table.h: ../tools/table.py ../tools/s1c88.csv
	python3 ../tools/table.py > include/table.h

#%.wasm: %.wasm.full %.wasm.dwarf
#	$(EMSCRIPTEN_ROOT)/tools/wasm-sourcemap.py $< -w $@ -p $(CURDIR) -s -u "http://localhost:9000/libminimon.wasm.map" -o $(@:.wasm=.wasm.map) --dwarfdump-output=$(@:.wasm=.wasm.dwarf)

#%.wasm.dwarf: %.wasm.full
#	$(DUMP) --all $< > $@

$(TARGET): include/table.h $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cc include/table.h include/*.h
	$(CC) $(CPPFLAGS) $< -c -o $@

$(DEPDIR):
	mkdir -p $(DEPDIR)

.phony: all clean disassembly wasi-libc
