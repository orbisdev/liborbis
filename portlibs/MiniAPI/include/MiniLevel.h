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

#ifndef MiniLevelH
#define MiniLevelH

// std
#include <stdlib.h>

// mini API
#include "MiniCommon.h"
#include "MiniModels.h"
#include "MiniShader.h"

//-----------------------------------------------------------------------------
// Structures
//-----------------------------------------------------------------------------

/**
* Level item
* The item is organized as follow:
*
*            top
*         ---------
*        /|      /|
*       / |  back |
*       --------  |
* left |  |----|--| right
*      | front | /
*      |/      |/
*       -------
*       bottom
*
*/
typedef struct
{
    int   m_Left;
    int   m_Right;
    int   m_Front;
    int   m_Back;
    int   m_Up;
    int   m_Down;
    float m_X;
    float m_Y;
    float m_Width;
    float m_Height;
} MINI_LevelItem;

/**
* Level draw info
*/
typedef struct
{
    float*             m_pSurfaceVB;
    float*             m_pSphereVB;
    float              m_SphereRadius;
    MINI_Index*        m_pSphereIndexes;
    unsigned int       m_SurfaceVertexCount;
    unsigned int       m_SphereVertexCount;
    unsigned int       m_SphereIndexCount;
    MINI_VertexFormat* m_pVertexFormat;
    MINI_VertexFormat* m_pSphereVertexFormat;
    MINI_Shader*       m_pShader;
    MINI_Shader*       m_pSphereShader;
    GLuint             m_ShaderProgram;
    GLuint             m_SoilTextureIndex;
    GLuint             m_WallTextureIndex;
    GLuint             m_CeilTextureIndex;
    GLuint             m_SphereTextureIndex;
    int                m_MapMode;
} MINI_LevelDrawInfo;

#ifdef __cplusplus
    extern "C"
    {
#endif

        //-----------------------------------------------------------------------------
        // Level edition functions
        //-----------------------------------------------------------------------------

        /**
        * Generates level from a map
        *@param pMap - map containing the level to generate
        *@param mapWidth - map width
        *@param mapHeight - map height
        *@param pItemWidth - item width
        *@param pItemHeight - item height
        *@param[out] pLevel - generated level
        *@return 1 on success, otherwise 0
        */
        int miniGenerateLevel(const char*              pMap,
                                    unsigned int       mapWidth,
                                    unsigned int       mapHeight,
                              const float*             pItemWidth,
                              const float*             pItemHeight,
                                    MINI_LevelItem**   pLevel);

        //-----------------------------------------------------------------------------
        // Level validation functions
        //-----------------------------------------------------------------------------

        /**
        * Checks if a body in movement intersects a wall
        *@param pBodyStart - body movement start position
        *@param pBodyEnd - body movement end position
        *@param pLevel - level
        *@param levelItemWidth - level item width
        *@param levelItemHeight - level item height
        *@param levelItemCount - level item count
        */
        int miniBodyIntersectWall(const MINI_Vector2*   pBodyStart,
                                  const MINI_Vector2*   pBodyEnd,
                                  const MINI_LevelItem* pLevel,
                                        float           levelItemWidth,
                                        float           levelItemHeight,
                                        int             levelItemCount);

        /**
        * Checks if the next position is valid
        *@param pLevel - level
        *@param levelItemWidth - level item width
        *@param levelItemHeight - level item height
        *@param levelItemCount - level item count
        *@param pNewPos - new position to check
        *@return 1 if new position is valid, otherwise 0
        */
        int miniIsNextPosValid(const MINI_LevelItem* pLevel,
                                     float           levelItemWidth,
                                     float           levelItemHeight,
                                     int             levelItemCount,
                               const MINI_Vector3*   pNewPos);

        /**
        * Validates the next position
        *@param pLevel - level
        *@param levelItemWidth - level item width
        *@param levelItemHeight - level item height
        *@param levelItemCount - level item count
        *@param pPlayer - sphere describing the player
        *@param[in, out] pNewPos - proposed new position, corrected position on function ends
        */
        void miniValidateNextPos(const MINI_LevelItem* pLevel,
                                       float           levelItemWidth,
                                       float           levelItemHeight,
                                       int             levelItemCount,
                                 const MINI_Sphere*    pPlayer,
                                       MINI_Vector3*   pNewPos);

        //-----------------------------------------------------------------------------
        // Level drawing functions
        //-----------------------------------------------------------------------------

        /**
        * Draw the level item
        *@param pTranslate - translation to apply to item
        *@param angle - angle to apply to item
        *@param pRotateAxis - angle rotation axis
        *@param pDrawInfo - draw info
        */
        void miniDrawLevelItem(const MINI_Vector3*       pTranslate,
                                     float               angle,
                               const MINI_Vector3*       pRotateAxis,
                               const MINI_LevelDrawInfo* pDrawInfo);

        /**
        * Draw the level
        *@param pLevel - level
        *@param levelItemWidth - level item width (x axis)
        *@param levelItemHeight - level item height (z axis)
        *@param levelItemHeight - level item length (y axis)
        *@param levelItemCount - level item count
        *@param pPlayer - sphere describing the player
        *@param playerAngle - angle representing the player direction on the y axis
        *@param pDrawInfo - draw info
        */
        void miniDrawLevel(const MINI_LevelItem*     pLevel,
                                 float               levelItemWidth,
                                 float               levelItemHeight,
                                 float               levelItemLength,
                                 int                 levelItemCount,
                           const MINI_Sphere*        pPlayer,
                                 float               playerAngle,
                           const MINI_LevelDrawInfo* pDrawInfo);

        //-----------------------------------------------------------------------------
        // Level debugging functions
        //-----------------------------------------------------------------------------

        /**
        * Logs the level
        *@param pLevel - level to log
        *@param mapWidth - map width
        *@param mapHeight - map height
        */
        #if defined(_OS_IOS_) || defined(_OS_ANDROID_) || defined(_OS_WINDOWS_)
            void miniLogLevel(const MINI_LevelItem* pLevel,
                                    unsigned int    mapWidth,
                                    unsigned int    mapHeight);
        #endif

#ifdef __cplusplus
    }
#endif

//----------------------------------------------------------------------------
// Compiler
//----------------------------------------------------------------------------

// needed in mobile c compiler to link the .h file with the .c
#if defined(_OS_IOS_) || defined(_OS_ANDROID_) || defined(_OS_WINDOWS_)
    #include "MiniLevel.c"
#endif

#endif
