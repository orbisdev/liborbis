/*
 * liborbis 
 * Copyright (C) 2015,2016,2017,2018 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/orbisdev/liborbis
 */
#include <stdlib.h>
#include <string.h>
#include <debugnet.h>
#include <orbisFile.h>
#include <stdlib.h>
#include "orbisGl.h"

OrbisGlConfig *orbisGlConf=NULL;
int orbisgl_external_conf=-1;

EGLint attribs[] = 
{
	EGL_RED_SIZE, 8,
	EGL_GREEN_SIZE, 8,
	EGL_BLUE_SIZE, 8,
	EGL_ALPHA_SIZE, 8,
	EGL_DEPTH_SIZE, 16,
	EGL_STENCIL_SIZE, 0,
	EGL_SAMPLE_BUFFERS, 0,
	EGL_SAMPLES, 0,
	EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
	EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
	EGL_NONE,
};

EGLint ctx_attribs[] = 
{
	EGL_CONTEXT_CLIENT_VERSION, 2,
	EGL_NONE,
};

EGLint window_attribs[] = {
	EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
	EGL_NONE,
};

void orbisGlSwapBuffers()
{
	if(orbisGlConf)
	{
		if(orbisGlConf->orbisgl_initialized==1)
		{
			eglSwapBuffers(orbisGlConf->display, orbisGlConf->surface);
		}
	}
}
void orbisGlDestroyProgram(GLuint program_id)
{
	int ret;
	if(orbisGlConf && program_id>0)
	{
		if(orbisGlConf->orbisgl_initialized==1)
		{
			glDeleteProgram(program_id);
			ret=glGetError();
			if(ret)
			{
				debugNetPrintf(ERROR,"[ORBIS_GL] glDeleteProgram failed: 0x%08X\n", ret);
			}
		}
	}
}
void orbisGlShaderLog(GLuint shader_id)
{
	int ret;
	if(orbisGlConf && shader_id>0)
	{
		if(orbisGlConf->orbisgl_initialized==1)
		{		
			GLint log_length;
			glGetShaderiv(shader_id,GL_INFO_LOG_LENGTH,&log_length);
			if(log_length)
			{
				GLchar log_buffer[log_length];
				glGetShaderInfoLog(shader_id,log_length,NULL,log_buffer);
				ret=glGetError();
				if(ret)
				{
					debugNetPrintf(ERROR,"[ORBISGL] glGetShaderInfoLog failed: 0x%08X\n",ret);
					return;
				}
				debugNetPrintf(ERROR,"[ORBISGL] shader compilation with log:\n%s\n",log_buffer);
			}
			else
			{
				debugNetPrintf(DEBUG,"[ORBISGL] shader compilantion no log\n");
			}
		}
	}
}
void orbisGlProgramLog(GLuint program_id)
{
	int ret;
	if(orbisGlConf && program_id>0)
	{
		if(orbisGlConf->orbisgl_initialized==1)
		{
			GLint log_length;
			glGetProgramiv(program_id,GL_INFO_LOG_LENGTH,&log_length);
			if(log_length)
			{
				GLchar log_buffer[log_length];
				glGetProgramInfoLog(program_id,log_length,NULL,log_buffer);
				ret=glGetError();
				if(ret)
				{
					debugNetPrintf(ERROR,"[ORBISGL] glGetProgramInfoLog failed: 0x%08X\n",ret);
					return;
				}
				debugNetPrintf(DEBUG,"[ORBISGL] program link with log:\n%s\n",log_buffer);
			}
			else
			{
				debugNetPrintf(DEBUG,"[ORBISGL] program link no log\n");
			}
		}
	}
}
GLuint orbisGlCompileShader(const GLenum type,const GLchar* source) 
{
	GLuint shader_id;
	GLint compile_status;
	int ret;
	
	if(orbisGlConf)
	{
		if(orbisGlConf->orbisgl_initialized==1)
		{
			shader_id=glCreateShader(type);
			
			if(!shader_id)
			{
				ret=glGetError();
				debugNetPrintf(ERROR,"[ORBISGL] glCreateShader failed: 0x%08X\n",ret);
				return 0;
			}
			glShaderSource(shader_id,1,(const GLchar **)&source,NULL);
			ret=glGetError();
			if(ret)
			{
				debugNetPrintf(ERROR,"[ORBISGL] glShaderSource failed: 0x%08X\n",ret);
				//avoid this by now some weird behaviour
				//return 0;
			}
			glCompileShader(shader_id);
			ret=glGetError();
			if(ret)
			{	
				debugNetPrintf(ERROR,"[ORBISGL] glCompileShader failed: 0x%08X\n",ret);
				return 0;
			}
			glGetShaderiv(shader_id,GL_COMPILE_STATUS,&compile_status);
			ret=glGetError();
			if(ret)
			{
				debugNetPrintf(ERROR,"[ORBISGL] glGetShaderiv failed: 0x%08X\n",ret);
				return 0;
			}
			if (!compile_status)
			{
					
					debugNetPrintf(ERROR,"[ORBISGL] shader compilation failed\n");
					orbisGlShaderLog(shader_id);
					return 0;
			}
			debugNetPrintf(DEBUG,"[ORBISGL] shader compilation shader_id=%d done\n",shader_id);
			orbisGlShaderLog(shader_id);
			return shader_id;
		}
	}
	debugNetPrintf(ERROR,"[ORBISGL] orbisGl is not initialized\n");
	
	return 0;
}
GLuint orbisGlLinkProgram(const GLuint vertex_shader,const GLuint fragment_shader)
{
	GLuint program_id;
	int ret;
	GLint link_status;
	if(orbisGlConf)
	{
		if(orbisGlConf->orbisgl_initialized==1)
		{
			program_id=glCreateProgram();
			if(!program_id)
			{
				ret=glGetError();
				debugNetPrintf(ERROR,"[ORBIS_GL] glCreateProgram failed: 0x%08X\n", ret);
				return 0;
			}
			glAttachShader(program_id,vertex_shader);
			ret=glGetError();
			if(ret)
			{
				debugNetPrintf(ERROR,"[ORBISGL] glAttachShader(vertex_shader) failed: 0x%08X\n",ret);
				return 0;
			}
			glAttachShader(program_id,fragment_shader);
			ret=glGetError();
			if(ret)
			{
				debugNetPrintf(ERROR,"[ORBISGL] glAttachShader(fragment_shader) failed: 0x%08X\n",ret);
				return 0;
			}
			glLinkProgram(program_id);
			ret=glGetError();
			if(ret)
			{
				debugNetPrintf(ERROR,"[ORBISGL] glLinkProgram() failed: 0x%08X\n",ret);
				return 0;
			}
			glGetProgramiv(program_id, GL_LINK_STATUS,&link_status);
			if (!link_status)
			{
				debugNetPrintf(ERROR,"[ORBISGL] program link failed\n");
				orbisGlProgramLog(program_id);
				return 0;
			}
			debugNetPrintf(INFO,"[ORBISGL] link program_id=%d done\n",program_id);
			orbisGlProgramLog(program_id);
			return program_id;
		}
	}
	debugNetPrintf(ERROR,"[ORBISGL] orbisGl is not initialized\n");
	
	return 0;
}


