/*
 * darts - Dart game checkout solver.
 *
 * Written in 2016 by Ricardo Garcia <r@rg3.name>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along
 * with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */
#include <algorithm>

#include "segment.h"

Segment::Segment()
    : value(0), difficulty(0), name()
{
}

Segment::Segment(int v, const std::string &n, int d)
    : value(v), difficulty(d), name(n)
{
}

Segment::Segment(const Segment &other)
    : value(other.value), difficulty(other.difficulty), name(other.name)
{
}

Segment::Segment(Segment &&other)
    : Segment()
{
    swap(other);
}

Segment::~Segment()
{
}

const Segment &
Segment::operator=(Segment other)
{
    swap(other);
    return *this;
}

void
Segment::swap(Segment &other)
{
    std::swap(value, other.value);
    std::swap(difficulty, other.difficulty);
    name.swap(other.name);
}

std::string
Segment::get_name() const
{
    return name;
}

int
Segment::get_difficulty() const
{
    return difficulty;
}

Segment::operator int() const
{
    return value;
}
