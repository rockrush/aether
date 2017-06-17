CXX = clang++
CXXFLAGS += -I${VULKAN_SDK}/include -std=c++11
LDFLAGS += -L${VULKAN_SDK}/lib -lvulkan

all: sim

.PHONY: clean
clean:
	@rm -f sim
