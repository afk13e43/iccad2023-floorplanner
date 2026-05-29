# ICCAD 2023 Problem D — build the solver and checker.
#
#   make            build prog (solver) and checker
#   make run CASE=06 [SEC=60] [TH=8]   solve one case and verify it
#   make clean

CXX      ?= g++
CXXFLAGS ?= -O2 -std=c++17 -static -pthread   # -static avoids the MinGW libstdc++ DLL mismatch

prog: src/floorplan_sp.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

checker: src/checker.cpp
	$(CXX) -O2 -std=c++17 $< -o $@

all: prog checker

CASE ?= 06
SEC  ?= 60
TH   ?= 8
run: prog checker
	./prog testcases/case$(CASE)-input.txt results/out$(CASE).txt $(SEC) $(TH)
	./checker testcases/case$(CASE)-input.txt results/out$(CASE).txt

clean:
	rm -f prog prog2 progg checker prog.exe prog2.exe progg.exe checker.exe a.out

.PHONY: all run clean
