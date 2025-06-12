CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

SRCS = ../main.cpp ../game.cpp loop_desktop.cpp
TARGET = ../game

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f ../game