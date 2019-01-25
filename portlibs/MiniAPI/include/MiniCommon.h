/*****************************************************************************
 * ==> MiniCommon -----------------------------------------------------------*
 *****************************************************************************
 * Description : This module provides the common functions and declarations  *
 *               used by the whole API                                       *
 * Developer   : Jean-Milost Reymond                                         *
 * Copyright   : 2015 - 2018, this file is part of the Minimal API. You are  *
 *               free to copy or redistribute this file, modify it, or use   *
 *               it for your own projects, commercial or not. This file is   *
 *               provided "as is", without ANY WARRANTY OF ANY KIND          *
 *****************************************************************************/

#ifndef MiniCommonH
#define MiniCommonH

// openGL
#if defined(_OS_IOS_) || defined(_OS_ANDROID_) || defined(_OS_WINDOWS_)
    #include <gles2.h>
    #include <gles2ext.h>
#elif defined(__APPLE__)
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
#elif defined(__PS4__)
    #include <debugnet.h>
    #include <orbisGl.h>
#elif defined(__CODEGEARC__) || defined(__GNUC__)
    #include <GLES2/gl2.h>
    #include <EGL/egl.h>

    // missing in RAD studio OpenGL header
    #define GL_CLAMP_TO_EDGE 0x812F
#endif

//-----------------------------------------------------------------------------
// Global defines
//-----------------------------------------------------------------------------

#define M_MINI_VERSION 2.2

// __CCR__ and __CCR_MINOR__ macros may be undefined on older compiler versions,
// so define them if needed
#ifndef __CCR__
    #define __CCR__ 0
#endif

#ifndef __CCR_MINOR__
    #define __CCR_MINOR__ 0
#endif

//-----------------------------------------------------------------------------
// Common aliases
//-----------------------------------------------------------------------------
#if defined(__PS4__)
#include <ps4link.h>
#define M_MINI_FILE_OPEN  fopen
#define M_MINI_FILE_CLOSE fclose
#define M_MINI_FILE_READ  fread
#define M_MINI_FILE_SEEK  fseek
#else
#define M_MINI_FILE_OPEN  fopen
#define M_MINI_FILE_CLOSE fclose
#define M_MINI_FILE_READ  fread
#define M_MINI_FILE_SEEK  fseek
#endif

#ifdef __cplusplus
    extern "C"
    {
#endif

        //----------------------------------------------------------------------------
        // File functions
        //----------------------------------------------------------------------------

        /**
        * Gets the size of a file
        *@param pFileName - file name for which the size should be get
        *@return file size in bytes, 0 on error or if file is empty
        */
        long miniGetFileSize(const char* pFileName);

        //----------------------------------------------------------------------------
        // Math functions
        //----------------------------------------------------------------------------

        /**
        * Gets minimum value between 2 values
        *@param pA - first value to compare
        *@param pB - second value to compare
        *@return minimum value
        */
        void miniMin(const float* pA, const float* pB, float* pR);

        /**
        * Gets maximum value between 2 values
        *@param pA - first value to compare
        *@param pB - second value to compare
        *@return maximum value
        */
        void miniMax(const float* pA, const float* pB, float* pR);

        /**
        * Checks if a value is between 2 values
        *@param pV - value to test
        *@param pS - start value
        *@param pE - end value
        *@param pTolerance - tolerance for calculation
        *@return 1 if value is between values, otherwise 0
        */
        int miniValueIsBetween(const float* pV,
                               const float* pS,
                               const float* pE,
                               const float* pTolerance);

        /**
        * Rounds a value to the closest integer
        *@param pValue - value to round
        *@param[out] pR - rounded value
        */
        void miniRound(const float* pValue, float* pR);

        /**
        * Rounds a value to his next exponent
        *@param pValue - value to round
        *@param exp - exponent to which the value will be rounded
        *@param[out] pR - rounded value
        */
        void miniRoundToExp(const float* pValue, unsigned exp, float* pR);

#ifdef __cplusplus
    }
#endif

//----------------------------------------------------------------------------
// Compiler
//----------------------------------------------------------------------------

// needed in mobile c compiler to link the .h file with the .c
#if defined(_OS_IOS_) || defined(_OS_ANDROID_) || defined(_OS_WINDOWS_)
    #include "MiniCommon.c"
#endif

#endif
