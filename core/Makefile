all: libretro wasm

clean:
	rm -Rf table.h
	make -C wasm clean
	make -C libretro clean

wasm: table.h
	make -C wasm

libretro: table.h
	make -C libretro

table.h: ../tools/table.py ../tools/s1c88.csv
	python3 ../tools/table.py > table.h

.PHONY: all clean wasm libretro
