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

#include "MiniRenderer.h"

//----------------------------------------------------------------------------
void miniBeginScene(float r, float g, float b, float a)
{
    // clear scene background and depth buffer
    glClearColor(r, g, b, a);
    glClearDepthf(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void miniEndScene(void)
{}
//----------------------------------------------------------------------------
void miniDrawBuffer(float*             pVertices,
                    unsigned           vertexCount,
                    MINI_EDrawStyle    drawStyle,
                    MINI_VertexFormat* pVertexFormat,
                    MINI_Shader*       pShader)
{
    GLvoid*      pCoords;
    GLvoid*      pNormals;
    GLvoid*      pTexCoords;
    GLvoid*      pColors;
    GLsizei      stride;
    unsigned int offset;

    // calculate vertex stride
    stride = pVertexFormat->m_Stride;

    offset = 0;

    // send vertices to shader
    pCoords = &pVertices[offset];
    glVertexAttribPointer(pShader->m_VertexSlot,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          stride * sizeof(float),
                          pCoords);

    offset += 3;

    // do use normals?
    if (pVertexFormat->m_UseNormals)
    {
        // send normals to shader
        pNormals = &pVertices[offset];
        glVertexAttribPointer(pShader->m_TexCoordSlot,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              stride * sizeof(float),
                              pNormals);

        offset += 3;
    }

    // do use textures?
    if (pVertexFormat->m_UseTextures)
    {
        // send textures to shader
        pTexCoords = &pVertices[offset];
        glVertexAttribPointer(pShader->m_TexCoordSlot,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              stride * sizeof(float),
                              pTexCoords);

        offset += 2;
    }

    // do use colors?
    if (pVertexFormat->m_UseColors)
    {
        // send colors to shader
        pColors = &pVertices[offset];
        glVertexAttribPointer(pShader->m_ColorSlot,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              stride * sizeof(float),
                              pColors);
    }

    // draw the buffer
    switch (drawStyle)
    {
        case E_Triangles:     glDrawArrays(GL_TRIANGLES,      0, vertexCount); break;
        case E_TriangleStrip: glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount); break;
        case E_TriangleFan:   glDrawArrays(GL_TRIANGLE_FAN,   0, vertexCount); break;
    }
}
//----------------------------------------------------------------------------
void miniDrawSurface(float*             pVertices,
                     unsigned           vertexCount,
                     MINI_VertexFormat* pVertexFormat,
                     MINI_Shader*       pShader)
{
    // enable position slot
    glEnableVertexAttribArray(pShader->m_VertexSlot);

    // enable normal slot
    if (pVertexFormat->m_UseNormals)
        glEnableVertexAttribArray(pShader->m_NormalSlot);

    // enable texture slot
    if (pVertexFormat->m_UseTextures)
        glEnableVertexAttribArray(pShader->m_TexCoordSlot);

    // enable color slot
    if (pVertexFormat->m_UseColors)
        glEnableVertexAttribArray(pShader->m_ColorSlot);

    // draw the vertex buffer
    miniDrawBuffer(pVertices,
                   vertexCount,
                   E_TriangleStrip,
                   pVertexFormat,
                   pShader);

    // disable vertices slots from shader
    glDisableVertexAttribArray(pShader->m_VertexSlot);

    // disable normal slot
    if (pVertexFormat->m_UseNormals)
        glDisableVertexAttribArray(pShader->m_NormalSlot);

    // disable texture slot
    if (pVertexFormat->m_UseTextures)
        glDisableVertexAttribArray(pShader->m_TexCoordSlot);

    // disable color slot
    if (pVertexFormat->m_UseColors)
        glDisableVertexAttribArray(pShader->m_ColorSlot);
}
//----------------------------------------------------------------------------
void miniDrawSphere(float*             pVertices,
                    unsigned           vertexCount,
                    MINI_Index*        pIndexes,
                    unsigned           indexCount,
                    MINI_VertexFormat* pVertexFormat,
                    MINI_Shader*       pShader)
{
    unsigned i;

    // enable position slot
    glEnableVertexAttribArray(pShader->m_VertexSlot);

    // enable normal slot
    if (pVertexFormat->m_UseNormals)
        glEnableVertexAttribArray(pShader->m_NormalSlot);

    // enable texture slot
    if (pVertexFormat->m_UseTextures)
        glEnableVertexAttribArray(pShader->m_TexCoordSlot);

    // enable color slot
    if (pVertexFormat->m_UseColors)
        glEnableVertexAttribArray(pShader->m_ColorSlot);

    // iterate through vertex buffers to draw
    for (i = 0; i < indexCount; ++i)
        // draw the vertex buffer
        miniDrawBuffer(&pVertices[pIndexes[i].m_Start],
                       pIndexes[i].m_Length / pVertexFormat->m_Stride,
                       E_TriangleStrip,
                       pVertexFormat,
                       pShader);

    // disable vertices slots from shader
    glDisableVertexAttribArray(pShader->m_VertexSlot);

    // disable normal slot
    if (pVertexFormat->m_UseNormals)
        glDisableVertexAttribArray(pShader->m_NormalSlot);

    // disable texture slot
    if (pVertexFormat->m_UseTextures)
        glDisableVertexAttribArray(pShader->m_TexCoordSlot);

    // disable color slot
    if (pVertexFormat->m_UseColors)
        glDisableVertexAttribArray(pShader->m_ColorSlot);
}
//----------------------------------------------------------------------------
void miniDrawCylinder(float*             pVertices,
                      unsigned           vertexCount,
                      MINI_VertexFormat* pVertexFormat,
                      MINI_Shader*       pShader)
{
    // enable position slot
    glEnableVertexAttribArray(pShader->m_VertexSlot);

    // enable normal slot
    if (pVertexFormat->m_UseNormals)
        glEnableVertexAttribArray(pShader->m_NormalSlot);

    // enable texture slot
    if (pVertexFormat->m_UseTextures)
        glEnableVertexAttribArray(pShader->m_TexCoordSlot);

    // enable color slot
    if (pVertexFormat->m_UseColors)
        glEnableVertexAttribArray(pShader->m_ColorSlot);

    // draw the vertex buffer
    miniDrawBuffer(pVertices,
                   (vertexCount + 1) * 2,
                   E_TriangleStrip,
                   pVertexFormat,
                   pShader);

    // disable vertices slots from shader
    glDisableVertexAttribArray(pShader->m_VertexSlot);

    // disable normal slot
    if (pVertexFormat->m_UseNormals)
        glDisableVertexAttribArray(pShader->m_NormalSlot);

    // disable texture slot
    if (pVertexFormat->m_UseTextures)
        glDisableVertexAttribArray(pShader->m_TexCoordSlot);

    // disable color slot
    if (pVertexFormat->m_UseColors)
        glDisableVertexAttribArray(pShader->m_ColorSlot);
}
//----------------------------------------------------------------------------
void miniDrawDisk(float*             pVertices,
                  unsigned           vertexCount,
                  MINI_VertexFormat* pVertexFormat,
                  MINI_Shader*       pShader)
{
    // enable position slot
    glEnableVertexAttribArray(pShader->m_VertexSlot);

    // enable normal slot
    if (pVertexFormat->m_UseNormals)
        glEnableVertexAttribArray(pShader->m_NormalSlot);

    // enable texture slot
    if (pVertexFormat->m_UseTextures)
        glEnableVertexAttribArray(pShader->m_TexCoordSlot);

    // enable color slot
    if (pVertexFormat->m_UseColors)
        glEnableVertexAttribArray(pShader->m_ColorSlot);

    // draw the vertex buffer
    miniDrawBuffer(pVertices,
                   vertexCount,
                   E_TriangleFan,
                   pVertexFormat,
                   pShader);

    // disable vertices slots from shader
    glDisableVertexAttribArray(pShader->m_VertexSlot);

    // disable normal slot
    if (pVertexFormat->m_UseNormals)
        glDisableVertexAttribArray(pShader->m_NormalSlot);

    // disable texture slot
    if (pVertexFormat->m_UseTextures)
        glDisableVertexAttribArray(pShader->m_TexCoordSlot);

    // disable color slot
    if (pVertexFormat->m_UseColors)
        glDisableVertexAttribArray(pShader->m_ColorSlot);
}
//----------------------------------------------------------------------------
void miniDrawRing(float*             pVertices,
                  unsigned           vertexCount,
                  MINI_VertexFormat* pVertexFormat,
                  MINI_Shader*       pShader)
{
    // enable position slot
    glEnableVertexAttribArray(pShader->m_VertexSlot);

    // enable normal slot
    if (pVertexFormat->m_UseNormals)
        glEnableVertexAttribArray(pShader->m_NormalSlot);

    // enable texture slot
    if (pVertexFormat->m_UseTextures)
        glEnableVertexAttribArray(pShader->m_TexCoordSlot);

    // enable color slot
    if (pVertexFormat->m_UseColors)
        glEnableVertexAttribArray(pShader->m_ColorSlot);

    // draw the vertex buffer
    miniDrawBuffer(pVertices,
                   vertexCount,
                   E_TriangleStrip,
                   pVertexFormat,
                   pShader);

    // disable vertices slots from shader
    glDisableVertexAttribArray(pShader->m_VertexSlot);

    // disable normal slot
    if (pVertexFormat->m_UseNormals)
        glDisableVertexAttribArray(pShader->m_NormalSlot);

    // disable texture slot
    if (pVertexFormat->m_UseTextures)
        glDisableVertexAttribArray(pShader->m_TexCoordSlot);

    // disable color slot
    if (pVertexFormat->m_UseColors)
        glDisableVertexAttribArray(pShader->m_ColorSlot);
}
//----------------------------------------------------------------------------
void miniDrawSpiral(float*             pVertices,
                    unsigned           vertexCount,
                    MINI_Index*        pIndexes,
                    unsigned           indexCount,
                    MINI_VertexFormat* pVertexFormat,
                    MINI_Shader*       pShader)
{
    unsigned i;

    // enable position slot
    glEnableVertexAttribArray(pShader->m_VertexSlot);

    // enable normal slot
    if (pVertexFormat->m_UseNormals)
        glEnableVertexAttribArray(pShader->m_NormalSlot);

    // enable texture slot
    if (pVertexFormat->m_UseTextures)
        glEnableVertexAttribArray(pShader->m_TexCoordSlot);

    // enable color slot
    if (pVertexFormat->m_UseColors)
        glEnableVertexAttribArray(pShader->m_ColorSlot);

    // iterate through vertex buffers to draw
    for (i = 0; i < indexCount; ++i)
        // draw the vertex buffer
        miniDrawBuffer(pVertices + pIndexes[i].m_Start,
                       pIndexes[i].m_Length,
                       pIndexes[i].m_GlCmd,
                       pVertexFormat,
                       pShader);

    // disable vertices slots from shader
    glDisableVertexAttribArray(pShader->m_VertexSlot);

    // disable normal slot
    if (pVertexFormat->m_UseNormals)
        glDisableVertexAttribArray(pShader->m_NormalSlot);

    // disable texture slot
    if (pVertexFormat->m_UseTextures)
        glDisableVertexAttribArray(pShader->m_TexCoordSlot);

    // disable color slot
    if (pVertexFormat->m_UseColors)
        glDisableVertexAttribArray(pShader->m_ColorSlot);
}
//----------------------------------------------------------------------------
void miniDrawMDL(MINI_MDLModel* pModel, MINI_Shader* pShader, int index)
{
    MINI_Frame*  pFrame;
    MINI_Mesh*   pMesh;
    float*       pVB;
    unsigned int i;

    // enable position slot
    glEnableVertexAttribArray(pShader->m_VertexSlot);

    // enable normal slot
    if (pModel->m_pVertexFormat->m_UseNormals)
        glEnableVertexAttribArray(pShader->m_NormalSlot);

    // enable texture slot
    if (pModel->m_pVertexFormat->m_UseTextures)
        glEnableVertexAttribArray(pShader->m_TexCoordSlot);

    // enable color slot
    if (pModel->m_pVertexFormat->m_UseColors)
        glEnableVertexAttribArray(pShader->m_ColorSlot);

    // get current frame
    pFrame = &pModel->m_pFrame[index];

    // iterate through vertices to draw
    for (i = 0; i < pFrame->m_MeshCount; ++i)
    {
        // get current vertices
        pMesh = &pFrame->m_pMesh[i];
        pVB   = &pMesh->m_pVertexBuffer[0];

        // draw the vertex buffer
        miniDrawBuffer(pVB,
                       pMesh->m_VertexCount,
                       E_Triangles,
                       pModel->m_pVertexFormat,
                       pShader);
    }

    // disable vertices slots from shader
    glDisableVertexAttribArray(pShader->m_VertexSlot);

    // disable normal slot
    if (pModel->m_pVertexFormat->m_UseNormals)
        glDisableVertexAttribArray(pShader->m_NormalSlot);

    // disable texture slot
    if (pModel->m_pVertexFormat->m_UseTextures)
        glDisableVertexAttribArray(pShader->m_TexCoordSlot);

    // disable color slot
    if (pModel->m_pVertexFormat->m_UseColors)
        glDisableVertexAttribArray(pShader->m_ColorSlot);
}
//----------------------------------------------------------------------------
void miniDrawMD2(MINI_MD2Model* pModel, MINI_Shader* pShader, int index)
{
    MINI_Frame*  pFrame;
    MINI_Mesh*   pMesh;
    float*       pVB;
    unsigned int i;

    // enable position slot
    glEnableVertexAttribArray(pShader->m_VertexSlot);

    // enable normal slot
    if (pModel->m_pVertexFormat->m_UseNormals)
        glEnableVertexAttribArray(pShader->m_NormalSlot);

    // enable texture slot
    if (pModel->m_pVertexFormat->m_UseTextures)
        glEnableVertexAttribArray(pShader->m_TexCoordSlot);

    // enable color slot
    if (pModel->m_pVertexFormat->m_UseColors)
        glEnableVertexAttribArray(pShader->m_ColorSlot);

    // get current frame
    pFrame = &pModel->m_pFrame[index];

    // iterate through vertices to draw
    for (i = 0; i < pFrame->m_MeshCount; ++i)
    {
        // get current vertices
        pMesh = &pFrame->m_pMesh[i];
        pVB   = &pMesh->m_pVertexBuffer[0];

        // draw vertices
        if (pMesh->m_IsTriangleStrip)
            miniDrawBuffer(pVB,
                           pMesh->m_VertexCount,
                           E_TriangleStrip,
                           pModel->m_pVertexFormat,
                           pShader);
        else
            miniDrawBuffer(pVB,
                           pMesh->m_VertexCount,
                           E_TriangleFan,
                           pModel->m_pVertexFormat,
                           pShader);
    }

    // disable vertices slots from shader
    glDisableVertexAttribArray(pShader->m_VertexSlot);

    // disable normal slot
    if (pModel->m_pVertexFormat->m_UseNormals)
        glDisableVertexAttribArray(pShader->m_NormalSlot);

    // disable texture slot
    if (pModel->m_pVertexFormat->m_UseTextures)
        glDisableVertexAttribArray(pShader->m_TexCoordSlot);

    // disable color slot
    if (pModel->m_pVertexFormat->m_UseColors)
        glDisableVertexAttribArray(pShader->m_ColorSlot);
}
//----------------------------------------------------------------------------
void miniDrawLandscape(MINI_Mesh*         pMesh,
                       MINI_VertexFormat* pVertexFormat,
                       MINI_Shader*       pShader)
{
    // enable position slot
    glEnableVertexAttribArray(pShader->m_VertexSlot);

    // enable normal slot
    if (pVertexFormat->m_UseNormals)
        glEnableVertexAttribArray(pShader->m_NormalSlot);

    // enable texture slot
    if (pVertexFormat->m_UseTextures)
        glEnableVertexAttribArray(pShader->m_TexCoordSlot);

    // enable color slot
    if (pVertexFormat->m_UseColors)
        glEnableVertexAttribArray(pShader->m_ColorSlot);

    // draw the landscape model
    miniDrawBuffer(pMesh->m_pVertexBuffer,
                   pMesh->m_VertexCount / pVertexFormat->m_Stride,
                   E_Triangles,
                   pVertexFormat,
                   pShader);

    // disable vertices slots from shader
    glDisableVertexAttribArray(pShader->m_VertexSlot);

    // disable normal slot
    if (pVertexFormat->m_UseNormals)
        glDisableVertexAttribArray(pShader->m_NormalSlot);

    // disable texture slot
    if (pVertexFormat->m_UseTextures)
        glDisableVertexAttribArray(pShader->m_TexCoordSlot);

    // disable color slot
    if (pVertexFormat->m_UseColors)
        glDisableVertexAttribArray(pShader->m_ColorSlot);
}
//----------------------------------------------------------------------------
void miniDrawRover(float*             pVertices,
                   unsigned           vertexCount,
                   MINI_MdlCmds*      pMdlCmds,
                   MINI_Index*        pIndexes,
                   unsigned           indexCount,
                   MINI_VertexFormat* pVertexFormat,
                   MINI_Shader*       pShader)
{
    unsigned i;

    // enable position slot
    glEnableVertexAttribArray(pShader->m_VertexSlot);

    // enable normal slot
    if (pVertexFormat->m_UseNormals)
        glEnableVertexAttribArray(pShader->m_NormalSlot);

    // enable texture slot
    if (pVertexFormat->m_UseTextures)
        glEnableVertexAttribArray(pShader->m_TexCoordSlot);

    // enable color slot
    if (pVertexFormat->m_UseColors)
        glEnableVertexAttribArray(pShader->m_ColorSlot);

    // iterate through vertex fan buffers to draw
    for (i = 0; i < indexCount; ++i)
    {
        // configure culling
        if (pMdlCmds[i].m_CullMode == 0)
        {
            // disabled
            glDisable(GL_CULL_FACE);
            glCullFace(GL_NONE);
        }
        else
        {
            // enabled
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
        }

        // draw the next rover vertex buffer
        if (pMdlCmds[i].m_GLCmd == 0)
            miniDrawBuffer(&pVertices[pIndexes[i].m_Start],
                           pIndexes[i].m_Length / pVertexFormat->m_Stride,
                           E_TriangleStrip,
                           pVertexFormat,
                           pShader);
        else
            miniDrawBuffer(&pVertices[pIndexes[i].m_Start],
                           pIndexes[i].m_Length / pVertexFormat->m_Stride,
                           E_TriangleFan,
                           pVertexFormat,
                           pShader);
    }

    // disable vertices slots from shader
    glDisableVertexAttribArray(pShader->m_VertexSlot);

    // disable normal slot
    if (pVertexFormat->m_UseNormals)
        glDisableVertexAttribArray(pShader->m_NormalSlot);

    // disable texture slot
    if (pVertexFormat->m_UseTextures)
        glDisableVertexAttribArray(pShader->m_TexCoordSlot);

    // disable color slot
    if (pVertexFormat->m_UseColors)
        glDisableVertexAttribArray(pShader->m_ColorSlot);
}
//----------------------------------------------------------------------------
