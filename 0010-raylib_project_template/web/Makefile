EMCC = emcc
EMCCFLAGS = -Wall -std=c++17 -Os -DPLATFORM_WEB
EMCC_LDFLAGS = ~/Desktop/raylib/build_html5/raylib/libraylib.a \
               -I/home/user/Desktop/raylib/build_html5/raylib/include \
               -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=67108864 \
               --shell-file ~/Desktop/raylib/src/minshell.html

SRCS = ../main.cpp ../game.cpp loop_web.cpp
TARGET = ../game.html

all: $(TARGET)

$(TARGET): $(SRCS)
	$(EMCC) -o $(TARGET) $(SRCS) $(EMCCFLAGS) $(EMCC_LDFLAGS)

clean:
	rm -f ../game.html
