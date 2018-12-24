/*
 * liborbis 
 * Copyright (C) 2015,2016,2017,2018 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/orbisdev/liborbis
 */
#ifndef _ORBISFILE_H_
#define _ORBISFILE_H_

#ifdef __cplusplus
extern "C" {
#endif
	
int orbisOpen(const char *file, int flags, int mode);
int orbisClose(int fd);
int orbisRead(int fd, void *data, size_t size);
int orbisWrite(int fd, const void *data, size_t size);
int orbisLseek(int fd, int offset, int whence);
char * orbisFileGetFileContent(const char *file);
int orbisFileInit();
void orbisFileFinish();
	
	

#ifdef __cplusplus
}
#endif

#endif