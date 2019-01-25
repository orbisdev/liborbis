/*****************************************************************************
 * ==> MiniModels -----------------------------------------------------------*
 *****************************************************************************
 * Description : This module provides the functions to generate models like  *
 *               characters, landscapes, ...                                 *
 * Developer   : Jean-Milost Reymond                                         *
 * Copyright   : 2015 - 2018, this file is part of the Minimal API. You are  *
 *               free to copy or redistribute this file, modify it, or use   *
 *               it for your own projects, commercial or not. This file is   *
 *               provided "as is", without ANY WARRANTY OF ANY KIND          *
 *****************************************************************************/

#include "MiniModels.h"

// std
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#if defined(__PS4__)
#include <string.h>
#else
#include <memory.h>
#endif

//----------------------------------------------------------------------------
// Quake models normal table
//----------------------------------------------------------------------------
float g_NormalTable[] =
{
    -0.525731f,  0.000000f,  0.850651f,
    -0.442863f,  0.238856f,  0.864188f,
    -0.295242f,  0.000000f,  0.955423f,
    -0.309017f,  0.500000f,  0.809017f,
    -0.162460f,  0.262866f,  0.951056f,
     0.000000f,  0.000000f,  1.000000f,
     0.000000f,  0.850651f,  0.525731f,
    -0.147621f,  0.716567f,  0.681718f,
     0.147621f,  0.716567f,  0.681718f,
     0.000000f,  0.525731f,  0.850651f,
     0.309017f,  0.500000f,  0.809017f,
     0.525731f,  0.000000f,  0.850651f,
     0.295242f,  0.000000f,  0.955423f,
     0.442863f,  0.238856f,  0.864188f,
     0.162460f,  0.262866f,  0.951056f,
    -0.681718f,  0.147621f,  0.716567f,
    -0.809017f,  0.309017f,  0.500000f,
    -0.587785f,  0.425325f,  0.688191f,
    -0.850651f,  0.525731f,  0.000000f,
    -0.864188f,  0.442863f,  0.238856f,
    -0.716567f,  0.681718f,  0.147621f,
    -0.688191f,  0.587785f,  0.425325f,
    -0.500000f,  0.809017f,  0.309017f,
    -0.238856f,  0.864188f,  0.442863f,
    -0.425325f,  0.688191f,  0.587785f,
    -0.716567f,  0.681718f, -0.147621f,
    -0.500000f,  0.809017f, -0.309017f,
    -0.525731f,  0.850651f,  0.000000f,
     0.000000f,  0.850651f, -0.525731f,
    -0.238856f,  0.864188f, -0.442863f,
     0.000000f,  0.955423f, -0.295242f,
    -0.262866f,  0.951056f, -0.162460f,
     0.000000f,  1.000000f,  0.000000f,
     0.000000f,  0.955423f,  0.295242f,
    -0.262866f,  0.951056f,  0.162460f,
     0.238856f,  0.864188f,  0.442863f,
     0.262866f,  0.951056f,  0.162460f,
     0.500000f,  0.809017f,  0.309017f,
     0.238856f,  0.864188f, -0.442863f,
     0.262866f,  0.951056f, -0.162460f,
     0.500000f,  0.809017f, -0.309017f,
     0.850651f,  0.525731f,  0.000000f,
     0.716567f,  0.681718f,  0.147621f,
     0.716567f,  0.681718f, -0.147621f,
     0.525731f,  0.850651f,  0.000000f,
     0.425325f,  0.688191f,  0.587785f,
     0.864188f,  0.442863f,  0.238856f,
     0.688191f,  0.587785f,  0.425325f,
     0.809017f,  0.309017f,  0.500000f,
     0.681718f,  0.147621f,  0.716567f,
     0.587785f,  0.425325f,  0.688191f,
     0.955423f,  0.295242f,  0.000000f,
     1.000000f,  0.000000f,  0.000000f,
     0.951056f,  0.162460f,  0.262866f,
     0.850651f, -0.525731f,  0.000000f,
     0.955423f, -0.295242f,  0.000000f,
     0.864188f, -0.442863f,  0.238856f,
     0.951056f, -0.162460f,  0.262866f,
     0.809017f, -0.309017f,  0.500000f,
     0.681718f, -0.147621f,  0.716567f,
     0.850651f,  0.000000f,  0.525731f,
     0.864188f,  0.442863f, -0.238856f,
     0.809017f,  0.309017f, -0.500000f,
     0.951056f,  0.162460f, -0.262866f,
     0.525731f,  0.000000f, -0.850651f,
     0.681718f,  0.147621f, -0.716567f,
     0.681718f, -0.147621f, -0.716567f,
     0.850651f,  0.000000f, -0.525731f,
     0.809017f, -0.309017f, -0.500000f,
     0.864188f, -0.442863f, -0.238856f,
     0.951056f, -0.162460f, -0.262866f,
     0.147621f,  0.716567f, -0.681718f,
     0.309017f,  0.500000f, -0.809017f,
     0.425325f,  0.688191f, -0.587785f,
     0.442863f,  0.238856f, -0.864188f,
     0.587785f,  0.425325f, -0.688191f,
     0.688191f,  0.587785f, -0.425325f,
    -0.147621f,  0.716567f, -0.681718f,
    -0.309017f,  0.500000f, -0.809017f,
     0.000000f,  0.525731f, -0.850651f,
    -0.525731f,  0.000000f, -0.850651f,
    -0.442863f,  0.238856f, -0.864188f,
    -0.295242f,  0.000000f, -0.955423f,
    -0.162460f,  0.262866f, -0.951056f,
     0.000000f,  0.000000f, -1.000000f,
     0.295242f,  0.000000f, -0.955423f,
     0.162460f,  0.262866f, -0.951056f,
    -0.442863f, -0.238856f, -0.864188f,
    -0.309017f, -0.500000f, -0.809017f,
    -0.162460f, -0.262866f, -0.951056f,
     0.000000f, -0.850651f, -0.525731f,
    -0.147621f, -0.716567f, -0.681718f,
     0.147621f, -0.716567f, -0.681718f,
     0.000000f, -0.525731f, -0.850651f,
     0.309017f, -0.500000f, -0.809017f,
     0.442863f, -0.238856f, -0.864188f,
     0.162460f, -0.262866f, -0.951056f,
     0.238856f, -0.864188f, -0.442863f,
     0.500000f, -0.809017f, -0.309017f,
     0.425325f, -0.688191f, -0.587785f,
     0.716567f, -0.681718f, -0.147621f,
     0.688191f, -0.587785f, -0.425325f,
     0.587785f, -0.425325f, -0.688191f,
     0.000000f, -0.955423f, -0.295242f,
     0.000000f, -1.000000f,  0.000000f,
     0.262866f, -0.951056f, -0.162460f,
     0.000000f, -0.850651f,  0.525731f,
     0.000000f, -0.955423f,  0.295242f,
     0.238856f, -0.864188f,  0.442863f,
     0.262866f, -0.951056f,  0.162460f,
     0.500000f, -0.809017f,  0.309017f,
     0.716567f, -0.681718f,  0.147621f,
     0.525731f, -0.850651f,  0.000000f,
    -0.238856f, -0.864188f, -0.442863f,
    -0.500000f, -0.809017f, -0.309017f,
    -0.262866f, -0.951056f, -0.162460f,
    -0.850651f, -0.525731f,  0.000000f,
    -0.716567f, -0.681718f, -0.147621f,
    -0.716567f, -0.681718f,  0.147621f,
    -0.525731f, -0.850651f,  0.000000f,
    -0.500000f, -0.809017f,  0.309017f,
    -0.238856f, -0.864188f,  0.442863f,
    -0.262866f, -0.951056f,  0.162460f,
    -0.864188f, -0.442863f,  0.238856f,
    -0.809017f, -0.309017f,  0.500000f,
    -0.688191f, -0.587785f,  0.425325f,
    -0.681718f, -0.147621f,  0.716567f,
    -0.442863f, -0.238856f,  0.864188f,
    -0.587785f, -0.425325f,  0.688191f,
    -0.309017f, -0.500000f,  0.809017f,
    -0.147621f, -0.716567f,  0.681718f,
    -0.425325f, -0.688191f,  0.587785f,
    -0.162460f, -0.262866f,  0.951056f,
     0.442863f, -0.238856f,  0.864188f,
     0.162460f, -0.262866f,  0.951056f,
     0.309017f, -0.500000f,  0.809017f,
     0.147621f, -0.716567f,  0.681718f,
     0.000000f, -0.525731f,  0.850651f,
     0.425325f, -0.688191f,  0.587785f,
     0.587785f, -0.425325f,  0.688191f,
     0.688191f, -0.587785f,  0.425325f,
    -0.955423f,  0.295242f,  0.000000f,
    -0.951056f,  0.162460f,  0.262866f,
    -1.000000f,  0.000000f,  0.000000f,
    -0.850651f,  0.000000f,  0.525731f,
    -0.955423f, -0.295242f,  0.000000f,
    -0.951056f, -0.162460f,  0.262866f,
    -0.864188f,  0.442863f, -0.238856f,
    -0.951056f,  0.162460f, -0.262866f,
    -0.809017f,  0.309017f, -0.500000f,
    -0.864188f, -0.442863f, -0.238856f,
    -0.951056f, -0.162460f, -0.262866f,
    -0.809017f, -0.309017f, -0.500000f,
    -0.681718f,  0.147621f, -0.716567f,
    -0.681718f, -0.147621f, -0.716567f,
    -0.850651f,  0.000000f, -0.525731f,
    -0.688191f,  0.587785f, -0.425325f,
    -0.587785f,  0.425325f, -0.688191f,
    -0.425325f,  0.688191f, -0.587785f,
    -0.425325f, -0.688191f, -0.587785f,
    -0.587785f, -0.425325f, -0.688191f,
    -0.688191f, -0.587785f, -0.425325f
};
//----------------------------------------------------------------------------
// Quake 1 texture colors table
//----------------------------------------------------------------------------
unsigned char g_MDLColors[] =
{
    0,   0,   0,   15,  15,  15,  31,  31,  31,
    47,  47,  47,  63,  63,  63,  75,  75,  75,
    91,  91,  91,  107, 107, 107, 123, 123, 123,
    139, 139, 139, 155, 155, 155, 171, 171, 171,
    187, 187, 187, 203, 203, 203, 219, 219, 219,
    235, 235, 235, 15,  11,  7,   23,  15,  11,
    31,  23,  11,  39,  27,  15,  47,  35,  19,
    55,  43,  23,  63,  47,  23,  75,  55,  27,
    83,  59,  27,  91,  67,  31,  99,  75,  31,
    107, 83,  31,  115, 87,  31,  123, 95,  35,
    131, 103, 35,  143, 111, 35,  11,  11,  15,
    19,  19,  27,  27,  27,  39,  39,  39,  51,
    47,  47,  63,  55,  55,  75,  63,  63,  87,
    71,  71,  103, 79,  79,  115, 91,  91,  127,
    99,  99,  139, 107, 107, 151, 115, 115, 163,
    123, 123, 175, 131, 131, 187, 139, 139, 203,
    0,   0,   0,   7,   7,   0,   11,  11,  0,
    19,  19,  0,   27,  27,  0,   35,  35,  0,
    43,  43,  7,   47,  47,  7,   55,  55,  7,
    63,  63,  7,   71,  71,  7,   75,  75,  11,
    83,  83,  11,  91,  91,  11,  99,  99,  11,
    107, 107, 15,  7,   0,   0,   15,  0,   0,
    23,  0,   0,   31,  0,   0,   39,  0,   0,
    47,  0,   0,   55,  0,   0,   63,  0,   0,
    71,  0,   0,   79,  0,   0,   87,  0,   0,
    95,  0,   0,   103, 0,   0,   111, 0,   0,
    119, 0,   0,   127, 0,   0,   19,  19,  0,
    27,  27,  0,   35,  35,  0,   47,  43,  0,
    55,  47,  0,   67,  55,  0,   75,  59,  7,
    87,  67,  7,   95,  71,  7,   107, 75,  11,
    119, 83,  15,  131, 87,  19,  139, 91,  19,
    151, 95,  27,  163, 99,  31,  175, 103, 35,
    35,  19,  7,   47,  23,  11,  59,  31,  15,
    75,  35,  19,  87,  43,  23,  99,  47,  31,
    115, 55,  35,  127, 59,  43,  143, 67,  51,
    159, 79,  51,  175, 99,  47,  191, 119, 47,
    207, 143, 43,  223, 171, 39,  239, 203, 31,
    255, 243, 27,  11,  7,   0,   27,  19,  0,
    43,  35,  15,  55,  43,  19,  71,  51,  27,
    83,  55,  35,  99,  63,  43,  111, 71,  51,
    127, 83,  63,  139, 95,  71,  155, 107, 83,
    167, 123, 95,  183, 135, 107, 195, 147, 123,
    211, 163, 139, 227, 179, 151, 171, 139, 163,
    159, 127, 151, 147, 115, 135, 139, 103, 123,
    127, 91,  111, 119, 83,  99,  107, 75,  87,
    95,  63,  75,  87,  55,  67,  75,  47,  55,
    67,  39,  47,  55,  31,  35,  43,  23,  27,
    35,  19,  19,  23,  11,  11,  15,  7,   7,
    187, 115, 159, 175, 107, 143, 163, 95,  131,
    151, 87,  119, 139, 79,  107, 127, 75,  95,
    115, 67,  83,  107, 59,  75,  95,  51,  63,
    83,  43,  55,  71,  35,  43,  59,  31,  35,
    47,  23,  27,  35,  19,  19,  23,  11,  11,
    15,  7,   7,   219, 195, 187, 203, 179, 167,
    191, 163, 155, 175, 151, 139, 163, 135, 123,
    151, 123, 111, 135, 111, 95,  123, 99,  83,
    107, 87,  71,  95,  75,  59,  83,  63,  51,
    67,  51,  39,  55,  43,  31,  39,  31,  23,
    27,  19,  15,  15,  11,  7,   111, 131, 123,
    103, 123, 111, 95,  115, 103, 87,  107, 95,
    79,  99,  87,  71,  91,  79,  63,  83,  71,
    55,  75,  63,  47,  67,  55,  43,  59,  47,
    35,  51,  39,  31,  43,  31,  23,  35,  23,
    15,  27,  19,  11,  19,  11,  7,   11,  7,
    255, 243, 27,  239, 223, 23,  219, 203, 19,
    203, 183, 15,  187, 167, 15,  171, 151, 11,
    155, 131, 7,   139, 115, 7,   123, 99,  7,
    107, 83,  0,   91,  71,  0,   75,  55,  0,
    59,  43,  0,   43,  31,  0,   27,  15,  0,
    11,  7,   0,   0,   0,   255, 11,  11,  239,
    19,  19,  223, 27,  27,  207, 35,  35,  191,
    43,  43,  175, 47,  47,  159, 47,  47,  143,
    47,  47,  127, 47,  47,  111, 47,  47,  95,
    43,  43,  79,  35,  35,  63,  27,  27,  47,
    19,  19,  31,  11,  11,  15,  43,  0,   0,
    59,  0,   0,   75,  7,   0,   95,  7,   0,
    111, 15,  0,   127, 23,  7,   147, 31,  7,
    163, 39,  11,  183, 51,  15,  195, 75,  27,
    207, 99,  43,  219, 127, 59,  227, 151, 79,
    231, 171, 95,  239, 191, 119, 247, 211, 139,
    167, 123, 59,  183, 155, 55,  199, 195, 55,
    231, 227, 87,  127, 191, 255, 171, 231, 255,
    215, 255, 255, 103, 0,   0,   139, 0,   0,
    179, 0,   0,   215, 0,   0,   255, 0,   0,
    255, 243, 147, 255, 247, 199, 255, 255, 255,
    159, 91,  83
};
//----------------------------------------------------------------------------
// MDL functions
//----------------------------------------------------------------------------
void miniReadMDLHeader(FILE* pFile, MINI_MDLHeader* pHeader)
{
    // read header from file
    M_MINI_FILE_READ(&pHeader->m_ID,             sizeof(unsigned int),           1, pFile);
    M_MINI_FILE_READ(&pHeader->m_Version,        sizeof(unsigned int),           1, pFile);
    M_MINI_FILE_READ(&pHeader->m_Scale,          sizeof(pHeader->m_Scale),       1, pFile);
    M_MINI_FILE_READ(&pHeader->m_Translate,      sizeof(pHeader->m_Translate),   1, pFile);
    M_MINI_FILE_READ(&pHeader->m_BoundingRadius, sizeof(float),                  1, pFile);
    M_MINI_FILE_READ(&pHeader->m_EyePosition,    sizeof(pHeader->m_EyePosition), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_SkinCount,      sizeof(unsigned int),           1, pFile);
    M_MINI_FILE_READ(&pHeader->m_SkinWidth,      sizeof(unsigned int),           1, pFile);
    M_MINI_FILE_READ(&pHeader->m_SkinHeight,     sizeof(unsigned int),           1, pFile);
    M_MINI_FILE_READ(&pHeader->m_VertexCount,    sizeof(unsigned int),           1, pFile);
    M_MINI_FILE_READ(&pHeader->m_PolygonCount,   sizeof(unsigned int),           1, pFile);
    M_MINI_FILE_READ(&pHeader->m_FrameCount,     sizeof(unsigned int),           1, pFile);
    M_MINI_FILE_READ(&pHeader->m_SyncType,       sizeof(unsigned int),           1, pFile);
    M_MINI_FILE_READ(&pHeader->m_Flags,          sizeof(unsigned int),           1, pFile);
    M_MINI_FILE_READ(&pHeader->m_Size,           sizeof(float),                  1, pFile);
}
//----------------------------------------------------------------------------
void miniReadMDLSkin(FILE* pFile, const MINI_MDLHeader* pHeader, MINI_MDLSkin* pSkin)
{
    // calculate texture size
    pSkin->m_TexLen = pHeader->m_SkinWidth * pHeader->m_SkinHeight;

    M_MINI_FILE_READ(&pSkin->m_Group, sizeof(unsigned int), 1, pFile);

    pSkin->m_pTime = 0;

    // is a group of textures?
    if (!pSkin->m_Group)
    {
        pSkin->m_Count = 1;

        // create memory for texture
        pSkin->m_pData = (unsigned char*)malloc(pSkin->m_TexLen);

        // read texture from file
        M_MINI_FILE_READ(pSkin->m_pData, pSkin->m_TexLen, 1, pFile);

        return;
    }

    M_MINI_FILE_READ(&pSkin->m_Count, sizeof(unsigned int), 1, pFile);

    if (!pSkin->m_Count)
        return;

    // create memory for time table
    pSkin->m_pTime = (float*)malloc(pSkin->m_Count * sizeof(float));

    // read time table from file
    M_MINI_FILE_READ(pSkin->m_pTime, pSkin->m_Count * sizeof(float), 1, pFile);

    // create memory for texture
    pSkin->m_pData = (unsigned char*)malloc(pSkin->m_TexLen * pSkin->m_Count);

    // read texture from file
    M_MINI_FILE_READ(pSkin->m_pData, pSkin->m_TexLen * pSkin->m_Count, 1, pFile);
}
//----------------------------------------------------------------------------
void miniReadMDLTextureCoord(FILE* pFile, MINI_MDLTextureCoord* pTexCoord)
{
    // read texture coordinates from file
    M_MINI_FILE_READ(&pTexCoord->m_OnSeam, sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pTexCoord->m_U,      sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pTexCoord->m_V,      sizeof(unsigned int), 1, pFile);
}
//----------------------------------------------------------------------------
void miniReadMDLPolygon(FILE* pFile, MINI_MDLPolygon* pPolygon)
{
    // read polygon from file
    M_MINI_FILE_READ(&pPolygon->m_FacesFront,  sizeof(unsigned int),            1, pFile);
    M_MINI_FILE_READ(&pPolygon->m_VertexIndex, sizeof(pPolygon->m_VertexIndex), 1, pFile);
}
//----------------------------------------------------------------------------
void miniReadMDLVertex(FILE* pFile, MINI_MDLVertex* pVertex)
{
    // read vertex from file
    M_MINI_FILE_READ(&pVertex->m_Vertex,      sizeof(pVertex->m_Vertex), 1, pFile);
    M_MINI_FILE_READ(&pVertex->m_NormalIndex, sizeof(unsigned char),     1, pFile);
}
//----------------------------------------------------------------------------
void miniReadMDLFrame(FILE* pFile, const MINI_MDLHeader* pHeader, MINI_MDLFrame* pFrame)
{
    unsigned int i;

    // read frame bounding box
    miniReadMDLVertex(pFile, &pFrame->m_BoundingBoxMin);
    miniReadMDLVertex(pFile, &pFrame->m_BoundingBoxMax);

    // read frame name
    M_MINI_FILE_READ(&pFrame->m_Name, sizeof(char), 16, pFile);

    // create frame vertex buffer
    pFrame->m_pVertex =
            (MINI_MDLVertex*)malloc(sizeof(MINI_MDLVertex) * pHeader->m_VertexCount);

    // read frame vertices
    for (i = 0; i < pHeader->m_VertexCount; ++i)
        miniReadMDLVertex(pFile, &pFrame->m_pVertex[i]);
}
//----------------------------------------------------------------------------
void miniReadMDLFrameGroup(      FILE*               pFile,
                           const MINI_MDLHeader*     pHeader,
                                 MINI_MDLFrameGroup* pFrameGroup)
{
    M_MINI_FILE_READ(&pFrameGroup->m_Type, sizeof(unsigned int), 1, pFile);

    // is a single frame or a group of frames?
    if (!pFrameGroup->m_Type)
    {
        pFrameGroup->m_Count = 1;

        // create frame vertex buffer
        pFrameGroup->m_pFrame = (MINI_MDLFrame*)malloc(sizeof(MINI_MDLFrame) * pFrameGroup->m_Count);

        miniReadMDLFrame(pFile, pHeader, &pFrameGroup->m_pFrame[0]);
        return;
    }

    printf("NOT IMPLEMENTED");
}
//----------------------------------------------------------------------------
void miniUncompressMDLTexture(const MINI_MDLSkin* pSkin,
                                    unsigned int  index,
                                    MINI_Texture* pTexture)
{
    unsigned int offset;
    unsigned int i;

    // calculate texture length and start offset
    pTexture->m_Length = pSkin->m_TexLen;
    offset             = pTexture->m_Length * index;

    // allocate memory for the texture pixels
    pTexture->m_pPixels = (unsigned char*)malloc(sizeof(unsigned char) * pTexture->m_Length * 3);

    // convert indexed 8 bits texture to RGB 24 bits
    for (i = 0; i < pTexture->m_Length; ++i)
    {
        pTexture->m_pPixels[(i * 3)]     = g_MDLColors[pSkin->m_pData[offset + i] * 3];
        pTexture->m_pPixels[(i * 3) + 1] = g_MDLColors[pSkin->m_pData[offset + i] * 3 + 1];
        pTexture->m_pPixels[(i * 3) + 2] = g_MDLColors[pSkin->m_pData[offset + i] * 3 + 2];
    }
}
//----------------------------------------------------------------------------
void miniUncompressMDLVertex(const MINI_MDLHeader* pHeader,
                             const MINI_MDLVertex* pVertex,
                                   MINI_Vector3*   pResult)
{
    unsigned int i;
    float        vertex[3];

    // iterate through vertex coordinates
    for (i = 0; i < 3; ++i)
        // uncompress vertex using frame scale and translate values
        vertex[i] = (pHeader->m_Scale[i] * pVertex->m_Vertex[i]) + pHeader->m_Translate[i];

    // copy decompressed vertex to result
    pResult->m_X = vertex[0];
    pResult->m_Y = vertex[1];
    pResult->m_Z = vertex[2];
}
//----------------------------------------------------------------------------
int miniCreateMDLMesh(MINI_MDLHeader*       pHeader,
                      MINI_MDLFrameGroup*   pFrameGroups,
                      MINI_MDLSkin*         pSkin,
                      MINI_MDLTextureCoord* pTexCoord,
                      MINI_MDLPolygon*      pPolygon,
                      MINI_VertexFormat*    pVertexFormat,
                      unsigned              color,
                      MINI_MDLModel**       pMDLModel)
{
    unsigned int        index;
    unsigned int        vertexIndex;
    unsigned int        normalIndex;
    unsigned int        offset;
    unsigned int        vertexLength;
    int                 i;
    int                 j;
    int                 k;
    float               tu;
    float               tv;
    MINI_MDLFrameGroup* pSrcFrameGroup;
    MINI_MDLVertex*     pSrcVertex;
    MINI_Frame*         pMdlFrm;
    MINI_Mesh*          pMdlMesh;
    MINI_Vector3        vertex;

    // create MDL model
    *pMDLModel                    = (MINI_MDLModel*)    malloc(sizeof(MINI_MDLModel));
    (*pMDLModel)->m_pVertexFormat = (MINI_VertexFormat*)malloc(sizeof(MINI_VertexFormat));
    (*pMDLModel)->m_pFrame        = (MINI_Frame*)       malloc(sizeof(MINI_Frame) * pHeader->m_FrameCount);
    (*pMDLModel)->m_FrameCount    = pHeader->m_FrameCount;

    // calculate stride
    miniCalculateStride(pVertexFormat);

    // copy vertex format
    *((*pMDLModel)->m_pVertexFormat) = *pVertexFormat;

    vertexLength = sizeof(float) * pVertexFormat->m_Stride;

    // iterate through frames to create
    for (index = 0; index < pHeader->m_FrameCount; ++index)
    {
        // get source frame group from which meshes should be extracted
        pSrcFrameGroup = &pFrameGroups[index];

        // get current frame to populate
        pMdlFrm              = &(*pMDLModel)->m_pFrame[index];
        pMdlFrm->m_pMesh     = 0;
        pMdlFrm->m_MeshCount = 0;

        // iterate through meshes composing the frame
        for (i = 0; i < pSrcFrameGroup->m_Count; ++i)
        {
            // set mesh count
            ++pMdlFrm->m_MeshCount;

            // create new mesh
            if (!pMdlFrm->m_pMesh)
                pMdlFrm->m_pMesh = (MINI_Mesh*)malloc(sizeof(MINI_Mesh));
            else
                pMdlFrm->m_pMesh = (MINI_Mesh*)realloc(pMdlFrm->m_pMesh,
                                                       sizeof(MINI_Mesh) * pMdlFrm->m_MeshCount);

            // get current mesh to populate
            pMdlMesh = &pMdlFrm->m_pMesh[i];

            // populate newly created mesh
            pMdlMesh->m_pVertexBuffer = 0;
            pMdlMesh->m_VertexCount   = pHeader->m_PolygonCount * 3;

            // add new vertex
            if (!pMdlMesh->m_pVertexBuffer)
                pMdlMesh->m_pVertexBuffer = (float*)malloc(pMdlMesh->m_VertexCount * vertexLength);
            else
                pMdlMesh->m_pVertexBuffer = (float*)realloc(pMdlMesh->m_pVertexBuffer,
                                                            pMdlMesh->m_VertexCount * vertexLength);

            pMdlMesh->m_IsTriangleStrip = 0;

            vertexIndex = 0;

            // iterate through polygons to process
            for (j = 0; j < pHeader->m_PolygonCount; ++j)
                for (k = 0; k < 3; ++k)
                {
                    // get source vertex
                    pSrcVertex =
                            &pSrcFrameGroup->m_pFrame[i].m_pVertex[pPolygon[j].m_VertexIndex[k]];

                    // uncompress vertex
                    miniUncompressMDLVertex(pHeader, pSrcVertex, &vertex);

                    offset = vertexIndex;

                    // populate vertex
                    pMdlMesh->m_pVertexBuffer[offset]     = vertex.m_X;
                    pMdlMesh->m_pVertexBuffer[offset + 1] = vertex.m_Y;
                    pMdlMesh->m_pVertexBuffer[offset + 2] = vertex.m_Z;

                    offset += 3;

                    // do include normals?
                    if ((*pMDLModel)->m_pVertexFormat->m_UseNormals)
                    {
                        // calculate normal index in table
                        normalIndex = pSrcVertex->m_NormalIndex;

                        // extract normal
                        pMdlMesh->m_pVertexBuffer[offset]     = g_NormalTable[normalIndex];
                        pMdlMesh->m_pVertexBuffer[offset + 1] = g_NormalTable[normalIndex + 1];
                        pMdlMesh->m_pVertexBuffer[offset + 2] = g_NormalTable[normalIndex + 2];

                        offset += 3;
                    }

                    // do include texture coordinates?
                    if ((*pMDLModel)->m_pVertexFormat->m_UseTextures)
                    {
                        // get vertex texture coordinates
                        tu = pTexCoord[pPolygon[j].m_VertexIndex[k]].m_U;
                        tv = pTexCoord[pPolygon[j].m_VertexIndex[k]].m_V;

                        // is texture coordinate on the back face?
                        if (!pPolygon[j].m_FacesFront &&
                                pTexCoord[pPolygon[j].m_VertexIndex[k]].m_OnSeam)
                            // correct the texture coordinate to put it on the back face
                            tu += pHeader->m_SkinWidth * 0.5f;

                        // scale s and t to range from 0.0 to 1.0
                        tu = (tu + 0.5) / pHeader->m_SkinWidth;
                        tv = (tv + 0.5) / pHeader->m_SkinHeight;

                        // copy texture coordinates from source
                        pMdlMesh->m_pVertexBuffer[offset]     = tu;
                        pMdlMesh->m_pVertexBuffer[offset + 1] = tv;

                        offset += 2;
                    }

                    // do use colors?
                    if ((*pMDLModel)->m_pVertexFormat->m_UseColors)
                    {
                        pMdlMesh->m_pVertexBuffer[offset]     = (float)((color >> 24) & 0xFF) / 255.0f;
                        pMdlMesh->m_pVertexBuffer[offset + 1] = (float)((color >> 16) & 0xFF) / 255.0f;
                        pMdlMesh->m_pVertexBuffer[offset + 2] = (float)((color >> 8)  & 0xFF) / 255.0f;
                        pMdlMesh->m_pVertexBuffer[offset + 3] = (float)( color        & 0xFF) / 255.0f;
                    }

                    vertexIndex += pVertexFormat->m_Stride;
                }
        }
    }

    return 1;
}
//----------------------------------------------------------------------------
int miniLoadMDLModel(const unsigned char*     pName,
                           MINI_VertexFormat* pVertexFormat,
                           unsigned           color,
                           MINI_MDLModel**    pMDLModel,
                           MINI_Texture*      pTexture)
{
    FILE*                 pFile;
    MINI_MDLHeader*       pHeader;
    MINI_MDLSkin*         pSkin;
    MINI_MDLTextureCoord* pTexCoord;
    MINI_MDLPolygon*      pPolygon;
    MINI_MDLFrameGroup*   pFrameGroup;
    unsigned int          i;
    int                   result = 0;

    // open mdl file
    pFile = M_MINI_FILE_OPEN((const char*)pName, "rb");

    // succeeded?
    if (!pFile)
        return 0;

    // create mdl header
    pHeader = (MINI_MDLHeader*)malloc(sizeof(MINI_MDLHeader));

    // read file header
    miniReadMDLHeader(pFile, pHeader);

    // is mdl file and version correct?
    if ((pHeader->m_ID != M_MDL_ID) || ((float)pHeader->m_Version != M_MDL_Mesh_File_Version))
    {
        free(pHeader);
        return 0;
    }

    // read skins
    if (pHeader->m_SkinCount)
    {
        pSkin = (MINI_MDLSkin*)malloc(sizeof(MINI_MDLSkin) * pHeader->m_SkinCount);

        for (i = 0; i < pHeader->m_SkinCount; ++i)
            miniReadMDLSkin(pFile, pHeader, &pSkin[i]);
    }
    else
        pSkin = 0;

    // read texture coordinates
    if (pHeader->m_VertexCount)
    {
        pTexCoord = (MINI_MDLTextureCoord*)malloc(sizeof(MINI_MDLTextureCoord) * pHeader->m_VertexCount);

        for (i = 0; i < pHeader->m_VertexCount; ++i)
            miniReadMDLTextureCoord(pFile, &pTexCoord[i]);
    }
    else
        pTexCoord = 0;

    // read polygons
    if (pHeader->m_PolygonCount)
    {
        pPolygon = (MINI_MDLPolygon*)malloc(sizeof(MINI_MDLPolygon) * pHeader->m_PolygonCount);

        for (i = 0; i < pHeader->m_PolygonCount; ++i)
            miniReadMDLPolygon(pFile, &pPolygon[i]);
    }
    else
        pPolygon = 0;

    // read frames
    if (pHeader->m_FrameCount)
    {
        pFrameGroup = (MINI_MDLFrameGroup*)malloc(sizeof(MINI_MDLFrameGroup) * pHeader->m_FrameCount);

        for (i = 0; i < pHeader->m_FrameCount; ++i)
            miniReadMDLFrameGroup(pFile, pHeader, &pFrameGroup[i]);
    }
    else
        pFrameGroup = 0;

    // close MDL file
    M_MINI_FILE_CLOSE(pFile);

    // create mesh from file content
    result = miniCreateMDLMesh(pHeader,
                               pFrameGroup,
                               pSkin,
                               pTexCoord,
                               pPolygon,
                               pVertexFormat,
                               color,
                               pMDLModel);

    // extract texture from model
    miniUncompressMDLTexture(pSkin, 0, pTexture);

    // set texture size
    pTexture->m_Width  = pHeader->m_SkinWidth;
    pTexture->m_Height = pHeader->m_SkinHeight;

    // delete frame vertices
    for (i = 0; i < pHeader->m_FrameCount; ++i)
        if (pFrameGroup[i].m_pFrame)
        {
            free(pFrameGroup[i].m_pFrame->m_pVertex);
            free(pFrameGroup[i].m_pFrame);
        }

    // delete skin time table
    if (pSkin->m_pTime)
        free(pSkin->m_pTime);

    // delete skin data
    if (pSkin->m_pData)
        free(pSkin->m_pData);

    // delete MD2 structures
    free(pHeader);
    free(pSkin);
    free(pTexCoord);
    free(pPolygon);
    free(pFrameGroup);

    return result;
}
//----------------------------------------------------------------------------
void miniReleaseMDLModel(MINI_MDLModel* pMDLModel)
{
    unsigned int i;
    unsigned int j;

    // no model to delete?
    if (!pMDLModel)
        return;

    // iterate through meshes and delete each mesh
    for (i = 0; i < pMDLModel->m_FrameCount; ++i)
    {
        if (!pMDLModel->m_pFrame[i].m_pMesh)
            continue;

        // iterate through mesh vertex buffers and delete each vertex buffer
        for (j = 0; j < pMDLModel->m_pFrame[i].m_MeshCount; ++j)
            if (pMDLModel->m_pFrame[i].m_pMesh[j].m_pVertexBuffer)
                free(pMDLModel->m_pFrame[i].m_pMesh[j].m_pVertexBuffer);

        // delete meshes, if exist
        free(pMDLModel->m_pFrame[i].m_pMesh);
    }

    // delete frame list, if exists
    if (pMDLModel->m_pFrame)
        free(pMDLModel->m_pFrame);

    // delete vertex format, if exists
    if (pMDLModel->m_pVertexFormat)
        free(pMDLModel->m_pVertexFormat);

    free(pMDLModel);
}
//----------------------------------------------------------------------------
// MD2 functions
//----------------------------------------------------------------------------
void miniReadMD2Header(FILE* pFile, MINI_MD2Header* pHeader)
{
    // read header from file
    M_MINI_FILE_READ(&pHeader->m_ID,                 sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_Version,            sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_SkinWidth,          sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_SkinHeight,         sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_FrameSize,          sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_SkinCount,          sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_VertexCount,        sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_TextureCoordCount,  sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_PolygonCount,       sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_GlCmdsCount,        sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_FrameCount,         sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_SkinOffset,         sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_TextureCoordOffset, sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_PolygonOffset,      sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_FrameOffset,        sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_GlCmdsOffset,       sizeof(unsigned int), 1, pFile);
    M_MINI_FILE_READ(&pHeader->m_EndOffset,          sizeof(unsigned int), 1, pFile);
}
//----------------------------------------------------------------------------
void miniReadMD2Skin(FILE* pFile, MINI_MD2Skin* pSkin)
{
    // read skin name from file
    M_MINI_FILE_READ(&pSkin->m_Name, sizeof(char), 64, pFile);
}
//----------------------------------------------------------------------------
void miniReadMD2Vertex(FILE* pFile, MINI_MD2Vertex* pVertex)
{
    // read vertex from file
    M_MINI_FILE_READ(&pVertex->m_Vertex,      sizeof(pVertex->m_Vertex), 1, pFile);
    M_MINI_FILE_READ(&pVertex->m_NormalIndex, sizeof(unsigned char),     1, pFile);
}
//----------------------------------------------------------------------------
void miniReadMD2TextureCoord(FILE* pFile, MINI_MD2TextureCoord* pTexCoord)
{
    // read texture coordinates from file
    M_MINI_FILE_READ(&pTexCoord->m_U, sizeof(unsigned short), 1, pFile);
    M_MINI_FILE_READ(&pTexCoord->m_V, sizeof(unsigned short), 1, pFile);
}
//----------------------------------------------------------------------------
void miniReadMD2Frame(FILE* pFile, const MINI_MD2Header* pHeader, MINI_MD2Frame* pFrame)
{
    unsigned int i;

    // read vertex transformations
    M_MINI_FILE_READ(&pFrame->m_Scale,     sizeof(pFrame->m_Scale),     1, pFile);
    M_MINI_FILE_READ(&pFrame->m_Translate, sizeof(pFrame->m_Translate), 1, pFile);

    // read frame name
    M_MINI_FILE_READ(&pFrame->m_Name, sizeof(char), 16, pFile);

    // create md2 vertex list
    pFrame->m_pVertex = (MINI_MD2Vertex*)malloc(sizeof(MINI_MD2Vertex) * pHeader->m_VertexCount);

    // read frame vertices
    for (i = 0; i < pHeader->m_VertexCount; ++i)
        miniReadMD2Vertex(pFile, &pFrame->m_pVertex[i]);
}
//----------------------------------------------------------------------------
void miniReadMD2Polygon(FILE* pFile, MINI_MD2Polygon* pPolygon)
{
    // read polygon from file
    M_MINI_FILE_READ(&pPolygon->m_VertexIndex,       sizeof(pPolygon->m_VertexIndex),       1, pFile);
    M_MINI_FILE_READ(&pPolygon->m_TextureCoordIndex, sizeof(pPolygon->m_TextureCoordIndex), 1, pFile);
}
//----------------------------------------------------------------------------
void miniUncompressMD2Vertex(const MINI_MD2Frame*  pFrame,
                             const MINI_MD2Vertex* pVertex,
                                   MINI_Vector3*   pResult)
{
    unsigned int i;
    float        vertex[3];

    // iterate through vertex coordinates
    for (i = 0; i < 3; ++i)
        // uncompress vertex using frame scale and translate values
        vertex[i] = (pFrame->m_Scale[i] * pVertex->m_Vertex[i]) + pFrame->m_Translate[i];

    // copy decompressed vertex to result
    pResult->m_X = vertex[0];
    pResult->m_Y = vertex[1];
    pResult->m_Z = vertex[2];
}
//----------------------------------------------------------------------------
int miniCreateMD2Mesh(MINI_MD2Header*       pHeader,
                      MINI_MD2Frame*        pFrames,
                      MINI_MD2Skin*         pSkin,
                      MINI_MD2TextureCoord* pTexCoord,
                      MINI_MD2Polygon*      pPolygon,
                      int*                  pGlCmds,
                      MINI_VertexFormat*    pVertexFormat,
                      unsigned              color,
                      MINI_MD2Model**       pMD2Model)
{
    unsigned int    index;
    unsigned int    meshIndex;
    unsigned int    vertexIndex;
    unsigned int    normalIndex;
    unsigned int    offset;
    unsigned int    vertexLength;
    int             i;
    int*            pCurGlCmds;
    MINI_MD2Frame*  pSrcFrame;
    MINI_MD2Vertex* pSrcVertex;
    MINI_Frame*     pMdlFrm;
    MINI_Mesh*      pMdlMesh;
    MINI_Vector3    vertex;

    // create MD2 model
    *pMD2Model                    = (MINI_MD2Model*)    malloc(sizeof(MINI_MD2Model));
    (*pMD2Model)->m_pVertexFormat = (MINI_VertexFormat*)malloc(sizeof(MINI_VertexFormat));
    (*pMD2Model)->m_pFrame        = (MINI_Frame*)       malloc(sizeof(MINI_Frame) * pHeader->m_FrameCount);
    (*pMD2Model)->m_FrameCount    = pHeader->m_FrameCount;

    // calculate stride
    miniCalculateStride(pVertexFormat);

    // copy vertex format
    *((*pMD2Model)->m_pVertexFormat) = *pVertexFormat;

    vertexLength = sizeof(float) * pVertexFormat->m_Stride;

    // iterate through frames to create
    for (index = 0; index < pHeader->m_FrameCount; ++index)
    {
        // get source frame from which meshes should be extracted
        pSrcFrame = &pFrames[index];

        // get current frame to populate
        pMdlFrm              = &(*pMD2Model)->m_pFrame[index];
        pMdlFrm->m_pMesh     = 0;
        pMdlFrm->m_MeshCount = 0;

        pCurGlCmds =  pGlCmds;
        i          = *pCurGlCmds;
        meshIndex  =  0;

        // iterate through OpenGL commands (negative value is for triangle fan,
        // positive value is for triangle strip, 0 means list end)
        while (i)
        {
            // the first command is the number of vertices to process, already processed,
            // so skip it
            ++pCurGlCmds;

            // set mesh count
            pMdlFrm->m_MeshCount = meshIndex + 1;

            // create new mesh
            if (!pMdlFrm->m_pMesh)
                pMdlFrm->m_pMesh = (MINI_Mesh*)malloc(sizeof(MINI_Mesh));
            else
                pMdlFrm->m_pMesh = (MINI_Mesh*)realloc(pMdlFrm->m_pMesh,
                                                       sizeof(MINI_Mesh) * pMdlFrm->m_MeshCount);

            // get current mesh to populate
            pMdlMesh = &pMdlFrm->m_pMesh[meshIndex];

            // populate newly created mesh
            pMdlMesh->m_pVertexBuffer = 0;
            pMdlMesh->m_VertexCount   = 0;

            // determine vertices type to draw
            if (i < 0)
            {
                pMdlMesh->m_IsTriangleStrip = 0;
                i = -i;
            }
            else
                pMdlMesh->m_IsTriangleStrip = 1;

            vertexIndex = 0;

            // iterate through OpenGL commands to process
            for (; i > 0; --i)
            {
                ++pMdlMesh->m_VertexCount;

                // add new vertex
                if (!pMdlMesh->m_pVertexBuffer)
                    pMdlMesh->m_pVertexBuffer = (float*)malloc(vertexLength);
                else
                    pMdlMesh->m_pVertexBuffer = (float*)realloc(pMdlMesh->m_pVertexBuffer,
                                                                pMdlMesh->m_VertexCount * vertexLength);

                // get source vertex
                pSrcVertex = &pSrcFrame->m_pVertex[pCurGlCmds[2]];

                // uncompress vertex
                miniUncompressMD2Vertex(pSrcFrame, pSrcVertex, &vertex);

                offset = vertexIndex;

                // populate vertex
                pMdlMesh->m_pVertexBuffer[offset]     = vertex.m_X;
                pMdlMesh->m_pVertexBuffer[offset + 1] = vertex.m_Y;
                pMdlMesh->m_pVertexBuffer[offset + 2] = vertex.m_Z;

                offset += 3;

                // do include normals?
                if ((*pMD2Model)->m_pVertexFormat->m_UseNormals)
                {
                    // calculate normal index in table
                    normalIndex = pFrames[index].m_pVertex[pCurGlCmds[2]].m_NormalIndex * 3;

                    // extract normal
                    pMdlMesh->m_pVertexBuffer[offset]     = g_NormalTable[normalIndex];
                    pMdlMesh->m_pVertexBuffer[offset + 1] = g_NormalTable[normalIndex + 1];
                    pMdlMesh->m_pVertexBuffer[offset + 2] = g_NormalTable[normalIndex + 2];

                    offset += 3;
                }

                // do include texture coordinates?
                if ((*pMD2Model)->m_pVertexFormat->m_UseTextures)
                {
                    // copy texture coordinates from source
                    pMdlMesh->m_pVertexBuffer[offset]     = ((float*)pCurGlCmds)[0];
                    pMdlMesh->m_pVertexBuffer[offset + 1] = ((float*)pCurGlCmds)[1];

                    offset += 2;
                }

                // do use colors?
                if ((*pMD2Model)->m_pVertexFormat->m_UseColors)
                {
                    pMdlMesh->m_pVertexBuffer[offset]     = (float)((color >> 24) & 0xFF) / 255.0f;
                    pMdlMesh->m_pVertexBuffer[offset + 1] = (float)((color >> 16) & 0xFF) / 255.0f;
                    pMdlMesh->m_pVertexBuffer[offset + 2] = (float)((color >> 8)  & 0xFF) / 255.0f;
                    pMdlMesh->m_pVertexBuffer[offset + 3] = (float)( color        & 0xFF) / 255.0f;
                }

                vertexIndex += pVertexFormat->m_Stride;

                // go to next OpenGL command
                pCurGlCmds += 3;
            }

            ++meshIndex;

            // get next OpenGL command
            i = *pCurGlCmds;
        }
    }

    return 1;
}
//----------------------------------------------------------------------------
int miniLoadMD2Model(const unsigned char*     pName,
                           MINI_VertexFormat* pVertexFormat,
                           unsigned           color,
                           MINI_MD2Model**    pMD2Model)
{
    FILE*                 pFile;
    MINI_MD2Header*       pHeader;
    MINI_MD2Skin*         pSkin;
    MINI_MD2TextureCoord* pTexCoord;
    MINI_MD2Polygon*      pPolygon;
    int*                  pGlCmds;
    MINI_MD2Frame*        pFrame;
    unsigned int          i;
    int                   result = 0;

    // open md2 file
    pFile = M_MINI_FILE_OPEN((const char*)pName, "rb");

    // succeeded?
    if (!pFile)
        return 0;

    // create md2 header
    pHeader = (MINI_MD2Header*)malloc(sizeof(MINI_MD2Header));

    // read file header
    miniReadMD2Header(pFile, pHeader);

    // is md2 file and version correct?
    if ((pHeader->m_ID != M_MD2_ID) || ((float)pHeader->m_Version != M_MD2_Mesh_File_Version))
    {
        free(pHeader);
        return 0;
    }

    // create mesh buffers
    pSkin     = (MINI_MD2Skin*)        malloc(sizeof(MINI_MD2Skin)         * pHeader->m_SkinCount);
    pTexCoord = (MINI_MD2TextureCoord*)malloc(sizeof(MINI_MD2TextureCoord) * pHeader->m_TextureCoordCount);
    pPolygon  = (MINI_MD2Polygon*)     malloc(sizeof(MINI_MD2Polygon)      * pHeader->m_PolygonCount);
    pGlCmds   = (int*)                 malloc(sizeof(int)                  * pHeader->m_GlCmdsCount);
    pFrame    = (MINI_MD2Frame*)       malloc(sizeof(MINI_MD2Frame)        * pHeader->m_FrameCount);

    // go to skin offset
    M_MINI_FILE_SEEK(pFile, pHeader->m_SkinOffset, SEEK_SET);

    // read skins
    for (i = 0; i < pHeader->m_SkinCount; ++i)
        miniReadMD2Skin(pFile, &pSkin[i]);

    // go to texture coordinates offset
    M_MINI_FILE_SEEK(pFile, pHeader->m_TextureCoordOffset, SEEK_SET);

    // read texture coordinates
    for (i = 0; i < pHeader->m_TextureCoordCount; ++i)
        miniReadMD2TextureCoord(pFile, &pTexCoord[i]);

    // go to polygons offset
    M_MINI_FILE_SEEK(pFile, pHeader->m_PolygonOffset, SEEK_SET);

    // read polygons
    for (i = 0; i < pHeader->m_PolygonCount; ++i)
        miniReadMD2Polygon(pFile, &pPolygon[i]);

    // read OpenGL commands
    M_MINI_FILE_SEEK(pFile, pHeader->m_GlCmdsOffset, SEEK_SET);
    M_MINI_FILE_READ(pGlCmds, sizeof(unsigned int), pHeader->m_GlCmdsCount, pFile);

    // go to frames offset
    M_MINI_FILE_SEEK(pFile, pHeader->m_FrameOffset, SEEK_SET);

    // read frames
    for (i = 0; i < pHeader->m_FrameCount; ++i)
        miniReadMD2Frame(pFile, pHeader, &pFrame[i]);

    // close MD2 file
    M_MINI_FILE_CLOSE(pFile);

    // create mesh from file content
    result = miniCreateMD2Mesh(pHeader,
                               pFrame,
                               pSkin,
                               pTexCoord,
                               pPolygon,
                               pGlCmds,
                               pVertexFormat,
                               color,
                               pMD2Model);

    // delete frame vertices
    for (i = 0; i < pHeader->m_FrameCount; ++i)
        if (pFrame[i].m_pVertex)
            free(pFrame[i].m_pVertex);

    // delete MD2 structures
    free(pHeader);
    free(pSkin);
    free(pTexCoord);
    free(pPolygon);
    free(pGlCmds);
    free(pFrame);

    return result;
}
//----------------------------------------------------------------------------
void miniReleaseMD2Model(MINI_MD2Model* pMD2Model)
{
    unsigned int i;
    unsigned int j;

    // no model to delete?
    if (!pMD2Model)
        return;

    // iterate through meshes and delete each mesh
    for (i = 0; i < pMD2Model->m_FrameCount; ++i)
    {
        if (!pMD2Model->m_pFrame[i].m_pMesh)
            continue;

        // iterate through mesh vertex buffers and delete each vertex buffer
        for (j = 0; j < pMD2Model->m_pFrame[i].m_MeshCount; ++j)
            if (pMD2Model->m_pFrame[i].m_pMesh[j].m_pVertexBuffer)
                free(pMD2Model->m_pFrame[i].m_pMesh[j].m_pVertexBuffer);

        // delete meshes, if exist
        free(pMD2Model->m_pFrame[i].m_pMesh);
    }

    // delete frame list, if exists
    if (pMD2Model->m_pFrame)
        free(pMD2Model->m_pFrame);

    // delete vertex format, if exists
    if (pMD2Model->m_pVertexFormat)
        free(pMD2Model->m_pVertexFormat);

    free(pMD2Model);
}
//----------------------------------------------------------------------------
// Landscape creation functions
//----------------------------------------------------------------------------
int miniLoadLandscape(const unsigned char*  pFileName,
                            unsigned char** pData,
                            int*            pWidth,
                            int*            pHeight)
{
    FILE*          pFile;
    unsigned int   dataOffset;
    unsigned int   headerSize;
    unsigned int   bytesPerRow;
    unsigned int   bitmapSize;
    unsigned short bpp;
    unsigned short compressed;
    unsigned char  intBuffer[4];
    unsigned char  shortBuffer[2];

    // open bitmap file
    pFile = M_MINI_FILE_OPEN((const char*)pFileName, "rb");

    // succeeded?
    if (!pFile)
        return 0;

    // read bitmap signature
    M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);

    // is bitmap signature correct?
    if (shortBuffer[0] != 'B' || shortBuffer[1] != 'M')
        return 0;

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
            *pWidth = (unsigned int)(((unsigned char)intBuffer[3] << 24) |
                                     ((unsigned char)intBuffer[2] << 16) |
                                     ((unsigned char)intBuffer[1] << 8)  |
                                      (unsigned char)intBuffer[0]);

            // read bitmap height
            M_MINI_FILE_READ(&intBuffer[0], sizeof(unsigned char), 4, pFile);
            *pHeight = (unsigned int)(((unsigned char)intBuffer[3] << 24) |
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
                return 0;

            // read bitmap compressed flag
            M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);
            compressed = (unsigned short)(((unsigned char)shortBuffer[1] << 8) |
                                           (unsigned char)shortBuffer[0]);

            // is compressed?
            if (compressed)
                return 0;

            break;
        }

        // OS/2 V1
        case 12:
        {
            // read bitmap width
            M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);
            *pWidth = (unsigned int)(((unsigned char)shortBuffer[1] << 8) |
                                      (unsigned char)shortBuffer[0]);

            // read bitmap height
            M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);
            *pHeight = (unsigned int)(((unsigned char)shortBuffer[1] << 8) |
                                       (unsigned char)shortBuffer[0]);

            // skip next 2 bytes
            M_MINI_FILE_SEEK(pFile, 2, SEEK_CUR);

            // read bitmap bit per pixels
            M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);
            bpp = (unsigned short)(((unsigned char)shortBuffer[1] << 8) |
                                    (unsigned char)shortBuffer[0]);

            // is bpp supported?
            if (bpp != 24)
                return 0;

            break;
        }

        // Windows V4
        case 108:
        {
            // read bitmap width
            M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);
            *pWidth = (unsigned int)(((unsigned char)shortBuffer[1] << 8) |
                                      (unsigned char)shortBuffer[0]);

            // skip next 2 bytes
            M_MINI_FILE_SEEK(pFile, 2, SEEK_CUR);

            // read bitmap height
            M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);
            *pHeight = (unsigned int)(((unsigned char)shortBuffer[1] << 8) |
                                       (unsigned char)shortBuffer[0]);

            // skip next 4 bytes
            M_MINI_FILE_SEEK(pFile, 4, SEEK_CUR);

            // read bitmap bit per pixels
            M_MINI_FILE_READ(&shortBuffer[0], sizeof(unsigned char), 2, pFile);
            bpp = (unsigned short)(((unsigned char)shortBuffer[1] << 8) |
                                    (unsigned char)shortBuffer[0]);

            // is bpp supported?
            if (bpp != 24)
                return 0;

            break;
        }

        default:
            // unsupported bitmap format
            return 0;
    }

    bytesPerRow = (((*pWidth) * 3 + 3) / 4) * 4 - ((*pWidth) * 3 % 4);
    bitmapSize  = bytesPerRow * (*pHeight);
    *pData      = (unsigned char*)malloc(sizeof(unsigned char) * bitmapSize);

    // read bitmap data
    M_MINI_FILE_SEEK(pFile, dataOffset, SEEK_SET);
    M_MINI_FILE_READ(*pData, sizeof(unsigned char), bitmapSize, pFile);

    // close file
    M_MINI_FILE_CLOSE(pFile);

    return 1;
}
//----------------------------------------------------------------------------
void miniGenerateLandscapeMesh(unsigned char* pData,
                               int            mapX,
                               int            mapZ,
                               float          height,
                               float          scale,
                               MINI_Vector3** pMesh,
                               unsigned int*  pMeshSize)
{
    int          x;
    int          z;
    unsigned int index;
    float        scaleX;
    float        scaleZ;

    // calculate lansdcape data size and reserve memory for landscape mesh
    *pMeshSize = mapX * mapZ;
    *pMesh     = (MINI_Vector3*)malloc(*pMeshSize * sizeof(MINI_Vector3));

    // calculate scaling factor on x and z axis
    scaleX = -(((mapX - 1) * scale) / 2.0f);
    scaleZ =  (((mapZ - 1) * scale) / 2.0f);

    // loop through heightfield points and calculate coordinates for each point
    for (z = 0; z < mapZ; ++z)
        for (x = 0; x < mapX; ++x)
        {
            // calculate vertex index
            index = (z * mapZ) + x;

            // calculate landscape vertex
            (*pMesh)[index].m_X = scaleX + ((float)x * scale);
            (*pMesh)[index].m_Y = ((float)pData[index * 3] / 255.0f) * height;
            (*pMesh)[index].m_Z = scaleZ - ((float)z * scale);
        }
}
//----------------------------------------------------------------------------
void miniCreateLandscape(unsigned char*     pData,
                         int                mapX,
                         int                mapZ,
                         float              height,
                         float              scale,
                         MINI_VertexFormat* pVertexFormat,
                         unsigned int       color,
                         MINI_Mesh**        pMesh)
{
    int           x;
    int           z;
    unsigned int  index;
    unsigned int  landscapeSize;
    MINI_Vector3* pLandscape;
    MINI_Vector3  v1;
    MINI_Vector3  v2;
    MINI_Vector3  v3;
    MINI_Vector3  v4;
    MINI_Vector3  n1;
    MINI_Vector3  n2;
    MINI_Vector2  uv1;
    MINI_Vector2  uv2;
    MINI_Vector2  uv3;
    MINI_Vector2  uv4;
    MINI_Plane    p1;
    MINI_Plane    p2;

    // by default stride is equals to 3 (for vertex XYZ position)
    pVertexFormat->m_Stride = 3;

    // do include normals?
    if (pVertexFormat->m_UseNormals)
        pVertexFormat->m_Stride += 3;

    // do include textures?
    if (pVertexFormat->m_UseTextures)
        pVertexFormat->m_Stride += 2;

    // do include colors?
    if (pVertexFormat->m_UseColors)
        pVertexFormat->m_Stride += 4;

    // generate landscape XYZ vertex from grayscale image
    miniGenerateLandscapeMesh(pData,
                              mapX,
                              mapZ,
                              height,
                              scale,
                              &pLandscape,
                              &landscapeSize);

    // create landscape mesh
    *pMesh = (MINI_Mesh*)malloc(sizeof(MINI_Mesh));
    (*pMesh)->m_pVertexBuffer   = 0;
    (*pMesh)->m_VertexCount     = 0;
    (*pMesh)->m_IsTriangleStrip = 0;

    // loop through landscape XYZ vertices and generate mesh polygons
    for (z = 0; z < mapZ - 1; ++z)
        for (x = 0; x < mapX - 1; ++x)
        {
            // polygons are generated in the following order:
            // v1 -- v2
            //     /
            //    /
            // v3 -- v4

            // calculate vertex index
            index = (z * mapZ) + x;

            // calculate first vertex
            v1.m_X = pLandscape[index].m_X;
            v1.m_Y = pLandscape[index].m_Y;
            v1.m_Z = pLandscape[index].m_Z;

            // calculate second vertex
            v2.m_X = pLandscape[index + 1].m_X;
            v2.m_Y = pLandscape[index + 1].m_Y;
            v2.m_Z = pLandscape[index + 1].m_Z;

            // calculate next vertex index
            index = ((z + 1) * mapZ) + x;

            // calculate third vertex
            v3.m_X = pLandscape[index].m_X;
            v3.m_Y = pLandscape[index].m_Y;
            v3.m_Z = pLandscape[index].m_Z;

            // calculate fourth vertex
            v4.m_X = pLandscape[index + 1].m_X;
            v4.m_Y = pLandscape[index + 1].m_Y;
            v4.m_Z = pLandscape[index + 1].m_Z;

            // do include normals?
            if (pVertexFormat->m_UseNormals)
            {
                // calculate polygons planes
                miniPlaneFromPoints(&v1, &v2, &v3, &p1);
                miniPlaneFromPoints(&v2, &v3, &v4, &p2);

                // get first normal
                n1.m_X = p1.m_A;
                n1.m_Y = p1.m_B;
                n1.m_Z = p1.m_C;

                // get second normal
                n2.m_X = p2.m_A;
                n2.m_Y = p2.m_B;
                n2.m_Z = p2.m_C;
            }

            // do include colors?
            if (pVertexFormat->m_UseColors)
            {
                // generate texture coordinates
                uv1.m_X = 0.0f;
                uv1.m_Y = 0.0f;
                uv2.m_X = 1.0f;
                uv2.m_Y = 0.0f;
                uv3.m_X = 0.0f;
                uv3.m_Y = 1.0f;
                uv4.m_X = 1.0f;
                uv4.m_Y = 1.0f;
            }

            // add first polygon first vertex to buffer
            miniAddVertexToVB(pVertexFormat,
                              &v1,
                              &n1,
                              &uv1,
                              color,
                              &(*pMesh)->m_pVertexBuffer,
                              &(*pMesh)->m_VertexCount);

            // add first polygon second vertex to buffer
            miniAddVertexToVB(pVertexFormat,
                              &v2,
                              &n1,
                              &uv2,
                              color,
                              &(*pMesh)->m_pVertexBuffer,
                              &(*pMesh)->m_VertexCount);

            // add first polygon third vertex to buffer
            miniAddVertexToVB(pVertexFormat,
                              &v3,
                              &n1,
                              &uv3,
                              color,
                              &(*pMesh)->m_pVertexBuffer,
                              &(*pMesh)->m_VertexCount);

            // add second polygon first vertex to buffer
            miniAddVertexToVB(pVertexFormat,
                              &v2,
                              &n2,
                              &uv2,
                              color,
                              &(*pMesh)->m_pVertexBuffer,
                              &(*pMesh)->m_VertexCount);

            // add second polygon second vertex to buffer
            miniAddVertexToVB(pVertexFormat,
                              &v3,
                              &n2,
                              &uv3,
                              color,
                              &(*pMesh)->m_pVertexBuffer,
                              &(*pMesh)->m_VertexCount);

            // add second polygon third vertex to buffer
            miniAddVertexToVB(pVertexFormat,
                              &v4,
                              &n2,
                              &uv4,
                              color,
                              &(*pMesh)->m_pVertexBuffer,
                              &(*pMesh)->m_VertexCount);
        }

    // delete landscape XYZ vertices (no longer used as copied in mesh)
    if (pLandscape)
    {
        free(pLandscape);
        pLandscape = 0;
    }
}
//----------------------------------------------------------------------------
void miniReleaseLandscape(MINI_Mesh* pLandscape)
{
    if (!pLandscape)
        return;

    // delete the mesh vertex buffer
    if (pLandscape->m_pVertexBuffer)
        free(pLandscape->m_pVertexBuffer);

    // delete the mesh itself
    free(pLandscape);
}
//----------------------------------------------------------------------------
// Rover creation functions
//----------------------------------------------------------------------------
void miniCreateRover(MINI_VertexFormat* pVertexFormat,
                     float**            pVertices,
                     unsigned*          pVertexCount,
                     MINI_MdlCmds**     pMdlCmds,
                     MINI_Index**       pIndexes,
                     unsigned*          pIndexCount)
{
    // rover cockpit vertex buffer (part 1)
    const float roverCockpit_1[] =
    {
        // x    y      z     r      g      b      a
        -2.0f, -1.0f, -1.0f, 0.85f, 0.75f, 0.85f, 1.0f,
        -2.0f,  1.0f, -1.0f, 0.85f, 0.75f, 0.85f, 1.0f,
         2.0f, -1.0f, -1.0f, 0.85f, 0.75f, 0.85f, 1.0f,
         2.0f,  1.0f, -1.0f, 0.85f, 0.75f, 0.85f, 1.0f,
         2.0f,  1.0f,  1.0f, 0.85f, 0.75f, 0.85f, 1.0f,
        -2.0f,  1.0f, -1.0f, 0.85f, 0.75f, 0.85f, 1.0f,
        -2.0f,  1.0f,  1.0f, 0.85f, 0.75f, 0.85f, 1.0f,
        -2.0f, -1.0f,  1.0f, 0.85f, 0.75f, 0.85f, 1.0f,
         2.0f,  1.0f,  1.0f, 0.85f, 0.75f, 0.85f, 1.0f,
         2.0f, -1.0f,  1.0f, 0.85f, 0.75f, 0.85f, 1.0f,
         2.0f, -1.0f, -1.0f, 0.85f, 0.75f, 0.85f, 1.0f,
        -2.0f, -1.0f,  1.0f, 0.85f, 0.75f, 0.85f, 1.0f,
        -2.0f, -1.0f, -1.0f, 0.85f, 0.75f, 0.85f, 1.0f,
        -2.0f,  1.0f, -1.0f, 0.85f, 0.75f, 0.85f, 1.0f,
    };

    // rover cockpit vertex buffer (part 2)
    const float roverCockpit_2[] =
    {
        // x   y      z     r     g     b     a
        3.5f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        2.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        2.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        2.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        2.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        2.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    };

    // rover cockpit vertex buffer (part 3)
    const float roverCockpit_3[] =
    {
        // x    y      z     r     g     b     a
        -3.5f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -2.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -2.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -2.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -2.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -2.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    };

    // rover cockpit vertex buffer (part 4)
    const float roverCockpit_4[] =
    {
        // x    y    z      r      g      b      a
        1.0f, 1.0f, -0.25f, 0.45f, 0.35f, 0.45f, 1.0f,
        1.0f, 1.5f, -0.25f, 0.45f, 0.35f, 0.45f, 1.0f,
        1.5f, 1.0f, -0.25f, 0.45f, 0.35f, 0.45f, 1.0f,
        1.5f, 1.5f, -0.25f, 0.45f, 0.35f, 0.45f, 1.0f,
        1.5f, 1.5f,  0.25f, 0.45f, 0.35f, 0.45f, 1.0f,
        1.0f, 1.5f, -0.25f, 0.45f, 0.35f, 0.45f, 1.0f,
        1.0f, 1.5f,  0.25f, 0.45f, 0.35f, 0.45f, 1.0f,
        1.0f, 1.0f,  0.25f, 0.45f, 0.35f, 0.45f, 1.0f,
        1.5f, 1.5f,  0.25f, 0.45f, 0.35f, 0.45f, 1.0f,
        1.5f, 1.0f,  0.25f, 0.45f, 0.35f, 0.45f, 1.0f,
        1.5f, 1.0f, -0.25f, 0.45f, 0.35f, 0.45f, 1.0f,
        1.0f, 1.0f,  0.25f, 0.45f, 0.35f, 0.45f, 1.0f,
        1.0f, 1.0f, -0.25f, 0.45f, 0.35f, 0.45f, 1.0f,
        1.0f, 1.5f, -0.25f, 0.45f, 0.35f, 0.45f, 1.0f,
    };

    // rover cockpit vertex buffer (part 5)
    const float roverCockpit_5[] =
    {
        // x   y      z     r     g     b     a
        0.75f, 1.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.75f, 2.0f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.75f, 1.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.75f, 2.0f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.75f, 2.0f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.75f, 2.0f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.75f, 2.0f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.75f, 1.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.75f, 2.0f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.75f, 1.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
        1.75f, 1.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.75f, 1.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.75f, 1.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.75f, 2.0f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
    };

    // rover front left frame vertex buffer
    const float roverFrame_fl[] =
    {
        // x  y    z     r     g     b     a
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    };

    // rover front right frame vertex buffer
    const float roverFrame_fr[] =
    {
        // x  y      z     r     g     b     a
        0.0f, 0.0f, -1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
    };

    // rover front rear left vertex buffer
    const float roverFrame_rl[] =
    {
        // x  y     z     r     g     b     a
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    };

    // rover rear right frame vertex buffer
    const float roverFrame_rr[] =
    {
        // x  y      z     r     g     b     a
        0.0f, 0.0f, -1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.2f, 1.0f, 1.0f, 1.0f, 1.0f,
    };

    // rover front left wheel vertex buffer
    const float roverWheel_fl[] =
    {
        // x  y     z     r     g     b     a
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
    };

    // rover front right wheel vertex buffer
    const float roverWheel_fr[] =
    {
        // x  y      z     r     g     b     a
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
    };

    // rover rear left wheel vertex buffer
    const float roverWheel_rl[] =
    {
        // x  y     z     r     g     b     a
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
    };

    // rover rear right wheel vertex buffer
    const float roverWheel_rr[] =
    {
        // x  y      z     r     g     b     a
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -1.2f, 0.3f, 0.3f, 0.3f, 1.0f,
        0.0f, 0.0f, -2.2f, 0.3f, 0.3f, 0.3f, 1.0f,
    };

    const float     rover_frame_len_x =  0.1f;
    const float     rover_frame_len_y =  1.5f;
    const float     rover_frame_len   =  sqrtf(powf(rover_frame_len_x, 2.0f) + powf(rover_frame_len_y, 2.0f));
    #ifdef __CODEGEARC__
        const float rover_frame_angle =  asin(rover_frame_len_y / rover_frame_len);
    #else
        const float rover_frame_angle =  asinf(rover_frame_len_y / rover_frame_len);
    #endif
    const float     wheel_angle_1     =  (M_PI * 2.0f)         / 8.0f;
    const float     wheel_angle_2     = ((M_PI * 2.0f) * 2.0f) / 8.0f;
    const float     wheel_angle_3     = ((M_PI * 2.0f) * 3.0f) / 8.0f;
    const float     wheel_angle_4     = ((M_PI * 2.0f) * 4.0f) / 8.0f;
    const float     wheel_angle_5     = ((M_PI * 2.0f) * 5.0f) / 8.0f;
    const float     wheel_angle_6     = ((M_PI * 2.0f) * 6.0f) / 8.0f;
    const float     wheel_angle_7     = ((M_PI * 2.0f) * 7.0f) / 8.0f;
    const float     wheel_angle_8     =  (M_PI * 2.0f);
    const float     wheel_length      =  0.5f;
    #ifdef __CODEGEARC__
          float rover_frame_angle_1   =  rover_frame_angle                                       + asin(M_PI_2 / 3.0f);
          float rover_frame_angle_2   =  rover_frame_angle + (M_PI_2 - rover_frame_angle)        + asin(M_PI_2 / 3.0f);
          float rover_frame_angle_3   =  rover_frame_angle +  M_PI                               + asin(M_PI_2 / 3.0f);
          float rover_frame_angle_4   =  rover_frame_angle + (M_PI_2 - rover_frame_angle) + M_PI + asin(M_PI_2 / 3.0f);
    #else
          float rover_frame_angle_1   =  rover_frame_angle                                       + asinf(M_PI_2 / 3.0f);
          float rover_frame_angle_2   =  rover_frame_angle + (M_PI_2 - rover_frame_angle)        + asinf(M_PI_2 / 3.0f);
          float rover_frame_angle_3   =  rover_frame_angle +  M_PI                               + asinf(M_PI_2 / 3.0f);
          float rover_frame_angle_4   =  rover_frame_angle + (M_PI_2 - rover_frame_angle) + M_PI + asinf(M_PI_2 / 3.0f);
    #endif
          float     x                 =  1.25f;
          float     y                 = -1.0f;
          unsigned  offset            = 0;

    // configure the vertex format, because data are pre-defined
    pVertexFormat->m_UseColors   = 1;
    pVertexFormat->m_UseNormals  = 0;
    pVertexFormat->m_UseTextures = 0;

    miniCalculateStride(pVertexFormat);

    // create memory for vertices and indexes
    *pVertexCount = 182;
    *pIndexCount  = 13;
    *pVertices    = (float*)malloc(*pVertexCount * pVertexFormat->m_Stride * sizeof(float));
    *pIndexes     = (MINI_Index*)malloc(*pIndexCount * sizeof(MINI_Index));
    *pMdlCmds     = (MINI_MdlCmds*)malloc(*pIndexCount * sizeof(MINI_MdlCmds));

    // add the rover cockpit (part 1) to final vertex buffer
    (*pIndexes)[0].m_Start    = offset;
    (*pIndexes)[0].m_Length   = sizeof(roverCockpit_1) / sizeof(float);
    (*pMdlCmds)[0].m_GLCmd    = 0;
    (*pMdlCmds)[0].m_CullMode = 1;
    memcpy(*pVertices, roverCockpit_1, sizeof(roverCockpit_1));
    offset += sizeof(roverCockpit_1) / sizeof(float);

    // add the rover cockpit (part 2) to final vertex buffer
    (*pIndexes)[1].m_Start    = offset;
    (*pIndexes)[1].m_Length   = sizeof(roverCockpit_2) / sizeof(float);
    (*pMdlCmds)[1].m_GLCmd    = 1;
    (*pMdlCmds)[1].m_CullMode = 1;
    memcpy((*pVertices) + offset, roverCockpit_2, sizeof(roverCockpit_2));
    offset += sizeof(roverCockpit_2) / sizeof(float);

    // add the rover cockpit (part 3) to final vertex buffer
    (*pIndexes)[2].m_Start    = offset;
    (*pIndexes)[2].m_Length   = sizeof(roverCockpit_3) / sizeof(float);
    (*pMdlCmds)[2].m_GLCmd    = 1;
    (*pMdlCmds)[2].m_CullMode = 1;
    memcpy((*pVertices) + offset, roverCockpit_3, sizeof(roverCockpit_3));
    offset += sizeof(roverCockpit_3) / sizeof(float);

    // add the rover cockpit (part 4) to final vertex buffer
    (*pIndexes)[3].m_Start    = offset;
    (*pIndexes)[3].m_Length   = sizeof(roverCockpit_4) / sizeof(float);
    (*pMdlCmds)[3].m_GLCmd    = 0;
    (*pMdlCmds)[3].m_CullMode = 1;
    memcpy((*pVertices) + offset, roverCockpit_4, sizeof(roverCockpit_4));
    offset += sizeof(roverCockpit_4) / sizeof(float);

    // add the rover cockpit (part 5) to final vertex buffer
    (*pIndexes)[4].m_Start    = offset;
    (*pIndexes)[4].m_Length   = sizeof(roverCockpit_5) / sizeof(float);
    (*pMdlCmds)[4].m_GLCmd    = 0;
    (*pMdlCmds)[4].m_CullMode = 1;
    memcpy((*pVertices) + offset, roverCockpit_5, sizeof(roverCockpit_5));
    offset += sizeof(roverCockpit_5) / sizeof(float);

    // add the rover front left frame to final vertex buffer
    (*pIndexes)[5].m_Start    = offset;
    (*pIndexes)[5].m_Length   = sizeof(roverFrame_fl) / sizeof(float);
    (*pMdlCmds)[5].m_GLCmd    = 0;
    (*pMdlCmds)[5].m_CullMode = 1;
    memcpy((*pVertices) + offset, roverFrame_fl, sizeof(roverFrame_fl));

    // calculate and apply the final vertices position
    (*pVertices)[offset]      = x + rover_frame_len * cos(rover_frame_angle_3); (*pVertices)[offset + 1]  = y + rover_frame_len * sin(rover_frame_angle_3);
    (*pVertices)[offset + 7]  = x + rover_frame_len * cos(rover_frame_angle_2); (*pVertices)[offset + 8]  = y + rover_frame_len * sin(rover_frame_angle_2);
    (*pVertices)[offset + 14] = x + rover_frame_len * cos(rover_frame_angle_4); (*pVertices)[offset + 15] = y + rover_frame_len * sin(rover_frame_angle_4);
    (*pVertices)[offset + 21] = x + rover_frame_len * cos(rover_frame_angle_1); (*pVertices)[offset + 22] = y + rover_frame_len * sin(rover_frame_angle_1);
    (*pVertices)[offset + 28] = x + rover_frame_len * cos(rover_frame_angle_1); (*pVertices)[offset + 29] = y + rover_frame_len * sin(rover_frame_angle_1);
    (*pVertices)[offset + 35] = x + rover_frame_len * cos(rover_frame_angle_2); (*pVertices)[offset + 36] = y + rover_frame_len * sin(rover_frame_angle_2);
    (*pVertices)[offset + 42] = x + rover_frame_len * cos(rover_frame_angle_2); (*pVertices)[offset + 43] = y + rover_frame_len * sin(rover_frame_angle_2);
    (*pVertices)[offset + 49] = x + rover_frame_len * cos(rover_frame_angle_3); (*pVertices)[offset + 50] = y + rover_frame_len * sin(rover_frame_angle_3);
    (*pVertices)[offset + 56] = x + rover_frame_len * cos(rover_frame_angle_1); (*pVertices)[offset + 57] = y + rover_frame_len * sin(rover_frame_angle_1);
    (*pVertices)[offset + 63] = x + rover_frame_len * cos(rover_frame_angle_4); (*pVertices)[offset + 64] = y + rover_frame_len * sin(rover_frame_angle_4);
    (*pVertices)[offset + 70] = x + rover_frame_len * cos(rover_frame_angle_4); (*pVertices)[offset + 71] = y + rover_frame_len * sin(rover_frame_angle_4);
    (*pVertices)[offset + 77] = x + rover_frame_len * cos(rover_frame_angle_3); (*pVertices)[offset + 78] = y + rover_frame_len * sin(rover_frame_angle_3);
    (*pVertices)[offset + 84] = x + rover_frame_len * cos(rover_frame_angle_3); (*pVertices)[offset + 85] = y + rover_frame_len * sin(rover_frame_angle_3);
    (*pVertices)[offset + 91] = x + rover_frame_len * cos(rover_frame_angle_2); (*pVertices)[offset + 92] = y + rover_frame_len * sin(rover_frame_angle_2);

    offset += sizeof(roverFrame_fl) / sizeof(float);

    // add the rover front right frame to final vertex buffer
    (*pIndexes)[6].m_Start    = offset;
    (*pIndexes)[6].m_Length   = sizeof(roverFrame_fr) / sizeof(float);
    (*pMdlCmds)[6].m_GLCmd    = 0;
    (*pMdlCmds)[6].m_CullMode = 1;
    memcpy((*pVertices) + offset, roverFrame_fr, sizeof(roverFrame_fr));

    // calculate and apply the final vertices position
    (*pVertices)[offset]      = x + rover_frame_len * cos(rover_frame_angle_3); (*pVertices)[offset + 1]  = y + rover_frame_len * sin(rover_frame_angle_3);
    (*pVertices)[offset + 7]  = x + rover_frame_len * cos(rover_frame_angle_2); (*pVertices)[offset + 8]  = y + rover_frame_len * sin(rover_frame_angle_2);
    (*pVertices)[offset + 14] = x + rover_frame_len * cos(rover_frame_angle_4); (*pVertices)[offset + 15] = y + rover_frame_len * sin(rover_frame_angle_4);
    (*pVertices)[offset + 21] = x + rover_frame_len * cos(rover_frame_angle_1); (*pVertices)[offset + 22] = y + rover_frame_len * sin(rover_frame_angle_1);
    (*pVertices)[offset + 28] = x + rover_frame_len * cos(rover_frame_angle_1); (*pVertices)[offset + 29] = y + rover_frame_len * sin(rover_frame_angle_1);
    (*pVertices)[offset + 35] = x + rover_frame_len * cos(rover_frame_angle_2); (*pVertices)[offset + 36] = y + rover_frame_len * sin(rover_frame_angle_2);
    (*pVertices)[offset + 42] = x + rover_frame_len * cos(rover_frame_angle_2); (*pVertices)[offset + 43] = y + rover_frame_len * sin(rover_frame_angle_2);
    (*pVertices)[offset + 49] = x + rover_frame_len * cos(rover_frame_angle_3); (*pVertices)[offset + 50] = y + rover_frame_len * sin(rover_frame_angle_3);
    (*pVertices)[offset + 56] = x + rover_frame_len * cos(rover_frame_angle_1); (*pVertices)[offset + 57] = y + rover_frame_len * sin(rover_frame_angle_1);
    (*pVertices)[offset + 63] = x + rover_frame_len * cos(rover_frame_angle_4); (*pVertices)[offset + 64] = y + rover_frame_len * sin(rover_frame_angle_4);
    (*pVertices)[offset + 70] = x + rover_frame_len * cos(rover_frame_angle_4); (*pVertices)[offset + 71] = y + rover_frame_len * sin(rover_frame_angle_4);
    (*pVertices)[offset + 77] = x + rover_frame_len * cos(rover_frame_angle_3); (*pVertices)[offset + 78] = y + rover_frame_len * sin(rover_frame_angle_3);
    (*pVertices)[offset + 84] = x + rover_frame_len * cos(rover_frame_angle_3); (*pVertices)[offset + 85] = y + rover_frame_len * sin(rover_frame_angle_3);
    (*pVertices)[offset + 91] = x + rover_frame_len * cos(rover_frame_angle_2); (*pVertices)[offset + 92] = y + rover_frame_len * sin(rover_frame_angle_2);

    offset += sizeof(roverFrame_fr) / sizeof(float);

    // calculate values for the next frame
    #ifdef __CODEGEARC__
        rover_frame_angle_1 =  rover_frame_angle                                       + asin(M_PI_2 / 3.0f);
        rover_frame_angle_2 =  rover_frame_angle + (M_PI_2 - rover_frame_angle) + M_PI + asin(M_PI_2 / 3.0f);
        rover_frame_angle_3 =  rover_frame_angle +  M_PI                               + asin(M_PI_2 / 3.0f);
        rover_frame_angle_4 =  rover_frame_angle + (M_PI_2 - rover_frame_angle)        + asin(M_PI_2 / 3.0f);
    #else
        rover_frame_angle_1 =  rover_frame_angle                                       + asinf(M_PI_2 / 3.0f);
        rover_frame_angle_2 =  rover_frame_angle + (M_PI_2 - rover_frame_angle) + M_PI + asinf(M_PI_2 / 3.0f);
        rover_frame_angle_3 =  rover_frame_angle +  M_PI                               + asinf(M_PI_2 / 3.0f);
        rover_frame_angle_4 =  rover_frame_angle + (M_PI_2 - rover_frame_angle)        + asinf(M_PI_2 / 3.0f);
    #endif
    x                       = -1.25f;

    // add the rover rear left frame to final vertex buffer
    (*pIndexes)[7].m_Start    = offset;
    (*pIndexes)[7].m_Length   = sizeof(roverFrame_rl) / sizeof(float);
    (*pMdlCmds)[7].m_GLCmd    = 0;
    (*pMdlCmds)[7].m_CullMode = 1;
    memcpy((*pVertices) + offset, roverFrame_rl, sizeof(roverFrame_rl));

    // calculate and apply the final vertices position
    (*pVertices)[offset]      = x - rover_frame_len * cos(rover_frame_angle_3); (*pVertices)[offset + 1]  = y + rover_frame_len * sin(rover_frame_angle_3);
    (*pVertices)[offset + 7]  = x - rover_frame_len * cos(rover_frame_angle_2); (*pVertices)[offset + 8]  = y + rover_frame_len * sin(rover_frame_angle_2);
    (*pVertices)[offset + 14] = x - rover_frame_len * cos(rover_frame_angle_4); (*pVertices)[offset + 15] = y + rover_frame_len * sin(rover_frame_angle_4);
    (*pVertices)[offset + 21] = x - rover_frame_len * cos(rover_frame_angle_1); (*pVertices)[offset + 22] = y + rover_frame_len * sin(rover_frame_angle_1);
    (*pVertices)[offset + 28] = x - rover_frame_len * cos(rover_frame_angle_1); (*pVertices)[offset + 29] = y + rover_frame_len * sin(rover_frame_angle_1);
    (*pVertices)[offset + 35] = x - rover_frame_len * cos(rover_frame_angle_2); (*pVertices)[offset + 36] = y + rover_frame_len * sin(rover_frame_angle_2);
    (*pVertices)[offset + 42] = x - rover_frame_len * cos(rover_frame_angle_2); (*pVertices)[offset + 43] = y + rover_frame_len * sin(rover_frame_angle_2);
    (*pVertices)[offset + 49] = x - rover_frame_len * cos(rover_frame_angle_3); (*pVertices)[offset + 50] = y + rover_frame_len * sin(rover_frame_angle_3);
    (*pVertices)[offset + 56] = x - rover_frame_len * cos(rover_frame_angle_1); (*pVertices)[offset + 57] = y + rover_frame_len * sin(rover_frame_angle_1);
    (*pVertices)[offset + 63] = x - rover_frame_len * cos(rover_frame_angle_4); (*pVertices)[offset + 64] = y + rover_frame_len * sin(rover_frame_angle_4);
    (*pVertices)[offset + 70] = x - rover_frame_len * cos(rover_frame_angle_4); (*pVertices)[offset + 71] = y + rover_frame_len * sin(rover_frame_angle_4);
    (*pVertices)[offset + 77] = x - rover_frame_len * cos(rover_frame_angle_3); (*pVertices)[offset + 78] = y + rover_frame_len * sin(rover_frame_angle_3);
    (*pVertices)[offset + 84] = x - rover_frame_len * cos(rover_frame_angle_3); (*pVertices)[offset + 85] = y + rover_frame_len * sin(rover_frame_angle_3);
    (*pVertices)[offset + 91] = x - rover_frame_len * cos(rover_frame_angle_2); (*pVertices)[offset + 92] = y + rover_frame_len * sin(rover_frame_angle_2);

    offset += sizeof(roverFrame_rl) / sizeof(float);

    // add the rover rear right frame to final vertex buffer
    (*pIndexes)[8].m_Start    = offset;
    (*pIndexes)[8].m_Length   = sizeof(roverFrame_rr) / sizeof(float);
    (*pMdlCmds)[8].m_GLCmd    = 0;
    (*pMdlCmds)[8].m_CullMode = 1;
    memcpy((*pVertices) + offset, roverFrame_rr, sizeof(roverFrame_rr));

    // calculate and apply the final vertices position
    (*pVertices)[offset]      = x - rover_frame_len * cos(rover_frame_angle_3); (*pVertices)[offset + 1]  = y + rover_frame_len * sin(rover_frame_angle_3);
    (*pVertices)[offset + 7]  = x - rover_frame_len * cos(rover_frame_angle_2); (*pVertices)[offset + 8]  = y + rover_frame_len * sin(rover_frame_angle_2);
    (*pVertices)[offset + 14] = x - rover_frame_len * cos(rover_frame_angle_4); (*pVertices)[offset + 15] = y + rover_frame_len * sin(rover_frame_angle_4);
    (*pVertices)[offset + 21] = x - rover_frame_len * cos(rover_frame_angle_1); (*pVertices)[offset + 22] = y + rover_frame_len * sin(rover_frame_angle_1);
    (*pVertices)[offset + 28] = x - rover_frame_len * cos(rover_frame_angle_1); (*pVertices)[offset + 29] = y + rover_frame_len * sin(rover_frame_angle_1);
    (*pVertices)[offset + 35] = x - rover_frame_len * cos(rover_frame_angle_2); (*pVertices)[offset + 36] = y + rover_frame_len * sin(rover_frame_angle_2);
    (*pVertices)[offset + 42] = x - rover_frame_len * cos(rover_frame_angle_2); (*pVertices)[offset + 43] = y + rover_frame_len * sin(rover_frame_angle_2);
    (*pVertices)[offset + 49] = x - rover_frame_len * cos(rover_frame_angle_3); (*pVertices)[offset + 50] = y + rover_frame_len * sin(rover_frame_angle_3);
    (*pVertices)[offset + 56] = x - rover_frame_len * cos(rover_frame_angle_1); (*pVertices)[offset + 57] = y + rover_frame_len * sin(rover_frame_angle_1);
    (*pVertices)[offset + 63] = x - rover_frame_len * cos(rover_frame_angle_4); (*pVertices)[offset + 64] = y + rover_frame_len * sin(rover_frame_angle_4);
    (*pVertices)[offset + 70] = x - rover_frame_len * cos(rover_frame_angle_4); (*pVertices)[offset + 71] = y + rover_frame_len * sin(rover_frame_angle_4);
    (*pVertices)[offset + 77] = x - rover_frame_len * cos(rover_frame_angle_3); (*pVertices)[offset + 78] = y + rover_frame_len * sin(rover_frame_angle_3);
    (*pVertices)[offset + 84] = x - rover_frame_len * cos(rover_frame_angle_3); (*pVertices)[offset + 85] = y + rover_frame_len * sin(rover_frame_angle_3);
    (*pVertices)[offset + 91] = x - rover_frame_len * cos(rover_frame_angle_2); (*pVertices)[offset + 92] = y + rover_frame_len * sin(rover_frame_angle_2);

    offset += sizeof(roverFrame_rr) / sizeof(float);

    x = -1.9f;
    y = -2.3f;

    // add the rover front left wheel to final vertex buffer
    (*pIndexes)[9].m_Start    = offset;
    (*pIndexes)[9].m_Length   = sizeof(roverWheel_fl) / sizeof(float);
    (*pMdlCmds)[9].m_GLCmd    = 0;
    (*pMdlCmds)[9].m_CullMode = 0;
    memcpy((*pVertices) + offset, roverWheel_fl, sizeof(roverWheel_fl));

    // calculate and apply the final vertices position
    (*pVertices)[offset]       = x + wheel_length * cos(wheel_angle_1); (*pVertices)[offset + 1]   = y + wheel_length * sin(wheel_angle_1);
    (*pVertices)[offset + 7]   = x + wheel_length * cos(wheel_angle_1); (*pVertices)[offset + 8]   = y + wheel_length * sin(wheel_angle_1);
    (*pVertices)[offset + 14]  = x + wheel_length * cos(wheel_angle_2); (*pVertices)[offset + 15]  = y + wheel_length * sin(wheel_angle_2);
    (*pVertices)[offset + 21]  = x + wheel_length * cos(wheel_angle_2); (*pVertices)[offset + 22]  = y + wheel_length * sin(wheel_angle_2);
    (*pVertices)[offset + 28]  = x + wheel_length * cos(wheel_angle_3); (*pVertices)[offset + 29]  = y + wheel_length * sin(wheel_angle_3);
    (*pVertices)[offset + 35]  = x + wheel_length * cos(wheel_angle_3); (*pVertices)[offset + 36]  = y + wheel_length * sin(wheel_angle_3);
    (*pVertices)[offset + 42]  = x + wheel_length * cos(wheel_angle_4); (*pVertices)[offset + 43]  = y + wheel_length * sin(wheel_angle_4);
    (*pVertices)[offset + 49]  = x + wheel_length * cos(wheel_angle_4); (*pVertices)[offset + 50]  = y + wheel_length * sin(wheel_angle_4);
    (*pVertices)[offset + 56]  = x + wheel_length * cos(wheel_angle_5); (*pVertices)[offset + 57]  = y + wheel_length * sin(wheel_angle_5);
    (*pVertices)[offset + 63]  = x + wheel_length * cos(wheel_angle_5); (*pVertices)[offset + 64]  = y + wheel_length * sin(wheel_angle_5);
    (*pVertices)[offset + 70]  = x + wheel_length * cos(wheel_angle_6); (*pVertices)[offset + 71]  = y + wheel_length * sin(wheel_angle_6);
    (*pVertices)[offset + 77]  = x + wheel_length * cos(wheel_angle_6); (*pVertices)[offset + 78]  = y + wheel_length * sin(wheel_angle_6);
    (*pVertices)[offset + 84]  = x + wheel_length * cos(wheel_angle_7); (*pVertices)[offset + 85]  = y + wheel_length * sin(wheel_angle_7);
    (*pVertices)[offset + 91]  = x + wheel_length * cos(wheel_angle_7); (*pVertices)[offset + 92]  = y + wheel_length * sin(wheel_angle_7);
    (*pVertices)[offset + 98]  = x + wheel_length * cos(wheel_angle_8); (*pVertices)[offset + 99]  = y + wheel_length * sin(wheel_angle_8);
    (*pVertices)[offset + 105] = x + wheel_length * cos(wheel_angle_8); (*pVertices)[offset + 106] = y + wheel_length * sin(wheel_angle_8);
    (*pVertices)[offset + 112] = x + wheel_length * cos(wheel_angle_1); (*pVertices)[offset + 113] = y + wheel_length * sin(wheel_angle_1);
    (*pVertices)[offset + 119] = x + wheel_length * cos(wheel_angle_1); (*pVertices)[offset + 120] = y + wheel_length * sin(wheel_angle_1);

    offset += sizeof(roverWheel_fl) / sizeof(float);

    // add the rover front right wheel to final vertex buffer
    (*pIndexes)[10].m_Start    = offset;
    (*pIndexes)[10].m_Length   = sizeof(roverWheel_fr) / sizeof(float);
    (*pMdlCmds)[10].m_GLCmd    = 0;
    (*pMdlCmds)[10].m_CullMode = 0;
    memcpy((*pVertices) + offset, roverWheel_fr, sizeof(roverWheel_fr));

    // calculate and apply the final vertices position
    (*pVertices)[offset]       = x + wheel_length * cos(wheel_angle_1); (*pVertices)[offset + 1]   = y + wheel_length * sin(wheel_angle_1);
    (*pVertices)[offset + 7]   = x + wheel_length * cos(wheel_angle_1); (*pVertices)[offset + 8]   = y + wheel_length * sin(wheel_angle_1);
    (*pVertices)[offset + 14]  = x + wheel_length * cos(wheel_angle_2); (*pVertices)[offset + 15]  = y + wheel_length * sin(wheel_angle_2);
    (*pVertices)[offset + 21]  = x + wheel_length * cos(wheel_angle_2); (*pVertices)[offset + 22]  = y + wheel_length * sin(wheel_angle_2);
    (*pVertices)[offset + 28]  = x + wheel_length * cos(wheel_angle_3); (*pVertices)[offset + 29]  = y + wheel_length * sin(wheel_angle_3);
    (*pVertices)[offset + 35]  = x + wheel_length * cos(wheel_angle_3); (*pVertices)[offset + 36]  = y + wheel_length * sin(wheel_angle_3);
    (*pVertices)[offset + 42]  = x + wheel_length * cos(wheel_angle_4); (*pVertices)[offset + 43]  = y + wheel_length * sin(wheel_angle_4);
    (*pVertices)[offset + 49]  = x + wheel_length * cos(wheel_angle_4); (*pVertices)[offset + 50]  = y + wheel_length * sin(wheel_angle_4);
    (*pVertices)[offset + 56]  = x + wheel_length * cos(wheel_angle_5); (*pVertices)[offset + 57]  = y + wheel_length * sin(wheel_angle_5);
    (*pVertices)[offset + 63]  = x + wheel_length * cos(wheel_angle_5); (*pVertices)[offset + 64]  = y + wheel_length * sin(wheel_angle_5);
    (*pVertices)[offset + 70]  = x + wheel_length * cos(wheel_angle_6); (*pVertices)[offset + 71]  = y + wheel_length * sin(wheel_angle_6);
    (*pVertices)[offset + 77]  = x + wheel_length * cos(wheel_angle_6); (*pVertices)[offset + 78]  = y + wheel_length * sin(wheel_angle_6);
    (*pVertices)[offset + 84]  = x + wheel_length * cos(wheel_angle_7); (*pVertices)[offset + 85]  = y + wheel_length * sin(wheel_angle_7);
    (*pVertices)[offset + 91]  = x + wheel_length * cos(wheel_angle_7); (*pVertices)[offset + 92]  = y + wheel_length * sin(wheel_angle_7);
    (*pVertices)[offset + 98]  = x + wheel_length * cos(wheel_angle_8); (*pVertices)[offset + 99]  = y + wheel_length * sin(wheel_angle_8);
    (*pVertices)[offset + 105] = x + wheel_length * cos(wheel_angle_8); (*pVertices)[offset + 106] = y + wheel_length * sin(wheel_angle_8);
    (*pVertices)[offset + 112] = x + wheel_length * cos(wheel_angle_1); (*pVertices)[offset + 113] = y + wheel_length * sin(wheel_angle_1);
    (*pVertices)[offset + 119] = x + wheel_length * cos(wheel_angle_1); (*pVertices)[offset + 120] = y + wheel_length * sin(wheel_angle_1);

    offset += sizeof(roverWheel_fr) / sizeof(float);

    x = 1.9f;

    // add the rover rear left wheel to final vertex buffer
    (*pIndexes)[11].m_Start    = offset;
    (*pIndexes)[11].m_Length   = sizeof(roverWheel_rl) / sizeof(float);
    (*pMdlCmds)[11].m_GLCmd    = 0;
    (*pMdlCmds)[11].m_CullMode = 0;
    memcpy((*pVertices) + offset, roverWheel_rl, sizeof(roverWheel_rl));

    // calculate and apply the final vertices position
    (*pVertices)[offset]       = x + wheel_length * cos(wheel_angle_1); (*pVertices)[offset + 1]   = y + wheel_length * sin(wheel_angle_1);
    (*pVertices)[offset + 7]   = x + wheel_length * cos(wheel_angle_1); (*pVertices)[offset + 8]   = y + wheel_length * sin(wheel_angle_1);
    (*pVertices)[offset + 14]  = x + wheel_length * cos(wheel_angle_2); (*pVertices)[offset + 15]  = y + wheel_length * sin(wheel_angle_2);
    (*pVertices)[offset + 21]  = x + wheel_length * cos(wheel_angle_2); (*pVertices)[offset + 22]  = y + wheel_length * sin(wheel_angle_2);
    (*pVertices)[offset + 28]  = x + wheel_length * cos(wheel_angle_3); (*pVertices)[offset + 29]  = y + wheel_length * sin(wheel_angle_3);
    (*pVertices)[offset + 35]  = x + wheel_length * cos(wheel_angle_3); (*pVertices)[offset + 36]  = y + wheel_length * sin(wheel_angle_3);
    (*pVertices)[offset + 42]  = x + wheel_length * cos(wheel_angle_4); (*pVertices)[offset + 43]  = y + wheel_length * sin(wheel_angle_4);
    (*pVertices)[offset + 49]  = x + wheel_length * cos(wheel_angle_4); (*pVertices)[offset + 50]  = y + wheel_length * sin(wheel_angle_4);
    (*pVertices)[offset + 56]  = x + wheel_length * cos(wheel_angle_5); (*pVertices)[offset + 57]  = y + wheel_length * sin(wheel_angle_5);
    (*pVertices)[offset + 63]  = x + wheel_length * cos(wheel_angle_5); (*pVertices)[offset + 64]  = y + wheel_length * sin(wheel_angle_5);
    (*pVertices)[offset + 70]  = x + wheel_length * cos(wheel_angle_6); (*pVertices)[offset + 71]  = y + wheel_length * sin(wheel_angle_6);
    (*pVertices)[offset + 77]  = x + wheel_length * cos(wheel_angle_6); (*pVertices)[offset + 78]  = y + wheel_length * sin(wheel_angle_6);
    (*pVertices)[offset + 84]  = x + wheel_length * cos(wheel_angle_7); (*pVertices)[offset + 85]  = y + wheel_length * sin(wheel_angle_7);
    (*pVertices)[offset + 91]  = x + wheel_length * cos(wheel_angle_7); (*pVertices)[offset + 92]  = y + wheel_length * sin(wheel_angle_7);
    (*pVertices)[offset + 98]  = x + wheel_length * cos(wheel_angle_8); (*pVertices)[offset + 99]  = y + wheel_length * sin(wheel_angle_8);
    (*pVertices)[offset + 105] = x + wheel_length * cos(wheel_angle_8); (*pVertices)[offset + 106] = y + wheel_length * sin(wheel_angle_8);
    (*pVertices)[offset + 112] = x + wheel_length * cos(wheel_angle_1); (*pVertices)[offset + 113] = y + wheel_length * sin(wheel_angle_1);
    (*pVertices)[offset + 119] = x + wheel_length * cos(wheel_angle_1); (*pVertices)[offset + 120] = y + wheel_length * sin(wheel_angle_1);

    offset += sizeof(roverWheel_rl) / sizeof(float);

    // add the rover rear right wheel to final vertex buffer
    (*pIndexes)[12].m_Start    = offset;
    (*pIndexes)[12].m_Length   = sizeof(roverWheel_rr) / sizeof(float);
    (*pMdlCmds)[12].m_GLCmd    = 0;
    (*pMdlCmds)[12].m_CullMode = 0;
    memcpy((*pVertices) + offset, roverWheel_rr, sizeof(roverWheel_rr));

    // calculate and apply the final vertices position
    (*pVertices)[offset]       = x + wheel_length * cos(wheel_angle_1); (*pVertices)[offset + 1]   = y + wheel_length * sin(wheel_angle_1);
    (*pVertices)[offset + 7]   = x + wheel_length * cos(wheel_angle_1); (*pVertices)[offset + 8]   = y + wheel_length * sin(wheel_angle_1);
    (*pVertices)[offset + 14]  = x + wheel_length * cos(wheel_angle_2); (*pVertices)[offset + 15]  = y + wheel_length * sin(wheel_angle_2);
    (*pVertices)[offset + 21]  = x + wheel_length * cos(wheel_angle_2); (*pVertices)[offset + 22]  = y + wheel_length * sin(wheel_angle_2);
    (*pVertices)[offset + 28]  = x + wheel_length * cos(wheel_angle_3); (*pVertices)[offset + 29]  = y + wheel_length * sin(wheel_angle_3);
    (*pVertices)[offset + 35]  = x + wheel_length * cos(wheel_angle_3); (*pVertices)[offset + 36]  = y + wheel_length * sin(wheel_angle_3);
    (*pVertices)[offset + 42]  = x + wheel_length * cos(wheel_angle_4); (*pVertices)[offset + 43]  = y + wheel_length * sin(wheel_angle_4);
    (*pVertices)[offset + 49]  = x + wheel_length * cos(wheel_angle_4); (*pVertices)[offset + 50]  = y + wheel_length * sin(wheel_angle_4);
    (*pVertices)[offset + 56]  = x + wheel_length * cos(wheel_angle_5); (*pVertices)[offset + 57]  = y + wheel_length * sin(wheel_angle_5);
    (*pVertices)[offset + 63]  = x + wheel_length * cos(wheel_angle_5); (*pVertices)[offset + 64]  = y + wheel_length * sin(wheel_angle_5);
    (*pVertices)[offset + 70]  = x + wheel_length * cos(wheel_angle_6); (*pVertices)[offset + 71]  = y + wheel_length * sin(wheel_angle_6);
    (*pVertices)[offset + 77]  = x + wheel_length * cos(wheel_angle_6); (*pVertices)[offset + 78]  = y + wheel_length * sin(wheel_angle_6);
    (*pVertices)[offset + 84]  = x + wheel_length * cos(wheel_angle_7); (*pVertices)[offset + 85]  = y + wheel_length * sin(wheel_angle_7);
    (*pVertices)[offset + 91]  = x + wheel_length * cos(wheel_angle_7); (*pVertices)[offset + 92]  = y + wheel_length * sin(wheel_angle_7);
    (*pVertices)[offset + 98]  = x + wheel_length * cos(wheel_angle_8); (*pVertices)[offset + 99]  = y + wheel_length * sin(wheel_angle_8);
    (*pVertices)[offset + 105] = x + wheel_length * cos(wheel_angle_8); (*pVertices)[offset + 106] = y + wheel_length * sin(wheel_angle_8);
    (*pVertices)[offset + 112] = x + wheel_length * cos(wheel_angle_1); (*pVertices)[offset + 113] = y + wheel_length * sin(wheel_angle_1);
    (*pVertices)[offset + 119] = x + wheel_length * cos(wheel_angle_1); (*pVertices)[offset + 120] = y + wheel_length * sin(wheel_angle_1);
}
//----------------------------------------------------------------------------
