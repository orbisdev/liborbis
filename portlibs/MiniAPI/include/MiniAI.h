/*****************************************************************************
 * ==> MiniAI ---------------------------------------------------------------*
 *****************************************************************************
 * Description : This module provides the functions required for artificial  *
 *               intelligence                                                *
 * Developer   : Jean-Milost Reymond                                         *
 * Copyright   : 2015 - 2018, this file is part of the Minimal API. You are  *
 *               free to copy or redistribute this file, modify it, or use   *
 *               it for your own projects, commercial or not. This file is   *
 *               provided "as is", without ANY WARRANTY OF ANY KIND          *
 *****************************************************************************/

#ifndef MiniAIH
#define MiniAIH

// mini api
#include "MiniCommon.h"

//----------------------------------------------------------------------------
// Enumerators
//----------------------------------------------------------------------------

/**
* Artificial intelligence task enumeration
*/
typedef enum
{
    E_BT_Watching,
    E_BT_Searching,
    E_BT_Attacking,
    E_BT_Being_Hurt,
    E_BT_Dying
}
MINI_EBotTask;

//-----------------------------------------------------------------------------
// Structures
//-----------------------------------------------------------------------------

/**
* Item for artificial intelligence task manager
*/
typedef struct
{
    MINI_EBotTask m_Task;
    void*         m_pData;
}
MINI_BotItem;

//----------------------------------------------------------------------------
// Callbacks
//----------------------------------------------------------------------------

/**
* Called while bot is watching
*@param elapsedTime - elapsed time since last task was processed
*@param pItem - running task item
*@return 1 if task is done, otherwise 0
*/
typedef int (*ITfOnWatching)(float elapsedTime, MINI_BotItem* pItem);

/**
* Called while bot is searching
*@param elapsedTime - elapsed time since last task was processed
*@param pItem - running task item
*@return 1 if task is done, otherwise 0
*/
typedef int (*ITfOnSearching)(float elapsedTime, MINI_BotItem* pItem);

/**
* Called while bot is attacking
*@param elapsedTime - elapsed time since last task was processed
*@param pItem - running task item
*@return 1 if task is done, otherwise 0
*/
typedef int (*ITfOnAttacking)(float elapsedTime, MINI_BotItem* pItem);

/**
* Called while bot is being hurt
*@param elapsedTime - elapsed time since last task was processed
*@param pItem - running task item
*@return 1 if task is done, otherwise 0
*/
typedef int (*ITfOnBeingHurt)(float elapsedTime, MINI_BotItem* pItem);

/**
* Called while bot is dying
*@param elapsedTime - elapsed time since last task was processed
*@param pItem - running task item
*@return 1 if task is done, otherwise 0
*/
typedef int (*ITfOnDying)(float elapsedTime, MINI_BotItem* pItem);

/**
* Called when a new task sould be attributed
*@param elapsedTime - elapsed time since last task was processed
*@param pItem - running task item
*@return 1 if task is done, otherwise 0
*/
typedef void (*ITfOnNewTask)(float elapsedTime, MINI_BotItem* pItem);

#ifdef __cplusplus
    extern "C"
    {
#endif

        //----------------------------------------------------------------------------
        // AI functions
        //----------------------------------------------------------------------------

        /**
        * Execute the AI tasks
        *@param elapsedTime - elapsed time since last task was processed
        *@param pItems - task items
        *@param count - task items count
        *@param fOnWatching - OnWatching callback
        *@param fOnSearching - OnSearching callback
        *@param fOnAttacking - OnAttacking callback
        *@param fOnBeingHurt - OnBeingHurt callback
        *@param fOnDying - OnDying callback
        *@param fOnNewTask - OnNewTask callback
        */
        void miniExecuteTasks(float          elapsedTime,
                              MINI_BotItem*  pItems,
                              unsigned       count,
                              ITfOnWatching  fOnWatching,
                              ITfOnSearching fOnSearching,
                              ITfOnAttacking fOnAttacking,
                              ITfOnBeingHurt fOnBeingHurt,
                              ITfOnDying     fOnDying,
                              ITfOnNewTask   fOnNewTask);

#ifdef __cplusplus
    }
#endif

//----------------------------------------------------------------------------
// Compiler
//----------------------------------------------------------------------------

// needed in mobile c compiler to link the .h file with the .c
#if defined(_OS_IOS_) || defined(_OS_ANDROID_) || defined(_OS_WINDOWS_)
    #include "MiniAI.c"
#endif

#endif
