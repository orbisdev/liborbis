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

#include "MiniVertex.h"

// std
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------
// Vertex functions
//----------------------------------------------------------------------------
void miniCalculateStride(MINI_VertexFormat* pVertexFormat)
{
    pVertexFormat->m_Stride = 3;

    // do use normals?
    if (pVertexFormat->m_UseNormals)
        pVertexFormat->m_Stride += 3;

    // do use textures?
    if (pVertexFormat->m_UseTextures)
        pVertexFormat->m_Stride += 2;

    // do use colors?
    if (pVertexFormat->m_UseColors)
        pVertexFormat->m_Stride += 4;
}
//----------------------------------------------------------------------------
void miniAddVertexToVB(MINI_VertexFormat* pVertexFormat,
                       MINI_Vector3*      pVertex,
                       MINI_Vector3*      pNormal,
                       MINI_Vector2*      pUV,
                       unsigned int       color,
                       float**            pVB,
                       unsigned int*      pVertexCount)
{
    unsigned int offset = *pVertexCount;

    // allocate memory for new vertex
    if (!(*pVertexCount))
        *pVB = (float*)malloc(pVertexFormat->m_Stride * sizeof(float));
    else
        *pVB = (float*)realloc(*pVB,
                             ((*pVertexCount) + pVertexFormat->m_Stride) * sizeof(float));

    // copy vertex from source
    (*pVB)[offset]     = pVertex->m_X;
    (*pVB)[offset + 1] = pVertex->m_Y;
    (*pVB)[offset + 2] = pVertex->m_Z;

    offset += 3;

    // do include normals?
    if (pVertexFormat->m_UseNormals)
    {
        // copy normal from source
        (*pVB)[offset]     = pNormal->m_X;
        (*pVB)[offset + 1] = pNormal->m_Y;
        (*pVB)[offset + 2] = pNormal->m_Z;

        offset += 3;
    }

    // do include texture coordinates?
    if (pVertexFormat->m_UseTextures)
    {
        // copy texture coordinates from source
        (*pVB)[offset]     = pUV->m_X;
        (*pVB)[offset + 1] = pUV->m_Y;

        offset += 2;
    }

    // do include colors?
    if (pVertexFormat->m_UseColors)
    {
        // set color data
        (*pVB)[offset]     = (float)((color >> 24) & 0xFF) / 255.0f;
        (*pVB)[offset + 1] = (float)((color >> 16) & 0xFF) / 255.0f;
        (*pVB)[offset + 2] = (float)((color >> 8)  & 0xFF) / 255.0f;
        (*pVB)[offset + 3] = (float) (color        & 0xFF) / 255.0f;
    }

    // update vertex count
    *pVertexCount += pVertexFormat->m_Stride;
}
//----------------------------------------------------------------------------
GLuint miniLoadTexture(const char* pFileName)
{
    FILE*          pFile;
    GLuint         index;
    unsigned int   dataOffset;
    unsigned int   headerSize;
    unsigned int   width;
    unsigned int   height;
    unsigned int   x;
    unsigned int   y;
    unsigned int   bytesPerRow;
    unsigned int   bitmapSize;
    unsigned char  c;
    unsigned short bpp;
    unsigned short compressed;
    unsigned char* pBitmapData;
    unsigned char* pPixels;
    unsigned char  intBuffer[4];
    unsigned char  shortBuffer[2];

    // open bitmap file
    pFile = M_MINI_FILE_OPEN((const char*)pFileName, "rb");

    // succeeded?
    if (!pFile)
        return GL_INVALID_VALUE;

    // read bitmap signature
    M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);

    // is bitmap signature correct?
    if (shortBuffer[0] != 'B' || shortBuffer[1] != 'M')
        return GL_INVALID_VALUE;

    // skip 8 next bytes
    M_MINI_FILE_SEEK(pFile, 8, SEEK_CUR);

    // read data offset
    M_MINI_FILE_READ(&intBuffer[0], sizeof(unsigned char), 4, pFile);
    dataOffset = (unsigned int)(((unsigned char)intBuffer[3] << 24) |
                                ((unsigned char)intBuffer[2] << 16) |
                                ((unsigned char)intBuffer[1] << 8)  |
                                 (unsigned char)intBuffer[0]);

    // read header size
    M_MINI_FILE_READ(&intBuffer[0], sizeof(unsigned char), 4, pFile);
    headerSize = (unsigned int)(((unsigned char)intBuffer[3] << 24) |
                                ((unsigned char)intBuffer[2] << 16) |
                                ((unsigned char)intBuffer[1] << 8)  |
                                 (unsigned char)intBuffer[0]);

    // search for bitmap type
    switch (headerSize)
    {
        // V3
        case 40:
        {
            // read bitmap width
            M_MINI_FILE_READ(&intBuffer[0], sizeof(unsigned char), 4, pFile);
            width = (unsigned int)(((unsigned char)intBuffer[3] << 24) |
                                   ((unsigned char)intBuffer[2] << 16) |
                                   ((unsigned char)intBuffer[1] << 8)  |
                                    (unsigned char)intBuffer[0]);

            // read bitmap height
            M_MINI_FILE_READ(&intBuffer[0], sizeof(unsigned char), 4, pFile);
            height = (unsigned int)(((unsigned char)intBuffer[3] << 24) |
                                    ((unsigned char)intBuffer[2] << 16) |
                                    ((unsigned char)intBuffer[1] << 8)  |
                                     (unsigned char)intBuffer[0]);

            // skip next 2 bytes
            M_MINI_FILE_SEEK(pFile, 2, SEEK_CUR);

            // read bitmap bit per pixels
            M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);
            bpp = (unsigned short)(((unsigned char)shortBuffer[1] << 8) |
                                    (unsigned char)shortBuffer[0]);

            // is bpp supported?
            if (bpp != 24)
                return GL_INVALID_VALUE;

            // read bitmap compressed flag
            M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);
            compressed = (unsigned short)(((unsigned char)shortBuffer[1] << 8) |
                                           (unsigned char)shortBuffer[0]);

            // is compressed?
            if (compressed)
                return GL_INVALID_VALUE;

            break;
        }

        // OS/2 V1
        case 12:
        {
            // read bitmap width
            M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);
            width = (unsigned int)(((unsigned char)shortBuffer[1] << 8) |
                                    (unsigned char)shortBuffer[0]);

            // read bitmap height
            M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);
            height = (unsigned int)(((unsigned char)shortBuffer[1] << 8) |
                                     (unsigned char)shortBuffer[0]);

            // skip next 2 bytes
            M_MINI_FILE_SEEK(pFile, 2, SEEK_CUR);

            // read bitmap bit per pixels
            M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);
            bpp = (unsigned short)(((unsigned char)shortBuffer[1] << 8) |
                                    (unsigned char)shortBuffer[0]);

            // is bpp supported?
            if (bpp != 24)
                return GL_INVALID_VALUE;

            break;
        }

        // Windows V4
        case 108:
        {
            // read bitmap width
            M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);
            width = (unsigned int)(((unsigned char)shortBuffer[1] << 8) |
                                    (unsigned char)shortBuffer[0]);

            // skip next 2 bytes
            M_MINI_FILE_SEEK(pFile, 2, SEEK_CUR);

            // read bitmap height
            M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);
            height = (unsigned int)(((unsigned char)shortBuffer[1] << 8) |
                                     (unsigned char)shortBuffer[0]);

            // skip next 4 bytes
            M_MINI_FILE_SEEK(pFile, 4, SEEK_CUR);

            // read bitmap bit per pixels
            M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);
            bpp = (unsigned short)(((unsigned char)shortBuffer[1] << 8) |
                                    (unsigned char)shortBuffer[0]);

            // is bpp supported?
            if (bpp != 24)
                return GL_INVALID_VALUE;

            break;
        }

        default:
            // unsupported bitmap format
            return GL_INVALID_VALUE;
    }

    bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
    bitmapSize  = bytesPerRow * height;
    pBitmapData = (unsigned char*)malloc(sizeof(unsigned char) * bitmapSize);

    // read bitmap data
    M_MINI_FILE_SEEK(pFile, dataOffset, SEEK_SET);
    M_MINI_FILE_READ(pBitmapData, sizeof(unsigned char), bitmapSize, pFile);

    // close file
    M_MINI_FILE_CLOSE(pFile);

    pPixels = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 3);

    // get bitmap data into right format
    for (y = 0; y < height; ++y)
        for (x = 0; x < width; ++x)
            for (c = 0; c < 3; ++c)
                pPixels[3 * (width * y + x) + c] =
                        pBitmapData[bytesPerRow * y + 3 * (width - x - 1) + (2 - c)];

    // create new OpenGL texture
    glGenTextures(1, &index);
    glBindTexture(GL_TEXTURE_2D, index);

    // set texture filtering
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // set texture wrapping mode
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // generate texture from bitmap data
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 width,
                 height,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 pPixels);

    // delete buffers
    free(pPixels);
    free(pBitmapData);

    return index;
}
//----------------------------------------------------------------------------
