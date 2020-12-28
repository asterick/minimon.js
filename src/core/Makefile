all: libretro wasm

clean:
	rm -Rf table.h
	make -C wasm clean
	make -C libretro clean

wasm: table.h
	make -C wasm

libretro: table.h
	make -C libretro

table.h: table.py ../s1c88.csv
	python3 table.py > table.h

.PHONY: all clean wasm libretro
