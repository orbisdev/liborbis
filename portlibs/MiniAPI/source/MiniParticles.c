/*****************************************************************************
 * ==> MiniParticles --------------------------------------------------------*
 *****************************************************************************
 * Description : This module provides the functions to manage particles      *
 * Developer   : Jean-Milost Reymond                                         *
 * Copyright   : 2015 - 2018, this file is part of the Minimal API. You are  *
 *               free to copy or redistribute this file, modify it, or use   *
 *               it for your own projects, commercial or not. This file is   *
 *               provided "as is", without ANY WARRANTY OF ANY KIND          *
 *****************************************************************************/

#include "MiniParticles.h"

// std
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------
// Particles functions
//----------------------------------------------------------------------------
MINI_Particle* miniEmitParticle(MINI_Particles* pParticles,
                                MINI_Vector3*   pStartPos,
                                MINI_Vector3*   pStartDir,
                                MINI_Vector3*   pStartVelocity,
                                unsigned        count)
{
    unsigned index;

    // no particle system?
    if (!pParticles)
        return 0;

    // is particle system saturated?
    if (pParticles->m_Count >= count)
        return 0;

    // create new particle and add it to system
    if (!pParticles->m_Count)
        pParticles->m_pParticles = (MINI_Particle*)malloc(sizeof(MINI_Particle));
    else
        pParticles->m_pParticles =
                (MINI_Particle*)realloc(pParticles->m_pParticles,
                                       (pParticles->m_Count + 1) * sizeof(MINI_Particle));

    // increase system count
    index = pParticles->m_Count;
    ++pParticles->m_Count;

    // initialize newly created particle
    pParticles->m_pParticles[index].m_Position.m_X  = pStartPos->m_X;
    pParticles->m_pParticles[index].m_Position.m_Y  = pStartPos->m_Y;
    pParticles->m_pParticles[index].m_Position.m_Z  = pStartPos->m_Z;
    pParticles->m_pParticles[index].m_Direction.m_X = pStartDir->m_X;
    pParticles->m_pParticles[index].m_Direction.m_Y = pStartDir->m_Y;
    pParticles->m_pParticles[index].m_Direction.m_Z = pStartDir->m_Z;
    pParticles->m_pParticles[index].m_Velocity.m_X  = pStartVelocity->m_X;
    pParticles->m_pParticles[index].m_Velocity.m_Y  = pStartVelocity->m_Y;
    pParticles->m_pParticles[index].m_Velocity.m_Z  = pStartVelocity->m_Z;

    return &pParticles->m_pParticles[index];
}
//----------------------------------------------------------------------------
void miniMoveParticle(MINI_Particle* pParticle, float frameCount)
{
    // no particle system?
    if (!pParticle)
        return;

    // calculate next particle position
    pParticle->m_Position.m_X += (pParticle->m_Direction.m_X * pParticle->m_Velocity.m_X * frameCount);
    pParticle->m_Position.m_Y += (pParticle->m_Direction.m_Y * pParticle->m_Velocity.m_Y * frameCount);
    pParticle->m_Position.m_Z += (pParticle->m_Direction.m_Z * pParticle->m_Velocity.m_Z * frameCount);
}
//----------------------------------------------------------------------------
void miniDeleteParticle(MINI_Particles* pParticles, unsigned index)
{
    unsigned i;

    // no particle system?
    if (!pParticles)
        return;

    // is particle system empty?
    if (!pParticles->m_Count)
        return;

    // is index out of bounds?
    if (index >= pParticles->m_Count)
        return;

    // move all following particles one position on the left
    if (index != pParticles->m_Count - 1)
        /* unfortunatly memcpy doesn't work for now
        memcpy(pParticles->m_pParticles[index],
               pParticles->m_pParticles[index + 1],
              (pParticles->m_Count - (index + 1)) * sizeof(MINI_Particle));
        */
        for (i = index; i < pParticles->m_Count - 1; ++i)
        {
            pParticles->m_pParticles[i].m_Position.m_X  = pParticles->m_pParticles[i + 1].m_Position.m_X;
            pParticles->m_pParticles[i].m_Position.m_Y  = pParticles->m_pParticles[i + 1].m_Position.m_Y;
            pParticles->m_pParticles[i].m_Position.m_Z  = pParticles->m_pParticles[i + 1].m_Position.m_Z;
            pParticles->m_pParticles[i].m_Direction.m_X = pParticles->m_pParticles[i + 1].m_Direction.m_X;
            pParticles->m_pParticles[i].m_Direction.m_Y = pParticles->m_pParticles[i + 1].m_Direction.m_Y;
            pParticles->m_pParticles[i].m_Direction.m_Z = pParticles->m_pParticles[i + 1].m_Direction.m_Z;
            pParticles->m_pParticles[i].m_Velocity.m_X  = pParticles->m_pParticles[i + 1].m_Velocity.m_X;
            pParticles->m_pParticles[i].m_Velocity.m_Y  = pParticles->m_pParticles[i + 1].m_Velocity.m_Y;
            pParticles->m_pParticles[i].m_Velocity.m_Z  = pParticles->m_pParticles[i + 1].m_Velocity.m_Z;
        }

    // delete last particle
    pParticles->m_pParticles =
            (MINI_Particle*)realloc(pParticles->m_pParticles,
                                   (pParticles->m_Count - 1) * sizeof(MINI_Particle));

    // decrease count
    --pParticles->m_Count;
}
//----------------------------------------------------------------------------
void miniClearParticles(MINI_Particles* pParticles)
{
    // no particle system?
    if (!pParticles)
        return;

    // clear particle system
    free(pParticles->m_pParticles);
    pParticles->m_pParticles = 0;
};
//----------------------------------------------------------------------------
