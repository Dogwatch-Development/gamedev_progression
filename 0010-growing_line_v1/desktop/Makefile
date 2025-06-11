CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

SRCS = ../main.cpp ../game.cpp loop_desktop.cpp
TARGET = ../growing_line

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f ../growing_line