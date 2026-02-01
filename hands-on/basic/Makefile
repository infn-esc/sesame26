.PHONY: all clean

all: release debug

clean:
	rm -f pi_time deb/pi_time

release: pi_time

debug: deb/pi_time

CXX := g++
CXXFLAGS := -std=c++20 -Wall -march=native

pi_time: pi_time.cpp Makefile
	$(CXX) $< $(CXXFLAGS) -o $@

deb/pi_time: pi_time.cpp Makefile
	mkdir -p deb
	$(CXX) $< $(CXXFLAGS) -Og -g -o $@
