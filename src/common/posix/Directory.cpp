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

#include "posix/Directory.h"

/*************************************************************************/
/* Posix::Directory                                                      */
/*************************************************************************/
int Posix::Directory::Create( const char* path, mode_t mode )
{
    if( 0 != ::mkdir( path, mode ) )
        return errno;

    return 0;
}

int Posix::Directory::Remove( const char* path )
{
    if( 0 != ::rmdir( path ) )
        return errno;

    return 0;
}

Posix::Directory::Directory()
: mDir( NULL )
{
}

Posix::Directory::Directory( const char* name )
: mDir( NULL )
{
    int code = Open( name );
    assert( 0 == code );
}

Posix::Directory::~Directory()
{
    int code = Close();
    assert( 0 == code );
}

long int Posix::Directory::tell() const
{
    return ::telldir( mDir );
}

int Posix::Directory::Open( const char* name )
{
    int code = Close();
    if( 0 != code )
        return code;

    mDir = ::opendir( name );
    if( !isValid() )
        return errno;

    return 0;
}

int Posix::Directory::Close()
{
    if( isValid() )
    {
        int code = ::closedir( mDir );
        if( 0 != code )
            return errno;
    }

    mDir = NULL;
    return 0;
}

int Posix::Directory::Read( dirent& entry, dirent*& result )
{
    return ::readdir_r( mDir, &entry, &result );
}

void Posix::Directory::Seek( long int loc )
{
    ::seekdir( mDir, loc );
}

void Posix::Directory::Rewind()
{
    ::rewinddir( mDir );
}