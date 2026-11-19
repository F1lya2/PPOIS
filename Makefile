CXX ?= g++

CXXFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c++17

SOURCES := $(wildcard *.cpp */*.cpp)

HEADERS := $(wildcard *.hpp */*.hpp *.h */*.h)

TARGET = vermes

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) $(SOURCES) $(LDFLAGS) -o $@

clean:
	rm -f $(TARGET)
