/*****************************************************************************
 * ==> MiniCollisions -------------------------------------------------------*
 *****************************************************************************
 * Description : This module provides the functions required to detect the   *
 *               collisions inside a 2d or 3d world.                         *
 * Developer   : Jean-Milost Reymond                                         *
 * Copyright   : 2015 - 2018, this file is part of the Minimal API. You are  *
 *               free to copy or redistribute this file, modify it, or use   *
 *               it for your own projects, commercial or not. This file is   *
 *               provided "as is", without ANY WARRANTY OF ANY KIND          *
 *****************************************************************************/

#ifndef MiniCollisionH
#define MiniCollisionH

// mini API
#include "MiniCommon.h"
#include "MiniGeometry.h"

//----------------------------------------------------------------------------
// Global defines
//----------------------------------------------------------------------------

#define M_MINI_Epsilon 1.0E-3 // epsilon value used for tolerance

//----------------------------------------------------------------------------
// Structures
//----------------------------------------------------------------------------

/**
* Polygon
*/
typedef struct
{
    MINI_Vector3 m_v[3];
} MINI_Polygon;

// Aligned-axis bounding box tree node prototype
typedef struct MINI_AABBNode MINI_AABBNode;

/**
* Aligned-axis bounding box tree node
*/
struct MINI_AABBNode
{
    MINI_AABBNode* m_pParent;
    MINI_AABBNode* m_pLeft;
    MINI_AABBNode* m_pRight;
    MINI_Box*      m_pBox;
    MINI_Polygon*  m_pPolygons;
    unsigned       m_PolygonsCount;
};

