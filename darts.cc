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
#include <iostream>
#include <sstream>

#include "segment.h"
#include "score_solver.h"

using std::cerr;
using std::cout;
using std::endl;
using std::stringstream;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " SCORE NUMBER_OF_DARTS" << endl;
        return 1;
    }

    int score;
    int darts;

    stringstream score_str(argv[1]);
    stringstream darts_str(argv[2]);

    score_str >> score;
    darts_str >> darts;

    if (score_str.fail() || darts_str.fail())
    {
        cerr << "Error: bad score or number of darts" << endl;
        return 2;
    }

    ScoreSolver solver;
    auto solution = solver.solve(score, darts);

    if (solution.size() == 0)
    {
        cout << "No checkout" << endl;
    }
    else
    {
        bool first = true;
	for (auto &i : solution)
        {
            cout << (first?"":" ") << i.get_name();
            first = false;
        }
        cout << endl;
    }

    return 0;
}
