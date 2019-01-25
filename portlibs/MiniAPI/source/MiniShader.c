/*****************************************************************************
 * ==> MiniShader -----------------------------------------------------------*
 *****************************************************************************
 * Description : This module provides the functions to compile and manage    *
 *               shader programs, and provides several common and simple     *
 *               ready-to-use programs                                       *
 * Developer   : Jean-Milost Reymond                                         *
 * Copyright   : 2015 - 2018, this file is part of the Minimal API. You are  *
 *               free to copy or redistribute this file, modify it, or use   *
 *               it for your own projects, commercial or not. This file is   *
 *               provided "as is", without ANY WARRANTY OF ANY KIND          *
 *****************************************************************************/

#include "MiniShader.h"

// std
#include <stdio.h>

//----------------------------------------------------------------------------
// Common shader programs
//----------------------------------------------------------------------------
const char* miniGetVSColored(void)
{
    return "precision mediump float;"
           "attribute vec4 mini_vPosition;"
           "attribute vec4 mini_vColor;"
           "uniform   mat4 mini_uProjection;"
           "uniform   mat4 mini_uModelview;"
           "varying   vec4 mini_fColor;"
           "void main(void)"
           "{"
           "    mini_fColor = mini_vColor;"
           "    gl_Position = mini_uProjection * mini_uModelview * mini_vPosition;"
           "}";
}
//----------------------------------------------------------------------------
const char* miniGetFSColored(void)
{
    return "precision mediump float;"
           "varying lowp vec4 mini_fColor;"
           "void main(void)"
           "{"
           "    gl_FragColor = mini_fColor;"
           "}";
}
//----------------------------------------------------------------------------
const char* miniGetVSTextured(void)
{
    return "precision mediump float;"
           "attribute vec4 mini_vPosition;"
           "attribute vec4 mini_vColor;"
           "attribute vec2 mini_vTexCoord;"
           "uniform   mat4 mini_uProjection;"
           "uniform   mat4 mini_uModelview;"
           "varying   vec4 mini_fColor;"
           "varying   vec2 mini_fTexCoord;"
           "void main(void)"
           "{"
           "    mini_fColor    = mini_vColor;"
           "    mini_fTexCoord = mini_vTexCoord;"
           "    gl_Position    = mini_uProjection * mini_uModelview * mini_vPosition;"
           "}";
}
//----------------------------------------------------------------------------
const char* miniGetFSTextured(void)
{
    return "precision mediump float;"
           "uniform sampler2D mini_sColorMap;"
           "varying lowp vec4 mini_fColor;"
           "varying      vec2 mini_fTexCoord;"
           "void main(void)"
           "{"
           "    gl_FragColor = mini_fColor * texture2D(mini_sColorMap, mini_fTexCoord);"
           "}";
}
//----------------------------------------------------------------------------
const char* miniGetVSTexAlpha(void)
{
    return "precision mediump float;"
           "attribute vec4  mini_vPosition;"
           "attribute vec4  mini_vColor;"
           "attribute vec2  mini_vTexCoord;"
           "uniform   mat4  mini_uProjection;"
           "uniform   mat4  mini_uModelview;"
           "uniform   float mini_uAlpha;"
           "varying   vec4  mini_fColor;"
           "varying   vec2  mini_fTexCoord;"
           "varying   float mini_fAlpha;"
           "void main(void)"
           "{"
           "    mini_fColor    = mini_vColor;"
           "    mini_fTexCoord = mini_vTexCoord;"
           "    mini_fAlpha    = mini_uAlpha;"
           "    gl_Position    = mini_uProjection * mini_uModelview * mini_vPosition;"
           "}";
}
//----------------------------------------------------------------------------
const char* miniGetFSTexAlpha(void)
{
    return "precision mediump float;"
           "uniform sampler2D  mini_sColorMap;"
           "varying lowp vec4  mini_fColor;"
           "varying      vec2  mini_fTexCoord;"
           "varying      float mini_fAlpha;"
           "void main(void)"
           "{"
           "    vec4 color   = mini_fColor * texture2D(mini_sColorMap, mini_fTexCoord);"
           "    gl_FragColor = vec4(color.x, color.y, color.z, mini_fAlpha);"
           "}";
}
//----------------------------------------------------------------------------
// Shader compilation and linking functions
//----------------------------------------------------------------------------
GLuint miniCreateAndCompileShader(const char* pSource, GLenum shaderType)
{
    GLuint shaderHandle;
    GLint  compileSuccess;
    GLchar messages[256];
    printf("pSource : %p\n%s\n", pSource, pSource);
    // create and compile shader program
    shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &pSource, 0);    //craa
printf("shaderHandle : %d\n", shaderHandle);
    glCompileShader(shaderHandle);

    // get compiler result
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);

    // succeeded?
    if (compileSuccess == GL_FALSE)
    {
        // show error message (in console)
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        printf("compile glsl error : %s\n", messages);
    }

    return shaderHandle;
}
//----------------------------------------------------------------------------
GLuint miniCompileShaders(const char* pVShader,const char* pFShader)
{
    GLuint programHandle;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLint  linkSuccess;
    GLchar messages[256];

    // create and compile vertex and fragment shader programs
    vertexShader   = miniCreateAndCompileShader(pVShader, GL_VERTEX_SHADER);
    fragmentShader = miniCreateAndCompileShader(pFShader, GL_FRAGMENT_SHADER);

    // link shader programs
    programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);

    // get linker result
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);

    // succeeded?
    if (linkSuccess == GL_FALSE)
    {
        // show error message (in console)
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        printf("compile glsl error : %s\n", messages);
    }

    return programHandle;
}
//----------------------------------------------------------------------------