GLuint orbisGlCreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename)
{
    debugNetPrintf(DEBUG,"[ORBISGL] orbisGlLoadShaders %s %s\n",vertexShaderFilename,fragmentShaderFilename);
	char * vsSource=(char *)orbisFileGetFileContent(vertexShaderFilename);
	if(vsSource==NULL)
	{
	    debugNetPrintf(DEBUG,"[ORBISGL] can't open vertex shader at %s\n",vertexShaderFilename);
		return 0;
	}
	GLuint vs=orbisGlCompileShader(GL_VERTEX_SHADER,vsSource);
	if(vs==0)
	{
	    debugNetPrintf(DEBUG,"[ORBISGL] can't compile vertex shader at %s\n",vertexShaderFilename);
		return 0;
	}
	free(vsSource);
	char * fsSource=(char *)orbisFileGetFileContent(fragmentShaderFilename);
	if(fsSource==NULL)
	{
	    debugNetPrintf(DEBUG,"[ORBISGL] can't open fragment shader at %s\n",vertexShaderFilename);
		return 0;
	}
	GLuint fs=orbisGlCompileShader(GL_FRAGMENT_SHADER,fsSource);
	if(fs==0)
	{
	    debugNetPrintf(DEBUG,"[ORBISGL] can't compile fragment shader at %s\n",vertexShaderFilename);
	}
	free(fsSource);
	GLuint po=orbisGlLinkProgram(vs,fs);
	if(po==0)
	{
	    debugNetPrintf(DEBUG,"[ORBISGL] can't link program with vertex shader %d and fragment shader %d\n",vs,fs);
		return 0;
	}
	glDeleteShader(vs);
	glDeleteShader(fs);
	
	return po;
}

