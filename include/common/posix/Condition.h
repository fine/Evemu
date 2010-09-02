/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of EVEmu: EVE Online Server Emulator
    Copyright 2006 - 2008 The EVEmu Team
    For the latest information visit http://evemu.mmoforge.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
    Author:     Bloody.Rabbit
*/

#ifndef __POSIX__CONDITION_H__INCL__
#define __POSIX__CONDITION_H__INCL__

#include "posix/Mutex.h"

namespace Posix
{
    /**
     * @brief A wrapper around <code>pthread_cond_t</code>.
     *
     * @author Bloody.Rabbit
     */
    class Condition
    {
    public:
        class Attribute;

        /// A default attribute used for new conditions.
        static const Attribute DEFAULT_ATTRIBUTE;

        /**
         * @brief A primary constructor.
         *
         * @param[in] attr An attribute for the condition.
         */
        Condition( const Attribute& attr = DEFAULT_ATTRIBUTE );
        /// A destructor.
        ~Condition();

        /**
         * @brief Signals the condition.
         *
         * @return A value returned by <code>pthread_cond_signal</code>.
         */
        int Signal();
        /**
         * @brief Broadcasts the condition.
         *
         * @return A value returned by <code>pthread_cond_broadcast</code>.
         */
        int Broadcast();

        /**
         * @brief Waits for the condition.
         *
         * @param[in] mutex The mutex this condition is bound to.
         *
         * @return A value returned by <code>pthread_cond_wait</code>.
         */
        int Wait( Mutex& mutex );
        /**
         * @brief Waits for the condition with timeout.
         *
         * @param[in] mutex The mutex this condition is bound to.
         * @param[in] time  Time until which to wait.
         *
         * @return A value returned by <code>pthread_cond_timedwait</code>.
         */
        int TimedWait( Mutex& mutex, const timespec* time );

    protected:
        /// The condition variable itself.
        pthread_cond_t mCondition;
    };
}

#endif /* !__POSIX__CONDITION_H__INCL__ */
