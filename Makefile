# Define compiler
CXX=g++

# Compiler flags
CXXFLAGS=-Wall -g

# Decide which program to build based on the argument
ifeq ($(PROBLEM),1)
  SRC=birthday_presents_party.cc
  TARGET=birthday_party
else ifeq ($(PROBLEM),2)
  SRC=atmospheric_temperature_reading_module.cc
  TARGET=temperature_module
endif

# Default to build and run the specified program
all: build run

# Build the program
build: $(TARGET)

$(TARGET): $(SRC:.cc=.o)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

# Run the program
run:
	./$(TARGET)

# Clean up
clean:
	rm -f birthday_party temperature_module *.o
