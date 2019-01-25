/*****************************************************************************
 * ==> MiniCommon -----------------------------------------------------------*
 *****************************************************************************
 * Description : This module provides the common functions and declarations  *
 *               used by the whole API                                       *
 * Developer   : Jean-Milost Reymond                                         *
 * Copyright   : 2015 - 2018, this file is part of the Minimal API. You are  *
 *               free to copy or redistribute this file, modify it, or use   *
 *               it for your own projects, commercial or not. This file is   *
 *               provided "as is", without ANY WARRANTY OF ANY KIND          *
 *****************************************************************************/

#include "MiniCommon.h"

// std
#include <stdio.h>
#include <math.h>

//----------------------------------------------------------------------------
// File functions
//----------------------------------------------------------------------------
long miniGetFileSize(const char* pFileName)
{
    FILE* pFile;
    long  fileSize;

    // open the file
    pFile = fopen(pFileName, "rb");

    // succeeded?
    if (!pFile)
        return 0;

    // measure the file size
    fseek(pFile, 0, SEEK_END);
    fileSize = ftell(pFile);
    fclose(pFile);

    return fileSize;
}
//----------------------------------------------------------------------------
// Math functions
//----------------------------------------------------------------------------
void miniMin(const float* pA, const float* pB, float* pR)
{
    if (*pA < *pB)
    {
        *pR = *pA;
        return;
    }

    *pR = *pB;
}
//----------------------------------------------------------------------------
void miniMax(const float* pA, const float* pB, float* pR)
{
    if (*pA > *pB)
    {
        *pR = *pA;
        return;
    }

    *pR = *pB;
}
//----------------------------------------------------------------------------
int miniValueIsBetween(const float* pV,
                       const float* pS,
                       const float* pE,
                       const float* pTolerance)
{
    float minVal;
    float maxVal;

    miniMin(pS, pE, &minVal);
    miniMax(pS, pE, &maxVal);

    // check if each value is between start and end limits considering tolerance
    if (*pV >= (minVal - *pTolerance) && *pV <= (maxVal + *pTolerance))
        return 1;

    return 0;
}
//----------------------------------------------------------------------------
void miniRound(const float* pValue, float* pR)
{
    if (*pValue < 0.0f)
        *pR = ceil(*pValue - 0.5f);
    else
        *pR = floor(*pValue + 0.5f);
}
//----------------------------------------------------------------------------
void miniRoundToExp(const float* pValue, unsigned exp, float* pR)
{
    const float value = *pValue * pow(10.0f, exp);

    miniRound(&value, pR);

    *pR *= pow(0.1f, exp);
}
//----------------------------------------------------------------------------
