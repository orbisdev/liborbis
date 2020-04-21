# This samples require freetype library to be installed
ifneq ("$(wildcard $(PS4SDK)/include/ft2build.h)","")
SUBDIRS += minimp3 next_wave_shader
endif

# This samples require SDL2 library to be installed
ifneq ("$(wildcard $(PS4SDK)/include/SDL2/SDL_config.h)","")
SUBDIRS += sdl
endif

# Samples not compiling for now
# SUBDIRS += a_simple_shader rotozoom Woody_Menu a_simpler_shader drawing tetris

SUBDIRS += copperbars next starfield modplayer

all:
	for dir in $(SUBDIRS); do \
        $(MAKE) -C $$dir; \
    done

install:

clean:
	for dir in $(SUBDIRS); do \
        $(MAKE) -C $$dir clean; \
    done
