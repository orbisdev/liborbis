/*****************************************************************************
 * ==> MiniGeometry ---------------------------------------------------------*
 *****************************************************************************
 * Description : This module provides the funcions required to calculate the *
 *               geometry in an euclidean space                              *
 * Developer   : Jean-Milost Reymond                                         *
 * Copyright   : 2015 - 2018, this file is part of the Minimal API. You are  *
 *               free to copy or redistribute this file, modify it, or use   *
 *               it for your own projects, commercial or not. This file is   *
 *               provided "as is", without ANY WARRANTY OF ANY KIND          *
 *****************************************************************************/

#ifndef MiniGeometryH
#define MiniGeometryH

// mini api
#include "MiniCommon.h"

// add -I/usr/lib64/clang/4.0.1/include -mavx
//#include <immintrin.h>
// will include avxintrin.h for AVX support

//----------------------------------------------------------------------------
// Structures
//----------------------------------------------------------------------------

/**
* Euclidean 2D vector
*/
typedef struct
{
    float m_X;
    float m_Y;
} MINI_Vector2 __attribute__ ((aligned (16)));

/**
* Euclidean 3D vector
*/
typedef struct
{
    float m_X; // x coordinate for the 3D vector
    float m_Y; // y coordinate for the 3D vector
    float m_Z; // z coordinate for the 3D vector
} MINI_Vector3 __attribute__ ((aligned (16)));

/**
* Euclidean plane
*/
typedef struct
{
    float m_A;
    float m_B;
    float m_C;
    float m_D;
} MINI_Plane __attribute__ ((aligned (16)));

/**
* Euclidean ray
*/
typedef struct
{
    MINI_Vector3 m_Pos;
    MINI_Vector3 m_Dir;
    MINI_Vector3 m_InvDir;
} MINI_Ray __attribute__ ((aligned (16)));

/**
* 4x4 matrix
*/
typedef struct
{
    float m_Table[4][4]; // 4x4 matrix array
} MINI_Matrix __attribute__ ((aligned (16)));

/**
* Quaternion
*/
typedef struct
{
    float m_X;
    float m_Y;
    float m_Z;
    float m_W;
} MINI_Quaternion __attribute__ ((aligned (16)));

/**
* 2D point
*/
typedef struct
{
    float m_X;
    float m_Y;
} MINI_Point __attribute__ ((aligned (16)));

/**
* 2D size
*/
typedef struct
{
    float m_Width;
    float m_Height;
} MINI_Size __attribute__ ((aligned (16)));

/**
* 2D circle
*/
typedef struct
{
    MINI_Point m_Pos;
    float      m_Radius;
} MINI_Circle __attribute__ ((aligned (16)));

/**
* 2D rectangle
*/
typedef struct
{
    MINI_Point m_Pos;  // rect left and top corner position
    MINI_Size  m_Size;
} MINI_Rect __attribute__ ((aligned (16)));

/**
* 3D sphere
*/
typedef struct
{
    MINI_Vector3 m_Pos;
    float        m_Radius;
} MINI_Sphere __attribute__ ((aligned (16)));

/**
* 3D aligned-axis box
*/
typedef struct
{
    MINI_Vector3 m_Min;
    MINI_Vector3 m_Max;
} MINI_Box __attribute__ ((aligned (16)));

