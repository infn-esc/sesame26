# ─────────────────────────────────────────────────────────────────
# Circle-of-Life Makefile – builds CPU version always, CUDA only
# when circle_of_life.cu exists.
# ----------------------------------------------------------------

CXX   := g++
NVCC  := nvcc
CXXFLAGS := -std=c++20 -O2
INCLUDES := -I gif-h -I .

CPU_SRC  := circle_of_life.cpp
CPU_BIN  := circle_of_life

# Detect CUDA source automatically
CUDA_SRC := $(wildcard circle_of_life.cu)
CUDA_BIN := circle_of_life_cuda

# Clone gif-h once and fetch stb_image.h once
gif-h:
	git clone https://github.com/charlietangora/gif-h || true

stb_image.h:
	curl -L -o stb_image.h https://raw.githubusercontent.com/nothings/stb/master/stb_image.h

# -------------------- targets -----------------------------------
.PHONY: all serial cuda clean run

# If CUDA_SRC is empty, ‘all’ builds only serial
ifeq ($(CUDA_SRC),)
all: serial
else
all: serial cuda
endif

serial: gif-h stb_image.h $(CPU_SRC)
	$(CXX) $(CPU_SRC) $(CXXFLAGS) $(INCLUDES) -o $(CPU_BIN)

# CUDA target only generated when the .cu file exists
ifeq ($(CUDA_SRC),)
cuda:
	@echo "Skipping CUDA build (circle_of_life.cu not found)."
else
cuda: gif-h stb_image.h $(CUDA_SRC)
	$(NVCC) $(CUDA_SRC) -std=c++20 -O2 $(INCLUDES) -o $(CUDA_BIN)
endif

clean:
	rm -f $(CPU_BIN) $(CUDA_BIN)

run: all
	./$(CPU_BIN)
ifeq ($(CUDA_SRC),$(wildcard $(CUDA_SRC)))
	./$(CUDA_BIN)
endif
