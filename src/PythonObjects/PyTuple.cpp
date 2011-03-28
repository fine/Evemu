/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of EVEmu: EVE Online Server Emulator
    Copyright 2006 - 2009 The EVEmu Team
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
    Author:     Captnoord
*/
#include "EvemuPCH.h"

#include "Common.h"
#include "NGLog.h"
#include "Log.h"

#include "PyObjects.h"
#include "PyChameleon.h"
#include "PyTuple.h"

/************************************************************************/
/* PyTuple                                                              */
/************************************************************************/
PyTuple::PyTuple() : mType(PyTypeTuple), mRefcnt(1), mHash(&PyTuple::_hash)
{
}

PyTuple::PyTuple( size_t elementCount ) : mType(PyTypeTuple), mRefcnt(1), mHash(&PyTuple::_hash)
{
    if (elementCount > PY_TUPLE_ELEMENT_MAX)
    {
        Log.Error("PyTuple", "constructor is requested to allocate a stupid amount of elements: %d", elementCount);
        ASCENT_ASSERT(false);
        return;
    }

    resize(elementCount);
}

PyTuple::~PyTuple()
{
    clear();
}

/* this is kinda slow because we are resizing a vector */
PyChameleon & PyTuple::operator[]( const int index )
{
    if (index < 0)
        return PyErrorIterator;
    if (index+1 > (int)mTuple.size())
        mTuple.resize(index+1);
    PyChameleon & itr = *mTuple[index];
    return itr;
}

size_t PyTuple::size()
{
    return mTuple.size();
}

void PyTuple::clear()
{
    iterator itr = mTuple.begin();
    for (; itr != mTuple.end(); itr++)
    {
        PyChameleon* obj = *itr;
        SafeDelete(obj);
    }
    mTuple.clear();
}

uint8 PyTuple::gettype()
{
    return mType;
}

void PyTuple::IncRef()
{
    mRefcnt++;
}

void PyTuple::DecRef()
{
    mRefcnt--;
    if (mRefcnt <= 0)
        PyDelete(this);
}

uint32 PyTuple::hash()
{
    return (this->*mHash)();
}

uint32 PyTuple::_hash()
{
    PyTuple & tuple = *(PyTuple*)this;
    uint32 hash = 5381;
    for (int i = 0; i < (int)tuple.size(); i++)
    {
        uint32 hashChunk = PyObject_Hash(tuple[i].getPyObject());
        hash = (hash << 3) + hashChunk;
    }
    return hash;
}

/* quite tricky */
bool PyTuple::resize( size_t newTupleSize )
{
    size_t currentTupleSize = mTuple.size();

    /* if we are required to resize to our current size, don't do shit*/
    if (newTupleSize == currentTupleSize)
        return true;

    /* we need to grow or shrink */
    if (newTupleSize > currentTupleSize)
    {
        // debug breakpoint for the situation we are resizing a already existing object
        if (currentTupleSize != 0)
            ASCENT_HARDWARE_BREAKPOINT;

        /* we need to grow (this one is easy) */
        mTuple.resize(newTupleSize);

        for (size_t i = currentTupleSize; i < newTupleSize; i++)
        {
            mTuple[int(i)] = new PyChameleon();
        }
    }
    else
    {
        ASCENT_HARDWARE_BREAKPOINT;// here because we need to evaluate its workings.
        /* we need to shrink (a bit harder) */
        for(size_t i = currentTupleSize; i > newTupleSize; i--)
        {
            PyChameleon* obj = mTuple[int(i)];
            SafeDelete(obj);
        }
        mTuple.resize(newTupleSize);
    }
    return true;
}

PyObject* PyTuple::GetItem( const int index )
{
    if (index < 0)
        return NULL;
    if (index+1 > (int)mTuple.size())
        mTuple.resize(index+1);

    PyChameleon * itr = mTuple[index];
    return itr->getPyObject();
}

int PyTuple::GetInt( const int index )
{
    PyChameleon * itr = mTuple[index];
    PyObject * object = itr->getPyObject();
    if (object->gettype() != PyTypeInt)
    {
        Log.Error("PyTuple", "GetInt: trying to get int from a not int location");
        return 0;
    }

    PyInt * intobj = (PyInt*)object;
    return intobj->GetValue();
}

double PyTuple::GetFloat( const int index )
{
    PyChameleon * itr = mTuple[index];
    PyObject * object = itr->getPyObject();
    if (object->gettype() != PyTypeReal)
    {
        Log.Error("PyTuple", "GetFloat: trying to get float from a not float location");
        return 0;
    }

    PyFloat * floatobj = (PyFloat*)object;
    return floatobj->GetValue();

}

std::string PyTuple::GetString( const int index )
{
    PyChameleon * itr = mTuple[index];
    PyObject * object = itr->getPyObject();
    if (object->gettype() != PyTypeString)
    {
        Log.Error("PyTuple", "GetString: trying to get string from a not string location");
        return 0;
    }

    PyString * strobj = (PyString*)object;
    std::string str = strobj->content();
    return str;
}

bool PyTuple::GetString( const int index, std::string& rStr )
{
    if (index > (int)mTuple.size())
        return false;

    PyChameleon * itr = mTuple[index];
    PyObject * object = itr->getPyObject();

    if (object->gettype() != PyTypeString)
        return false;

    PyString * strobj = (PyString*)object;

    rStr.clear(); // make sure its empty
    rStr.append(strobj->content(), strobj->length());

    return true;
}

template<typename T>
void PyTuple::set_item(const int index, T* pObject)
{
    if (index+1 > (int)mTuple.size())
        mTuple.resize(index+1);
    PyChameleon * itr = mTuple[index];
    itr->setPyObject((PyObject*)pObject);
}


