# Updated Makefile to support both native and HTML5 builds

# Compiler and flags for native build
CXX = g++
CXXFLAGS = -Wall -std=c++17 -I/usr/include
LDFLAGS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

# Compiler and flags for HTML5 build
EMCC = emcc
EMCCFLAGS = -Wall -std=c++17 -Os -DPLATFORM_WEB
EMCC_LDFLAGS = ~/Desktop/raylib/build_html5/raylib/libraylib.a \
               -I/home/user/Desktop/raylib/build_html5/raylib/include \
               -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=67108864 \
               --shell-file ~/Desktop/raylib/src/minshell.html

# Source files and targets
SRCS = main.cpp
TARGET = dot_game_v3
HTML5_TARGET = dot_game_v3.html

# Native build target
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

# HTML5 build target
$(HTML5_TARGET): $(SRCS)
	$(EMCC) -o $(HTML5_TARGET) $(SRCS) $(EMCCFLAGS) $(EMCC_LDFLAGS)

# Clean up native build
clean:
	rm -f $(TARGET)

# Clean up HTML5 build
clean-html5:
	rm -f $(HTML5_TARGET)