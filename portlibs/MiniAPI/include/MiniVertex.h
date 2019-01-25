/*****************************************************************************
 * ==> MiniVertex -----------------------------------------------------------*
 *****************************************************************************
 * Description : This module provides the functions required to manage       *
 *               geometrical vertices                                        *
 * Developer   : Jean-Milost Reymond                                         *
 * Copyright   : 2015 - 2018, this file is part of the Minimal API. You are  *
 *               free to copy or redistribute this file, modify it, or use   *
 *               it for your own projects, commercial or not. This file is   *
 *               provided "as is", without ANY WARRANTY OF ANY KIND          *
 *****************************************************************************/

#ifndef MiniVertexH
#define MiniVertexH

// mini API
#include "MiniCommon.h"
#include "MiniGeometry.h"

//----------------------------------------------------------------------------
// Structures
//----------------------------------------------------------------------------

/**
* Vertex format
*/
typedef struct
{
    int      m_UseNormals;
    int      m_UseTextures;
    int      m_UseColors;
    unsigned m_Stride;
} MINI_VertexFormat;

/**
* Vertex index
*/
typedef struct
{
    int m_Start;
    int m_Length;
    int m_GlCmd;
} MINI_Index;

/**
* Texture
*/
typedef struct
{
    unsigned char* m_pPixels;
    unsigned int   m_Length;
    int            m_Width;
    int            m_Height;
} MINI_Texture;

#ifdef __cplusplus
    extern "C"
    {
#endif

        //----------------------------------------------------------------------------
        // Vertex functions
        //----------------------------------------------------------------------------

        /**
        * Calculates vertex stride
        *@param[in, out] pVertexFormat - vertex format for which stride should be calculated
        */
        void miniCalculateStride(MINI_VertexFormat* pVertexFormat);

        /**
        * Adds a vertex to a vertex buffer
        *@param pVertexFormat - vertex format
        *@param pVertex - vertex to add
        *@param pNormal - normal to add
        *@param pUV - texture coordinate to add
        *@param color - vertex color
        *@param[out] pVB - vertex buffer to add to
        *@param[out] pVertexCount - vertex buffer count
        */
        void miniAddVertexToVB(MINI_VertexFormat* pVertexFormat,
                               MINI_Vector3*      pVertex,
                               MINI_Vector3*      pNormal,
                               MINI_Vector2*      pUV,
                               unsigned int       color,
                               float**            pVB,
                               unsigned int*      pVertexCount);

        /**
        * Loads texture from bitmap file
        *@param pFileName - bitmap file name to load from
        *@return newly generated texture index, GL_INVALID_VALUE on error
        *@note For now only most common bitmaps are loaded, some bitmap types may be unsupported
        */
        GLuint miniLoadTexture(const char* pFileName);

#ifdef __cplusplus
    }
#endif

//----------------------------------------------------------------------------
// Compiler
//----------------------------------------------------------------------------

// needed in mobile c compiler to link the .h file with the .c
#if defined(_OS_IOS_) || defined(_OS_ANDROID_) || defined(_OS_WINDOWS_)
    #include "MiniVertex.c"
#endif

#endif // MiniVertexH
