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

#include "MiniCollision.h"

// std
#include <stdlib.h>
#include <math.h>

//----------------------------------------------------------------------------
// Box calculation
//----------------------------------------------------------------------------
void miniAddPolygonToBoundingBox(const MINI_Polygon* pPolygon,
                                       MINI_Box*     pBox,
                                       int*          pEmpty)
{
    float    r;
    unsigned i;

    // iterate through polygon vertices
    for (i = 0; i < 3; ++i)
    {
        // is box empty?
        if (*pEmpty)
        {
            // initialize bounding box with first vertex
             pBox->m_Min = pPolygon->m_v[i];
             pBox->m_Max = pPolygon->m_v[i];
            *pEmpty      = 0;
            continue;
        }

        // search for box min edge
        miniMin(&pBox->m_Min.m_X, &pPolygon->m_v[i].m_X, &r);
        pBox->m_Min.m_X = r;
        miniMin(&pBox->m_Min.m_Y, &pPolygon->m_v[i].m_Y, &r);
        pBox->m_Min.m_Y = r;
        miniMin(&pBox->m_Min.m_Z, &pPolygon->m_v[i].m_Z, &r);
        pBox->m_Min.m_Z = r;

        // search for box max edge
        miniMax(&pBox->m_Max.m_X, &pPolygon->m_v[i].m_X, &r);
        pBox->m_Max.m_X = r;
        miniMax(&pBox->m_Max.m_Y, &pPolygon->m_v[i].m_Y, &r);
        pBox->m_Max.m_Y = r;
        miniMax(&pBox->m_Max.m_Z, &pPolygon->m_v[i].m_Z, &r);
        pBox->m_Max.m_Z = r;
    }
}
//----------------------------------------------------------------------------
// Collision detection
//----------------------------------------------------------------------------
int miniPointInPolygon(const MINI_Vector3* pP,
                       const MINI_Vector3* pV1,
                       const MINI_Vector3* pV2,
                       const MINI_Vector3* pV3)
{
    MINI_Vector3 nPToV1;
    MINI_Vector3 nPToV2;
    MINI_Vector3 nPToV3;
    float        a1;
    float        a2;
    float        a3;
    float        angleResult;

    /*
    * check if the point p is inside the polygon in the following manner:
    *
    *                  V1                         V1
    *                  /\                         /\
    *                 /  \                       /  \
    *                / *p \                  *P /    \
    *               /      \                   /      \
    *            V2 -------- V3             V2 -------- V3
    *
    * calculate the vectors between the point p and each polygon vertex, then
    * calculate the angle formed by each of these vectors. If the sum of the
    * angles is equal to a complete circle, i.e. 2 * pi in radians, then the
    * point p is inside the polygon limits, otherwise the point is outside. It
    * is assumed that the point to check belongs to the polygon's plane
    */
    miniSub(pV1, pP, &nPToV1);
    miniSub(pV2, pP, &nPToV2);
    miniSub(pV3, pP, &nPToV3);
    miniNormalize(&nPToV1, &nPToV1);
    miniNormalize(&nPToV2, &nPToV2);
    miniNormalize(&nPToV3, &nPToV3);

    // calculate the angles using the scalar product of each vectors. We use
    // the following algorithms:
    // A1 = NPToV1.x * NPToV2.x + NPToV1.y * NPToV2.y + NPToV1.z * NPToV2.z
    // A2 = NPToV2.x * NPToV3.x + NPToV2.y * NPToV3.y + NPToV2.z * NPToV3.z
    // A3 = NPToV3.x * NPToV1.x + NPToV3.y * NPToV1.y + NPToV3.z * NPToV1.z
    miniDot(&nPToV1, &nPToV2, &a1);
    miniDot(&nPToV2, &nPToV3, &a2);
    miniDot(&nPToV3, &nPToV1, &a3);

    // limit a1 to avoid rounding errors
    if (a1 > 1.0f)
        a1 = 1.0f;
    else
    if (a1 < -1.0f)
        a1 = -1.0f;

    // limit a2 to avoid rounding errors
    if (a2 > 1.0f)
        a2 = 1.0f;
    else
    if (a2 < -1.0f)
        a2 = -1.0f;

    // limit a3 to avoid rounding errors
    if (a3 > 1.0f)
        a3 = 1.0f;
    else
    if (a3 < -1.0f)
        a3 = -1.0f;

    // calculate the sum of all angles
    angleResult = acos(a1) + acos(a2) + acos(a3);

    // if sum is equal or higher to 6.28 radians then point P is inside polygon
    if (angleResult >= 6.28f)
        return 1;

    return 0;
}
//----------------------------------------------------------------------------
int miniPointInRect(const MINI_Point* pPoint, const MINI_Rect* pRect)
{
    return (pPoint->m_X >= pRect->m_Pos.m_X                         &&
            pPoint->m_Y >= pRect->m_Pos.m_Y                         &&
            pPoint->m_X <= pRect->m_Pos.m_X + pRect->m_Size.m_Width &&
            pPoint->m_Y <= pRect->m_Pos.m_Y + pRect->m_Size.m_Height);
}
//----------------------------------------------------------------------------
int miniPointInSphere(const MINI_Vector3* pPoint, const MINI_Sphere* pSphere)
{
    float        distance;
    MINI_Vector3 length;

    // calculate the distance between test point and the center of the sphere
    miniSub(pPoint, &pSphere->m_Pos, &length);
    miniLength(&length, &distance);

    // check if distance is shorter than the radius of the sphere and return
    // result
    return (distance <= pSphere->m_Radius);
}
//----------------------------------------------------------------------------
int miniPointInBox(const MINI_Vector3* pPoint, const MINI_Box* pBox)
{
    return (pPoint->m_X >= pBox->m_Min.m_X && pPoint->m_X <= pBox->m_Max.m_X &&
            pPoint->m_Y >= pBox->m_Min.m_Y && pPoint->m_Y <= pBox->m_Max.m_Y &&
            pPoint->m_Z >= pBox->m_Min.m_Z && pPoint->m_Z <= pBox->m_Max.m_Z);
}
//----------------------------------------------------------------------------
int miniLines2DIntersect(const MINI_Vector2* pL1Start,
                         const MINI_Vector2* pL1End,
                         const MINI_Vector2* pL2Start,
                         const MINI_Vector2* pL2End,
                               MINI_Vector2* pR)
{
    MINI_Vector2 l1;
    MINI_Vector2 l2;
    MINI_Vector2 l2l1;
    float        dot;
    float        t;
    float        u;

    pR->m_X = 0.0f;
    pR->m_Y = 0.0f;

    l1.m_X = pL1End->m_X - pL1Start->m_X;
    l1.m_Y = pL1End->m_Y - pL1Start->m_Y;

    l2.m_X = pL2End->m_X - pL2Start->m_X;
    l2.m_Y = pL2End->m_Y - pL2Start->m_Y;

    dot = l1.m_X * l2.m_Y - l1.m_Y * l2.m_X;

    // if dot product of l1 dot l2 is equals to 0, then the lines are parallel
    // and have infinite intersection points
    if (!dot)
        return 0;

    l2l1.m_X = pL2Start->m_X - pL1Start->m_X;
    l2l1.m_Y = pL2Start->m_Y - pL1Start->m_Y;

    t = (l2l1.m_X * l2.m_Y - l2l1.m_Y * l2.m_X) / dot;

    if (t < 0.0f || t > 1.0f)
        return 0;

    u = (l2l1.m_X * l1.m_Y - l2l1.m_Y * l1.m_X) / dot;

    if (u < 0.0f || u > 1.0f)
        return 0;

    pR->m_X = pL1Start->m_X + t * l1.m_X;
    pR->m_Y = pL1Start->m_Y + t * l1.m_Y;

    return 1;
}
//----------------------------------------------------------------------------
int miniRectsIntersect(const MINI_Rect* pFirstRect, const MINI_Rect* pSecondRect)
{
    return !(pFirstRect->m_Pos.m_X                               <= pSecondRect->m_Pos.m_X + pSecondRect->m_Size.m_Width  &&
             pFirstRect->m_Pos.m_X + pFirstRect->m_Size.m_Width  >= pSecondRect->m_Pos.m_X                                &&
             pFirstRect->m_Pos.m_Y                               <= pSecondRect->m_Pos.m_Y + pSecondRect->m_Size.m_Height &&
             pFirstRect->m_Pos.m_Y + pFirstRect->m_Size.m_Height >= pSecondRect->m_Pos.m_Y);
}
//----------------------------------------------------------------------------
int miniSpheresIntersect(const MINI_Sphere* pFirstSphere, const MINI_Sphere* pSecondSphere)
{
    MINI_Vector3 dist;
    float        length;

    // calculate the distance between the both sphere centers
    dist.m_X = fabs(pFirstSphere->m_Pos.m_X - pSecondSphere->m_Pos.m_X);
    dist.m_Y = fabs(pFirstSphere->m_Pos.m_Y - pSecondSphere->m_Pos.m_Y);
    dist.m_Z = fabs(pFirstSphere->m_Pos.m_Z - pSecondSphere->m_Pos.m_Z);

    // calculate the length between the both sphere centers
    miniLength(&dist, &length);

    // the spheres are on collision if the length between the centers is lower
    // than or equal to the sum of the both sphere radius
    return (length <= (pFirstSphere->m_Radius + pSecondSphere->m_Radius));
}
//----------------------------------------------------------------------------
int miniCircleRectIntersect(const MINI_Circle* pCircle, const MINI_Rect* pRect)
{
    MINI_Point circleDistance;
    float      cornerDistanceSq;

    // calculate distance between circle and rect
    circleDistance.m_X = fabs(pCircle->m_Pos.m_X - pRect->m_Pos.m_X);
    circleDistance.m_Y = fabs(pCircle->m_Pos.m_Y - pRect->m_Pos.m_Y);

    // is distance too high to be collided?
    if (circleDistance.m_X > (pRect->m_Size.m_Width / 2.0f + pCircle->m_Radius))
        return 0;
    else
    if (circleDistance.m_Y > (pRect->m_Size.m_Height / 2.0f + pCircle->m_Radius))
        return 0;

    // is distance too small to be not collided?
    if (circleDistance.m_X <= (pRect->m_Size.m_Width / 2.0f))
        return 1;
    else
    if (circleDistance.m_Y <= (pRect->m_Size.m_Height / 2.0f))
        return 1;

    // calculate distance between circle and nearest rect corner
    cornerDistanceSq = pow((circleDistance.m_X - pRect->m_Size.m_Width  / 2.0f), 2.0f) +
                       pow((circleDistance.m_Y - pRect->m_Size.m_Height / 2.0f), 2.0f);

    // is distance between circle and nearest rect corner too small?
    if (cornerDistanceSq <= pow(pCircle->m_Radius, 2.0f))
        return 1;

    return 0;
}
//----------------------------------------------------------------------------
int miniRayPlaneIntersect(const MINI_Plane*   pPl,
                          const MINI_Vector3* pPos,
                          const MINI_Vector3* pDir,
                                MINI_Vector3* pR)
{
    MINI_Vector3 n;
    float        dot;
    float        nDot;
    float        temp;

    // get the normal of the plane
    n.m_X = pPl->m_A;
    n.m_Y = pPl->m_B;
    n.m_Z = pPl->m_C;

    // calculate the angle between the line and the normal to the plane
    miniDot(&n, pDir, &dot);

    // if normal to the plane is perpendicular to the line, then the line is
    // either parallel to the plane and there are no solutions or the line is
    // on the plane in which case there are an infinite number of solutions
    if (!dot)
        return 0;

    miniDot(&n, pPos, &nDot);

    temp = ((pPl->m_D + nDot) / dot);

    // calculates the intersection point
    pR->m_X = (pPos->m_X - (temp * pDir->m_X));
    pR->m_Y = (pPos->m_Y - (temp * pDir->m_Y));
    pR->m_Z = (pPos->m_Z - (temp * pDir->m_Z));

    return 1;
}
//----------------------------------------------------------------------------
int miniLinePlaneIntersect(const MINI_Plane*   pPl,
                           const MINI_Vector3* pP1,
                           const MINI_Vector3* pP2,
                                 MINI_Vector3* pR)
{
    MINI_Vector3 dir;
    MINI_Vector3 nDir;

    // calculates the direction of the line
    miniSub(pP2, pP1, &dir);
    miniNormalize(&dir, &nDir);

    return miniRayPlaneIntersect(pPl, pP1, &nDir, pR);
}
//----------------------------------------------------------------------------
int miniPolygonsIntersect(MINI_Vector3* pP1V1, MINI_Vector3* pP1V2, MINI_Vector3* pP1V3,
                          MINI_Vector3* pP2V1, MINI_Vector3* pP2V2, MINI_Vector3* pP2V3)
{
    MINI_Plane   plane1;
    MINI_Plane   plane2;
    MINI_Vector3 v1v2LineS;
    MINI_Vector3 v1v2LineE;
    MINI_Vector3 v2v3LineS;
    MINI_Vector3 v2v3LineE;
    MINI_Vector3 v3v1LineS;
    MINI_Vector3 v3v1LineE;
    MINI_Vector3 ov1ov2LineS;
    MINI_Vector3 ov1ov2LineE;
    MINI_Vector3 ov2ov3LineS;
    MINI_Vector3 ov2ov3LineE;
    MINI_Vector3 ov3ov1LineS;
    MINI_Vector3 ov3ov1LineE;
    MINI_Vector3 normal1;
    MINI_Vector3 normal2;
    MINI_Vector3 p1pt1;
    MINI_Vector3 p1pt2;
    MINI_Vector3 p1pt3;
    MINI_Vector3 p2pt1;
    MINI_Vector3 p2pt2;
    MINI_Vector3 p2pt3;
    MINI_Vector3 p;
    int          p1ptsCount;
    int          p2ptsCount;
    float        planesDot;
    float        dist1v1;
    float        dist1v2;
    float        dist1v3;
    float        dist2v1;
    float        dist2v2;
    float        dist2v3;
    float        result;
    float        tolerance = 1.0E-3;

    // get planes from polygons
    miniPlaneFromPoints(pP1V1, pP1V2, pP1V3, &plane1);
    miniPlaneFromPoints(pP2V1, pP2V2, pP2V3, &plane2);

    // are planes merged?
    if (((plane1.m_A ==  plane2.m_A)  &&
         (plane1.m_B ==  plane2.m_B)  &&
         (plane1.m_C ==  plane2.m_C)  &&
         (plane1.m_D ==  plane2.m_D)) ||
        ((plane1.m_A == -plane2.m_A)  &&
         (plane1.m_B == -plane2.m_B)  &&
         (plane1.m_C == -plane2.m_C)  &&
         (plane1.m_D == -plane2.m_D)))
    {
        // is any vertex inside other polygon?
        if (miniPointInPolygon(pP1V1, pP2V1, pP2V2, pP2V3) ||
            miniPointInPolygon(pP1V2, pP2V1, pP2V2, pP2V3) ||
            miniPointInPolygon(pP1V3, pP2V1, pP2V2, pP2V3) ||
            miniPointInPolygon(pP2V1, pP1V1, pP1V2, pP1V3) ||
            miniPointInPolygon(pP2V2, pP1V1, pP1V2, pP1V3) ||
            miniPointInPolygon(pP2V3, pP1V1, pP1V2, pP1V3))
            return 1;

        // create polygon lines
        v1v2LineS   = *pP1V1;
        v1v2LineE   = *pP1V2;
        v2v3LineS   = *pP1V2;
        v2v3LineE   = *pP1V3;
        v3v1LineS   = *pP1V3;
        v3v1LineE   = *pP1V1;
        ov1ov2LineS = *pP2V1;
        ov1ov2LineE = *pP2V2;
        ov2ov3LineS = *pP2V2;
        ov2ov3LineE = *pP2V3;
        ov3ov1LineS = *pP2V3;
        ov3ov1LineE = *pP2V1;

        miniGetShortestDistance(&v1v2LineS,   &v1v2LineE,
                                &ov1ov2LineS, &ov1ov2LineE, &tolerance, &result);

        // is shortest distance between lines equal to 0?
        if (result < tolerance)
            return 1;

        miniGetShortestDistance(&v2v3LineS,   &v2v3LineE,
                                &ov1ov2LineS, &ov1ov2LineE, &tolerance, &result);

        // is shortest distance between lines equal to 0?
        if (result < tolerance)
            return 1;

        miniGetShortestDistance(&v3v1LineS,   &v3v1LineE,
                                &ov1ov2LineS, &ov1ov2LineE, &tolerance, &result);

        // is shortest distance between lines equal to 0?
        if (result < tolerance)
            return 1;

        miniGetShortestDistance(&v1v2LineS,   &v1v2LineE,
                                &ov2ov3LineS, &ov2ov3LineE, &tolerance, &result);

        // is shortest distance between lines equal to 0?
        if (result < tolerance)
            return 1;

        miniGetShortestDistance(&v2v3LineS,   &v2v3LineE,
                                &ov2ov3LineS, &ov2ov3LineE, &tolerance, &result);

        // is shortest distance between lines equal to 0?
        if (result < tolerance)
            return 1;

        miniGetShortestDistance(&v3v1LineS,   &v3v1LineE,
                                &ov2ov3LineS, &ov2ov3LineE, &tolerance, &result);

        // is shortest distance between lines equal to 0?
        if (result < tolerance)
            return 1;

        miniGetShortestDistance(&v1v2LineS,   &v1v2LineE,
                                &ov3ov1LineS, &ov3ov1LineE, &tolerance, &result);

        // is shortest distance between lines equal to 0?
        if (result < tolerance)
            return 1;

        miniGetShortestDistance(&v2v3LineS,   &v2v3LineE,
                                &ov3ov1LineS, &ov3ov1LineE, &tolerance, &result);

        // is shortest distance between lines equal to 0?
        if (result < tolerance)
            return 1;

        miniGetShortestDistance(&v3v1LineS,   &v3v1LineE,
                                &ov3ov1LineS, &ov3ov1LineE, &tolerance, &result);

        // is shortest distance between lines equal to 0?
        if (result < tolerance)
            return 1;

        return 0;
    }

    // get plane normals
    normal1.m_X = plane1.m_A;
    normal1.m_Y = plane1.m_B;
    normal1.m_Z = plane1.m_C;

    normal2.m_X = plane2.m_A;
    normal2.m_Y = plane2.m_B;
    normal2.m_Z = plane2.m_C;

    // calculate angle between planes
    miniDot(&normal1, &normal2, &planesDot);

    // are plane parallels?
    if (planesDot == 1.0f || planesDot == -1.0f)
        // planes are parallels but not merged, no collision is possible
        return 0;

    // calculate distance from each first polygon vertex to second polygon plane
    miniDistanceToPlane(pP1V1, &plane2, &dist1v1);
    miniDistanceToPlane(pP1V2, &plane2, &dist1v2);
    miniDistanceToPlane(pP1V3, &plane2, &dist1v3);

    // prepare list containing first polygon intersection points
    p1ptsCount = 0;

    // is first polygon V1 to V2 line segment intersects second polygon plane?
    if ((dist1v1 >= 0.0f && dist1v2 < 0.0f) || (dist1v1 < 0.0f && dist1v2 >= 0.0f))
        // calculate intersection point and add to list on success
        if (miniLinePlaneIntersect(&plane2, pP1V1, pP1V2, &p))
        {
            p1pt1 = p;
            ++p1ptsCount;
        }

    // is first polygon V2 to V3 line segment intersects second polygon plane?
    if ((dist1v2 >= 0.0f && dist1v3 < 0.0f) || (dist1v2 < 0.0f && dist1v3 >= 0.0f))
        // calculate intersection point and add to list on success
        if (miniLinePlaneIntersect(&plane2, pP1V2, pP1V3, &p))
        {
            p1pt2 = p;
            ++p1ptsCount;
        }

    // is first polygon V3 to V1 line segment intersects second polygon plane?
    if ((dist1v3 >= 0.0f && dist1v1 < 0.0f) || (dist1v3 < 0.0f && dist1v1 >= 0.0f))
        // calculate intersection point and add to list on success
        if (miniLinePlaneIntersect(&plane2, pP1V3, pP1V1, &p))
        {
            p1pt3 = p;
            ++p1ptsCount;
        }

    // were the first polygon 2 intersection point found?
    if (p1ptsCount != 2)
        return 0;

    // calculate distance from each second polygon vertex to first polygon plane
    miniDistanceToPlane(pP2V1, &plane1, &dist2v1);
    miniDistanceToPlane(pP2V2, &plane1, &dist2v2);
    miniDistanceToPlane(pP2V3, &plane1, &dist2v3);

    // prepare list containing second polygon intersection points
    p2ptsCount = 0;

    // is second polygon V1 to V2 line segment intersects first polygon plane?
    if ((dist2v1 >= 0.0f && dist2v2 < 0.0f) || (dist2v1 < 0.0f && dist2v2 >= 0.0f))
        // calculate intersection point and add to list on success
        if (miniLinePlaneIntersect(&plane1, pP2V1, pP2V2, &p))
        {
            p2pt1 = p;
            ++p2ptsCount;
        }

    // is second polygon V2 to V3 line segment intersects first polygon plane?
    if ((dist2v2 >= 0.0f && dist2v3 < 0.0f) || (dist2v2 < 0.0f && dist2v3 >= 0.0f))
        // calculate intersection point and add to list on success
        if (miniLinePlaneIntersect(&plane1, pP2V2, pP2V3, &p))
        {
            p2pt2 = p;
            ++p2ptsCount;
        }

    // is second polygon V3 to V1 line segment intersects first polygon plane?
    if ((dist2v3 >= 0.0f && dist2v1 < 0.0f) || (dist2v3 < 0.0f && dist2v1 >= 0.0f))
        // calculate intersection point and add to list on success
        if (miniLinePlaneIntersect(&plane1, pP2V3, pP2V1, &p))
        {
            p2pt3 = p;
            ++p2ptsCount;
        }

    // were the second polygon 2 intersection point found?
    if (p2ptsCount != 2)
        return 0;

    // first and second polygon intersection points are on the same line, so
    // check if calculated first and second polygon segments intersect
    if (miniVectorIsBetween(&p1pt1, &p2pt1, &p2pt2, &tolerance))
        return 1;

    if (miniVectorIsBetween(&p1pt2, &p2pt1, &p2pt2, &tolerance))
        return 1;

    if (miniVectorIsBetween(&p2pt1, &p1pt1, &p1pt2, &tolerance))
        return 1;

    if (miniVectorIsBetween(&p2pt2, &p1pt1, &p1pt2, &tolerance))
        return 1;

    return 0;
}
//----------------------------------------------------------------------------
int miniRayPolygonIntersect(const MINI_Ray* pRay, const MINI_Polygon* pPolygon)
{
    MINI_Plane   polygonPlane;
    MINI_Vector3 pointOnPlane;

    // create a plane using the 3 vertices of the polygon
    miniPlaneFromPoints(&pPolygon->m_v[0], &pPolygon->m_v[1], &pPolygon->m_v[2], &polygonPlane);

    // calculate the intersection point
    if (!miniRayPlaneIntersect(&polygonPlane, &pRay->m_Pos, &pRay->m_Dir, &pointOnPlane))
        return 0;

    // check if calculated point is inside the polygon
    return miniPointInPolygon(&pointOnPlane, &pPolygon->m_v[0], &pPolygon->m_v[1], &pPolygon->m_v[2]);
}
//----------------------------------------------------------------------------
int miniLinePolygonIntersect(const MINI_Vector3* pP1,
                             const MINI_Vector3* pP2,
                             const MINI_Polygon* pPolygon)
{
    MINI_Plane   polygonPlane;
    MINI_Vector3 pointOnPlane;

    // create a plane using the 3 vertices of the polygon
    miniPlaneFromPoints(&pPolygon->m_v[0], &pPolygon->m_v[1], &pPolygon->m_v[2], &polygonPlane);

    // calculate the intersection point
    if (!miniLinePlaneIntersect(&polygonPlane, pP1, pP2, &pointOnPlane))
        return 0;

    // check if calculated point is inside the polygon
    return miniPointInPolygon(&pointOnPlane, &pPolygon->m_v[0], &pPolygon->m_v[1], &pPolygon->m_v[2]);
}
//----------------------------------------------------------------------------
int miniRayBoxIntersect(const MINI_Ray* pRay, const MINI_Box* pBox)
{
    float tx1;
    float tx2;
    float ty1;
    float ty2;
    float tz1;
    float tz2;
    float txn;
    float txf;
    float tyn;
    float tyf;
    float tzn;
    float tzf;
    float tnear;
    float tfar;

    // get infinite value
    const float inf = 1.0f / 0.0f;

    // calculate nearest point where ray intersects box on x coordinate
    if (pRay->m_InvDir.m_X != inf)
        tx1 = ((pBox->m_Min.m_X - pRay->m_Pos.m_X) * pRay->m_InvDir.m_X);
    else
    if ((pBox->m_Min.m_X - pRay->m_Pos.m_X) < 0.0f)
        tx1 = -inf;
    else
        tx1 = inf;

    // calculate farthest point where ray intersects box on x coordinate
    if (pRay->m_InvDir.m_X != inf)
        tx2 = ((pBox->m_Max.m_X - pRay->m_Pos.m_X) * pRay->m_InvDir.m_X);
    else
    if ((pBox->m_Max.m_X - pRay->m_Pos.m_X) < 0.0f)
        tx2 = -inf;
    else
        tx2 = inf;

    // calculate nearest point where ray intersects box on y coordinate
    if (pRay->m_InvDir.m_Y != inf)
        ty1 = ((pBox->m_Min.m_Y - pRay->m_Pos.m_Y) * pRay->m_InvDir.m_Y);
    else
    if ((pBox->m_Min.m_Y - pRay->m_Pos.m_Y) < 0.0f)
        ty1 = -inf;
    else
        ty1 = inf;

    // calculate farthest point where ray intersects box on y coordinate
    if (pRay->m_InvDir.m_Y != inf)
        ty2 = ((pBox->m_Max.m_Y - pRay->m_Pos.m_Y) * pRay->m_InvDir.m_Y);
    else
    if ((pBox->m_Max.m_Y - pRay->m_Pos.m_Y) < 0.0f)
        ty2 = -inf;
    else
        ty2 = inf;

    // calculate nearest point where ray intersects box on z coordinate
    if (pRay->m_InvDir.m_Z != inf)
        tz1 = ((pBox->m_Min.m_Z - pRay->m_Pos.m_Z) * pRay->m_InvDir.m_Z);
    else
    if ((pBox->m_Min.m_Z - pRay->m_Pos.m_Z) < 0.0f)
        tz1 = -inf;
    else
        tz1 = inf;

    // calculate farthest point where ray intersects box on z coordinate
    if (pRay->m_InvDir.m_Z != inf)
        tz2 = ((pBox->m_Max.m_Z - pRay->m_Pos.m_Z) * pRay->m_InvDir.m_Z);
    else
    if ((pBox->m_Max.m_Z - pRay->m_Pos.m_Z) < 0.0f)
        tz2 = -inf;
    else
        tz2 = inf;

    // calculate near/far intersection on each axis
    miniMin(&tx1, &tx2, &txn);
    miniMax(&tx1, &tx2, &txf);
    miniMin(&ty1, &ty2, &tyn);
    miniMax(&ty1, &ty2, &tyf);
    miniMin(&tz1, &tz2, &tzn);
    miniMax(&tz1, &tz2, &tzf);

    // calculate final near/far intersection point
    miniMax(&tyn, &tzn,   &tnear);
    miniMax(&txn, &tnear, &tnear);
    miniMin(&tyf, &tzf,  &tfar);
    miniMin(&txf, &tfar, &tfar);

    // check if ray intersects box
    return (tfar >= tnear);
}
//----------------------------------------------------------------------------
int miniSpherePolygonIntersectAndSlide(const MINI_Sphere*  pSphere,
                                       const MINI_Polygon* pPolygon,
                                             MINI_Plane*   pSlidingPlane)
{
    float        testPoint1;
    float        testPoint2;
    MINI_Vector3 sphereNormal;
    MINI_Vector3 pointOnSphere;
    MINI_Vector3 pointOnPlane;
    MINI_Vector3 pointOnTriangle;
    MINI_Plane   polygonPlane;

    // create a plane using the 3 vertices of the polygon
    miniPlaneFromPoints(&pPolygon->m_v[0], &pPolygon->m_v[1], &pPolygon->m_v[2], &polygonPlane);

    // calculate the distance between the center of the sphere and the plane
    miniDistanceToPlane(&pSphere->m_Pos, &polygonPlane, &testPoint1);

    // calculate the normal of the distance sphere-plane using the positive or
    // negative value of the calculated distance between plane and position
    if (testPoint1 < 0.0f)
    {
        sphereNormal.m_X = polygonPlane.m_A;
        sphereNormal.m_Y = polygonPlane.m_B;
        sphereNormal.m_Z = polygonPlane.m_C;
    }
    else
    {
        sphereNormal.m_X = -polygonPlane.m_A;
        sphereNormal.m_Y = -polygonPlane.m_B;
        sphereNormal.m_Z = -polygonPlane.m_C;
    }

    // calculate the point who the segment from center of sphere in the
    // direction of the plane will cross the border of the sphere
    pointOnSphere.m_X = pSphere->m_Pos.m_X + (pSphere->m_Radius * sphereNormal.m_X);
    pointOnSphere.m_Y = pSphere->m_Pos.m_Y + (pSphere->m_Radius * sphereNormal.m_Y);
    pointOnSphere.m_Z = pSphere->m_Pos.m_Z + (pSphere->m_Radius * sphereNormal.m_Z);

    // calculate the distance between the border of the sphere and the plane
    miniDistanceToPlane(&pointOnSphere, &polygonPlane, &testPoint2);

    // if the test points are on each side of the plane, then the sphere cross
    // the plane. We assume that the segment from the center of the sphere to
    // the direction of the plane can never be co-planar
    if ((testPoint1 <= 0.0f && testPoint2 >= 0.0) ||
        (testPoint2 <= 0.0f && testPoint1 >= 0.0))
    {
        // calculate who the segment cross the plane
        if (testPoint1 == 0.0)
            // if testPoint1 is equal to 0, the center of the sphere cross the
            // plane
            pointOnPlane = pSphere->m_Pos;
        else
        if (testPoint2 == 0.0)
            // if testPoint2 is equal to 0, the border of the sphere cross the
            // plane
            pointOnPlane = pointOnSphere;
        else
            // calculate the intersection point
            miniLinePlaneIntersect(&polygonPlane,
                                   &pSphere->m_Pos,
                                   &pointOnSphere,
                                   &pointOnPlane);

        // check if calculated point is inside the polygon
        if (miniPointInPolygon(&pointOnPlane,
                               &pPolygon->m_v[0],
                               &pPolygon->m_v[1],
                               &pPolygon->m_v[2]))
        {
            // if yes, the sphere collide the polygon. In this case, we copy
            // the plane and we returns true
            *pSlidingPlane = polygonPlane;
            return 1;
        }
        else
        {
            // otherwise check if the sphere collide the border of the polygon.
            // First we calculate the test point on the border of the polygon
            miniClosestPointOnPolygon(&pointOnPlane,
                                      &pPolygon->m_v[0],
                                      &pPolygon->m_v[1],
                                      &pPolygon->m_v[2],
                                      &pointOnTriangle);

            // check if calculated point is inside the sphere
            if (miniPointInSphere(&pointOnTriangle, pSphere))
            {
                // if yes, the sphere collide the polygon. In this case, we
                // copy the plane and we returns true
                *pSlidingPlane = polygonPlane;
                return 1;
            }
        }
    }

    // no collision was found
    return 0;
}
//----------------------------------------------------------------------------
// Collision reaction
//----------------------------------------------------------------------------
void miniGetSlidingPoint(const MINI_Plane*   pSlidingPlane,
                         const MINI_Vector3* pPosition,
                         const float*        pRadius,
                               MINI_Vector3* pR)
{
    float        distanceToPlane;
    MINI_Plane   plane;
    MINI_Vector3 planeRatio;
    MINI_Vector3 pointBeyondPlane;
    MINI_Vector3 pointOnPlane;

    plane = *pSlidingPlane;

    // calculate the distance between the center of the sphere and the plane
    miniDistanceToPlane(pPosition, &plane, &distanceToPlane);

    // check if value is negative
    if (distanceToPlane < 0.0f)
    {
        // invert the plane
        plane.m_A = -plane.m_A;
        plane.m_B = -plane.m_B;
        plane.m_C = -plane.m_C;
        plane.m_D = -plane.m_D;
    }

    // calculate the direction of the segment position - plane
    planeRatio.m_X = *pRadius * plane.m_A;
    planeRatio.m_Y = *pRadius * plane.m_B;
    planeRatio.m_Z = *pRadius * plane.m_C;

    // calculate who the segment perpendicular to the plane, from the center
    // of the sphere, cross the collision sphere. Normally this point is beyond
    // the plane
    pointBeyondPlane.m_X = pPosition->m_X - planeRatio.m_X;
    pointBeyondPlane.m_Y = pPosition->m_Y - planeRatio.m_Y;
    pointBeyondPlane.m_Z = pPosition->m_Z - planeRatio.m_Z;

    // calculate the point where the segment "center of the sphere - point beyond
    // the plane" cross the collision plane
    miniLinePlaneIntersect(pSlidingPlane,
                           pPosition,
                           &pointBeyondPlane,
                           &pointOnPlane);

    // from point calculated below, we add the radius of the sphere, and we
    // returns the value
    pR->m_X = pointOnPlane.m_X + planeRatio.m_X;
    pR->m_Y = pointOnPlane.m_Y + planeRatio.m_Y;
    pR->m_Z = pointOnPlane.m_Z + planeRatio.m_Z;
}
//----------------------------------------------------------------------------
// Polygons extraction
//----------------------------------------------------------------------------
void miniAddPolygon(const float*         pVB,
                          unsigned       v1,
                          unsigned       v2,
                          unsigned       v3,
                          MINI_Polygon** pPolygons,
                          unsigned*      pPolygonsCount)
{
    // polygon array already contains polygons?
    if (!(*pPolygonsCount))
    {
        // no, add new first polygon in array
        *pPolygons     = (MINI_Polygon*)malloc(sizeof(MINI_Polygon));
        *pPolygonsCount = 1;
    }
    else
    {
        // yes, increase the polygons count and add new polygon inside array
        ++(*pPolygonsCount);
        *pPolygons = (MINI_Polygon*)realloc(*pPolygons,
                                            *pPolygonsCount * sizeof(MINI_Polygon));
    }

    // copy polygon
    (*pPolygons)[*pPolygonsCount - 1].m_v[0].m_X = pVB[v1];
    (*pPolygons)[*pPolygonsCount - 1].m_v[0].m_Y = pVB[v1 + 1];
    (*pPolygons)[*pPolygonsCount - 1].m_v[0].m_Z = pVB[v1 + 2];
    (*pPolygons)[*pPolygonsCount - 1].m_v[1].m_X = pVB[v2];
    (*pPolygons)[*pPolygonsCount - 1].m_v[1].m_Y = pVB[v2 + 1];
    (*pPolygons)[*pPolygonsCount - 1].m_v[1].m_Z = pVB[v2 + 2];
    (*pPolygons)[*pPolygonsCount - 1].m_v[2].m_X = pVB[v3];
    (*pPolygons)[*pPolygonsCount - 1].m_v[2].m_Y = pVB[v3 + 1];
    (*pPolygons)[*pPolygonsCount - 1].m_v[2].m_Z = pVB[v3 + 2];
}
//----------------------------------------------------------------------------
int miniGetPolygonsFromVB(const float*         pVB,
                                unsigned       length,
                                unsigned       type,
                                unsigned       stride,
                                MINI_Polygon** pPolygons,
                                unsigned*      pPolygonsCount)
{
    unsigned i;
    unsigned index;

    // no data to extract from?
    if (!length)
        return 1;

    // search for vertex type
    switch (type)
    {
        // triangles
        case 0:
        {
            // calculate iteration step
            const unsigned step = (stride * 3);

            // iterate through source vertices
            for (i = 0; i < length; i += step)
                // extract polygon from source buffer
                miniAddPolygon(pVB,
                               i,
                               i +  stride,
                               i + (stride * 2),
                               pPolygons,
                               pPolygonsCount);

            return 1;
        }

        // triangle strip
        case 1:
        {
            // calculate length to read in triangle strip buffer
            const unsigned stripLength = (length - (stride * 2));
                           index       = 0;

            // iterate through source vertices
            for (i = 0; i < stripLength; i += stride)
            {
                // extract polygon from source buffer, revert odd polygons
                if (!index || !(index % 2))
                    miniAddPolygon(pVB,
                                   i,
                                   i +  stride,
                                   i + (stride * 2),
                                   pPolygons,
                                   pPolygonsCount);
                else
                    miniAddPolygon(pVB,
                                   i +  stride,
                                   i,
                                   i + (stride * 2),
                                   pPolygons,
                                   pPolygonsCount);

                ++index;
            }

            return 1;
        }

        // triangle fan
        case 2:
        {
            // calculate length to read in triangle fan buffer
            const unsigned fanLength = (length - stride);

            // iterate through source vertices
            for (i = stride; i < fanLength; i += stride)
                // extract polygon from source buffer
                miniAddPolygon(pVB,
                               0,
                               i,
                               i + stride,
                               pPolygons,
                               pPolygonsCount);

            return 1;
        }

        // quads
        case 3:
        {
            // calculate iteration step
            const unsigned step = (stride * 4);

            // iterate through source vertices
            for (i = 0; i < length; i += step)
            {
                // calculate vertices position
                const unsigned v1 = i;
                const unsigned v2 = i +  stride;
                const unsigned v3 = i + (stride * 2);
                const unsigned v4 = i + (stride * 3);

                // extract polygons from source buffer
                miniAddPolygon(pVB, v1, v2, v3, pPolygons, pPolygonsCount);
                miniAddPolygon(pVB, v3, v2, v4, pPolygons, pPolygonsCount);
            }

            return 1;
        }

        // quad strip
        case 4:
        {
            // calculate iteration step
            const unsigned step = (stride * 2);

            // calculate length to read in triangle strip buffer
            const unsigned stripLength = (length - (stride * 2));

            // iterate through source vertices
            for (i = 0; i < stripLength; i += step)
            {
                // calculate vertices position
                const unsigned v1 = i;
                const unsigned v2 = i +  stride;
                const unsigned v3 = i + (stride * 2);
                const unsigned v4 = i + (stride * 3);

                // extract polygons from source buffer
                miniAddPolygon(pVB, v1, v2, v3, pPolygons, pPolygonsCount);
                miniAddPolygon(pVB, v3, v2, v4, pPolygons, pPolygonsCount);
            }

            return 1;
        }

        default:
            return 0;
    }
}
//----------------------------------------------------------------------------
void miniReleasePolygons(MINI_Polygon* pPolygons)
{
    if (pPolygons)
        free(pPolygons);
}
//----------------------------------------------------------------------------
// Aligned-Axis Bounding Box tree
//----------------------------------------------------------------------------
void miniCutBox(const MINI_Box* pBox, MINI_Box* pLeftBox, MINI_Box* pRightBox)
{
    float    x;
    float    y;
    float    z;
    unsigned longestAxis;

    // calculate each edge length
    x = fabs(pBox->m_Max.m_X - pBox->m_Min.m_X);
    y = fabs(pBox->m_Max.m_Y - pBox->m_Min.m_Y);
    z = fabs(pBox->m_Max.m_Z - pBox->m_Min.m_Z);

    // search for longest axis
    if (x >= y && x >= z)
        longestAxis = 0;
    else
    if (y >= x && y >= z)
        longestAxis = 1;
    else
        longestAxis = 2;

    // cut box
    switch (longestAxis)
    {
        // cut on x axis
        case 0:
            pLeftBox->m_Min     = pBox->m_Min;
            pLeftBox->m_Max     = pBox->m_Max;
            pLeftBox->m_Max.m_X = pBox->m_Min.m_X + (x / 2.0f);

            pRightBox->m_Min     = pBox->m_Min;
            pRightBox->m_Max     = pBox->m_Max;
            pRightBox->m_Min.m_X = pBox->m_Min.m_X + (x / 2.0f);
            break;

        // cut on y axis
        case 1:
            pLeftBox->m_Min     = pBox->m_Min;
            pLeftBox->m_Max     = pBox->m_Max;
            pLeftBox->m_Max.m_Y = pBox->m_Min.m_Y + (y / 2.0f);

            pRightBox->m_Min     = pBox->m_Min;
            pRightBox->m_Max     = pBox->m_Max;
            pRightBox->m_Min.m_Y = pBox->m_Min.m_Y + (y / 2.0f);
            break;

        // cut on z axis
        case 2:
            pLeftBox->m_Min     = pBox->m_Min;
            pLeftBox->m_Max     = pBox->m_Max;
            pLeftBox->m_Max.m_Z = pBox->m_Min.m_Z + (z / 2.0f);

            pRightBox->m_Min     = pBox->m_Min;
            pRightBox->m_Max     = pBox->m_Max;
            pRightBox->m_Min.m_Z = pBox->m_Min.m_Z + (z / 2.0f);
            break;
    }
}
//----------------------------------------------------------------------------
int miniPopulateTree(      MINI_AABBNode* pNode,
                     const MINI_Polygon*  pPolygons,
                           unsigned       polygonsCount)
{
    unsigned      i;
    unsigned      j;
    MINI_Box      leftBox;
    MINI_Box      rightBox;
    MINI_Polygon* pLeftPolygons      = 0;
    MINI_Polygon* pRightPolygons     = 0;
    unsigned      leftPolygonsCount  = 0;
    unsigned      rightPolygonsCount = 0;
    unsigned      insideLeft         = 0;
    unsigned      insideRight        = 0;
    int           boxEmpty           = 1;
    int           canResolveLeft     = 0;
    int           canResolveRight    = 0;
    int           result             = 0;

    // initialize node content
    pNode->m_pParent       = 0;
    pNode->m_pLeft         = 0;
    pNode->m_pRight        = 0;
    pNode->m_pBox          = 0;
    pNode->m_pPolygons     = 0;
    pNode->m_PolygonsCount = 0;

    // create a collision box
    pNode->m_pBox = (MINI_Box*)malloc(sizeof(MINI_Box));

    // iterate through polygons to divide
    for (i = 0; i < polygonsCount; ++i)
        // calculate bounding box
        miniAddPolygonToBoundingBox(&pPolygons[i], pNode->m_pBox, &boxEmpty);

    // divide box in 2 sub-boxes
    miniCutBox(pNode->m_pBox, &leftBox, &rightBox);

    // iterate again through polygons to divide
    for (i = 0; i < polygonsCount; ++i)
    {
        insideLeft  = 0;
        insideRight = 0;

        // check if first polygon vertice belongs to left or right sub-box
        for (j = 0; j < 3; ++j)
            if (miniPointInBox(&pPolygons[i].m_v[j], &leftBox))
                ++insideLeft;
            else
                ++insideRight;

        // do include polygon in left or right list?
        if (insideLeft >= insideRight)
        {
            // left node polygon array already contains polygons?
            if (!leftPolygonsCount)
            {
                // no, add new first polygon in array
                pLeftPolygons     = (MINI_Polygon*)malloc(sizeof(MINI_Polygon));
                leftPolygonsCount = 1;
            }
            else
            {
                // yes, increase the polygons count and add new polygon inside array
                ++leftPolygonsCount;
                pLeftPolygons = (MINI_Polygon*)realloc(pLeftPolygons,
                                                       leftPolygonsCount * sizeof(MINI_Polygon));
            }

            // copy polygon
            pLeftPolygons[leftPolygonsCount - 1] = pPolygons[i];
        }
        else
        {
            // right node polygon array already contains polygons?
            if (!rightPolygonsCount)
            {
                // no, add new first polygon in array
                pRightPolygons    = (MINI_Polygon*)malloc(sizeof(MINI_Polygon));
                rightPolygonsCount = 1;
            }
            else
            {
                // yes, increase the polygons count and add new polygon inside array
                ++rightPolygonsCount;
                pRightPolygons = (MINI_Polygon*)realloc(pRightPolygons,
                                                        rightPolygonsCount * sizeof(MINI_Polygon));
            }

            // copy polygon
            pRightPolygons[rightPolygonsCount - 1] = pPolygons[i];
        }
    }

    canResolveLeft  = (leftPolygonsCount  && leftPolygonsCount  < polygonsCount);
    canResolveRight = (rightPolygonsCount && rightPolygonsCount < polygonsCount);

    // leaf reached?
    if (!canResolveLeft && !canResolveRight)
    {
        // copy left polygons in polygon list
        for (i = 0; i < leftPolygonsCount; ++i)
        {
            // node polygon array already contains polygons?
            if (!pNode->m_PolygonsCount)
            {
                // no, add new first polygon in array
                pNode->m_pPolygons     = (MINI_Polygon*)malloc(sizeof(MINI_Polygon));
                pNode->m_PolygonsCount = 1;
            }
            else
            {
                // yes, increase the polygons count and add new polygon inside array
                ++pNode->m_PolygonsCount;
                pNode->m_pPolygons = (MINI_Polygon*)realloc(pNode->m_pPolygons,
                                                            pNode->m_PolygonsCount * sizeof(MINI_Polygon));
            }

            // copy polygon
            for (j = 0; j < 3 ; ++j)
                pNode->m_pPolygons[pNode->m_PolygonsCount - 1].m_v[j] = pLeftPolygons[i].m_v[j];
        }

        // copy right polygons in polygon list
        for (i = 0; i < rightPolygonsCount; ++i)
        {
            // node polygon array already contains polygons?
            if (!pNode->m_PolygonsCount)
            {
                // no, add new first polygon in array
                pNode->m_pPolygons     = (MINI_Polygon*)malloc(sizeof(MINI_Polygon));
                pNode->m_PolygonsCount = 1;
            }
            else
            {
                // yes, increase the polygons count and add new polygon inside array
                ++pNode->m_PolygonsCount;
                pNode->m_pPolygons = (MINI_Polygon*)realloc(pNode->m_pPolygons,
                                                            pNode->m_PolygonsCount * sizeof(MINI_Polygon));
            }

            // copy polygon
            for (j = 0; j < 3 ; ++j)
                pNode->m_pPolygons[pNode->m_PolygonsCount - 1].m_v[j] = pRightPolygons[i].m_v[j];
        }

        // delete left list, as it will no more be used
        if (pLeftPolygons)
            free(pLeftPolygons);

        // delete right list, as it will no more be used
        if (pRightPolygons)
            free(pRightPolygons);

        return 1;
    }

    // do create left node?
    if (canResolveLeft)
    {
        // create left node
        pNode->m_pLeft = (MINI_AABBNode*)malloc(sizeof(MINI_AABBNode));

        result |= miniPopulateTree(pNode->m_pLeft, pLeftPolygons, leftPolygonsCount);

        // set node parent. IMPORTANT must be done after the call to miniPopulateTree()
        pNode->m_pLeft->m_pParent = pNode;

        // delete current list, as it will no more be used
        if (pLeftPolygons)
            free(pLeftPolygons);
    }

    // do create right node?
    if (canResolveRight)
    {
        // create right node
        pNode->m_pRight = (MINI_AABBNode*)malloc(sizeof(MINI_AABBNode));

        result |= miniPopulateTree(pNode->m_pRight, pRightPolygons, rightPolygonsCount);

        // set node parent. IMPORTANT must be done after the call to miniPopulateTree()
        pNode->m_pRight->m_pParent = pNode;

        // delete current list, as it will no more be used
        if (pRightPolygons)
            free(pRightPolygons);
    }

    return result;
}
//----------------------------------------------------------------------------
int miniResolveTree(MINI_Ray*      pRay,
                    MINI_AABBNode* pNode,
                    MINI_Polygon** pPolygons,
                    unsigned*      pPolygonsCount)
{
    unsigned i;
    unsigned j;
    int      leftResolved  = 0;
    int      rightResolved = 0;

    // no node to resolve? (this should never happen, but...)
    if (!pNode)
        return 0;

    // is leaf?
    if (!pNode->m_pLeft && !pNode->m_pRight)
    {
        for (i = 0; i < pNode->m_PolygonsCount; ++i)
        {
            // polygon array already contains polygons?
            if (!(*pPolygonsCount))
            {
                // no, add new first polygon in array
                *pPolygons     = (MINI_Polygon*)malloc(sizeof(MINI_Polygon));
                *pPolygonsCount = 1;
            }
            else
            {
                // yes, increase the polygons count and add new polygon inside array
                ++(*pPolygonsCount);
                *pPolygons = (MINI_Polygon*)realloc(*pPolygons,
                                                    *pPolygonsCount * sizeof(MINI_Polygon));
            }

            // copy polygon
            for (j = 0; j < 3; ++j)
                (*pPolygons)[*pPolygonsCount - 1].m_v[j] = pNode->m_pPolygons[i].m_v[j];
        }

        return 1;
    }

    // check if ray intersects the left box
    if (pNode->m_pLeft && miniRayBoxIntersect(pRay, pNode->m_pLeft->m_pBox))
        // resolve left node
        leftResolved = miniResolveTree(pRay, pNode->m_pLeft, pPolygons, pPolygonsCount);

    // check if ray intersects the right box
    if (pNode->m_pRight && miniRayBoxIntersect(pRay, pNode->m_pRight->m_pBox))
        // resolve right node
        rightResolved = miniResolveTree(pRay, pNode->m_pRight, pPolygons, pPolygonsCount);

    return (leftResolved || rightResolved);
}
//----------------------------------------------------------------------------
void miniReleaseNode(MINI_AABBNode* pNode)
{
    // delete bounding box, if exists
    if (pNode->m_pBox)
        free(pNode->m_pBox);

    // delete polygon array, if exists
    if (pNode->m_pPolygons)
        free(pNode->m_pPolygons);

    pNode->m_PolygonsCount = 0;
}
//----------------------------------------------------------------------------
void miniReleaseTree(MINI_AABBNode* pNode)
{
    if (!pNode)
        return;

    // release all children on left side
    if (pNode->m_pLeft)
        miniReleaseTree(pNode->m_pLeft);

    // release all children on right side
    if (pNode->m_pRight)
        miniReleaseTree(pNode->m_pRight);

    // delete node content
    miniReleaseNode(pNode);

    // delete node
    free(pNode);
}
//----------------------------------------------------------------------------
