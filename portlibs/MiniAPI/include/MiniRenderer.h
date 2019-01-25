/*****************************************************************************
 * ==> MiniRenderer ---------------------------------------------------------*
 *****************************************************************************
 * Description : This module provides the functions required to draw the     *
 *               objects inside a scene                                      *
 * Developer   : Jean-Milost Reymond                                         *
 * Copyright   : 2015 - 2018, this file is part of the Minimal API. You are  *
 *               free to copy or redistribute this file, modify it, or use   *
 *               it for your own projects, commercial or not. This file is   *
 *               provided "as is", without ANY WARRANTY OF ANY KIND          *
 *****************************************************************************/

#ifndef MiniRendererH
#define MiniRendererH

// mini API
#include "MiniCommon.h"
#include "MiniModels.h"
#include "MiniShader.h"

//----------------------------------------------------------------------------
// Enumerators
//----------------------------------------------------------------------------

/**
* Draw style enumeration
*/
typedef enum
{
    E_Triangles,
    E_TriangleStrip,
    E_TriangleFan
} MINI_EDrawStyle;

#ifdef __cplusplus
    extern "C"
    {
#endif

        //----------------------------------------------------------------------------
        // Rendering functions
        //----------------------------------------------------------------------------

        /**
        * Begins to draw a scene
        *@param r - scene background color red component
        *@param g - scene background color green component
        *@param b - scene background color blue component
        *@param a - scene background color alpha component
        */
        void miniBeginScene(float r, float g, float b, float a);

        /**
        * Ends to draw a scene
        */
        void miniEndScene(void);

        /**
        * Draws a vertex buffer
        *@param pVertices - surface vertex buffer
        *@param vertexCount - vertex count in buffer
        *@param drawStyle - draw style
        *@param pVertexFormat - vertex format to use
        *@param pShader - shader parameters
        */
        void miniDrawBuffer(float*             pVertices,
                            unsigned           vertexCount,
                            MINI_EDrawStyle    drawStyle,
                            MINI_VertexFormat* pVertexFormat,
                            MINI_Shader*       pShader);

        /**
        * Draws a surface
        *@param pVertices - surface vertex buffer
        *@param vertexCount - vertex count in buffer
        *@param pVertexFormat - vertex format to use
        *@param pShader - shader parameters
        */
        void miniDrawSurface(float*             pVertices,
                             unsigned           vertexCount,
                             MINI_VertexFormat* pVertexFormat,
                             MINI_Shader*       pShader);

        /**
        * Draws a sphere
        *@param pVertices - sphere vertex buffer
        *@param vertexCount - vertex count in buffer
        *@param pIndexes - sphere indices buffer
        *@param indexCount - indice count in buffer
        *@param pVertexFormat - vertex format to use
        *@param pShader - shader parameters
        */
        void miniDrawSphere(float*             pVertices,
                            unsigned           vertexCount,
                            MINI_Index*        pIndexes,
                            unsigned           indexCount,
                            MINI_VertexFormat* pVertexFormat,
                            MINI_Shader*       pShader);

        /**
        * Draws a cylinder
        *@param pVertices - cylinder vertex buffer
        *@param vertexCount - vertex count in buffer
        *@param pVertexFormat - vertex format to use
        *@param pShader - shader parameters
        */
        void miniDrawCylinder(float*             pVertices,
                              unsigned           vertexCount,
                              MINI_VertexFormat* pVertexFormat,
                              MINI_Shader*       pShader);

        /**
        * Draws a disk
        *@param pVertices - disk vertex buffer
        *@param vertexCount - vertex count in buffer
        *@param pVertexFormat - vertex format to use
        *@param pShader - shader parameters
        */
        void miniDrawDisk(float*             pVertices,
                          unsigned           vertexCount,
                          MINI_VertexFormat* pVertexFormat,
                          MINI_Shader*       pShader);

        /**
        * Draws a ring
        *@param pVertices - ring vertex buffer
        *@param vertexCount - vertex count in buffer
        *@param pVertexFormat - vertex format to use
        *@param pShader - shader parameters
        */
        void miniDrawRing(float*             pVertices,
                          unsigned           vertexCount,
                          MINI_VertexFormat* pVertexFormat,
                          MINI_Shader*       pShader);

        /**
        * Draws a spiral
        *@param pVertices - spiral vertex buffer
        *@param vertexCount - vertex count in buffer
        *@param pIndexes - spiral indices buffer
        *@param indexCount - indice count in buffer
        *@param pVertexFormat - vertex format to use
        *@param pShader - shader parameters
        */
        void miniDrawSpiral(float*             pVertices,
                            unsigned           vertexCount,
                            MINI_Index*        pIndexes,
                            unsigned           indexCount,
                            MINI_VertexFormat* pVertexFormat,
                            MINI_Shader*       pShader);

        /**
        * Draws a MDL model
        *@param pModel - model to draw
        *@param pShader - shader parameters
        *@param index - model frame index
        */
        void miniDrawMDL(MINI_MDLModel* pModel, MINI_Shader* pShader, int index);

        /**
        * Draws a MD2 model
        *@param pModel - model to draw
        *@param pShader - shader parameters
        *@param index - model frame index
        */
        void miniDrawMD2(MINI_MD2Model* pModel, MINI_Shader* pShader, int index);

        /**
        * Draws a landscape model
        *@param pMesh - landscape mesh to draw
        *@param pVertexFormat - vertex format to use
        *@param pShader - shader parameters
        */
        void miniDrawLandscape(MINI_Mesh*         pMesh,
                               MINI_VertexFormat* pVertexFormat,
                               MINI_Shader*       pShader);

        /**
        * Draws a rover model
        *@param pVertices - model vertices
        *@param pVertexCount - vertex count
        *@param pMdlCmds - model commands
        *@param pIndexes - index buffer
        *@param pIndexCount - index count
        *@param pVertexFormat - vertex format
        *@param pShader - shader parameters
        */
        void miniDrawRover(float*             pVertices,
                           unsigned           vertexCount,
                           MINI_MdlCmds*      pMdlCmds,
                           MINI_Index*        pIndexes,
                           unsigned           indexCount,
                           MINI_VertexFormat* pVertexFormat,
                           MINI_Shader*       pShader);

#ifdef __cplusplus
    }
#endif

//----------------------------------------------------------------------------
// Compiler
//----------------------------------------------------------------------------

// needed in mobile c compiler to link the .h file with the .c
#if defined(_OS_IOS_) || defined(_OS_ANDROID_) || defined(_OS_WINDOWS_)
    #include "MiniRenderer.c"
#endif

#endif
