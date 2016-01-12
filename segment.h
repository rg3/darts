#ifndef SEGMENT_HEADER_
#define SEGMENT_HEADER_
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

#include <string>

// Dartboard segment. It has a value in points, a unique name in the dartboard
// ("D19", "T20", etc) and a difficulty score. For convenience, segments can be
// cast to integers and the resulting value is the segment value.

class Segment
{
public:
    Segment(int value, const std::string &name, int difficulty);
    Segment(const Segment &other);
    Segment(Segment &&other);
    ~Segment();
    const Segment &operator=(Segment other);
    void swap(Segment &other);
    std::string get_name() const;
    int get_difficulty() const;
    operator int() const;

private:
    Segment();
    int value;
    int difficulty;
    std::string name;
};

#endif
