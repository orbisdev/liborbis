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

#include "MiniAI.h"

//----------------------------------------------------------------------------
// AI functions
//----------------------------------------------------------------------------
void miniExecuteTasks(float          elapsedTime,
                      MINI_BotItem*  pItems,
                      unsigned       count,
                      ITfOnWatching  fOnWatching,
                      ITfOnSearching fOnSearching,
                      ITfOnAttacking fOnAttacking,
                      ITfOnBeingHurt fOnBeingHurt,
                      ITfOnDying     fOnDying,
                      ITfOnNewTask   fOnNewTask)
{
    int i;
    int taskCompleted;

    if (!pItems)
        return;

    if (!fOnWatching)
        return;

    if (!fOnSearching)
        return;

    if (!fOnAttacking)
        return;

    if (!fOnBeingHurt)
        return;

    if (!fOnDying)
        return;

    if (!fOnNewTask)
        return;

    // iterate through tasks to execute
    for (i = 0; i < count; ++i)
    {
        // dispatch the task and excute it
        switch (pItems[i].m_Task)
        {
            case E_BT_Watching:   taskCompleted = fOnWatching (elapsedTime, &pItems[i]); break;
            case E_BT_Searching:  taskCompleted = fOnSearching(elapsedTime, &pItems[i]); break;
            case E_BT_Attacking:  taskCompleted = fOnAttacking(elapsedTime, &pItems[i]); break;
            case E_BT_Being_Hurt: taskCompleted = fOnBeingHurt(elapsedTime, &pItems[i]); break;
            case E_BT_Dying:      taskCompleted = fOnDying    (elapsedTime, &pItems[i]); break;
            default:                                                                     continue;
        }

        // if task was completed, notify the caller
        if (taskCompleted)
            fOnNewTask(elapsedTime, &pItems[i]);
    }
}
//----------------------------------------------------------------------------