GLuint orbisGlCreateTexture(const GLsizei width,const GLsizei height,const GLenum type,const GLvoid* pixels)
{
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	if(texture_id>0)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pixels);
		//glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	return texture_id;
}
void orbisGlDestroyTexture(GLuint texture_id)
{
	int ret;
	if(orbisGlConf)
	{
		if(orbisGlConf->orbisgl_initialized==1)
		{
		
			glDeleteTextures(1,&texture_id);
			ret=glGetError();
			if(ret)
			{
				debugNetPrintf(ERROR,"[ORBISGL] glDeleteTextures failed: 0x%08X\n",ret);
			}
		}
	}
}
void orbisGlDestroyDisplay()
{
	int ret;
	if(orbisGlConf)
	{
		if(orbisGlConf->orbisgl_initialized==1)
		{
			ret=eglTerminate(orbisGlConf->display);
			if(!ret)
			{
				ret=eglGetError();
				debugNetPrintf(ERROR,"[ORBISGL] eglTerminate failed: 0x%08X\n",ret);
			}
			orbisGlConf->display=EGL_NO_DISPLAY;
		}
	}
}
void orbisGlDestroySurface()
{
	int ret;
	if(orbisGlConf)
	{
		if(orbisGlConf->orbisgl_initialized==1)
		{
			ret=eglDestroySurface(orbisGlConf->display,orbisGlConf->surface);
			if (!ret)
			{
				ret=eglGetError();
				debugNetPrintf(ERROR,"[ORBISGL] eglDestroySurface failed: 0x%08X\n",ret);
			}
			orbisGlConf->surface=EGL_NO_SURFACE;
		}
	}
}
void orbisGlDestroyContext()
{
	int ret;
	if(orbisGlConf)
	{
		if(orbisGlConf->orbisgl_initialized==1)
		{		
			ret=eglDestroyContext(orbisGlConf->display,orbisGlConf->context);
			if (!ret)
			{
				ret=eglGetError();
				debugNetPrintf(ERROR,"[ORBISGL] eglDestroyContext failed: 0x%08X\n",ret);
			}
			orbisGlConf->context=EGL_NO_CONTEXT;
		}
	}
}
void orbisGlFinish()
{
	if(orbisgl_external_conf!=1)
	{
		if(orbisGlConf->orbisgl_initialized==1)
		{		
			orbisGlDestroyContext();
			orbisGlDestroySurface();
			orbisGlDestroyDisplay();			
		}
		orbisGlConf->orbisgl_initialized=-1;
		debugNetPrintf(DEBUG,"[ORBISGL] liborbisGl finished\n");
	}
}

