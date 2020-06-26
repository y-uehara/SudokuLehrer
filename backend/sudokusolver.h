#ifndef SUDOKUSOLVER_H
#define SUDOKUSOLVER_H

#include <utility>


class SudokuSolver
{
public:
    SudokuSolver();

    static const int SudokuTuples[27][9];

    enum ResultType
    {
        FOUND,
        NOT_FOUND,
        CONTRADICT
    };


    struct SudokuSolverResult
    {
        ResultType result;
        int x;
        int y;
        int num;
    };

    SudokuSolverResult solve(int *board);

private:
    void removeCandidate(int num, std::pair<int, int>p, int *candidates);

    SudokuSolverResult solve_simple(const int *board, int *candidates);
    SudokuSolverResult solve_heuristic(const int *board, int *candidates);
    SudokuSolverResult solve_negation(const int *board, int *candidates);

    bool solve_heuristic_iter(int *candidates, int combiLength, int *combi);

    int countBit(unsigned int c, unsigned int digit);
    int createCombinationArray(int **array, unsigned int n, unsigned int r);
    void dump(int *candidates);

};



#endif // SUDOKUSOLVER_H
