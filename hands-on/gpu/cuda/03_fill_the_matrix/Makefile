
NVCC ?= nvcc
TARGET ?= ex03.out
SRCS := matrix.cu
OBJS := $(SRCS:.cu=.o)
NVCCFLAGS ?= -O2 -std=c++20



.PHONY: build clean run debug release help

all: build

# Build the executable from object(s)
build: $(TARGET)

$(TARGET): $(OBJS)
	$(NVCC) $(NVCCFLAGS) $^ -o $@

# Compile .cu -> .o
%.o: %.cu
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS) $(TARGET)
	@echo "Cleaned."

run: $(TARGET)
	./$(TARGET)

# Debug build (adds debug symbols and disables optimizations)
debug: NVCCFLAGS += -G -g -O0
debug: clean $(TARGET)

# Release build (higher optimization)
release: NVCCFLAGS += -O3
release: clean $(TARGET)

help:
	@echo "Available targets:"
	@echo "  build   - compile the project
	@echo "  run     - run the produced executable"
	@echo "  clean   - remove build artifacts"
	@echo "  debug   - build with debug flags"
	@echo "  release - build optimized executable"
	@echo "  help    - this message"