#ifdef __cplusplus
    extern "C"
    {
#endif

        //----------------------------------------------------------------------------
        // Box calculation
        //----------------------------------------------------------------------------

        /**
        * Adds a polygon inside an existing bounding box
        *@param pPolygon - polygon to add
        *@param pBox - bounding box in which polygon should be added
        *@param[in, out] pEmpty - if 1, box is empty and still no contains any polygon
        */
        void miniAddPolygonToBoundingBox(const MINI_Polygon* pPolygon,
                                               MINI_Box*     pBox,
                                               int*          pEmpty);

        //----------------------------------------------------------------------------
        // Collision detection
        //----------------------------------------------------------------------------

        /**
        * Checks if a point is inside a polygon
        *@param pP - point to check
        *@param pV1 - first polygon vector
        *@param pV2 - second polygon vector
        *@param pV3 - third polygon vector
        *@return 1 if point is inside the polygon, otherwise 0
        */
        int miniPointInPolygon(const MINI_Vector3* pP,
                               const MINI_Vector3* pV1,
                               const MINI_Vector3* pV2,
                               const MINI_Vector3* pV3);

        /**
        * Checks if a point is inside a rect
        *@param pPoint - point to check
        *@param pRect - rect to check against
        *@return 1 if point is inside the rect, otherwise 0
        */
        int miniPointInRect(const MINI_Point* pPoint, const MINI_Rect* pRect);

        /**
        * Checks if a point is inside a sphere
        *@param pPoint - point to check
        *@param pSphere - sphere to check against
        *@return 1 if point is inside the sphere, otherwise 0
        */
        int miniPointInSphere(const MINI_Vector3* pPoint, const MINI_Sphere* pSphere);

        /**
        * Checks if a point is inside a box
        *@param pPoint - point to check
        *@param pBox - box to check against
        *@return 1 if point is inside the box, otherwise 0
        */
        int miniPointInBox(const MINI_Vector3* pPoint, const MINI_Box* pBox);

        /**
        * Checks if 2 line segments intersect
        *@param pL1Start - line 1 start
        *@param pL1End - line 1 end
        *@param pL2Start - line 2 start
        *@param pL2End - line 2 end
        *@param[out] pR - on intersection, the point where the lines intersect
        *@return 1 if lines intersect, otherwise 0
        */
        int miniLines2DIntersect(const MINI_Vector2* pL1Start,
                                 const MINI_Vector2* pL1End,
                                 const MINI_Vector2* pL2Start,
                                 const MINI_Vector2* pL2End,
                                       MINI_Vector2* pR);

        /**
        * Checks if a rectangle intersects another rectangle
        *@param pFirstRect - first rectangle to check
        *@param pSecondRect - second rectangle to check against
        *@return 1 if rects intersect, otherwise 0
        */
        int miniRectsIntersect(const MINI_Rect* pFirstRect, const MINI_Rect* pSecondRect);

        /**
        * Checks if a sphere intersects another sphere
        *@param pFirstSphere - first sphere to check
        *@param pSecondSphere - second sphere to check against
        *@return 1 if spheres intersect, otherwise 0
        */
        int miniSpheresIntersect(const MINI_Sphere* pFirstSphere, const MINI_Sphere* pSecondSphere);

        /**
        * Checks if a circle intersects a rectangle
        *@param pCircle - circle to check
        *@param pRect - rectangle to check against
        *@return 1 if shapes intersect, otherwise 0
        */
        int miniCircleRectIntersect(const MINI_Circle* pCircle, const MINI_Rect* pRect);

        /**
        * Checks if a plane intesects a ray
        *@param pPl - plane
        *@param pPos -ray position
        *@param pDir - ray direction
        *@param[out] pR - resulting point where plane intersects ray
        *@return 1 if plane intersects ray, otherwise 0
        */
        int miniRayPlaneIntersect(const MINI_Plane*   pPl,
                                  const MINI_Vector3* pPos,
                                  const MINI_Vector3* pDir,
                                        MINI_Vector3* pR);

        /**
        * Checks if a line intesects a plane
        *@param pPl - plane
        *@param pP1 - line first point
        *@param pP2 - line second point
        *@param[out] pR - resulting point where plane intersects line
        *@return 1 if plane intersects line, otherwise 0
        */
        int miniLinePlaneIntersect(const MINI_Plane*   pPl,
                                   const MINI_Vector3* pP1,
                                   const MINI_Vector3* pP2,
                                         MINI_Vector3* pR);

        /**
        * Checks if a polygon intersects another polygon
        *@param pP1V1 - first polygon first vertex
        *@param pP1V2 - first polygon second vertex
        *@param pP1V3 - first polygon third vertex
        *@param pP2V1 - second polygon first vertex
        *@param pP2V2 - second polygon second vertex
        *@param pP2V3 - second polygon third vertex
        *@return 1 if polygons intersect, otherwise 0
        */
        int miniPolygonsIntersect(MINI_Vector3* pP1V1, MINI_Vector3* pP1V2, MINI_Vector3* pP1V3,
                                  MINI_Vector3* pP2V1, MINI_Vector3* pP2V2, MINI_Vector3* pP2V3);

        /**
        * Checks if a ray intersects a polygon
        *@param pRay - ray
        *@param pPolygon - polygon
        *@return 1 if ray intersects polygon, otherwise 0
        */
        int miniRayPolygonIntersect(const MINI_Ray* pRay, const MINI_Polygon* pPolygon);

        /**
        * Checks if a line intersects a polygon
        *@param pP1 - line first point
        *@param pP2 - line second point
        *@param pPolygon - polygon
        *@return 1 if ray intersects polygon, otherwise 0
        */
        int miniLinePolygonIntersect(const MINI_Vector3* pP1,
                                     const MINI_Vector3* pP2,
                                     const MINI_Polygon* pPolygon);

        /**
        * Checks if a ray intersects a box
        *@param pRay - ray
        *@param pBox - box
        *@return 1 if ray intersects box, otherwise 0
        */
        int miniRayBoxIntersect(const MINI_Ray* pRay, const MINI_Box* pBox);

        /**
        * Checks if a sphere intersects a polygon, and gets the resulting sliding plane
        *@param pSphere - sphere to check
        *@param pPolygon - polygon to check
        *@param pSlidingPlane - resulting sliding plane
        *@return 1 if ray intersects polygon, otherwise 0
        */
        int miniSpherePolygonIntersectAndSlide(const MINI_Sphere*  pSphere,
                                               const MINI_Polygon* pPolygon,
                                                     MINI_Plane*   pSlidingPlane);

        //----------------------------------------------------------------------------
        // Collision reaction
        //----------------------------------------------------------------------------

        /**
        * Gets the next position while sliding on the plane
        *@param pSlidingPlane - sliding plane
        *@param pPosition - current position
        *@param pRadius - radius around the current position
        *@param pR - resulting position
        */
        void miniGetSlidingPoint(const MINI_Plane*   pSlidingPlane,
                                 const MINI_Vector3* pPosition,
                                 const float*        pRadius,
                                       MINI_Vector3* pR);

        //----------------------------------------------------------------------------
        // Polygons extraction
        //----------------------------------------------------------------------------

        /**
        * Adds polygon to array
        *@param pVB - source vertex buffer
        *@param v1 - first polygon vertex index in vertex buffer
        *@param v2 - second polygon vertex index in vertex buffer
        *@param v3 - third polygon vertex index in vertex buffer
        *@param[out] pPolygons - polygon array that contains generated polygons
        *@param[out] pPolygonsCount - polygons count contained in array
        *@note Generated polygons should be deleted by calling miniReleasePolygons()
        *      when useless
        */
        void miniAddPolygon(const float*         pVB,
                                  unsigned       v1,
                                  unsigned       v2,
                                  unsigned       v3,
                                  MINI_Polygon** pPolygons,
                                  unsigned*      pPolygonsCount);

        /**
        * Gets polygons from vertex buffer
        *@param pVB - source vertex buffer
        *@param length - vertex buffer length
        *@param type - polygons type as arranged in vertex buffer, where:
        *              0 = triangles
        *              1 = triangle strip
        *              2 = triangle fan
        *              3 = quads
        *              4 = quad strip
        *@param[out] pPolygons - polygon array that contains generated polygons
        *@param[out] pPolygonsCount - polygons count contained in array
        *@return 1 on success, otherwise 0
        *@note Generated polygons should be deleted by calling miniReleasePolygons()
        *      when useless
        */
        int miniGetPolygonsFromVB(const float*         pVB,
                                        unsigned       length,
                                        unsigned       type,
                                        unsigned       stride,
                                        MINI_Polygon** pPolygons,
                                        unsigned*      pPolygonsCount);

        /**
        * Releases polygons previously created by GetPolygonsFromVB() or AddPolygon()
        *@param pPolygons - polygon array to release
        */
        void miniReleasePolygons(MINI_Polygon* pPolygons);

        //----------------------------------------------------------------------------
        // Aligned-Axis Bounding Box tree
        //----------------------------------------------------------------------------

        /**
        * Cuts box on the longest axis
        *@param pBox - box to cut
        *@param[out] pLeftBox - resulting left box
        *@param[out] pRightBox - resulting right box
        */
        void miniCutBox(const MINI_Box* pBox, MINI_Box* pLeftBox, MINI_Box* pRightBox);

        /**
        * Populates AABB tree
        *@param pNode - root or parent node to create from
        *@param pPolygons - source polygon array
        *@param polygonsCount - polygon array count
        *@return 1 on success, otherwise 0
        */
        int miniPopulateTree(      MINI_AABBNode* pNode,
                             const MINI_Polygon*  pPolygons,
                                   unsigned       polygonsCount);

        /**
        * Resolves AABB tree
        *@param pRay - ray against which tree boxes will be tested
        *@param pNode - root or parent node to resolve
        *@param[out] pPolygons - polygons belonging to boxes hit by ray
        *@param[out] polygonsCount - polygon array count
        *@return 1 on success, otherwise 0
        */
        int miniResolveTree(MINI_Ray*      pRay,
                            MINI_AABBNode* pNode,
                            MINI_Polygon** pPolygons,
                            unsigned*      pPolygonsCount);

        /**
        * Releases node content
        *@param pNode - node for which content should be released
        *@note Only the node content is released, the node itself is not released
        */
        void miniReleaseNode(MINI_AABBNode* pNode);

        /**
        * Releases tree content
        *@param pNode - root node from which content should be released
        */
        void miniReleaseTree(MINI_AABBNode* pNode);

#ifdef __cplusplus
    }
#endif

//----------------------------------------------------------------------------
// Compiler
//----------------------------------------------------------------------------

// needed in mobile c compiler to link the .h file with the .c
#if defined(_OS_IOS_) || defined(_OS_ANDROID_) || defined(_OS_WINDOWS_)
    #include "MiniCollision.c"
#endif

#endif
