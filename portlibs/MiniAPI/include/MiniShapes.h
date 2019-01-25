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

#ifndef MiniShapesH
#define MiniShapesH

// mini API
#include "MiniVertex.h"

#ifdef __cplusplus
    extern "C"
    {
#endif

        //----------------------------------------------------------------------------
        // Shapes creation functions
        //----------------------------------------------------------------------------

        /**
        * Creates a surface
        *@param pWidth - surface width
        *@param pHeight - surface height
        *@param color - color in RGBA format
        *@param[in, out] pVertexFormat - vertex format to use
        *@param[out] pVertices - resulting vertex buffer
        *@param[out] pVertexCount - vertex count in buffer
        */
        void miniCreateSurface(const float*             pWidth,
                               const float*             pHeight,
                                     unsigned int       color,
                                     MINI_VertexFormat* pVertexFormat,
                                     float**            pVertices,
                                     unsigned*          pVertexCount);

        /**
        * Creates a sphere
        *@param pRadius - sphere radius
        *@param slices - slices (longitude) number
        *@param stacks - stacks (latitude) number
        *@param color - color in RGBA format
        *@param[in, out] pVertexFormat - vertex format to use
        *@param[out] pVertices - resulting vertex buffer
        *@param[out] pVertexCount - vertex count in buffer
        *@param[out] pIndexes - index list containing offset and length of each vertex buffer composing sphere
        *@param[out] pIndexCount - resulting index count
        */
        void miniCreateSphere(const float*             pRadius,
                                    int                slices,
                                    int                stacks,
                                    unsigned           color,
                                    MINI_VertexFormat* pVertexFormat,
                                    float**            pVertices,
                                    unsigned*          pVertexCount,
                                    MINI_Index**       pIndexes,
                                    unsigned*          pIndexCount);

        /**
        * Creates a cylinder
        *@param pRadius - cylinder radius
        *@param pHeight - cylinder height
        *@param faces - number of faces composing the cylinder
        *@param color - color in RGBA format
        *@param[in, out] pVertexFormat - vertex format to use
        *@param[out] pVertices - resulting vertex buffer
        *@param[out] pVertexCount - vertex count in buffer
        */
        void miniCreateCylinder(const float*             pRadius,
                                const float*             pHeight,
                                      int                faces,
                                      unsigned           color,
                                      MINI_VertexFormat* pVertexFormat,
                                      float**            pVertices,
                                      unsigned*          pVertexCount);

        /**
        * Creates a disk
        *@param centerX - the disk center on the x axis
        *@param centerY - the disk center on the y axis
        *@param radius - disk radius
        *@param sliceCount - disk slice count
        *@param color - disk color
        *@param pVertexFormat - vertex format to apply
        *@param[out] pVertices - resulting vertex buffer
        *@param[out] pVertexCount - vertex count in buffer
        */
        void miniCreateDisk(float              centerX,
                            float              centerY,
                            float              radius,
                            unsigned           sliceCount,
                            unsigned           color,
                            MINI_VertexFormat* pVertexFormat,
                            float**            pVertices,
                            unsigned*          pVertexCount);

        /**
        * Creates a ring
        *@param centerX - the ring center on the x axis
        *@param centerY - the ring center on the y axis
        *@param minRadius - internal radius
        *@param maxRadius - external radius
        *@param sliceCount - slice count
        *@param minColor - internal edge color
        *@param maxColor - external edge color
        *@param pVertexFormat - vertex format to apply
        *@param[out] pVertices - resulting vertex buffer
        *@param[out] pVertexCount - vertex count in buffer
        */
        void miniCreateRing(float              centerX,
                            float              centerY,
                            float              minRadius,
                            float              maxRadius,
                            unsigned           sliceCount,
                            unsigned           minColor,
                            unsigned           maxColor,
                            MINI_VertexFormat* pVertexFormat,
                            float**            pVertices,
                            unsigned*          pVertexCount);

        /**
        * Creates a spiral
        *@param centerX - the spiral center on the x axis
        *@param centerY - the spiral center on the y axis
        *@param minRadius - internal radius
        *@param maxRadius - external radius
        *@param deltaMin - delta to apply to the min radius
        *@param deltaMax - delta to apply to the max radius
        *@param deltaZ - delta to apply to the z radius
        *@param stackCount - stack count
        *@param sliceCount - slice count
        *@param minColor - internal edge color
        *@param maxColor - external edge color
        *@param pVertexFormat - vertex format to apply
        *@param[out] pVertices - resulting vertex buffer
        *@param[out] pVertexCount - vertex count in buffer
        *@param[out] pIndexes - resulting index buffer
        *@param[out] pIndexCount - index count in buffer
        */
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
                              unsigned*          pIndexCount);

#ifdef __cplusplus
    }
#endif

//----------------------------------------------------------------------------
// Compiler
//----------------------------------------------------------------------------

// needed in mobile c compiler to link the .h file with the .c
#if defined(_OS_IOS_) || defined(_OS_ANDROID_) || defined(_OS_WINDOWS_)
    #include "MiniShapes.c"
#endif

#endif // MiniShapesH
