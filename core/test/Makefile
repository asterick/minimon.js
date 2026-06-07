# Native regression harness: builds the core with the system compiler,
# runs the embedded BIOS (no cartridge) for 30 virtual seconds, and
# diffs the per-second state hashes against the checked-in baseline.
#
#   make check      build + compare against baseline-bios.txt (CI runs this)
#   make baseline   regenerate baseline-bios.txt after an intentional
#                   behavior change (call it out in the PR!)
#   ./harness rom.min [seconds]   hash a real ROM for local verification

CXX ?= c++
CXXFLAGS = -std=gnu++17 -O2 -Wall -I .. -I ../include

SOURCES = $(filter-out ../src/retroarch.cc, $(wildcard ../src/*.cc))
SECONDS = 30

harness: harness.cc $(SOURCES) ../include/*.h ../table.h
	$(CXX) $(CXXFLAGS) $(SOURCES) harness.cc -o $@

../table.h:
	$(MAKE) -C .. table.h

check: harness
	./harness - $(SECONDS) > current-bios.txt
	diff -u baseline-bios.txt current-bios.txt
	./harness exercise $(SECONDS) > current-exercise.txt
	diff -u baseline-exercise.txt current-exercise.txt
	@echo "OK: BIOS and exercise runs match baselines"

baseline: harness
	./harness - $(SECONDS) > baseline-bios.txt
	./harness exercise $(SECONDS) > baseline-exercise.txt

clean:
	rm -f harness current-bios.txt current-exercise.txt

.PHONY: check baseline clean
