#ifndef SCORE_SOLVER_HEADER_
#define SCORE_SOLVER_HEADER_
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

#include <vector>

class Segment;

// The score solver class has a method, solve(), that tries to solve a given
// score with the given number of darts, and returns a vector of segments that
// solve it. If no solution is possible, an empty vector is returned.

class ScoreSolver
{
public:
    ScoreSolver();
    std::vector<Segment> solve(int score, int darts);

private:
    // Auxiliar class that stores a partial or complete solution to a given
    // score. It contains a vector of chosen segments (so far) and a difficulty
    // score that's recalculated every time the solution is modified. Solutions
    // can be sorted by difficulty.
    class Solution
    {
    public:
        Solution();
        Solution(const Solution &other);
	Solution(Solution &&other);
        ~Solution();
        void swap(Solution &other);
        const Solution &operator=(Solution other);

        // Combine the current solution with a given segment, and create a new
        // solution with it.
        Solution combine(const Segment &s);

        // Add a new segment to the solution.
        void push(const Segment &s);

        // Remove the last segment from the solution.
        void pop();

        // Other methods.
        bool empty() const;
        bool operator<(const Solution &other) const;
        std::vector<Segment> get_segments() const;

    private:
        void recalculate_difficulty();

        float difficulty;
        std::vector<Segment> segments;
    };

    // This is the big, recursive, solution finding method that operates by
    // trying to choose a new segment and solve the remaining score with the
    // remaining darts. It's a backtracking strategy.
    //
    // Arguments:
    //
    // score: This is the given score we want to solve at this level.
    //
    // darts: The number of darts we have remaining.
    //
    // current_segments: The list of segments we can choose from in this turn.
    // This helps us choose the finishing dart first from the list of finisher
    // segments, and all other darts from the global set of segments. It also
    // means the finishing dart will be the first one in the solution and we
    // want to reverse the segment order when returning it.
    //
    // next_segments: The list of segments to be used for every other dart
    // starting with the next one. In practice, this is always the full list of
    // all segments.
    //
    // partial: The current partial solution in this level.
    //
    // all_solutions: A list of all found solutions. When one is found, it's
    // pushed back to this vector.
    //
    void solve_using(int score, int darts,
                     const std::vector<Segment> &current_segments,
                     const std::vector<Segment> &next_segments,
                     Solution &partial,
                     std::vector<Solution> &all_solutions);
};

#endif
