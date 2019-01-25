/*****************************************************************************
 * ==> MiniLevel ------------------------------------------------------------*
 *****************************************************************************
 * Description : This module provides a ray casting engine to draw a small   *
 *               game level like Wolfenstein                                 *
 * Developer   : Jean-Milost Reymond                                         *
 * Copyright   : 2015 - 2018, this file is part of the Minimal API. You are  *
 *               free to copy or redistribute this file, modify it, or use   *
 *               it for your own projects, commercial or not. This file is   *
 *               provided "as is", without ANY WARRANTY OF ANY KIND          *
 *****************************************************************************/

#include "MiniLevel.h"

// std
#include <math.h>

// mini api
#include "MiniCollision.h"
#include "MiniRenderer.h"

//-----------------------------------------------------------------------------
// Level edition functions
//-----------------------------------------------------------------------------
int miniGenerateLevel(const char*            pMap,
                            unsigned int     mapWidth,
                            unsigned int     mapHeight,
                      const float*           pItemWidth,
                      const float*           pItemHeight,
                            MINI_LevelItem** pLevel)
{
    unsigned int x;
    unsigned int y;
    unsigned int offset;
    float        levelWidth;
    float        levelHeight;

    // create the level array and calculate the width and height
    *pLevel      = (MINI_LevelItem*)malloc(sizeof(MINI_LevelItem) * mapWidth * mapHeight);
     levelWidth  = (*pItemWidth  * mapWidth);
     levelHeight = (*pItemHeight * mapHeight);

    offset = 0;

    // iterate through items to create
    for (y = 0; y < mapHeight; ++y)
        for (x = 0; x < mapWidth; ++x)
        {
            // calculate the item center
            (*pLevel)[offset].m_X      = -(levelWidth  * 0.5f) + (*pItemWidth  * x) - (*pItemWidth  * 0.5f);
            (*pLevel)[offset].m_Y      = -(levelHeight * 0.5f) + (*pItemHeight * y) - (*pItemHeight * 0.5f);
            (*pLevel)[offset].m_Width  = *pItemWidth;
            (*pLevel)[offset].m_Height = *pItemHeight;

            // is an empty item?
            if (pMap[offset] == '*')
            {
                // calculate which walls are visible
                (*pLevel)[offset].m_Left  = !x                   || (pMap[offset - 1]        == ' ');
                (*pLevel)[offset].m_Right = (x == mapWidth  - 1) || (pMap[offset + 1]        == ' ');
                (*pLevel)[offset].m_Front = (y == mapHeight - 1) || (pMap[offset + mapWidth] == ' ');
                (*pLevel)[offset].m_Back  = !y                   || (pMap[offset - mapWidth] == ' ');
                (*pLevel)[offset].m_Up    = 1;
                (*pLevel)[offset].m_Down  = 1;
            }
            else
            {
                (*pLevel)[offset].m_Left  = 0;
                (*pLevel)[offset].m_Right = 0;
                (*pLevel)[offset].m_Front = 0;
                (*pLevel)[offset].m_Back  = 0;
                (*pLevel)[offset].m_Up    = 0;
                (*pLevel)[offset].m_Down  = 0;
            }

            ++offset;
        }

    return 1;
}
//------------------------------------------------------------------------------
int miniBodyIntersectWall(const MINI_Vector2*   pBodyStart,
                          const MINI_Vector2*   pBodyEnd,
                          const MINI_LevelItem* pLevel,
                                float           levelItemWidth,
                                float           levelItemHeight,
                                int             levelItemCount)
{
    int          i;
    MINI_Vector2 itemVertex1;
    MINI_Vector2 itemVertex2;
    MINI_Vector2 itemVertex3;
    MINI_Vector2 itemVertex4;
    MINI_Vector2 intersection;
    MINI_Point   bodyStart;
    MINI_Point   bodyEnd;
    MINI_Rect    rect;
    int          isBodyOutOfLevel = 1;

    rect.m_Size.m_Width  = levelItemWidth;
    rect.m_Size.m_Height = levelItemHeight;

    // iterate through level items
    for (i = 0; i < levelItemCount; ++i)
    {
        // calculate the rect surrounding the item
        rect.m_Pos.m_X = pLevel[i].m_X + (levelItemWidth  * 0.5f);
        rect.m_Pos.m_Y = pLevel[i].m_Y + (levelItemHeight * 0.5f);

        // check if the body is inside the current item
        if (isBodyOutOfLevel)
        {
            bodyStart.m_X = pBodyStart->m_X;
            bodyStart.m_Y = pBodyStart->m_Y;
            bodyEnd.m_X   = pBodyEnd->m_X;
            bodyEnd.m_Y   = pBodyEnd->m_Y;

            if (miniPointInRect(&bodyStart, &rect) || miniPointInRect(&bodyEnd, &rect))
                isBodyOutOfLevel = 0;
        }

        // calculate item vertices
        itemVertex1.m_X = rect.m_Pos.m_X;
        itemVertex1.m_Y = rect.m_Pos.m_Y;
        itemVertex2.m_X = rect.m_Pos.m_X;
        itemVertex2.m_Y = rect.m_Pos.m_Y + levelItemHeight;
        itemVertex3.m_X = rect.m_Pos.m_X + levelItemWidth;
        itemVertex3.m_Y = rect.m_Pos.m_Y + levelItemHeight;
        itemVertex4.m_X = rect.m_Pos.m_X + levelItemWidth;
        itemVertex4.m_Y = rect.m_Pos.m_Y;

        // is body crossing a wall?
        if ((pLevel[i].m_Left  && miniLines2DIntersect(pBodyStart, pBodyEnd, &itemVertex1, &itemVertex2, &intersection)) ||
            (pLevel[i].m_Front && miniLines2DIntersect(pBodyStart, pBodyEnd, &itemVertex2, &itemVertex3, &intersection)) ||
            (pLevel[i].m_Right && miniLines2DIntersect(pBodyStart, pBodyEnd, &itemVertex3, &itemVertex4, &intersection)) ||
            (pLevel[i].m_Back  && miniLines2DIntersect(pBodyStart, pBodyEnd, &itemVertex4, &itemVertex1, &intersection)))
            return 1;
    }

    // return false if body is inside the level, otherwise true
    return isBodyOutOfLevel;
}
//------------------------------------------------------------------------------
int miniIsNextPosValid(const MINI_LevelItem* pLevel,
                             float           levelItemWidth,
                             float           levelItemHeight,
                             int             levelItemCount,
                       const MINI_Vector3*   pNewPos)
{
    int        i;
    MINI_Point pt;
    MINI_Rect  rect;

    // get the player position
    pt.m_X = pNewPos->m_X;
    pt.m_Y = pNewPos->m_Z;

    // set the rect size
    rect.m_Size.m_Width  = levelItemWidth;
    rect.m_Size.m_Height = levelItemHeight;

    // iterate through level items
    for (i = 0; i < levelItemCount; ++i)
    {
        // calculate the rect surrounding the item
        rect.m_Pos.m_X = pLevel[i].m_X + (levelItemWidth  * 0.5f);
        rect.m_Pos.m_Y = pLevel[i].m_Y + (levelItemHeight * 0.5f);

        // check if the player is inside the current item
        if (!miniPointInRect(&pt, &rect))
            continue;

        // going nowhere? (it's the case if the next item has no ground)
        return (pLevel[i].m_Down);
    }

    // next position is outside the map
    return 0;
}
//------------------------------------------------------------------------------
void miniValidateNextPos(const MINI_LevelItem* pLevel,
                               float           levelItemWidth,
                               float           levelItemHeight,
                               int             levelItemCount,
                         const MINI_Sphere*    pPlayer,
                               MINI_Vector3*   pNewPos)
{
    int          i;
    float        distToPlane;
    MINI_Point   pt;
    MINI_Vector3 pos;
    MINI_Vector3 n;
    MINI_Rect    rect;
    MINI_Plane   slidingPlane;

    // get the player position
    pt.m_X = pPlayer->m_Pos.m_X;
    pt.m_Y = pPlayer->m_Pos.m_Z;

    // set the rect size
    rect.m_Size.m_Width  = levelItemWidth;
    rect.m_Size.m_Height = levelItemHeight;

    // iterate through level items
    for (i = 0; i < levelItemCount; ++i)
    {
        // calculate the rect surrounding the item
        rect.m_Pos.m_X = pLevel[i].m_X + (levelItemWidth  * 0.5f);
        rect.m_Pos.m_Y = pLevel[i].m_Y + (levelItemHeight * 0.5f);

        // check if the player is inside the current item
        if (!miniPointInRect(&pt, &rect))
            continue;

        // item contains a wall on the left?
        if (pLevel[i].m_Left)
        {
            // calculate the wall position
            pos.m_X = pLevel[i].m_X + (levelItemWidth * 0.5f) + pPlayer->m_Radius;
            pos.m_Y = 0.0f;
            pos.m_Z = pLevel[i].m_Y +  levelItemHeight;

            // get the wall normal
            n.m_X = 1.0f;
            n.m_Y = 0.0f;
            n.m_Z = 0.0f;

            // get the wall plane and calculate the distance between the new pos and the wall
            miniPlaneFromPointNormal(&pos, &n, &slidingPlane);
            miniDistanceToPlane(pNewPos, &slidingPlane, &distToPlane);

            // found a collision? (It's the case if the next position is beyond the wall plane)
            if (distToPlane <= 0.0f)
                // correct the next position (just the x position, thus the player will slide along the wall)
                pNewPos->m_X = pos.m_X;
        }

        // item contains a wall on the right?
        if (pLevel[i].m_Right)
        {
            // calculate the wall position
            pos.m_X = pLevel[i].m_X + levelItemWidth + (levelItemWidth * 0.5f) - pPlayer->m_Radius;
            pos.m_Y = 0.0f;
            pos.m_Z = pLevel[i].m_Y + levelItemHeight;

            // get the wall normal
            n.m_X = -1.0f;
            n.m_Y =  0.0f;
            n.m_Z =  0.0f;

            // get the wall plane and calculate the distance between the new pos and the wall
            miniPlaneFromPointNormal(&pos, &n, &slidingPlane);
            miniDistanceToPlane(pNewPos, &slidingPlane, &distToPlane);

            // found a collision? (It's the case if the next position is beyond the wall plane)
            if (distToPlane <= 0.0f)
                // correct the next position (just the x position, thus the player will slide along the wall)
                pNewPos->m_X = pos.m_X;
        }

        // item contains a wall on the back?
        if (pLevel[i].m_Back)
        {
            // calculate the wall position
            pos.m_X = pLevel[i].m_X +  levelItemWidth;
            pos.m_Y = 0.0f;
            pos.m_Z = pLevel[i].m_Y + (levelItemHeight * 0.5f) + pPlayer->m_Radius;

            // get the wall normal
            n.m_X = 0.0f;
            n.m_Y = 0.0f;
            n.m_Z = 1.0f;

            // get the wall plane and calculate the distance between the new pos and the wall
            miniPlaneFromPointNormal(&pos, &n, &slidingPlane);
            miniDistanceToPlane(pNewPos, &slidingPlane, &distToPlane);

            // found a collision? (It's the case if the next position is beyond the wall plane)
            if (distToPlane <= 0.0f)
                // correct the next position (just the z position, thus the player will slide along the wall)
                pNewPos->m_Z = pos.m_Z;
        }

        // item contains a wall on the front?
        if (pLevel[i].m_Front)
        {
            // calculate the wall position
            pos.m_X = pLevel[i].m_X + levelItemWidth;
            pos.m_Y = 0.0f;
            pos.m_Z = pLevel[i].m_Y + levelItemHeight + (levelItemHeight * 0.5f) - pPlayer->m_Radius;

            // get the wall normal
            n.m_X =  0.0f;
            n.m_Y =  0.0f;
            n.m_Z = -1.0f;

            // get the wall plane and calculate the distance between the new pos and the wall
            miniPlaneFromPointNormal(&pos, &n, &slidingPlane);
            miniDistanceToPlane(pNewPos, &slidingPlane, &distToPlane);

            // found a collision? (It's the case if the next position is beyond the wall plane)
            if (distToPlane <= 0.0f)
                // correct the next position (just the z position, thus the player will slide along the wall)
                pNewPos->m_Z = pos.m_Z;
        }

        // is next position valid?
        if (!miniIsNextPosValid(pLevel, levelItemWidth, levelItemHeight, levelItemCount, pNewPos))
        {
            // limit the player X pos inside the rect bounds
            if (pPlayer->m_Pos.m_X < rect.m_Pos.m_X + pPlayer->m_Radius)
                pNewPos->m_X = rect.m_Pos.m_X + pPlayer->m_Radius;
            else
            if (pPlayer->m_Pos.m_X > (rect.m_Pos.m_X + rect.m_Size.m_Width) - pPlayer->m_Radius)
                pNewPos->m_X = (rect.m_Pos.m_X + rect.m_Size.m_Width) - pPlayer->m_Radius;
            else
                pNewPos->m_X = pPlayer->m_Pos.m_X;

            // limit the player Z pos inside the rect bounds
            if (pPlayer->m_Pos.m_Z < rect.m_Pos.m_Y + pPlayer->m_Radius)
                pNewPos->m_Z = rect.m_Pos.m_Y + pPlayer->m_Radius;
            else
            if (pPlayer->m_Pos.m_Z > (rect.m_Pos.m_Y + rect.m_Size.m_Height) - pPlayer->m_Radius)
                pNewPos->m_Z = (rect.m_Pos.m_Y + rect.m_Size.m_Height) - pPlayer->m_Radius;
            else
                pNewPos->m_Z = pPlayer->m_Pos.m_Z;
        }

        // all collisions are resolved
        return;
    }

    // next position is outside the map, reset it
    *pNewPos = pPlayer->m_Pos;
}
//------------------------------------------------------------------------------
void miniDrawLevelItem(const MINI_Vector3*       pTranslate,
                             float               angle,
                       const MINI_Vector3*       pRotateAxis,
                       const MINI_LevelDrawInfo* pDrawInfo)
{
    MINI_Matrix translateMatrix;
    MINI_Matrix rotateMatrix;
    MINI_Matrix modelViewMatrix;
    GLint       modelviewUniform;

    // build the model matrix
    miniGetTranslateMatrix(pTranslate, &translateMatrix);
    miniGetRotateMatrix(&angle, pRotateAxis, &rotateMatrix);
    miniMatrixMultiply(&rotateMatrix, &translateMatrix, &modelViewMatrix);

    // connect model view matrix to shader
    modelviewUniform = glGetUniformLocation(pDrawInfo->m_ShaderProgram, "mini_uModelview");
    glUniformMatrix4fv(modelviewUniform, 1, 0, &modelViewMatrix.m_Table[0][0]);

    // draw the item
    miniDrawSurface(pDrawInfo->m_pSurfaceVB,
                    pDrawInfo->m_SurfaceVertexCount,
                    pDrawInfo->m_pVertexFormat,
                    pDrawInfo->m_pShader);
}
//------------------------------------------------------------------------------
void miniDrawLevel(const MINI_LevelItem*     pLevel,
                         float               levelItemWidth,
                         float               levelItemHeight,
                         float               levelItemLength,
                         int                 levelItemCount,
                   const MINI_Sphere*        pPlayer,
                         float               playerAngle,
                   const MINI_LevelDrawInfo* pDrawInfo)
{
    unsigned     i;
    float        angle;
    MINI_Vector3 t;
    MINI_Vector3 r;
    MINI_Matrix  viewMatrix;
    MINI_Matrix  modelViewMatrix;
    GLint        viewUniform;
    GLint        modelViewUniform;

    // is map mode enabled? (it's a spacial mode for debugging purposes)
    if (pDrawInfo->m_MapMode)
    {
        r.m_X = 1.0f;
        r.m_Y = 0.0f;
        r.m_Z = 0.0f;

        angle = M_PI * 0.5f;

        miniGetRotateMatrix(&angle, &r, &viewMatrix);
    }
    else
    {
        r.m_X = 0.0f;
        r.m_Y = 1.0f;
        r.m_Z = 0.0f;

        miniGetRotateMatrix(&playerAngle, &r, &viewMatrix);
    }

    // connect model view matrix to shader
    viewUniform = glGetUniformLocation(pDrawInfo->m_ShaderProgram, "mini_uView");
    glUniformMatrix4fv(viewUniform, 1, 0, &viewMatrix.m_Table[0][0]);

    // iterate through the level items to draw
    for (i = 0; i < levelItemCount; ++i)
    {
        // bind soil texture
        glBindTexture(GL_TEXTURE_2D, pDrawInfo->m_SoilTextureIndex);

        // do draw the down item?
        if (pLevel[i].m_Down)
        {
            // calculate the translation to apply
            if (pDrawInfo->m_MapMode)
            {
                t.m_X =  pLevel[i].m_X + levelItemWidth;
                t.m_Y = -1.0f;
                t.m_Z =  pLevel[i].m_Y + levelItemHeight;
            }
            else
            {
                t.m_X =  pLevel[i].m_X   + levelItemWidth  - pPlayer->m_Pos.m_X;
                t.m_Y = -levelItemLength * 0.5f;
                t.m_Z =  pLevel[i].m_Y   + levelItemHeight - pPlayer->m_Pos.m_Z;
            }

            // set the rotation angle
            r.m_X = 1.0f;
            r.m_Y = 0.0f;
            r.m_Z = 0.0f;

            // calculate the rotation angle
            angle = M_PI * 0.5f;

            // draw the item
            miniDrawLevelItem(&t, angle, &r, pDrawInfo);
        }

        // in map mode, other surfaces aren't required
        if (pDrawInfo->m_MapMode)
            continue;

        // bind ceil texture
        glBindTexture(GL_TEXTURE_2D, pDrawInfo->m_CeilTextureIndex);

        // do draw the up item?
        if (pLevel[i].m_Up)
        {
            // calculate the translation to apply
            t.m_X = pLevel[i].m_X   + levelItemWidth  - pPlayer->m_Pos.m_X;
            t.m_Y = levelItemLength * 0.5f;
            t.m_Z = pLevel[i].m_Y   + levelItemHeight - pPlayer->m_Pos.m_Z;

            // set the rotation angle
            r.m_X = 1.0f;
            r.m_Y = 0.0f;
            r.m_Z = 0.0f;

            // calculate the rotation angle
            angle = -M_PI * 0.5f;

            // draw the item
            miniDrawLevelItem(&t, angle, &r, pDrawInfo);
        }

        // bind soil texture
        glBindTexture(GL_TEXTURE_2D, pDrawInfo->m_WallTextureIndex);

        // do draw the left item?
        if (pLevel[i].m_Left)
        {
            // calculate the translation to apply
            t.m_X = pLevel[i].m_X + (levelItemWidth * 0.5f) - pPlayer->m_Pos.m_X;
            t.m_Y = 0.0f;
            t.m_Z = pLevel[i].m_Y +  levelItemHeight        - pPlayer->m_Pos.m_Z;

            // set the rotation angle
            r.m_X = 0.0f;
            r.m_Y = 1.0f;
            r.m_Z = 0.0f;

            // calculate the rotation angle
            angle = -M_PI * 0.5f;

            // draw the item
            miniDrawLevelItem(&t, angle, &r, pDrawInfo);
        }

        // do draw the right item?
        if (pLevel[i].m_Right)
        {
            // calculate the translation to apply
            t.m_X = pLevel[i].m_X + levelItemWidth + (levelItemWidth * 0.5f) - pPlayer->m_Pos.m_X;
            t.m_Y =  0.0f;
            t.m_Z = pLevel[i].m_Y + levelItemHeight                          - pPlayer->m_Pos.m_Z;

            // set the rotation angle
            r.m_X = 0.0f;
            r.m_Y = 1.0f;
            r.m_Z = 0.0f;

            // calculate the rotation angle
            angle = M_PI * 0.5f;

            // draw the item
            miniDrawLevelItem(&t, angle, &r, pDrawInfo);
        }

        // do draw the back item?
        if (pLevel[i].m_Back)
        {
            // calculate the translation to apply
            t.m_X = pLevel[i].m_X +  levelItemWidth          - pPlayer->m_Pos.m_X;
            t.m_Y = 0.0f;
            t.m_Z = pLevel[i].m_Y + (levelItemHeight * 0.5f) - pPlayer->m_Pos.m_Z;

            // set the rotation angle
            r.m_X = 0.0f;
            r.m_Y = 1.0f;
            r.m_Z = 0.0f;

            // calculate the rotation angle
            angle = M_PI;

            // draw the item
            miniDrawLevelItem(&t, angle, &r, pDrawInfo);
        }

        // do draw the front item?
        if (pLevel[i].m_Front)
        {
            // calculate the translation to apply
            t.m_X = pLevel[i].m_X + levelItemWidth                             - pPlayer->m_Pos.m_X;
            t.m_Y = 0.0f;
            t.m_Z = pLevel[i].m_Y + levelItemHeight + (levelItemHeight * 0.5f) - pPlayer->m_Pos.m_Z;

            // set the rotation angle
            r.m_X = 0.0f;
            r.m_Y = 1.0f;
            r.m_Z = 0.0f;

            // calculate the rotation angle
            angle = 0.0f;

            // draw the item
            miniDrawLevelItem(&t, angle, &r, pDrawInfo);
        }
    }

    // in map mode, the player sphere should also be drawn
    if (pDrawInfo->m_MapMode)
    {
        // calculate the translation to apply
        t.m_X = pPlayer->m_Pos.m_X;
        t.m_Y = -1.0f;
        t.m_Z = pPlayer->m_Pos.m_Z;

        miniGetTranslateMatrix(&t, &modelViewMatrix);

        // connect model view matrix to shader
        modelViewUniform = glGetUniformLocation(pDrawInfo->m_ShaderProgram, "mini_uModelview");
        glUniformMatrix4fv(modelViewUniform, 1, 0, &modelViewMatrix.m_Table[0][0]);

        // bind soil texture
        glBindTexture(GL_TEXTURE_2D, pDrawInfo->m_SphereTextureIndex);

        miniDrawSphere(pDrawInfo->m_pSphereVB,
                       pDrawInfo->m_SphereVertexCount,
                       pDrawInfo->m_pSphereIndexes,
                       pDrawInfo->m_SphereIndexCount,
                       pDrawInfo->m_pSphereVertexFormat,
                       pDrawInfo->m_pSphereShader);
    }
}
//-----------------------------------------------------------------------------
#if defined(_OS_IOS_) || defined(_OS_ANDROID_) || defined(_OS_WINDOWS_)
    void miniLogLevel(const MINI_LevelItem* pLevel,
                            unsigned int    mapWidth,
                            unsigned int    mapHeight)
    {
        unsigned int x;
        unsigned int y;
        unsigned int offset;

        offset = 0;

        for (y = 0; y < mapHeight; ++y)
        {
            for (x = 0; x < mapWidth; ++x)
            {
                if (pLevel[offset].m_Back  &&
                    pLevel[offset].m_Front &&
                    pLevel[offset].m_Left  &&
                    pLevel[offset].m_Right)
                    printf("#");
                else
                if (pLevel[offset].m_Back && pLevel[offset].m_Front && pLevel[offset].m_Left)
                    printf("[");
                else
                if (pLevel[offset].m_Back && pLevel[offset].m_Front && pLevel[offset].m_Right)
                    printf("]");
                else
                if (pLevel[offset].m_Back && pLevel[offset].m_Left && pLevel[offset].m_Right)
                    printf("n");
                else
                if (pLevel[offset].m_Front && pLevel[offset].m_Left && pLevel[offset].m_Right)
                    printf("u");
                else
                if (pLevel[offset].m_Back && pLevel[offset].m_Front)
                    printf("=");
                else
                if (pLevel[offset].m_Left && pLevel[offset].m_Right)
                    printf("¦");
                else
                if (pLevel[offset].m_Left && pLevel[offset].m_Front)
                    printf("L");
                else
                if (pLevel[offset].m_Left && pLevel[offset].m_Back)
                    printf("f");
                else
                if (pLevel[offset].m_Right && pLevel[offset].m_Front)
                    printf("J");
                else
                if (pLevel[offset].m_Right && pLevel[offset].m_Back)
                    printf("1");
                else
                if (pLevel[offset].m_Back)
                    printf("-");
                else
                if (pLevel[offset].m_Front)
                    printf("_");
                else
                if (pLevel[offset].m_Left)
                    printf("|");
                else
                if (pLevel[offset].m_Right)
                    printf("|");
                else
                    printf(" ");

                ++offset;
            }

            printf("\n");
        }
    }
#endif
//-----------------------------------------------------------------------------
