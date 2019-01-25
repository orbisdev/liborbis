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

#include "MiniGeometry.h"

// std
#include <stdlib.h>
#include <math.h>

//----------------------------------------------------------------------------
// Distances calculation
//----------------------------------------------------------------------------
int miniVectorIsBetween(const MINI_Vector3* pP,
                        const MINI_Vector3* pS,
                        const MINI_Vector3* pE,
                        const float*        pTolerance)
{
    // check if each vector component is between start and end limits
    if (!miniValueIsBetween(&pP->m_X, &pS->m_X, &pE->m_X, pTolerance))
        return 0;

    if (!miniValueIsBetween(&pP->m_Y, &pS->m_Y, &pE->m_Y, pTolerance))
        return 0;

    if (!miniValueIsBetween(&pP->m_Z, &pS->m_Z, &pE->m_Z, pTolerance))
        return 0;

    return 1;
}
//----------------------------------------------------------------------------
void miniGetShortestDistance(const MINI_Vector3* pL1S,
                             const MINI_Vector3* pL1E,
                             const MINI_Vector3* pL2S,
                             const MINI_Vector3* pL2E,
                             const float*        pTolerance,
                                   float*        pR)
{
    MINI_Vector3 delta21;
    MINI_Vector3 delta43;
    MINI_Vector3 delta13;
    MINI_Vector3 dP;
    float        a;
    float        b;
    float        c;
    float        d;
    float        e;
    float        D;
    float        sc;
    float        sN;
    float        sD;
    float        tc;
    float        tN;
    float        tD;
    float        dotdP;

    // calculate p2 - p1, p4 - p3, and p1 - p3 line segments
    miniSub(pL1E, pL1S, &delta21);
    miniSub(pL2E, pL2S, &delta43);
    miniSub(pL1S, pL2S, &delta13);

    // calculate some needed values - a, c and D are always >= 0
    miniDot(&delta21, &delta21, &a);
    miniDot(&delta21, &delta43, &b);
    miniDot(&delta43, &delta43, &c);
    miniDot(&delta21, &delta13, &d);
    miniDot(&delta43, &delta13, &e);
    D = ((a * c) - (b * b));

    // sc = sN / sD, default sD = D >= 0
    sD = D;

    // tc = tN / tD, default tD = D >= 0
    tD = D;

    // compute the line parameters of the two closest points
    if (D < *pTolerance)
    {
        // the lines are almost parallel, force using point P0 on segment S1
        // to prevent possible division by 0 later
        sN = 0.0f;
        sD = 1.0f;
        tN = e;
        tD = c;
    }
    else
    {
        // get the closest points on the infinite lines
        sN = ((b * e) - (c * d));
        tN = ((a * e) - (b * d));

        // sc < 0 => the s=0 edge is visible
        if (sN < 0.0f)
        {
            sN = 0.0f;
            tN = e;
            tD = c;
        }
        else
        // sc > 1 => the s=1 edge is visible
        if (sN > sD)
        {
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    // tc < 0 => the t=0 edge is visible
    if (tN < 0.0f)
    {
        tN = 0.0f;

        // recompute sc for this edge
        if (-d < 0.0f)
            sN = 0.0f;
        else
        if (-d > a)
            sN = sD;
        else
        {
            sN = -d;
            sD =  a;
        }
    }
    else
    // tc > 1 => the t=1 edge is visible
    if (tN > tD)
    {
        tN = tD;

        // recompute sc for this edge
        if ((-d + b) < 0.0f)
            sN = 0.0f;
        else
        if ((-d + b) > a)
            sN = sD;
        else
        {
            sN = (-d + b);
            sD = a;
        }
    }

    // finally do the division to get sc and tc
    if (fabs(sN) < *pTolerance)
        sc = 0.0f;
    else
        sc = sN / sD;

    if (fabs(tN) < *pTolerance)
        tc = 0.0f;
    else
        tc = tN / tD;

    // get the difference of the two closest points
    dP.m_X = delta13.m_X + (sc * delta21.m_X) - (tc * delta43.m_X);
    dP.m_Y = delta13.m_Y + (sc * delta21.m_Y) - (tc * delta43.m_Y);
    dP.m_Z = delta13.m_Z + (sc * delta21.m_Z) - (tc * delta43.m_Z);

    // return the closest distance
    miniDot(&dP, &dP, &dotdP);
    *pR = sqrt(dotdP);
}
//----------------------------------------------------------------------------
void miniClosestPointOnLine(const MINI_Vector3* pSegStart,
                            const MINI_Vector3* pSegEnd,
                            const MINI_Vector3* pPoint,
                                  MINI_Vector3* pR)
{
    float        segLength;
    float        angle;
    MINI_Vector3 PToStart;
    MINI_Vector3 length;
    MINI_Vector3 normalizedLength;
    MINI_Vector3 p;

    // calculate the distance between the test point and the segment
    miniSub(pPoint, pSegStart, &PToStart);
    miniSub(pSegEnd, pSegStart, &length);
    miniLength(&length, &segLength);

    // calculate the direction of the segment
    miniNormalize(&length, &normalizedLength);

    // calculate the projection of the point on the segment
    miniDot(&normalizedLength, &PToStart, &angle);

    // check if projection is before the segment
    if (angle < 0.0)
        *pR = *pSegStart;
    else
    // check if projection is after the segment
    if (angle > segLength)
        *pR = *pSegEnd;
    else
    {
        // calculate the position of the projection on the segment
        p.m_X = normalizedLength.m_X * angle;
        p.m_Y = normalizedLength.m_Y * angle;
        p.m_Z = normalizedLength.m_Z * angle;

        // calculate and returns the point coordinate on the segment
    	miniAdd(pSegStart, &p, pR);
    }
}
//----------------------------------------------------------------------------
void miniClosestPointOnPolygon(const MINI_Vector3* pPoint,
                               const MINI_Vector3* pV1,
                               const MINI_Vector3* pV2,
                               const MINI_Vector3* pV3,
                                     MINI_Vector3* pR)
{
    float        dAB;
    float        dBC;
    float        dCA;
    float        min;
    MINI_Vector3 rab;
    MINI_Vector3 rbc;
    MINI_Vector3 rca;
    MINI_Vector3 vAB;
    MINI_Vector3 vBC;
    MINI_Vector3 vCA;

    // calculate the projections points on each edge of the triangle
    miniClosestPointOnLine(pV1, pV2, pPoint, &rab);
    miniClosestPointOnLine(pV2, pV3, pPoint, &rbc);
    miniClosestPointOnLine(pV3, pV1, pPoint, &rca);

    // calculate the distances between points below and test point
    miniSub(pPoint, &rab, &vAB);
    miniSub(pPoint, &rbc, &vBC);
    miniSub(pPoint, &rca, &vCA);

    // calculate the length of each segments
    miniLength(&vAB, &dAB);
    miniLength(&vBC, &dBC);
    miniLength(&vCA, &dCA);

    // calculate the shortest distance
    min = dAB;
    *pR = rab;

    // check if dBC is shortest
    if (dBC < min)
    {
        min = dBC;
        *pR = rbc;
    }

    // check if dCA is shortest
    if (dCA < min)
        *pR = rca;
}
//----------------------------------------------------------------------------
// Vector functions
//----------------------------------------------------------------------------
void miniAdd(const MINI_Vector3* pV1, const MINI_Vector3* pV2, MINI_Vector3* pR)
{
    pR->m_X = pV1->m_X + pV2->m_X;
    pR->m_Y = pV1->m_Y + pV2->m_Y;
    pR->m_Z = pV1->m_Z + pV2->m_Z;
}
//----------------------------------------------------------------------------
void miniSub(const MINI_Vector3* pV1, const MINI_Vector3* pV2, MINI_Vector3* pR)
{
    pR->m_X = pV1->m_X - pV2->m_X;
    pR->m_Y = pV1->m_Y - pV2->m_Y;
    pR->m_Z = pV1->m_Z - pV2->m_Z;
}
//----------------------------------------------------------------------------
void miniLength(const MINI_Vector3* pV, float* pR)
{
    *pR = sqrt((pV->m_X * pV->m_X) + (pV->m_Y * pV->m_Y) + (pV->m_Z * pV->m_Z));
}
//----------------------------------------------------------------------------
void miniNormalize(const MINI_Vector3* pV, MINI_Vector3* pR)
{
    float len;

    // calculate vector length
    miniLength(pV, &len);

    // no vector length?
    if (len == 0.0f)
    {
        pR->m_X = 0.0f;
        pR->m_Y = 0.0f;
        pR->m_Z = 0.0f;
        return;
    }

    // normalize vector (thus values will always be between 0.0f, and 1.0f)
    pR->m_X = (pV->m_X / len);
    pR->m_Y = (pV->m_Y / len);
    pR->m_Z = (pV->m_Z / len);
}
//----------------------------------------------------------------------------
void miniCross(const MINI_Vector3* pV1, const MINI_Vector3* pV2, MINI_Vector3* pR)
{
    pR->m_X = (pV1->m_Y * pV2->m_Z) - (pV2->m_Y * pV1->m_Z);
    pR->m_Y = (pV1->m_Z * pV2->m_X) - (pV2->m_Z * pV1->m_X);
    pR->m_Z = (pV1->m_X * pV2->m_Y) - (pV2->m_X * pV1->m_Y);
}
//----------------------------------------------------------------------------
void miniDot(const MINI_Vector3* pV1, const MINI_Vector3* pV2, float* pR)
{
    *pR = ((pV1->m_X * pV2->m_X) + (pV1->m_Y * pV2->m_Y) + (pV1->m_Z * pV2->m_Z));
}
//----------------------------------------------------------------------------
// Plane functions
//----------------------------------------------------------------------------
void miniPlaneFromPointNormal(const MINI_Vector3* pP, const MINI_Vector3* pN, MINI_Plane* pR)
{
    float d;

    // the a, b, and c components are only the normal of the plane
    pR->m_A = pN->m_X;
    pR->m_B = pN->m_Y;
    pR->m_C = pN->m_Z;

    // calculate plane d component using the aX + bY + cZ + d = 0 formula
    miniDot(pN, pP, &d);
    pR->m_D = -d;
}
//----------------------------------------------------------------------------
void miniPlaneFromPoints(const MINI_Vector3* pV1,
                         const MINI_Vector3* pV2,
                         const MINI_Vector3* pV3,
                               MINI_Plane*   pR)
{
    MINI_Vector3 e1;
    MINI_Vector3 e2;
    MINI_Vector3 normal;

    // calculate edge vectors
    miniSub(pV2, pV1, &e1);
    miniSub(pV3, pV1, &e2);

    // calculate the normal of the plane
    miniCross(&e1, &e2, &normal);
    miniNormalize(&normal, &normal);

    // calculate and return the plane
    miniPlaneFromPointNormal(pV1, &normal, pR);
}
//----------------------------------------------------------------------------
void miniDistanceToPlane(const MINI_Vector3* pP, const MINI_Plane* pPl, float* pR)
{
    MINI_Vector3 n;
    float        dist;

    // get the normal of the plane
    n.m_X = pPl->m_A;
    n.m_Y = pPl->m_B;
    n.m_Z = pPl->m_C;

    // calculate the distance between the plane and the point
    miniDot(&n, pP, &dist);
    *pR = dist + pPl->m_D;
}
//----------------------------------------------------------------------------
// Matrix functions
//----------------------------------------------------------------------------
void miniGetIdentity(MINI_Matrix* pR)
{
    pR->m_Table[0][0] = 1.0f; pR->m_Table[1][0] = 0.0f; pR->m_Table[2][0] = 0.0f; pR->m_Table[3][0] = 0.0f;
    pR->m_Table[0][1] = 0.0f; pR->m_Table[1][1] = 1.0f; pR->m_Table[2][1] = 0.0f; pR->m_Table[3][1] = 0.0f;
    pR->m_Table[0][2] = 0.0f; pR->m_Table[1][2] = 0.0f; pR->m_Table[2][2] = 1.0f; pR->m_Table[3][2] = 0.0f;
    pR->m_Table[0][3] = 0.0f; pR->m_Table[1][3] = 0.0f; pR->m_Table[2][3] = 0.0f; pR->m_Table[3][3] = 1.0f;
}
//----------------------------------------------------------------------------
void miniGetOrtho(const float* pLeft,
                  const float* pRight,
                  const float* pBottom,
                  const float* pTop,
                  const float* pZNear,
                  const float* pZFar,
                  MINI_Matrix* pR)
{
    // OpenGL specifications                                 can be rewritten as
    // |  2/(r-l)  0         0        -(r+l)/(r-l)  |        |  2/(r-l)  0        0        (r+l)/(l-r)  |
    // |  0        2/(t-b)   0        -(t+b)/(t-b)  |   =>   |  0        2/(t-b)  0        (t+b)/(b-t)  |
    // |  0        0        -2/(f-n)  -(f+n)/(f-n)  |        |  0        0        2/(n-f)  (f+n)/(n-f)  |
    // |  0        0         0         1            |        |  0        0        0        1            |

    // calculate matrix component values
    const float pfn = *pZFar  + *pZNear;
    const float mnf = *pZNear - *pZFar;
    const float prl = *pRight + *pLeft;
    const float mrl = *pRight - *pLeft;
    const float ptb = *pTop   + *pBottom;
    const float mtb = *pTop   - *pBottom;
    const float mlr = -mrl;
    const float mbt = -mtb;

    // build matrix
    pR->m_Table[0][0] = 2.0f / mrl; pR->m_Table[1][0] = 0.0f;       pR->m_Table[2][0] = 0.0f;       pR->m_Table[3][0] = prl / mlr;
    pR->m_Table[0][1] = 0.0f;       pR->m_Table[1][1] = 2.0f / mtb; pR->m_Table[2][1] = 0.0f;       pR->m_Table[3][1] = ptb / mbt;
    pR->m_Table[0][2] = 0.0f;       pR->m_Table[1][2] = 0.0f;       pR->m_Table[2][2] = 2.0f / mnf; pR->m_Table[3][2] = pfn / mnf;
    pR->m_Table[0][3] = 0.0f;       pR->m_Table[1][3] = 0.0f;       pR->m_Table[2][3] = 0.0f;       pR->m_Table[3][3] = 1.0f;
}
//----------------------------------------------------------------------------
void miniGetFrustum(const float* pLeft,
                    const float* pRight,
                    const float* pBottom,
                    const float* pTop,
                    const float* pZNear,
                    const float* pZFar,
                    MINI_Matrix* pR)
{
    // OpenGL specifications                                          can be rewritten as
    // |  2n/(r-l)     0             0             0         |        |  2n/(r-l)     0             0            0         |
    // |  0            2n/(t-b)      0             0         |   =>   |  0            2n/(t-b)      0            0         |
    // |  (r+l)/(r-l)  (t+b)/(t-b)  -(f+n)/(f-n)  -2fn/(f-n) |        |  (r+l)/(r-l)  (t+b)/(t-b)   (f+n)/(n-f)  2fn/(n-f) |
    // |  0            0            -1             0         |        |  0            0            -1            0         |
    // invalid for n <= 0, f <= 0, l = r, b = t, or n = f

    // calculate matrix component values
    const float x2n  =  2.0f   * *pZNear;
    const float x2nf =  x2n    * *pZFar;
    const float pfn  = *pZFar  + *pZNear;
    const float mnf  = *pZNear - *pZFar;
    const float prl  = *pRight + *pLeft;
    const float mrl  = *pRight - *pLeft;
    const float ptb  = *pTop   + *pBottom;
    const float mtb  = *pTop   - *pBottom;

    // build matrix
    pR->m_Table[0][0] = x2n / mrl; pR->m_Table[1][0] = 0.0f;      pR->m_Table[2][0] =  0.0f;      pR->m_Table[3][0] = 0.0f;
    pR->m_Table[0][1] = 0.0f;      pR->m_Table[1][1] = x2n / mtb; pR->m_Table[2][1] =  0.0f;      pR->m_Table[3][1] = 0.0f;
    pR->m_Table[0][2] = prl / mrl; pR->m_Table[1][2] = ptb / mtb; pR->m_Table[2][2] =  pfn / mnf; pR->m_Table[3][2] = x2nf / mnf;
    pR->m_Table[0][3] = 0.0f;      pR->m_Table[1][3] = 0.0f;      pR->m_Table[2][3] = -1.0f;      pR->m_Table[3][3] = 0.0f;
}
//----------------------------------------------------------------------------
void miniGetPerspective(const float* pFovyDeg,
                        const float* pAspect,
                        const float* pZNear,
                        const float* pZFar,
                        MINI_Matrix* pR)
{
    const float maxY    = *pZNear *  tanf(*pFovyDeg * M_PI / 360.0f);
    const float maxX    =  maxY   * *pAspect;
    const float negMaxY = -maxY;
    const float negMaxX = -maxX;

    miniGetFrustum(&negMaxX, &maxX, &negMaxY, &maxY, pZNear, pZFar, pR);
}
//----------------------------------------------------------------------------
void miniGetTranslateMatrix(const MINI_Vector3* pT, MINI_Matrix* pR)
{
    miniGetIdentity(pR);
    pR->m_Table[3][0] += pR->m_Table[0][0] * pT->m_X + pR->m_Table[1][0] * pT->m_Y + pR->m_Table[2][0] * pT->m_Z;
    pR->m_Table[3][1] += pR->m_Table[0][1] * pT->m_X + pR->m_Table[1][1] * pT->m_Y + pR->m_Table[2][1] * pT->m_Z;
    pR->m_Table[3][2] += pR->m_Table[0][2] * pT->m_X + pR->m_Table[1][2] * pT->m_Y + pR->m_Table[2][2] * pT->m_Z;
    pR->m_Table[3][3] += pR->m_Table[0][3] * pT->m_X + pR->m_Table[1][3] * pT->m_Y + pR->m_Table[2][3] * pT->m_Z;
}
//----------------------------------------------------------------------------
void miniGetRotateMatrix(const float* pAngle, const MINI_Vector3* pAxis, MINI_Matrix* pR)
{
    // calculate sinus, cosinus and inverted cosinus values
    const float c  = cosf(*pAngle);
    const float s  = sinf(*pAngle);
    const float ic = (1.0f - c);

    // create rotation matrix
    miniGetIdentity(pR);
    pR->m_Table[0][0] = (ic * pAxis->m_X * pAxis->m_X) +  c;
    pR->m_Table[1][0] = (ic * pAxis->m_X * pAxis->m_Y) - (s * pAxis->m_Z);
    pR->m_Table[2][0] = (ic * pAxis->m_X * pAxis->m_Z) + (s * pAxis->m_Y);
    pR->m_Table[0][1] = (ic * pAxis->m_Y * pAxis->m_X) + (s * pAxis->m_Z);
    pR->m_Table[1][1] = (ic * pAxis->m_Y * pAxis->m_Y) +  c;
    pR->m_Table[2][1] = (ic * pAxis->m_Y * pAxis->m_Z) - (s * pAxis->m_X);
    pR->m_Table[0][2] = (ic * pAxis->m_Z * pAxis->m_X) - (s * pAxis->m_Y);
    pR->m_Table[1][2] = (ic * pAxis->m_Z * pAxis->m_Y) + (s * pAxis->m_X);
    pR->m_Table[2][2] = (ic * pAxis->m_Z * pAxis->m_Z) +  c;
}
//----------------------------------------------------------------------------
void miniGetScaleMatrix(const MINI_Vector3* pFactor, MINI_Matrix* pR)
{
    miniGetIdentity(pR);
    pR->m_Table[0][0] *= pFactor->m_X; pR->m_Table[1][0] *= pFactor->m_Y; pR->m_Table[2][0] *= pFactor->m_Z;
    pR->m_Table[0][1] *= pFactor->m_X; pR->m_Table[1][1] *= pFactor->m_Y; pR->m_Table[2][1] *= pFactor->m_Z;
    pR->m_Table[0][2] *= pFactor->m_X; pR->m_Table[1][2] *= pFactor->m_Y; pR->m_Table[2][2] *= pFactor->m_Z;
    pR->m_Table[0][3] *= pFactor->m_X; pR->m_Table[1][3] *= pFactor->m_Y; pR->m_Table[2][3] *= pFactor->m_Z;
}
//----------------------------------------------------------------------------
void miniMatrixMultiply(const MINI_Matrix* pM1, const MINI_Matrix* pM2, MINI_Matrix* pR)
{
    int i;
    int j;

    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            pR->m_Table[i][j] = pM1->m_Table[i][0] * pM2->m_Table[0][j] +
                                pM1->m_Table[i][1] * pM2->m_Table[1][j] +
                                pM1->m_Table[i][2] * pM2->m_Table[2][j] +
                                pM1->m_Table[i][3] * pM2->m_Table[3][j];
}
//----------------------------------------------------------------------------
void miniInverse(const MINI_Matrix* pM, MINI_Matrix* pR, float* pDeterminant)
{
    float invDet;
    float t[3];
    float v[16];
    int   i;
    int   j;

    t[0] = pM->m_Table[2][2] * pM->m_Table[3][3] - pM->m_Table[2][3] * pM->m_Table[3][2];
    t[1] = pM->m_Table[1][2] * pM->m_Table[3][3] - pM->m_Table[1][3] * pM->m_Table[3][2];
    t[2] = pM->m_Table[1][2] * pM->m_Table[2][3] - pM->m_Table[1][3] * pM->m_Table[2][2];

    v[0] =  pM->m_Table[1][1] * t[0] - pM->m_Table[2][1] * t[1] + pM->m_Table[3][1] * t[2];
    v[4] = -pM->m_Table[1][0] * t[0] + pM->m_Table[2][0] * t[1] - pM->m_Table[3][0] * t[2];

    t[0] =  pM->m_Table[1][0] * pM->m_Table[2][1] - pM->m_Table[2][0] * pM->m_Table[1][1];
    t[1] =  pM->m_Table[1][0] * pM->m_Table[3][1] - pM->m_Table[3][0] * pM->m_Table[1][1];
    t[2] =  pM->m_Table[2][0] * pM->m_Table[3][1] - pM->m_Table[3][0] * pM->m_Table[2][1];

    v[8]  =  pM->m_Table[3][3] * t[0] - pM->m_Table[2][3] * t[1] + pM->m_Table[1][3] * t[2];
    v[12] = -pM->m_Table[3][2] * t[0] + pM->m_Table[2][2] * t[1] - pM->m_Table[1][2] * t[2];

    *pDeterminant = pM->m_Table[0][0] * v[0] +
                    pM->m_Table[0][1] * v[4] +
                    pM->m_Table[0][2] * v[8] +
                    pM->m_Table[0][3] * v[12];

    if (*pDeterminant == 0.0)
        return;

    t[0] = pM->m_Table[2][2] * pM->m_Table[3][3] - pM->m_Table[2][3] * pM->m_Table[3][2];
    t[1] = pM->m_Table[0][2] * pM->m_Table[3][3] - pM->m_Table[0][3] * pM->m_Table[3][2];
    t[2] = pM->m_Table[0][2] * pM->m_Table[2][3] - pM->m_Table[0][3] * pM->m_Table[2][2];

    v[1] = -pM->m_Table[0][1] * t[0] + pM->m_Table[2][1] * t[1] - pM->m_Table[3][1] * t[2];
    v[5] =  pM->m_Table[0][0] * t[0] - pM->m_Table[2][0] * t[1] + pM->m_Table[3][0] * t[2];

    t[0] = pM->m_Table[0][0] * pM->m_Table[2][1] - pM->m_Table[2][0] * pM->m_Table[0][1];
    t[1] = pM->m_Table[3][0] * pM->m_Table[0][1] - pM->m_Table[0][0] * pM->m_Table[3][1];
    t[2] = pM->m_Table[2][0] * pM->m_Table[3][1] - pM->m_Table[3][0] * pM->m_Table[2][1];

    v[9]  = -pM->m_Table[3][3] * t[0] - pM->m_Table[2][3] * t[1] - pM->m_Table[0][3] * t[2];
    v[13] =  pM->m_Table[3][2] * t[0] + pM->m_Table[2][2] * t[1] + pM->m_Table[0][2] * t[2];

    t[0] = pM->m_Table[1][2] * pM->m_Table[3][3] - pM->m_Table[1][3] * pM->m_Table[3][2];
    t[1] = pM->m_Table[0][2] * pM->m_Table[3][3] - pM->m_Table[0][3] * pM->m_Table[3][2];
    t[2] = pM->m_Table[0][2] * pM->m_Table[1][3] - pM->m_Table[0][3] * pM->m_Table[1][2];

    v[2] =  pM->m_Table[0][1] * t[0] - pM->m_Table[1][1] * t[1] + pM->m_Table[3][1] * t[2];
    v[6] = -pM->m_Table[0][0] * t[0] + pM->m_Table[1][0] * t[1] - pM->m_Table[3][0] * t[2];

    t[0] = pM->m_Table[0][0] * pM->m_Table[1][1] - pM->m_Table[1][0] * pM->m_Table[0][1];
    t[1] = pM->m_Table[3][0] * pM->m_Table[0][1] - pM->m_Table[0][0] * pM->m_Table[3][1];
    t[2] = pM->m_Table[1][0] * pM->m_Table[3][1] - pM->m_Table[3][0] * pM->m_Table[1][1];

    v[10] =  pM->m_Table[3][3] * t[0] + pM->m_Table[1][3] * t[1] + pM->m_Table[0][3] * t[2];
    v[14] = -pM->m_Table[3][2] * t[0] - pM->m_Table[1][2] * t[1] - pM->m_Table[0][2] * t[2];

    t[0] = pM->m_Table[1][2] * pM->m_Table[2][3] - pM->m_Table[1][3] * pM->m_Table[2][2];
    t[1] = pM->m_Table[0][2] * pM->m_Table[2][3] - pM->m_Table[0][3] * pM->m_Table[2][2];
    t[2] = pM->m_Table[0][2] * pM->m_Table[1][3] - pM->m_Table[0][3] * pM->m_Table[1][2];

    v[3] = -pM->m_Table[0][1] * t[0] + pM->m_Table[1][1] * t[1] - pM->m_Table[2][1] * t[2];
    v[7] =  pM->m_Table[0][0] * t[0] - pM->m_Table[1][0] * t[1] + pM->m_Table[2][0] * t[2];

    v[11] = -pM->m_Table[0][0] * (pM->m_Table[1][1] * pM->m_Table[2][3] - pM->m_Table[1][3] * pM->m_Table[2][1]) +
             pM->m_Table[1][0] * (pM->m_Table[0][1] * pM->m_Table[2][3] - pM->m_Table[0][3] * pM->m_Table[2][1]) -
             pM->m_Table[2][0] * (pM->m_Table[0][1] * pM->m_Table[1][3] - pM->m_Table[0][3] * pM->m_Table[1][1]);

    v[15] = pM->m_Table[0][0] * (pM->m_Table[1][1] * pM->m_Table[2][2] - pM->m_Table[1][2] * pM->m_Table[2][1]) -
            pM->m_Table[1][0] * (pM->m_Table[0][1] * pM->m_Table[2][2] - pM->m_Table[0][2] * pM->m_Table[2][1]) +
            pM->m_Table[2][0] * (pM->m_Table[0][1] * pM->m_Table[1][2] - pM->m_Table[0][2] * pM->m_Table[1][1]);

    invDet = 1.0 / *pDeterminant;

    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            pR->m_Table[i][j] = v[4 * i + j] * invDet;
}
//----------------------------------------------------------------------------
void miniApplyMatrixToVector(const MINI_Matrix* pM, const MINI_Vector3* pV, MINI_Vector3* pR)
{
    pR->m_X = (pV->m_X * pM->m_Table[0][0] + pV->m_Y * pM->m_Table[1][0] + pV->m_Z * pM->m_Table[2][0] + pM->m_Table[3][0]);
    pR->m_Y = (pV->m_X * pM->m_Table[0][1] + pV->m_Y * pM->m_Table[1][1] + pV->m_Z * pM->m_Table[2][1] + pM->m_Table[3][1]);
    pR->m_Z = (pV->m_X * pM->m_Table[0][2] + pV->m_Y * pM->m_Table[1][2] + pV->m_Z * pM->m_Table[2][2] + pM->m_Table[3][2]);
}
//----------------------------------------------------------------------------
void miniApplyMatrixToNormal(const MINI_Matrix* pM, const MINI_Vector3* pN, MINI_Vector3* pR)
{
    pR->m_X = (pN->m_X * pM->m_Table[0][0] + pN->m_Y * pM->m_Table[1][0] + pN->m_Z * pM->m_Table[2][0]);
    pR->m_Y = (pN->m_X * pM->m_Table[0][1] + pN->m_Y * pM->m_Table[1][1] + pN->m_Z * pM->m_Table[2][1]);
    pR->m_Z = (pN->m_X * pM->m_Table[0][2] + pN->m_Y * pM->m_Table[1][2] + pN->m_Z * pM->m_Table[2][2]);
}
//----------------------------------------------------------------------------
void miniUnproject(const MINI_Matrix*  pProj,
                   const MINI_Matrix*  pView,
                         MINI_Vector3* pRayPos,
                         MINI_Vector3* pRayDir)
{
    float        determinant;
    MINI_Matrix  invertProj;
    MINI_Matrix  invertView;
    MINI_Matrix  unprojectMat;
    MINI_Vector3 unprojRayPos;
    MINI_Vector3 unprojRayDir;

    // unproject the ray to make it in the viewport coordinates
    miniInverse(pProj, &invertProj, &determinant);
    miniInverse(pView, &invertView, &determinant);
    miniMatrixMultiply(&invertProj, &invertView, &unprojectMat);
    miniApplyMatrixToVector(&unprojectMat, pRayPos, &unprojRayPos);
    miniApplyMatrixToVector(&unprojectMat, pRayDir, &unprojRayDir);

    // copy resulting ray position
    pRayPos->m_X = unprojRayPos.m_X;
    pRayPos->m_Y = unprojRayPos.m_Y;
    pRayPos->m_Z = unprojRayPos.m_Z;

    // normalize and copy resulting ray direction
    miniNormalize(&unprojRayDir, pRayDir);
}
//----------------------------------------------------------------------------
// Quaternion functions
//----------------------------------------------------------------------------
void miniQuatFromAxis(float* pAngle, const MINI_Vector3* pAxis, MINI_Quaternion* pR)
{
    const float s = sinf(*pAngle * 0.5f);

    pR->m_X = (pAxis->m_X * s);
    pR->m_Y = (pAxis->m_Y * s);
    pR->m_Z = (pAxis->m_Z * s);
    pR->m_W =  cosf(*pAngle * 0.5f);
}
//----------------------------------------------------------------------------
void miniQuatFromEuler(float* pAngleX, float* pAngleY, float* pAngleZ, MINI_Quaternion* pR)
{
    const float cos_x_2 = cosf(0.5f * *pAngleX);
    const float cos_y_2 = cosf(0.5f * *pAngleY);
    const float cos_z_2 = cosf(0.5f * *pAngleZ);

    const float sin_x_2 = sinf(0.5f * *pAngleX);
    const float sin_y_2 = sinf(0.5f * *pAngleY);
    const float sin_z_2 = sinf(0.5f * *pAngleZ);

    // create the quaternion
    pR->m_X = cos_z_2 * cos_y_2 * sin_x_2 - sin_z_2 * sin_y_2 * cos_x_2;
    pR->m_Y = cos_z_2 * sin_y_2 * cos_x_2 + sin_z_2 * cos_y_2 * sin_x_2;
    pR->m_Z = sin_z_2 * cos_y_2 * cos_x_2 - cos_z_2 * sin_y_2 * sin_x_2;
    pR->m_W = cos_z_2 * cos_y_2 * cos_x_2 + sin_z_2 * sin_y_2 * sin_x_2;
}
//----------------------------------------------------------------------------
void miniQuatLengthSquared(const MINI_Quaternion* pQ, float* pR)
{
    *pR = ((pQ->m_X * pQ->m_X) +
           (pQ->m_Y * pQ->m_Y) +
           (pQ->m_Z * pQ->m_Z) +
           (pQ->m_W * pQ->m_W));
}
//----------------------------------------------------------------------------
void miniQuatLength(const MINI_Quaternion* pQ, float* pR)
{
    float norm;
    miniQuatLengthSquared(pQ, &norm);

    *pR = sqrt(norm);
}
//----------------------------------------------------------------------------
void miniQuatNormalize(const MINI_Quaternion* pQ, MINI_Quaternion* pR)
{
    float len;
    miniQuatLength(pQ, &len);

    if (len == 0.0f)
    {
        pR->m_X = 0.0f;
        pR->m_Y = 0.0f;
        pR->m_Z = 0.0f;
        pR->m_W = 0.0f;
        return;
    }

    pR->m_X = (pR->m_X / len);
    pR->m_Y = (pR->m_Y / len);
    pR->m_Z = (pR->m_Z / len);
    pR->m_W = (pR->m_W / len);
}
//----------------------------------------------------------------------------
void miniQuatDot(const MINI_Quaternion* pQ1, const MINI_Quaternion* pQ2, float* pR)
{
    *pR = ((pQ1->m_X * pQ2->m_X) +
           (pQ1->m_Y * pQ2->m_Y) +
           (pQ1->m_Z * pQ2->m_Z) +
           (pQ1->m_W * pQ2->m_W));
}
//----------------------------------------------------------------------------
void miniConjugate(const MINI_Quaternion* pQ, MINI_Quaternion* pR)
{
    pR->m_X = -pQ->m_X;
    pR->m_Y = -pQ->m_Y;
    pR->m_Z = -pQ->m_Z;
    pR->m_W =  pQ->m_W;
}
//----------------------------------------------------------------------------
void miniGetMatrix(const MINI_Quaternion* pQ, MINI_Matrix* pR)
{
    pR->m_Table[0][0] = 1.0f - 2.0f * (pQ->m_Y * pQ->m_Y + pQ->m_Z * pQ->m_Z); pR->m_Table[0][1] =        2.0f * (pQ->m_X * pQ->m_Y - pQ->m_Z * pQ->m_W); pR->m_Table[0][2] =        2.0f * (pQ->m_X * pQ->m_Z + pQ->m_Y * pQ->m_W); pR->m_Table[0][3] = 0.0f;
    pR->m_Table[1][0] =        2.0f * (pQ->m_X * pQ->m_Y + pQ->m_Z * pQ->m_W); pR->m_Table[1][1] = 1.0f - 2.0f * (pQ->m_X * pQ->m_X + pQ->m_Z * pQ->m_Z); pR->m_Table[1][2] =        2.0f * (pQ->m_Y * pQ->m_Z - pQ->m_X * pQ->m_W); pR->m_Table[1][3] = 0.0f;
    pR->m_Table[2][0] =        2.0f * (pQ->m_X * pQ->m_Z - pQ->m_Y * pQ->m_W); pR->m_Table[2][1] =        2.0f * (pQ->m_Y * pQ->m_Z + pQ->m_X * pQ->m_W); pR->m_Table[2][2] = 1.0f - 2.0f * (pQ->m_X * pQ->m_X + pQ->m_Y * pQ->m_Y); pR->m_Table[2][3] = 0.0f;
    pR->m_Table[3][0] = 0.0f;                                                  pR->m_Table[3][1] = 0.0f;                                                  pR->m_Table[3][2] = 0.0f;                                                  pR->m_Table[3][3] = 1.0f;
}
//----------------------------------------------------------------------------
