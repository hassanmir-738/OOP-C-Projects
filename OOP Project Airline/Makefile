# Makefile for SkyLink Airways Airline Management System
# C++ Object-Oriented Programming (PBL Assignment)

CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -Iinclude
TARGET = airline_system.exe

# Source files
SRCS = src/Flight.cpp \
       src/DomesticFlight.cpp \
       src/InternationalFlight.cpp \
       src/CharterFlight.cpp \
       src/Passenger.cpp \
       src/EconomyPassenger.cpp \
       src/BusinessPassenger.cpp \
       src/FirstClassPassenger.cpp \
       src/Ticket.cpp \
       src/Airline.cpp \
       src/Utils.cpp \
       src/main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	@echo [BUILD SUCCESS] Executable created: $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@del /Q src\*.o $(TARGET) 2>nul || rm -f src/*.o $(TARGET)
	@echo [CLEAN] Cleaned build artifacts.

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