#ifdef __cplusplus
    extern "C"
    {
#endif

        //----------------------------------------------------------------------------
        // Distances calculation
        //----------------------------------------------------------------------------

        /**
        * Checks if a point is between 2 vertices
        *@param pP - point to test
        *@param pS - start vertex
        *@param pE - End vertex
        *@param pTolerance - tolerance for calculation
        *@return 1 if value is between points, otherwise 0
        */
        int miniVectorIsBetween(const MINI_Vector3* pP,
                                const MINI_Vector3* pS,
                                const MINI_Vector3* pE,
                                const float*        pTolerance);

        /**
        * Gets the shortest distance between 2 lines
        *@param pL1S - first line start point
        *@param pL1E - first line end point
        *@param pL2S - second line start point
        *@param pL2E - second line end point
        *@param pTolerance - tolerance for calculation
        *@param[out] pR - resulting distance
        */
        void miniGetShortestDistance(const MINI_Vector3* pL1S,
                                     const MINI_Vector3* pL1E,
                                     const MINI_Vector3* pL2S,
                                     const MINI_Vector3* pL2E,
                                     const float*        pTolerance,
                                           float*        pR);

        /**
        * Calculates and gets the projection of a point on a segment
        *@param pSegStart - segment start
        *@param pSegEnd - segment end
        *@param pPoint - point for which projection must be calculated
        *@param pR - calculated point
        */
        void miniClosestPointOnLine(const MINI_Vector3* pSegStart,
                                    const MINI_Vector3* pSegEnd,
                                    const MINI_Vector3* pPoint,
                                          MINI_Vector3* pR);

        /**
        * Calculates and gets the projection of a point on a polygon
        *@param pPoint - point for which projection must be calculated
        *@param pV1 - polygon first vertex
        *@param pV2 - polygon second vertex
        *@param pV3 - polygon third vertex
        *@param pR - the calculated point
        */
        void miniClosestPointOnPolygon(const MINI_Vector3* pPoint,
                                       const MINI_Vector3* pV1,
                                       const MINI_Vector3* pV2,
                                       const MINI_Vector3* pV3,
                                             MINI_Vector3* pR);

        //----------------------------------------------------------------------------
        // Vector functions
        //----------------------------------------------------------------------------

        /**
        * Adds vector contents
        *@param pV1 - vector that contains values to be added
        *@param pV2 - vector to add to
        *@param[out] pR - resulting vector
        */
        void miniAdd(const MINI_Vector3* pV1, const MINI_Vector3* pV2, MINI_Vector3* pR);

        /**
        * Subtracts vector contents
        *@param pV1 - vector that contains values to be subtracted
        *@param pV2 - vector to subtract by
        *@param[out] pR - resulting vector
        */
        void miniSub(const MINI_Vector3* pV1, const MINI_Vector3* pV2, MINI_Vector3* pR);

        /**
        * Calculates vector length
        *@param pV - vector to calculate length
        *@param[out] pR - vector length
        */
        void miniLength(const MINI_Vector3* pV, float* pR);

        /**
        * Normalizes the vector
        *@param pV - vector to normalize
        *@param[out] pR - normailzed vector
        */
        void miniNormalize(const MINI_Vector3* pV, MINI_Vector3* pR);

        /**
        * Calculates cross product between 2 vectors
        *@param pV1 - first vector
        *@param pV2 - second vector
        *@param[out] pR - resulting vector
        */
        void miniCross(const MINI_Vector3* pV1, const MINI_Vector3* pV2, MINI_Vector3* pR);

        /**
        * Calculates dot product between 2 vectors
        *@param pV1 - first vector
        *@param pV2 - second vector
        *@param[out] pR - resulting angle
        */
        void miniDot(const MINI_Vector3* pV1, const MINI_Vector3* pV2, float* pR);

        //----------------------------------------------------------------------------
        // Plane functions
        //----------------------------------------------------------------------------

        /**
        * Creates a plane from a point and a normal
        *@param pP - point
        *@param pN - normal
        *@param[out] pR - resulting plane
        */
        void miniPlaneFromPointNormal(const MINI_Vector3* pP, const MINI_Vector3* pN, MINI_Plane* pR);

        /**
        * Creates a plane from 3 vertex
        *@param pV1 - first vector
        *@param pV2 - second vector
        *@param pV3 - third vector
        *@param[out] pR - resulting plane
        */
        void miniPlaneFromPoints(const MINI_Vector3* pV1,
                                 const MINI_Vector3* pV2,
                                 const MINI_Vector3* pV3,
                                       MINI_Plane*   pR);

        /**
        * Calculates a distance between a point and a plane
        *@param pP - point
        *@param pPl - plane
        *@param[out] - resulting distance
        */
        void miniDistanceToPlane(const MINI_Vector3* pP, const MINI_Plane* pPl, float* pR);

        //----------------------------------------------------------------------------
        // Matrix functions
        //----------------------------------------------------------------------------

        /**
        * Gets matrix identity
        *@param[out] pR - resulting identity matrix
        */
        void miniGetIdentity(MINI_Matrix* pR);

        /**
        * Gets orthogonal matrix
        *@param pLeft - left view rect edge
        *@param pRight - right view rect edge
        *@param pBottom - bottom view rect edge
        *@param pTop - top view rect edge
        *@param pZNear - z near clipping value
        *@param pZFar - z far clipping value
        *@param[out] pR - resulting orthogonal matrix
        */
        void miniGetOrtho(const float*       pLeft,
                          const float*       pRight,
                          const float*       pBottom,
                          const float*       pTop,
                          const float*       pZNear,
                          const float*       pZFar,
                                MINI_Matrix* pR);

        /**
        * Gets frustrum matrix
        *@param pLeft - left view rect edge
        *@param pRight - right view rect edge
        *@param pBottom - bottom view rect edge
        *@param pTop - top view rect edge
        *@param pZNear - z near clipping value
        *@param pZFar - z far clipping value
        *@param[out] pR - resulting frustrum matrix
        */
        void miniGetFrustum(const float*       pLeft,
                            const float*       pRight,
                            const float*       pBottom,
                            const float*       pTop,
                            const float*       pZNear,
                            const float*       pZFar,
                                  MINI_Matrix* pR);

        /**
        * Gets perspective matrix
        *@param pFovyDeg - field of view angle in radian
        *@param pAspect - aspect ratio
        *@param pZNear - z near clipping value
        *@param pZFar - z far clipping value
        *@param[out] pR - resulting perspective matrix
        */
        void miniGetPerspective(const float*       pFovyDeg,
                                const float*       pAspect,
                                const float*       pZNear,
                                const float*       pZFar,
                                      MINI_Matrix* pR);

        /**
        * Gets translation matrix
        *@param pT - translation vector
        *@param[out] pR - resulting translation matrix
        */
        void miniGetTranslateMatrix(const MINI_Vector3* pT, MINI_Matrix* pR);

        /**
        * Gets rotation matrix
        *@param pAngle - rotation angle, in radian
        *@param pAxis - rotation axis
        *@param[out] pR - resulting orthogonal matrix
        */
        void miniGetRotateMatrix(const float* pAngle, const MINI_Vector3* pAxis, MINI_Matrix* pR);

        /**
        * Gets scale matrix
        *@param pFactor - scale factor
        *@param[out] pR - resulting scale matrix
        */
        void miniGetScaleMatrix(const MINI_Vector3* pFactor, MINI_Matrix* pR);

        /**
        * Multiplies matrix by another matrix
        *@param pM1 - first matrix to multiply
        *@param pM2 - second matrix to multiply with
        *@param[out] pR - resulting orthogonal matrix
        */
        void miniMatrixMultiply(const MINI_Matrix* pM1, const MINI_Matrix* pM2, MINI_Matrix* pR);

        /**
        * Inverses a matrix
        *@param pM - matrix to inverse
        *@param[out] pR - inversed matrix
        *@param[out] pDeterminant - matrix determinant
        */
        void miniInverse(const MINI_Matrix* pM, MINI_Matrix* pR, float* pDeterminant);

        /**
        * Applies matrix to a vector
        *@param pM - matrix to apply
        *@param pV - vector on which matrix should be applied
        *@param[out] pR - resulting vector
        */
        void miniApplyMatrixToVector(const MINI_Matrix* pM, const MINI_Vector3* pV, MINI_Vector3* pR);

        /**
        * Applies matrix to a normal
        *@param pM - matrix to apply
        *@param pN - normal on which matrix should be applied
        *@param[out] pR - resulting normal
        */
        void miniApplyMatrixToNormal(const MINI_Matrix* pM, const MINI_Vector3* pN, MINI_Vector3* pR);

        /**
        * Unprojects a ray (i.e. transforms it in viewport coordinates)
        *@param pProj - projection matrix
        *@param pView - view matrix
        *@param[in, out] pRayPos - ray position, unprojected ray position on function ends
        *@param[in, out] pRayDir - ray direction, unprojected ray direction on function ends
        */
        void miniUnproject(const MINI_Matrix*  pProj,
                           const MINI_Matrix*  pView,
                                 MINI_Vector3* pRayPos,
                                 MINI_Vector3* pRayDir);

        //----------------------------------------------------------------------------
        // Quaternion functions
        //----------------------------------------------------------------------------

        /**
        * Gets a quaternion from an angle and a vector representing a rotation axis
        *@param pAngle - rotation angle
        *@param pAxis- rotation axis
        *@param[out] pR - resulting quaternion
        */
        void miniQuatFromAxis(float* pAngle, const MINI_Vector3* pAxis, MINI_Quaternion* pR);

        /**
        * Gets a quaternion from Euler angles
        *@param pAngleX - rotation angle on x axis
        *@param pAngleY - rotation angle on y axis
        *@param pAngleZ - rotation angle on z axis
        *@param[out] pR - resulting quaternion
        */
        void miniQuatFromEuler(float* pAngleX, float* pAngleY, float* pAngleZ, MINI_Quaternion* pR);

        /**
        * Gets the squared length of a quaternion
        *@param pQ - quaternion for which the norm should be get
        *@param[out] pR - the resulting norm
        */
        void miniQuatLengthSquared(const MINI_Quaternion* pQ, float* pR);

        /**
        * Gets the quaternion length
        *@param pQ - quaternion for which the length should be get
        *@param[out] pR - the resulting length
        */
        void miniQuatLength(const MINI_Quaternion* pQ, float* pR);

        /**
        * Normalizes the quaternion
        *@param pV - vector to normalize
        *@param[out] pR - normailzed vector
        */
        void miniQuatNormalize(const MINI_Quaternion* pQ, MINI_Quaternion* pR);

        /**
        * Calculates dot product between 2 quaternions
        *@param pQ1 - first quaternion
        *@param pQ2 - second quaternion
        *@param[out] pR - resulting angle
        */
        void miniQuatDot(const MINI_Quaternion* pQ1, const MINI_Quaternion* pQ2, float* pR);

        /**
        * Conjugates the quaternion
        *@param pQ - quaternion to conjugate
        *@param[out] pR - conjugated quaternion
        */
        void miniConjugate(const MINI_Quaternion* pQ, MINI_Quaternion* pR);

        /**
        * Gets a rotation matrix from a quaternion
        *@param pQ - quaternion from which the matrix should be get
        *@param[out] pR - rotation matrix
        */
        void miniGetMatrix(const MINI_Quaternion* pQ, MINI_Matrix* pR);

#ifdef __cplusplus
    }
#endif

//----------------------------------------------------------------------------
// Compiler
//----------------------------------------------------------------------------

// needed in mobile c compiler to link the .h file with the .c
#if defined(_OS_IOS_) || defined(_OS_ANDROID_) || defined(_OS_WINDOWS_)
    #include "MiniGeometry.c"
#endif

#endif // MiniGeometryH
