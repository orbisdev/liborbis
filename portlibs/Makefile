# This ports requires freetype library to be installed
ifneq ("$(wildcard $(PS4SDK)/include/ft2build.h)","")
SUBDIRS = libfreetype-gl libfreetype-gl2 
endif

SUBDIRS += libpng libsxmlc libz MiniAPI

all:
	for dir in $(SUBDIRS); do \
        $(MAKE) -C $$dir; \
    done

install:
	for dir in $(SUBDIRS); do \
        $(MAKE) -C $$dir install; \
    done

clean:
	for dir in $(SUBDIRS); do \
        $(MAKE) -C $$dir clean; \
    done
