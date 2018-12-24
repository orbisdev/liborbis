/*
 * liborbis 
 * Copyright (C) 2015,2016,2017,2018 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/orbisdev/liborbis
 */
#include <stdint.h>
#include <types/kernel.h>
#include <piglet.h>

#ifdef __cplusplus
extern "C" {
#endif
	
#define ATTR_ORBISGL_WIDTH 1920 
#define ATTR_ORBISGL_HEIGHT 1080


typedef struct OrbisGlConfig
{
	ScePglConfig pgl_config;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	EGLConfig config;
	EGLint num_configs;
	int width;
	int height;
	char orbisgl_initialized;
	
}OrbisGlConfig __attribute__ ((aligned (16)));



int orbisGlInit(unsigned int width,unsigned int height);
void orbisGlFinish();
int orbisGlInitWithConf(OrbisGlConfig *conf);
int orbisGlSetConf(OrbisGlConfig *conf);
int orbisGlCreateConf(unsigned int width,unsigned int height);
OrbisGlConfig *orbisGlGetConf();
void orbisGlDestroyTexture(GLuint texture_id);
void orbisGlDestroyProgram(GLuint program_id);
void orbisGlDestroyDisplay();
void orbisGlDestroySurface();
void orbisGlDestroyContext();
void orbisGlSwapBuffers();
void orbisGlShaderLog(GLuint shader_id);
void orbisGlProgramLog(GLuint program_id);
GLuint orbisGlCompileShader(const GLenum type,const GLchar* source);
GLuint orbisGlLinkProgram(const GLuint vertex_shader,const GLuint fragment_shader);
GLuint orbisGlCreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename);
GLuint orbisGlCreateTexture(const GLsizei width,const GLsizei height,const GLenum type,const GLvoid* pixels);
GLint orbisGlLoadShaders(const char* vertexShaderFilename, const char* fragmentShaderFilename);
#ifdef __cplusplus
}
#endif