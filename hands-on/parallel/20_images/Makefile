.PHONY: all clean

CXX := g++

all: test

clean:
	rm -f test

stb:
	git clone https://github.com/nothings/stb.git

libsixel:
	git clone git@github.com:saitoha/libsixel.git build/libsixel && cd build/libsixel && ./configure --without-libcurl --without-jpeg --without-png --without-pkgconfigdir --without-bashcompletiondir --without-zshcompletiondir --disable-python --prefix=$(shell realpath libsixel) && make -j`nproc` install && cd ../../ && rm -rf build

test: test.cc Makefile stb libsixel
	$(CXX) -std=c++20 -O3 -g -Istb -Ilibsixel/include -Wall -march=native $< -Llibsixel/lib -Wl,-rpath,libsixel/lib -lsixel -o $@

