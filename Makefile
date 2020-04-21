
SUBDIRS = libdebugnet libelfloader libmod liborbis2d liborbisAudio liborbisFile liborbisFileBrowser liborbisGl liborbisKeyboard liborbisNfs liborbisPad liborbisXbmFont libps4link portlibs

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