OrbisGlConfig *orbisGlGetConf()
{
	if(orbisGlConf)
	{
		return orbisGlConf;
	}
	
	return NULL; 
}
int orbisGlCreateConf(unsigned int width,unsigned int height)
{
	if(!orbisGlConf)
	{
		orbisGlConf=(OrbisGlConfig *)malloc(sizeof(OrbisGlConfig));
		
		memset(&orbisGlConf->pgl_config, 0, sizeof(orbisGlConf->pgl_config));
		{
			orbisGlConf->pgl_config.size=sizeof(orbisGlConf->pgl_config);
			orbisGlConf->pgl_config.flags=SCE_PGL_FLAGS_USE_COMPOSITE_EXT | SCE_PGL_FLAGS_USE_FLEXIBLE_MEMORY | 0x60;
			orbisGlConf->pgl_config.processOrder=1;
			orbisGlConf->pgl_config.systemSharedMemorySize=0x200000;
			orbisGlConf->pgl_config.videoSharedMemorySize=0x2400000;
			orbisGlConf->pgl_config.maxMappedFlexibleMemory=0xAA00000;
			orbisGlConf->pgl_config.drawCommandBufferSize=0xC0000;
			orbisGlConf->pgl_config.lcueResourceBufferSize=0x10000;
			orbisGlConf->pgl_config.dbgPosCmd_0x40=ATTR_ORBISGL_WIDTH;
			orbisGlConf->pgl_config.dbgPosCmd_0x44=ATTR_ORBISGL_HEIGHT;
			orbisGlConf->pgl_config.dbgPosCmd_0x48=0;
			orbisGlConf->pgl_config.dbgPosCmd_0x4C=0;
			orbisGlConf->pgl_config.unk_0x5C=2;
		}
		orbisGlConf->display=EGL_NO_DISPLAY;
		orbisGlConf->surface=EGL_NO_SURFACE;
		orbisGlConf->context=EGL_NO_CONTEXT;
		orbisGlConf->config=NULL;
		orbisGlConf->width=width;
		orbisGlConf->height=height;
		orbisGlConf->orbisgl_initialized=-1;	
		return 0;
	}
	if(orbisGlConf->orbisgl_initialized==1)
	{
		return orbisGlConf->orbisgl_initialized;
	}
	//something weird happened
	return -1;			
}
int orbisGlSetConf(OrbisGlConfig *conf)
{
	if(conf)
	{
		orbisGlConf=conf;
		orbisgl_external_conf=1;
		return orbisGlConf->orbisgl_initialized;
	}
	return 0; 
}
int orbisGlInitWithConf(OrbisGlConfig *conf)
{
	int ret;
	ret=orbisGlSetConf(conf);
	if(ret)
	{
		debugNetPrintf(DEBUG,"[ORBISGL] liborbisGl already initialized using configuration external\n");
		debugNetPrintf(DEBUG,"[ORBISGL] orbisgl_initialized=%d\n",orbisGlConf->orbisgl_initialized);
		debugNetPrintf(DEBUG,"[ORBISGL] ready to have a lot of fun...\n");
		return orbisGlConf->orbisgl_initialized;
	}
	else
	{
		return 0;
	}
}
int orbisGlInit(unsigned int width,unsigned int height)
{
	int ret;
	int major,minor;
	
	SceWindow render_window={0,width,height};
	
	if(orbisGlCreateConf(width,height)==1)
	{
			return orbisGlConf->orbisgl_initialized;
	}
	if (orbisGlConf->orbisgl_initialized==1) 
	{
		debugNetPrintf(INFO,"[ORBISGL] liborbisGl is already initialized!\n");
		return orbisGlConf->orbisgl_initialized;
	}
	
	ret=scePigletSetConfigurationVSH(&orbisGlConf->pgl_config);
	if(!ret) 
	{
		debugNetPrintf(ERROR,"[ORBISGL] scePigletSetConfigurationVSH failed 0x%08X.\n",ret);
		return -1;
	}
	debugNetPrintf(INFO,"[ORBISGL] scePigletSetConfigurationVSH success.\n");
	
	orbisGlConf->display=eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if(orbisGlConf->display==EGL_NO_DISPLAY) 
	{
		debugNetPrintf(ERROR,"[ORBISGL] eglGetDisplay failed.\n");
		return -1;
	}
	debugNetPrintf(INFO,"[ORBISGL] eglGetDisplay success.\n");
	
	ret=eglInitialize(orbisGlConf->display,&major,&minor);
	if(!ret)
	{
		ret=eglGetError();
		debugNetPrintf(ERROR,"[ORBISGL] eglInitialize failed: 0x%08X\n",ret);
		return -1;
	}
	debugNetPrintf(INFO,"[ORBISGL] EGL version major:%d, minor:%d\n",major,minor);
	
	ret=eglBindAPI(EGL_OPENGL_ES_API);
	if(!ret)
	{
		ret=eglGetError();
		debugNetPrintf(ERROR,"[ORBISGL] eglBindAPI failed: 0x%08X\n",ret);
		return -1;
	}
	debugNetPrintf(INFO,"[ORBISGL] eglBindAPI success.\n");
	
	ret=eglSwapInterval(orbisGlConf->display,0);
	if(!ret)
	{
		ret=eglGetError();
		debugNetPrintf(ERROR,"[ORBISGL] eglSwapInterval failed: 0x%08X\n",ret);
		return -1;
	}
	debugNetPrintf(INFO,"[ORBISGL] eglSwapInterval success.\n");
	
	ret=eglChooseConfig(orbisGlConf->display,attribs,&orbisGlConf->config,1,&orbisGlConf->num_configs);
	if(!ret)
	{
		ret=eglGetError();
		debugNetPrintf(ERROR,"[ORBISGL] eglChooseConfig failed: 0x%08X\n",ret);
		return -1;
	}
	if (orbisGlConf->num_configs!=1)
	{
		debugNetPrintf(ERROR,"[ORBISGL] No available configuration found.\n");
		return -1;
	}
	debugNetPrintf(INFO,"[ORBISGL] eglChooseConfig success.\n");
	

	orbisGlConf->surface=eglCreateWindowSurface(orbisGlConf->display,orbisGlConf->config,&render_window,window_attribs);
	if(orbisGlConf->surface==EGL_NO_SURFACE)
	{
		ret=eglGetError();
		debugNetPrintf(ERROR,"[ORBISGL] eglCreateWindowSurface failed: 0x%08X\n",ret);
		return -1;
	}
	debugNetPrintf(INFO,"[ORBISGL] eglCreateWindowSurface success.\n");

	orbisGlConf->context=eglCreateContext(orbisGlConf->display,orbisGlConf->config,EGL_NO_CONTEXT,ctx_attribs);
	if(orbisGlConf->context==EGL_NO_CONTEXT)
	{
		ret=eglGetError();
		debugNetPrintf(ERROR,"[ORBISGL] eglCreateContext failed: 0x%08X\n", ret);
		return -1;
	}
	debugNetPrintf(INFO,"[ORBISGL] eglCreateContext success.\n");

	ret=eglMakeCurrent(orbisGlConf->display,orbisGlConf->surface,orbisGlConf->surface,orbisGlConf->context);
	if(!ret) 
	{
		ret=eglGetError();
		debugNetPrintf(ERROR,"[ORBISGL] eglMakeCurrent failed: 0x%08X\n",ret);
		return -1;
	}
	debugNetPrintf(INFO,"[ORBISGL] eglMakeCurrent success.\n");
	const char *gl_exts = (char *) glGetString(GL_EXTENSIONS);
	debugNetPrintf(INFO,"[ORBISGL] GL_VENDOR:   \"%s\"\n", glGetString(GL_VENDOR));
	debugNetPrintf(INFO,"[ORBISGL] GL_VERSION:  \"%s\"\n", glGetString(GL_VERSION));
	debugNetPrintf(INFO,"[ORBISGL] GL_RENDERER: \"%s\"\n", glGetString(GL_RENDERER));
	debugNetPrintf(INFO,"[ORBISGL] SL_VERSION:  \"%s\"\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	debugNetPrintf(INFO,"[ORBISGL] GL_EXTs:     \"%s\"\n", gl_exts);
	orbisGlConf->orbisgl_initialized=1;
	debugNetPrintf(INFO,"[ORBISGL] liborbisGl initialized\n");

	return orbisGlConf->orbisgl_initialized;
}