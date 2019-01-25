/*****************************************************************************
 * ==> MiniShapes -----------------------------------------------------------*
 *****************************************************************************
 * Description : This module provides the functions to generate geometrical  *
 *               shapes, like sphere, cylinders, ...                         *
 * Developer   : Jean-Milost Reymond                                         *
 * Copyright   : 2015 - 2018, this file is part of the Minimal API. You are  *
 *               free to copy or redistribute this file, modify it, or use   *
 *               it for your own projects, commercial or not. This file is   *
 *               provided "as is", without ANY WARRANTY OF ANY KIND          *
 *****************************************************************************/

#include "MiniShapes.h"

// mini api
#include "MiniRenderer.h"

// std
#include <stdlib.h>
#include <math.h>

//----------------------------------------------------------------------------
// Shapes creation functions
//----------------------------------------------------------------------------
void miniCreateSurface(const float*             pWidth,
                       const float*             pHeight,
                             unsigned int       color,
                             MINI_VertexFormat* pVertexFormat,
                             float**            pVertices,
                             unsigned*          pVertexCount)
{
    int i;
    int index;
    int offset;

    // create a buffer template: false for negative values, true for positive
    int bufferTemplate[] =
    {
        0, 0,
        0, 1,
        1, 0,
        1, 1,
    };

    miniCalculateStride(pVertexFormat);

    *pVertexCount = 4;

    // generate vertices
    if (!(*pVertices))
        *pVertices = (float*)malloc(*pVertexCount * sizeof(float) * pVertexFormat->m_Stride);
    else
        *pVertices = (float*)realloc(*pVertices,
                                     *pVertexCount * sizeof(float) * pVertexFormat->m_Stride);

    offset = 0;

    // iterate through vertex to create
    for (i = 0; i < 4; ++i)
    {
        // calculate template buffer index
        index = i * 2;

        // populate vertex buffer
        if (bufferTemplate[index])
            (*pVertices)[offset] =   *pWidth  / 2.0f;
        else
            (*pVertices)[offset] = -(*pWidth) / 2.0f;

        if (bufferTemplate[index + 1])
            (*pVertices)[offset + 1] =   *pHeight  / 2.0f;
        else
            (*pVertices)[offset + 1] = -(*pHeight) / 2.0f;

        (*pVertices)[offset + 2] = 0.0f;

        offset += 3;

        // do include normals?
        if (pVertexFormat->m_UseNormals)
        {
            // calculate normals
            (*pVertices)[offset]     =  0.0f;
            (*pVertices)[offset + 1] =  0.0f;
            (*pVertices)[offset + 2] = -1.0f;

            offset += 3;
        }

        // do include texture?
        if (pVertexFormat->m_UseTextures)
        {
            // calculate texture u coordinate
            if (bufferTemplate[index])
                (*pVertices)[offset] = 1.0f;
            else
                (*pVertices)[offset] = 0.0f;

            // calculate texture v coordinate
            if (bufferTemplate[index + 1])
                (*pVertices)[offset + 1] = 1.0f;
            else
                (*pVertices)[offset + 1] = 0.0f;

            offset += 2;
        }

        // do include colors?
        if (pVertexFormat->m_UseColors)
        {
            // set color data
            (*pVertices)[offset]     = (float)((color >> 24) & 0xFF) / 255.0f;
            (*pVertices)[offset + 1] = (float)((color >> 16) & 0xFF) / 255.0f;
            (*pVertices)[offset + 2] = (float)((color >> 8)  & 0xFF) / 255.0f;
            (*pVertices)[offset + 3] = (float) (color        & 0xFF) / 255.0f;

            offset += 4;
        }
    }
}
//----------------------------------------------------------------------------
void miniCreateSphere(const float*             pRadius,
                            int                slices,
                            int                stacks,
                            unsigned           color,
                            MINI_VertexFormat* pVertexFormat,
                            float**            pVertices,
                            unsigned*          pVertexCount,
                            MINI_Index**       pIndexes,
                            unsigned*          pIndexCount)
{
    int   i;
    int   j;
    int   iIndex;
    int   fanLength;
    int   index;
    float majorStep;
    float minorStep;
    float a;
    float b;
    float r0;
    float r1;
    float z0;
    float z1;
    float c;
    float x;
    float y;

    miniCalculateStride(pVertexFormat);

    // initialize basic values
    majorStep     = (M_PI          / slices);
    minorStep     = ((2.0f * M_PI) / stacks);
    *pVertices    = 0;
    *pVertexCount = 0;
    *pIndexes     = 0;
    *pIndexCount  = 0;

    // iterate through vertex slices
    for (i = 0; i < slices; ++i)
    {
        // calculate values for next slice
        a  = i * majorStep;
        b  = a + majorStep;
        r0 = *pRadius * sinf(a);
        r1 = *pRadius * sinf(b);
        z0 = *pRadius * cosf(a);
        z1 = *pRadius * cosf(b);

        // generate new index in table
        if (!(*pIndexes))
            *pIndexes = (MINI_Index*)malloc(sizeof(MINI_Index));
        else
            *pIndexes = (MINI_Index*)realloc(*pIndexes,
                                            (*pIndexCount + 1) * sizeof(MINI_Index));

        // calculate current index and slice fan length
        iIndex    = *pIndexCount;
        fanLength = (stacks + 1) * pVertexFormat->m_Stride * 2;

        // increase index count
        ++(*pIndexCount);

        // populate index
        (*pIndexes)[iIndex].m_Start  = *pVertexCount;
        (*pIndexes)[iIndex].m_Length = fanLength;

        // calculate new vertex buffer length
        *pVertexCount += fanLength;

        // generate vertices
        if (!(*pVertices))
            *pVertices = (float*)malloc(*pVertexCount * sizeof(float));
        else
            *pVertices = (float*)realloc(*pVertices,
                                         *pVertexCount * sizeof(float));

        index = (*pIndexes)[iIndex].m_Start;

        // iterate through vertex stacks
        for (j = 0; j <= stacks; ++j)
        {
            c = j * minorStep;
            x = cosf(c);
            y = sinf(c);

            // set vertex data
            (*pVertices)[index]     = x * r0;
            (*pVertices)[index + 1] = y * r0;
            (*pVertices)[index + 2] = z0;

            index += 3;

            // do generate normals?
            if (pVertexFormat->m_UseNormals)
            {
                // set normals
                (*pVertices)[index]     = (x * r0) / *pRadius;
                (*pVertices)[index + 1] = (y * r0) / *pRadius;
                (*pVertices)[index + 2] = z0       / *pRadius;

                index += 3;
            }

            // do generate texture coordinates?
            if (pVertexFormat->m_UseTextures)
            {
                // add texture coordinates data to buffer
                (*pVertices)[index]     = ((float)j / (float)stacks);
                (*pVertices)[index + 1] = ((float)i / (float)slices);

                index += 2;
            }

            // do generate colors?
            if (pVertexFormat->m_UseColors)
            {
                // set color data
                (*pVertices)[index]     = (float)((color >> 24) & 0xFF) / 255.0f;
                (*pVertices)[index + 1] = (float)((color >> 16) & 0xFF) / 255.0f;
                (*pVertices)[index + 2] = (float)((color >> 8)  & 0xFF) / 255.0f;
                (*pVertices)[index + 3] = (float) (color        & 0xFF) / 255.0f;

                index += 4;
            }

            (*pVertices)[index]     = x * r1;
            (*pVertices)[index + 1] = y * r1;
            (*pVertices)[index + 2] = z1;

            index += 3;

            // do generate normals?
            if (pVertexFormat->m_UseNormals)
            {
                // set normals
                (*pVertices)[index]     = (x * r1) / *pRadius;
                (*pVertices)[index + 1] = (y * r1) / *pRadius;
                (*pVertices)[index + 2] =  z1      / *pRadius;

                index += 3;
            }

            // do generate texture coordinates?
            if (pVertexFormat->m_UseTextures)
            {
                // add texture coordinates data to buffer
                (*pVertices)[index]     = ( (float)j         / (float)stacks);
                (*pVertices)[index + 1] = (((float)i + 1.0f) / (float)slices);

                index += 2;
            }

            // do generate colors?
            if (pVertexFormat->m_UseColors)
            {
                // set color data
                (*pVertices)[index]     = (float)((color >> 24) & 0xFF) / 255.0f;
                (*pVertices)[index + 1] = (float)((color >> 16) & 0xFF) / 255.0f;
                (*pVertices)[index + 2] = (float)((color >> 8)  & 0xFF) / 255.0f;
                (*pVertices)[index + 3] = (float) (color        & 0xFF) / 255.0f;

                index += 4;
            }
        }
    }
}
//----------------------------------------------------------------------------
void miniCreateCylinder(const float*             pRadius,
                        const float*             pHeight,
                              int                faces,
                              unsigned           color,
                              MINI_VertexFormat* pVertexFormat,
                              float**            pVertices,
                              unsigned*          pVertexCount)
{
    int   i;
    int   index;
    float angle;
    float step;

    miniCalculateStride(pVertexFormat);

    // initialize vertex buffer
    *pVertices    = 0;
    *pVertexCount = (faces + 1) * 2 * pVertexFormat->m_Stride;

    // generate vertices
    if (!(*pVertices))
        *pVertices = (float*)malloc(*pVertexCount * sizeof(float));
    else
        *pVertices = (float*)realloc(*pVertices,
                                     *pVertexCount * sizeof(float));

    // calculate step to apply between faces
    step  = (2.0f * M_PI) / (float)faces;
    index = 0;

    // iterate through vertices to create
    for (i = 0; i < faces + 1; ++i)
    {
        // calculate angle
        angle = step * i;

        // set vertex data
        (*pVertices)[index]     =   *pRadius * cosf(angle);
        (*pVertices)[index + 1] = -(*pHeight / 2.0f);
        (*pVertices)[index + 2] =   *pRadius * sinf(angle);

        index += 3;

        // do generate normals?
        if (pVertexFormat->m_UseNormals)
        {
            // set normals
            (*pVertices)[index]     = cosf(angle);
            (*pVertices)[index + 1] = 0.0f;
            (*pVertices)[index + 2] = sinf(angle);

            index += 3;
        }

        // do generate texture coordinates?
        if (pVertexFormat->m_UseTextures)
        {
            // add texture coordinates data to buffer
            (*pVertices)[index]     = 1.0f / i;
            (*pVertices)[index + 1] = 0.0f;

            index += 2;
        }

        // do generate colors?
        if (pVertexFormat->m_UseColors)
        {
            // set color data
            (*pVertices)[index]     = (float)((color >> 24) & 0xFF) / 255.0f;
            (*pVertices)[index + 1] = (float)((color >> 16) & 0xFF) / 255.0f;
            (*pVertices)[index + 2] = (float)((color >> 8)  & 0xFF) / 255.0f;
            (*pVertices)[index + 3] = (float) (color        & 0xFF) / 255.0f;

            index += 4;
        }

        // set vertex data
        (*pVertices)[index]     =  *pRadius * cosf(angle);
        (*pVertices)[index + 1] = (*pHeight / 2.0f);
        (*pVertices)[index + 2] =  *pRadius * sinf(angle);

        index += 3;

        // do generate normals?
        if (pVertexFormat->m_UseNormals)
        {
            // set normals
            (*pVertices)[index]     = cosf(angle);
            (*pVertices)[index + 1] = 0.0f;
            (*pVertices)[index + 2] = sinf(angle);

            index += 3;
        }

        // do generate texture coordinates?
        if (pVertexFormat->m_UseTextures)
        {
            // add texture coordinates data to buffer
            (*pVertices)[index]     = 1.0f / i;
            (*pVertices)[index + 1] = 1.0f;

            index += 2;
        }

        // do generate colors?
        if (pVertexFormat->m_UseColors)
        {
            // set color data
            (*pVertices)[index]     = (float)((color >> 24) & 0xFF) / 255.0f;
            (*pVertices)[index + 1] = (float)((color >> 16) & 0xFF) / 255.0f;
            (*pVertices)[index + 2] = (float)((color >> 8)  & 0xFF) / 255.0f;
            (*pVertices)[index + 3] = (float) (color        & 0xFF) / 255.0f;

            index += 4;
        }
    }
}
//------------------------------------------------------------------------------
void miniCreateDisk(float              centerX,
                    float              centerY,
                    float              radius,
                    unsigned           sliceCount,
                    unsigned           color,
                    MINI_VertexFormat* pVertexFormat,
                    float**            pVertices,
                    unsigned*          pVertexCount)
{
    int   i;
    int   index;
    float x;
    float y;
    float angle;

    // calculate the slice step
    const float step  = (2.0f * M_PI) / (float)sliceCount;

    miniCalculateStride(pVertexFormat);

    *pVertices    = 0;
    *pVertexCount = 0;

    index = 0;
    angle = 0.0f;

    // iterate through disk slices to create
    for (i = 0; i <= sliceCount + 1; ++i)
    {
        // is the first point to calculate?
        if (!i)
        {
            // get the center
            x = centerX;
            y = centerY;
        }
        else
        {
            // calculate the current slice angle
            angle = step * (float)(i - 1);

            // calculate the slice point
            x = centerX + radius * cos(angle);
            y = centerY + radius * sin(angle);
        }

        // count the vertices
        ++(*pVertexCount);

        // add vertices to the buffer
        if (!(*pVertices))
            *pVertices = (float*)malloc(*pVertexCount * pVertexFormat->m_Stride * sizeof(float));
        else
            *pVertices = (float*)realloc(*pVertices, *pVertexCount * pVertexFormat->m_Stride * sizeof(float));

        // add min point in buffer
        (*pVertices)[index]     = x;
        (*pVertices)[index + 1] = y;
        (*pVertices)[index + 2] = 0.0f;

        index += 3;

        // do use normals?
        if (pVertexFormat->m_UseNormals)
        {
            // set normal data
            (*pVertices)[index]     = 0.0f;
            (*pVertices)[index + 1] = 0.0f;
            (*pVertices)[index + 2] = 1.0f;

            index += 3;
        }

        // do use textures?
        if (pVertexFormat->m_UseTextures)
        {
            // set texture data
            if (!i)
            {
                (*pVertices)[index]     = 0.5f;
                (*pVertices)[index + 1] = 0.5f;
            }
            else
            {
                (*pVertices)[index]     = 0.5f + (cos(angle) * 0.5f);
                (*pVertices)[index + 1] = 0.5f + (sin(angle) * 0.5f);
            }

            index += 2;
        }

        // do use colors?
        if (pVertexFormat->m_UseColors)
        {
            // set color data
            (*pVertices)[index]     = (float)((color >> 24) & 0xFF) / 255.0f;
            (*pVertices)[index + 1] = (float)((color >> 16) & 0xFF) / 255.0f;
            (*pVertices)[index + 2] = (float)((color >> 8)  & 0xFF) / 255.0f;
            (*pVertices)[index + 3] = (float) (color        & 0xFF) / 255.0f;

            index += 4;
        }
    }
}
//------------------------------------------------------------------------------
void miniCreateRing(float              centerX,
                    float              centerY,
                    float              minRadius,
                    float              maxRadius,
                    unsigned           sliceCount,
                    unsigned           minColor,
                    unsigned           maxColor,
                    MINI_VertexFormat* pVertexFormat,
                    float**            pVertices,
                    unsigned*          pVertexCount)
{
    int   i;
    int   index;
    float xA;
    float yA;
    float xB;
    float yB;
    float angle;
    float texU;

    // calculate the slice step
    const float step = (2.0f * M_PI) / (float)sliceCount;

    index = 0;

    miniCalculateStride(pVertexFormat);

    *pVertices    = 0;
    *pVertexCount = 0;

    // iterate through ring slices to create
    for (i = 0; i <= sliceCount; ++i)
    {
        // calculate the current slice angle
        angle = step * (float)i;

        // calculate the slice min point
        xA = centerX + minRadius * cos(angle);
        yA = centerY - minRadius * sin(angle);

        // calculate the slice max point
        xB = centerX + maxRadius * cos(angle);
        yB = centerY - maxRadius * sin(angle);

        // calculate texture u coordinate
        if (!i)
            texU = 0.0f;
        else
        if (i == sliceCount)
            texU = 1.0f;
        else
            texU = (float)i / (float)sliceCount;

        // count 2 vertices
        *pVertexCount += 2;

        // add vertices to the buffer
        if (!(*pVertices))
            *pVertices = (float*)malloc(*pVertexCount * pVertexFormat->m_Stride * sizeof(float));
        else
            *pVertices = (float*)realloc(*pVertices, *pVertexCount * pVertexFormat->m_Stride * sizeof(float));

        // add min point in buffer
        (*pVertices)[index]     = xA;
        (*pVertices)[index + 1] = yA;
        (*pVertices)[index + 2] = 0.0f;

        index += 3;

        // do use normals?
        if (pVertexFormat->m_UseNormals)
        {
            // set normal data
            (*pVertices)[index]     = 0.0f;
            (*pVertices)[index + 1] = 0.0f;
            (*pVertices)[index + 2] = 1.0f;

            index += 3;
        }

        // do use textures?
        if (pVertexFormat->m_UseTextures)
        {
            // set texture data
            (*pVertices)[index]     = texU;
            (*pVertices)[index + 1] = 0.0f;

            index += 2;
        }

        // do use colors?
        if (pVertexFormat->m_UseColors)
        {
            // set color data
            (*pVertices)[index]     = (float)((minColor >> 24) & 0xFF) / 255.0f;
            (*pVertices)[index + 1] = (float)((minColor >> 16) & 0xFF) / 255.0f;
            (*pVertices)[index + 2] = (float)((minColor >> 8)  & 0xFF) / 255.0f;
            (*pVertices)[index + 3] = (float) (minColor        & 0xFF) / 255.0f;

            index += 4;
        }

        // add max point in the buffer
        (*pVertices)[index]     = xB;
        (*pVertices)[index + 1] = yB;
        (*pVertices)[index + 2] = 0.0f;

        index += 3;

        // do use normals?
        if (pVertexFormat->m_UseNormals)
        {
            // set normal data
            (*pVertices)[index]     = 0.0f;
            (*pVertices)[index + 1] = 0.0f;
            (*pVertices)[index + 2] = 1.0f;

            index += 3;
        }

        // do use textures?
        if (pVertexFormat->m_UseTextures)
        {
            // set texture data
            (*pVertices)[index]     = texU;
            (*pVertices)[index + 1] = 1.0f;

            index += 2;
        }

        // do use colors?
        if (pVertexFormat->m_UseColors)
        {
            // set color data
            (*pVertices)[index]     = (float)((maxColor >> 24) & 0xFF) / 255.0f;
            (*pVertices)[index + 1] = (float)((maxColor >> 16) & 0xFF) / 255.0f;
            (*pVertices)[index + 2] = (float)((maxColor >> 8)  & 0xFF) / 255.0f;
            (*pVertices)[index + 3] = (float) (maxColor        & 0xFF) / 255.0f;

            index += 4;
        }
    }
}
//------------------------------------------------------------------------------
void miniCreateSpiral(float              centerX,
                      float              centerY,
                      float              minRadius,
                      float              maxRadius,
                      float              deltaMin,
                      float              deltaMax,
                      float              deltaZ,
                      unsigned           stackCount,
                      unsigned           sliceCount,
                      unsigned           minColor,
                      unsigned           maxColor,
                      MINI_VertexFormat* pVertexFormat,
                      float**            pVertices,
                      unsigned*          pVertexCount,
                      MINI_Index**       pIndexes,
                      unsigned*          pIndexCount)
{
    int   i;
    int   j;
    int   index;
    float xA;
    float yA;
    float xB;
    float yB;
    float angle;
    float z;
    float texU;

    // calculate the slice step
    const float step  = (2.0f * M_PI) / (float)sliceCount;

    miniCalculateStride(pVertexFormat);

    z     = 0.0f;
    index = 0;

    *pVertices    = 0;
    *pVertexCount = 0;

    // iterate through spiral stacks to create
    for (i = 0; i < stackCount; ++i)
    {
        *pIndexCount += 1;

        // add vertices to the buffer
        if (!(*pIndexes))
            *pIndexes = (MINI_Index*)malloc(*pIndexCount * sizeof(MINI_Index));
        else
            *pIndexes = (MINI_Index*)realloc(*pIndexes, *pIndexCount * sizeof(MINI_Index));

        // populate the next indice
        (*pIndexes)[i].m_Start  = index;
        (*pIndexes)[i].m_Length = (sliceCount + 1) * 2;
        (*pIndexes)[i].m_GlCmd  = E_TriangleStrip;

        // iterate through spiral slices to create
        for (j = 0; j <= sliceCount; ++j)
        {
            // calculate the current slice angle
            angle = step * (float)j;

            // calculate the slice min point
            xA = centerX + minRadius * cos(angle);
            yA = centerY + minRadius * sin(angle);

            // calculate the slice max point
            xB = centerX + maxRadius * cos(angle);
            yB = centerY + maxRadius * sin(angle);

            // calculate the spiral curve
            minRadius += deltaMin;
            maxRadius += deltaMax;

            // calculate the z position
            z -= deltaZ;

            // calculate texture u coordinate
            if (!j)
                texU = 0.0f;
            else
            if (j == sliceCount)
                texU = 1.0f;
            else
                texU = (float)j / (float)sliceCount;

            // count 2 vertices
            *pVertexCount += 2;

            // add vertices to the buffer
            if (!(*pVertices))
                *pVertices = (float*)malloc(*pVertexCount * pVertexFormat->m_Stride * sizeof(float));
            else
                *pVertices = (float*)realloc(*pVertices, *pVertexCount * pVertexFormat->m_Stride * sizeof(float));

            // add min point in buffer
            (*pVertices)[index]     = xA;
            (*pVertices)[index + 1] = yA;
            (*pVertices)[index + 2] = z;

            index += 3;

            // do use normals?
            if (pVertexFormat->m_UseNormals)
            {
                // set normal data
                (*pVertices)[index]     = 0.0f;
                (*pVertices)[index + 1] = 0.0f;
                (*pVertices)[index + 2] = 1.0f;

                index += 3;
            }

            // do use textures?
            if (pVertexFormat->m_UseTextures)
            {
                // set texture data
                (*pVertices)[index]     = texU;
                (*pVertices)[index + 1] = 0.0f;

                index += 2;
            }

            // do use colors?
            if (pVertexFormat->m_UseColors)
            {
                // set color data
                (*pVertices)[index]     = (float)((minColor >> 24) & 0xFF) / 255.0f;
                (*pVertices)[index + 1] = (float)((minColor >> 16) & 0xFF) / 255.0f;
                (*pVertices)[index + 2] = (float)((minColor >> 8)  & 0xFF) / 255.0f;
                (*pVertices)[index + 3] = (float) (minColor        & 0xFF) / 255.0f;

                index += 4;
            }

            // add max point in the buffer
            (*pVertices)[index]     = xB;
            (*pVertices)[index + 1] = yB;
            (*pVertices)[index + 2] = z;

            index += 3;

            // do use normals?
            if (pVertexFormat->m_UseNormals)
            {
                // set normal data
                (*pVertices)[index]     = 0.0f;
                (*pVertices)[index + 1] = 0.0f;
                (*pVertices)[index + 2] = 1.0f;

                index += 3;
            }

            // do use textures?
            if (pVertexFormat->m_UseTextures)
            {
                // set texture data
                (*pVertices)[index]     = texU;
                (*pVertices)[index + 1] = 1.0f;

                index += 2;
            }

            // do use colors?
            if (pVertexFormat->m_UseColors)
            {
                // set color data
                (*pVertices)[index]     = (float)((maxColor >> 24) & 0xFF) / 255.0f;
                (*pVertices)[index + 1] = (float)((maxColor >> 16) & 0xFF) / 255.0f;
                (*pVertices)[index + 2] = (float)((maxColor >> 8)  & 0xFF) / 255.0f;
                (*pVertices)[index + 3] = (float) (maxColor        & 0xFF) / 255.0f;

                index += 4;
            }
        }

        // correct the last values otherwise the spiral will appears broken
        minRadius -= deltaMin;
        maxRadius -= deltaMax;
        z         += deltaZ;
    }
}
//----------------------------------------------------------------------------
