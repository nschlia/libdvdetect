/*
  dvdetect DVD detection, analysis & DVDETECT lookup library

  Copyright (C) 2013-2015 Norbert Schlia <nschlia@dvdetect.de>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU LESSER GENERAL PUBLIC LICENSE for more details.

  You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*! \file vector_ptr.h
 *
 *  \brief
 *
 * Humble replacement for C++11 std::shared_ptr
 */

#pragma once

#include <vector>

#ifndef VECTOR_PTR_H
#define VECTOR_PTR_H

template < typename T, typename Alloc = std::allocator<T*> > class vector_ptr : public std::vector<T*, Alloc>
{
public:
    virtual ~vector_ptr()
    {
        deleteAll();
    }

    void clear()
    {
        deleteAll();

        std::vector<T*, Alloc>::clear();
    }

    vector_ptr<T, Alloc>& operator=(const vector_ptr<T, Alloc>& source)
    {
        copy(source);

        return *this;
    }

    void copy(const vector_ptr<T, Alloc> & source)
    {
        clear();

        this->resize(source.size());

        int n = 0;
        typename vector_ptr<T, Alloc>::const_iterator it;
        for (it = source.begin(); it != source.end(); ++it, n++)
        {
            T *dummy = new T;

            (*dummy) = *(*it);

            this->at(n) = dummy;
        }
    }

protected:
    void deleteAll()
    {
        typename std::vector<T*, Alloc>::iterator it;
        for (it = this->begin(); it != this->end(); ++it)
        {
            delete (*it);
        }
    }
};

#endif // VECTOR_PTR_H
