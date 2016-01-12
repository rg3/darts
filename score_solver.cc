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
#include <cmath>
#include <map>
#include <sstream>
#include <string>

#include "score_solver.h"
#include "segment.h"

using std::map;
using std::pow;
using std::reverse;
using std::sort;
using std::string;
using std::stringstream;
using std::vector;

namespace {
    // Auxiliar methods and data types.
    template <typename T>
       string to_str(const T &a)
       {
           stringstream aux;
           aux << a;
           return aux.str();
       }

    void generate_segments(
        vector<Segment> &all_segments,
        vector<Segment> &finishers)
    {
        // Constants.
        static const int FIRST = 1;
        static const int LAST = 20;

        vector<Segment> singles;
        vector<Segment> doubles;
        vector<Segment> triples;

        // Single value segments are the easiest ones. Make no distinctions.
        for (int i = LAST; i >= FIRST; --i)
        {
            singles.emplace_back(i, to_str(i), 1);
        }

        // Doubles are next, but they all are much more difficult.
        // Make frequent ones a bit easier.
        doubles.emplace_back(LAST * 2, "D" + to_str(LAST), 50);
        doubles.emplace_back((LAST - 1) * 2,"D" + to_str(LAST - 1), 50);
        for (int i = LAST - 2; i >= FIRST; --i)
        {
            string aux = to_str(i);
            doubles.emplace_back(i * 2, "D" + aux, 51);
        }

        // Triples are next, together with the Ring and the Bull.
        //
        // Make frequent ones easier. Difficulty scores are somewhat arbitrary
        // for this section, but they seem to work well.
        triples.emplace_back(LAST * 3, "T" + to_str(LAST), 100);
        triples.emplace_back((LAST - 1) * 3, "T" + to_str(LAST - 1), 100);
        for (int i = LAST - 2; i >= FIRST; --i)
        {
            string aux = to_str(i);
            triples.emplace_back(i * 3, "T" + aux, 101);
        }
        Segment bull(50, "Bull", 102);
        Segment ring(25, "Ring", 103);

        // Finishers.
        finishers.push_back(bull);
        copy(doubles.begin(), doubles.end(), back_inserter(finishers));

        // All posible scores.
        copy(singles.begin(), singles.end(), back_inserter(all_segments));
        copy(doubles.begin(), doubles.end(), back_inserter(all_segments));
        copy(triples.begin(), triples.end(), back_inserter(all_segments));
        all_segments.push_back(bull);
        all_segments.push_back(ring);
    }
}

ScoreSolver::ScoreSolver()
{
}

vector<Segment>
ScoreSolver::solve(int score, int darts)
{
    // Data needed for a recursive call.
    vector<Segment> all_segments;
    vector<Segment> finishers;
    Solution partial;
    vector<Solution> all_solutions;

    // Generate all dartboard segments, in two groups.
    generate_segments(all_segments, finishers);

    // Find all solutions to the given score and number of darts.
    solve_using(score, darts, finishers, all_segments, partial, all_solutions);

    // Cannot be solved.
    if (all_solutions.size() == 0)
        return vector<Segment>();

    // Sort them by difficulty and return the easiest solution.
    //
    // Note the solution is reversed due to the finishing dart being first in
    // the list. Hence the need to reverse the segments before returning.
    sort(all_solutions.begin(), all_solutions.end());
    auto ret = all_solutions.front().get_segments();
    reverse(ret.begin(), ret.end());
    return ret;
}

ScoreSolver::Solution::Solution()
    : difficulty(0.0f), segments()
{
    //recalculate_difficulty();
}

ScoreSolver::Solution::Solution(const Solution &other)
    : difficulty(other.difficulty), segments(other.segments)
{
}

ScoreSolver::Solution::Solution(Solution &&other)
    : Solution()
{
    swap(other);
}

ScoreSolver::Solution::~Solution()
{
}

void
ScoreSolver::Solution::swap(Solution &other)
{
    std::swap(difficulty, other.difficulty);
    segments.swap(other.segments);
}

const ScoreSolver::Solution &
ScoreSolver::Solution::operator=(Solution other)
{
    swap(other);
    return (*this);
}

ScoreSolver::Solution
ScoreSolver::Solution::combine(const Segment &s)
{
    Solution ret(*this);
    ret.push(s);
    return ret;
}

void
ScoreSolver::Solution::push(const Segment &s)
{
    segments.push_back(s);
    recalculate_difficulty();
}

void
ScoreSolver::Solution::pop()
{
    if (empty())
        return;
    segments.pop_back();
    recalculate_difficulty();
}

bool
ScoreSolver::Solution::empty() const
{
    return segments.empty();
}

bool
ScoreSolver::Solution::operator<(const Solution &other) const
{
    return (difficulty < other.difficulty);
}

vector<Segment>
ScoreSolver::Solution::get_segments() const
{
    return segments;
}

// The difficulty of a given solution is the added difficulty score of each
// segment, but this score can be increased moderately if we already have
// other darts in the segment, as hitting it again with a new dart can be more
// complicated.
void ScoreSolver::Solution::recalculate_difficulty()
{
    static const float BASE = 1.2f;

    map<string, int> counters;
    float total = 0.0f;
    int c;

    for (auto &i : segments)
    {
        string name = i.get_name();
        if (counters.count(name) == 0)
            c = 0;
        else
            c = counters[name];

        // The difficulty of a given segment is multiplied by 1.2^d, where
        // d is the number of darts already in that segment. This formula
        // helps prioritize plays that don't repeat segments and works well
        // with the difficulty scores we have chosen.
        total += i.get_difficulty() * pow(BASE, static_cast<float>(c));
        counters[name] = c + 1;
    }

    difficulty = total;
}

void
ScoreSolver::solve_using(int score, int darts,
                         const vector<Segment> &current_segments,
                         const vector<Segment> &next_segments,
                         Solution &partial,
                         vector<Solution> &all_solutions)
{
    // No solution.
    if (darts <= 0 || score <= 0)
        return;

    // Base case: last dart. Find out if a segment matches the required score.
    if (darts == 1)
    {
        auto i = find(current_segments.begin(), current_segments.end(), score);
        if (i == current_segments.end())
            return;

        all_solutions.emplace_back(partial.combine(*i));
        return;
    }

    // Recursive case: try every possible segment.
    for (auto &i : current_segments)
    {
        if (i > score)
            continue;

        // Exact match.
        if (i == score)
        {
            all_solutions.emplace_back(partial.combine(i));
        }

        // Try one segment with a recursive call.
        partial.push(i);
        solve_using(score - i, darts - 1,
                    next_segments, next_segments,
                    partial, all_solutions);
        partial.pop();
    }
}
