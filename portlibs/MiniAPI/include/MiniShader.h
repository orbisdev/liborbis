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

#ifndef MiniShaderH
#define MiniShaderH

// std
#include <stdio.h>

// mini API
#include "MiniCommon.h"

//----------------------------------------------------------------------------
// Structures
//----------------------------------------------------------------------------

/**
* Shader structure
*/
typedef struct
{
    unsigned int m_VertexSlot;
    unsigned int m_NormalSlot;
    unsigned int m_TexCoordSlot;
    unsigned int m_ColorSlot;
    unsigned int m_ModelSlot;
} MINI_Shader;

#ifdef __cplusplus
    extern "C"
    {
#endif

        //----------------------------------------------------------------------------
        // Common shader programs
        //----------------------------------------------------------------------------

        /**
        * Gets a vertex shader program containing slots for position and color
        *@return the shader program
        */
        const char* miniGetVSColored(void);

        /**
        * Gets a fragment shader program containing slots for position and color
        *@return the shader program
        */
        const char* miniGetFSColored(void);

        /**
        * Gets a vertex shader program containing slots for position, color and texture
        *@return the shader program
        */
        const char* miniGetVSTextured(void);

        /**
        * Gets a fragment shader program containing slots for position, color and texture
        *@return the shader program
        */
        const char* miniGetFSTextured(void);

        /**
        * Gets a vertex shader program containing slots for position, color, texture and alpha blending value
        *@return the shader program
        */
        const char* miniGetVSTexAlpha(void);

        /**
        * Gets a fragment shader program containing slots for position, color, texture and alpha blending value
        *@return the shader program
        */
        const char* miniGetFSTexAlpha(void);

        //----------------------------------------------------------------------------
        // Shader compilation and linking functions
        //----------------------------------------------------------------------------

        /**
        * Creates a new shader program and compile it
        *@param pSource - source shader program to create and compile
        *@param shaderType - shader type to create, parameters are:
        *                    -> GL_VERTEX_SHADER for vertex shader
        *                    -> GL_FRAGMENT_SHADER for fragment shader
        *@return compiled shader program identifier
        *@note This function logs the errors returned by OpenGL shader compiler
        */
        GLuint miniCreateAndCompileShader(const char* pSource, GLenum shaderType);

        /**
        * Creates a shader program then uses it compiles and links vertex and fragment shaders
        *@param pVShader - source vertex shader program to compile
        *@param pFShader - source vertex shader program to compile
        *@return newly created vertex program identifier
        *@note This function logs the errors returned by OpenGL shader compiler
        *@note Program should be deleted when useless by calling glDeleteProgram()
        */
        GLuint miniCompileShaders(const char* pVShader,const char* pFShader);

#ifdef __cplusplus
    }
#endif

//----------------------------------------------------------------------------
// Compiler
//----------------------------------------------------------------------------

// needed in mobile c compiler to link the .h file with the .c
#if defined(_OS_IOS_) || defined(_OS_ANDROID_) || defined(_OS_WINDOWS_)
    #include "MiniShader.c"
#endif

#endif
