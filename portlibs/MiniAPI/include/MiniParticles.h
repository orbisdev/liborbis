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

#ifndef MiniParticlesH
#define MiniParticlesH

// mini API
#include "MiniGeometry.h"

//----------------------------------------------------------------------------
// Structures
//----------------------------------------------------------------------------

/**
* Particle
*/
typedef struct
{
    MINI_Vector3 m_Position;
    MINI_Vector3 m_Direction;
    MINI_Vector3 m_Velocity;
} MINI_Particle;

/**
* Particles system
*/
typedef struct
{
    MINI_Particle* m_pParticles;
    unsigned       m_Count;
} MINI_Particles;

#ifdef __cplusplus
    extern "C"
    {
#endif

        //----------------------------------------------------------------------------
        // Particles functions
        //----------------------------------------------------------------------------

        /**
        * Emits a new particle
        *@param pParticles - particle system in which the new particle will be emitted
        *@param pStartPos - particle start position
        *@param pStartDir - particle start direction
        *@param pStartVelocity - particle start velocity
        *@param count - max particles the system contains
        */
        MINI_Particle* miniEmitParticle(MINI_Particles* pParticles,
                                        MINI_Vector3*   pStartPos,
                                        MINI_Vector3*   pStartDir,
                                        MINI_Vector3*   pStartVelocity,
                                        unsigned        count);

        /**
        * Moves a particle
        *@param pParticle - particle to move
        *@param frameCount - number of frames to apply since the last rendering
        */
        void miniMoveParticle(MINI_Particle* pParticle, float frameCount);

        /**
        * Deletes a particle
        *@param pParticles - particle system in which the particle shou;d be deleted
        *@param index - index of the particle to delete
        */
        void miniDeleteParticle(MINI_Particles* pParticles, unsigned index);

        /**
        * Clears all particles
        *@param pParticles - particle system to clear
        */
        void miniClearParticles(MINI_Particles* pParticles);

#ifdef __cplusplus
    }
#endif

//----------------------------------------------------------------------------
// Compiler
//----------------------------------------------------------------------------

// needed in mobile c compiler to link the .h file with the .c
#if defined(_OS_IOS_) || defined(_OS_ANDROID_) || defined(_OS_WINDOWS_)
    #include "MiniParticles.c"
#endif

#endif // MiniParticlesH
