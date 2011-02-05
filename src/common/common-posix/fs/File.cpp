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

#include "CommonPosix.h"

#include "posix/File.h"

using namespace common;
using namespace common::fs;

/*************************************************************************/
/* common::fs::File                                                      */
/*************************************************************************/
bool File::Rename( const char* nameOld, const char* nameNew )
{
    return 0 == posix::File::Rename( nameOld, nameNew );
}

bool File::Remove( const char* name )
{
    return 0 == posix::File::Remove( name );
}

File::File()
: mFile( new posix::File )
{
}

File::File( const char* name, Mode mode )
: mFile( new posix::File )
{
    bool success = Open( name, mode );
    assert( success );
}

File::~File()
{
    SafeDelete( mFile );
}

bool File::isValid() const
{
    return mFile->isValid();
}

bool File::GetSize( size_t& size ) const
{
    struct stat st;
    if( 0 != mFile->Stat( st ) )
        return false;

    size = st.st_size;
    return true;
}

bool File::Open( const char* name, Mode mode )
{
    static const mode_t MODE = S_IRUSR | S_IWUSR
                             | S_IRGRP
                             | S_IROTH;

    int flags = 0;

    if( MODE_ACCESS_READ == ( mode & MODE_ACCESS_READ ) )
    {
        if( MODE_ACCESS_WRITE == ( mode & MODE_ACCESS_WRITE ) )
            flags |= O_RDWR;
        else
            flags |= O_RDONLY;
    }
    else if( MODE_ACCESS_WRITE == ( mode & MODE_ACCESS_WRITE ) )
        flags |= O_WRONLY;

    if( MODE_OPEN_NEW == ( mode & MODE_OPEN_NEW ) )
        flags |= O_CREAT;
    if( MODE_OPEN_EXISTING != ( mode & MODE_OPEN_EXISTING ) )
        flags |= O_EXCL;
    if( MODE_OPEN_TRUNCATE == ( mode & MODE_OPEN_TRUNCATE ) )
        flags |= O_TRUNC;

    return 0 == mFile->Open( name, flags, MODE );
}

bool File::Close()
{
    return 0 == mFile->Close();
}

bool File::Seek( long int offset, Origin origin )
{
    static const int POSIX_ORIGIN[ ORIGIN_COUNT ] =
    {
        SEEK_SET, // ORIGIN_BEGIN
        SEEK_CUR, // ORIGIN_CURRENT
        SEEK_END  // ORIGIN_END
    };

    assert( 0 <= origin && origin < ORIGIN_COUNT );
    return 0 == mFile->Seek( offset, POSIX_ORIGIN[ origin ] );
}

bool File::Flush()
{
    return 0 == mFile->Sync();
}

stream::Error File::Read( util::Data& data, size_t* bytesRead )
{
    return mFile->Read( data, bytesRead );
}

stream::Error File::Write( const util::Data& data, size_t* bytesWritten )
{
    return mFile->Write( data, bytesWritten );
}