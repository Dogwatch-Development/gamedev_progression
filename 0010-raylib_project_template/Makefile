# Root Makefile to build both desktop and web targets

.PHONY: all desktop web clean clean-desktop clean-web

SRCS = main.cpp game.cpp

all: desktop web

# Build desktop target

desktop:
	$(MAKE) -C desktop

# Build web target

web:
	$(MAKE) -C web

# Clean all
clean: clean-desktop clean-web

clean-desktop:
	$(MAKE) -C desktop clean

clean-web:
	$(MAKE) -C web clean
