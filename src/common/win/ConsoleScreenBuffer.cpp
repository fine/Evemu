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

#include "CommonPCH.h"

#include "win/ConsoleScreenBuffer.h"

/*************************************************************************/
/* Win::ConsoleScreenBuffer                                              */
/*************************************************************************/
const Win::ConsoleScreenBuffer Win::ConsoleScreenBuffer::DEFAULT_OUTPUT_SCREEN(
    ::GetStdHandle( STD_OUTPUT_HANDLE )
);

Win::ConsoleScreenBuffer::ConsoleScreenBuffer( HANDLE handle )
: Win::Handle( handle )
{
}

Win::ConsoleScreenBuffer::ConsoleScreenBuffer( DWORD desiredAccess, DWORD shareMode )
: Win::Handle()
{
    BOOL success = Create( desiredAccess, shareMode );
    assert( TRUE == success );
}

BOOL Win::ConsoleScreenBuffer::Create( DWORD desiredAccess, DWORD shareMode )
{
    BOOL success = CloseEx();
    assert( TRUE == success );

    mHandle = ::CreateConsoleScreenBuffer( desiredAccess, shareMode,
                                           NULL, CONSOLE_TEXTMODE_BUFFER, NULL );
    return isValid();
}

BOOL Win::ConsoleScreenBuffer::SetActive()
{
    return ::SetConsoleActiveScreenBuffer( mHandle );
}

BOOL Win::ConsoleScreenBuffer::SetTextAttributes( WORD attributes )
{
    return ::SetConsoleTextAttribute( mHandle, attributes );
}
