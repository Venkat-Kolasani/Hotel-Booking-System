# Makefile for Hotel Booking System

# Compiler
CXX = g++
CXXFLAGS = -std=c++17 `wx-config --cxxflags`
LDFLAGS = `wx-config --libs`

# Sources
SRCS = main.cpp

# Objects
OBJS = $(SRCS:.cpp=.o)

# Executable
EXEC = hotel_booking_system

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)