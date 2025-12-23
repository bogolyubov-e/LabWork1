CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror -Isrc
GTEST_FLAGS = -lgtest -lgtest_main -pthread

SOURCES = src/BmpHeader.cpp src/BmpImage.cpp
OBJECTS = $(SOURCES:.cpp=.o)
DEMO_SOURCES = src/main.cpp
DEMO_TARGET = bmp_demo

TEST_SOURCES = tests/BmpImageTest.cpp
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)


TARGET = bmp_tests

.PHONY: all test clean format help

all: test

test: $(TARGET)

$(TARGET): $(OBJECTS) $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GTEST_FLAGS)

demo: $(DEMO_TARGET)

$(DEMO_TARGET): $(OBJECTS) $(DEMO_SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GTEST_FLAGS)

src/BmpHeader.o: src/BmpHeader.cpp src/BmpHeader.h
	$(CXX) $(CXXFLAGS) -c src/BmpHeader.cpp -o src/BmpHeader.o

src/BmpImage.o: src/BmpImage.cpp src/BmpImage.h src/IImage.h src/BmpHeader.h
	$(CXX) $(CXXFLAGS) -c src/BmpImage.cpp -o src/BmpImage.o

tests/BmpImageTest.o: tests/BmpImageTest.cpp src/BmpImage.h
	$(CXX) $(CXXFLAGS) -c tests/BmpImageTest.cpp -o tests/BmpImageTest.o

format:
	astyle -A1 -s4 --suffix=none src/*.cpp src/*.h tests/*.cpp

clean:
	rm -f src/*.o
	rm -f tests/*.o
	rm -f $(TARGET)
	rm -f assets/output_*.bmp
	rm -f assets/temp_*.bmp
	rm -f $(DEMO_TARGET)

help:
	@echo "make          - build and run tests"
	@echo "make test     - build tests"
	@echo "make demo     - build demo program"
	@echo "make format   - format source files"
	@echo "make clean    - remove build artifacts and output BMP files"
