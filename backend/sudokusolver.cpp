#include <vector>
#include <cmath>

#include "sudokusolver.h"


const int SudokuSolver::SudokuTuples[27][9] =
{
    // columns
    { 0,  1,  2,  3,  4,  5,  6,  7,  8},
    { 9, 10, 11, 12, 13, 14, 15, 16, 17},
    {18, 19, 20, 21, 22, 23, 24, 25, 26},
    {27, 28, 29, 30, 31, 32, 33, 34, 35},
    {36, 37, 38, 39, 40, 41, 42, 43, 44},
    {45, 46, 47, 48, 49, 50, 51, 52, 53},
    {54, 55, 56, 57, 58, 59, 60, 61, 62},
    {63, 64, 65, 66, 67, 68, 69, 70, 71},
    {72, 73, 74, 75, 76, 77, 78, 79, 80},
    // rows
    { 0,  9, 18, 27, 36, 45, 54, 63, 72},
    { 1, 10, 19, 28, 37, 46, 55, 64, 73},
    { 2, 11, 20, 29, 38, 47, 56, 65, 74},
    { 3, 12, 21, 30, 39, 48, 57, 66, 75},
    { 4, 13, 22, 31, 40, 49, 58, 67, 76},
    { 5, 14, 23, 32, 41, 50, 59, 68, 77},
    { 6, 15, 24, 33, 42, 51, 60, 69, 78},
    { 7, 16, 25, 34, 43, 52, 61, 70, 79},
    { 8, 17, 26, 35, 44, 53, 62, 71, 80},
    // boxes
    { 0,  1,  2,  9, 10, 11, 18, 19, 20},
    { 3,  4,  5, 12, 13, 14, 21, 22, 23},
    { 6,  7,  8, 15, 16, 17, 24, 25, 26},
    {27, 28, 29, 36, 37, 38, 45, 46, 47},
    {30, 31, 32, 39, 40, 41, 48, 49, 50},
    {33, 34, 35, 42, 43, 44, 51, 52, 53},
    {54, 55, 56, 63, 64, 65, 72, 73, 74},
    {57, 58, 59, 66, 67, 68, 75, 76, 77},
    {60, 61, 62, 69, 70, 71, 78, 79, 80}
};



SudokuSolver::SudokuSolver()
{

}


SudokuSolver::SudokuSolverResult SudokuSolver::solve(int *board)
{
    SudokuSolverResult ret;

    // prepare candidates table
    int *candidates = new int[9*9];

    for( int i = 0; i < 9 * 9; i++)
        candidates[i] = 0x1ff; // 111111111


    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(board[9 * j + i] != 0)
            {
                candidates[9 * j + i] = (0x1 << (board[9 * j + i]-1));
                removeCandidate(board[9 * j + i], std::make_pair(i, j), candidates);
            }
        }

    }


    // first step: simple approach
    ret = solve_simple(board, candidates);
    if(ret.result == ResultType::FOUND || ret.result == ResultType::CONTRADICT)
    {
        //printf("found: (%d %d) is %d\n", result->x, result->y, result->num);
        delete [] candidates;
        return ret;
    }


    // second step: heuristic approach
    ret = solve_heuristic(board, candidates);
    if(ret.result == ResultType::FOUND || ret.result == ResultType::CONTRADICT)
    {
        //printf("found: (%d %d) is %d\n", result->x, result->y, result->num);
        delete [] candidates;
        return ret;
    }

    // third step: negation
    ret = solve_negation(board, candidates);
    if(ret.result == ResultType::FOUND || ret.result == ResultType::CONTRADICT)
    {
        //printf("found: (%d %d) is %d\n", result->x, result->y, result->num);
        delete [] candidates;
        return ret;
    }


    if(ret.result == ResultType::NOT_FOUND)
    {
        //printf("result not found\n");
        dump(candidates);
        delete [] candidates;
        return ret;
    }

    delete [] candidates;

    return ret;
}

void SudokuSolver::dump(int *candidates)
{
    for(int i = 0; i < 9; i ++)
    {
        for(int j = 0; j < 9; j++)
        {
            printf("%03x ", candidates[9*i + j]);

            if(j % 3 == 2) printf(" ");
        }
        printf("\n");
        if(i % 3 ==2) printf("\n");
    }
}



void SudokuSolver::removeCandidate(int num, std::pair<int, int> p, int *candidates)
{
    std::vector<std::pair<int, int>> relations;

    // column
    for(int i = 0; i < 9; i++)
        relations.push_back(std::make_pair(p.first, i));
    // row
    for(int i = 0; i < 9; i++)
        relations.push_back(std::make_pair(i, p.second));
    // box
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            relations.push_back(std::make_pair(p.first / 3 * 3 + i, p.second / 3 * 3 + j));

    // remove candidate
    for(auto vp : relations)
    {
        if(vp != p)
        {
            //printf("remove candidates %d from %x ", 9 * vp.second + vp.first, candidates[9 * vp.second + vp.first]);
            candidates[9 * vp.second + vp.first] &= ~(0x1 << (num-1));
            //printf("to %x\n", candidates[9 * vp.second + vp.first]);
        }
    }
}


SudokuSolver::SudokuSolverResult SudokuSolver::solve_heuristic(const int *board, int *candidates)
{

    // bit array is like this
//    int combi[9] =
//    {
//        0x1fe, // 111111110
//        0x1fd, // 111111101
//        0x1fb, // 111111011
//        0x1f7, // 111110111
//        0x1ef, // 111101111
//        0x1df, // 111011111
//        0x1bf, // 110111111
//        0x17f, // 101111111
//        0x0ff  // 011111111
//    };

    int *combi = nullptr;

    for(int n = 8; n > 0; n--)
    {
        // create bit array of 9Cn
        createCombinationArray(&combi, 9, n);

        for(int i = 0; i < 9; i++)
        {
            if(!solve_heuristic_iter(candidates, n, combi))
            {
                // contradict found
                delete [] combi;
                SudokuSolver::SudokuSolverResult ret;
                ret.result = ResultType::CONTRADICT;
                ret.x = ret.y = ret.num = -1;
                return ret;
            }
        }
        delete [] combi;
    }

    return solve_simple(board, candidates);
}


bool SudokuSolver::solve_heuristic_iter(int *candidates, int combiLength, int *combi)
{
    for(auto tuple : SudokuTuples)
    {
        int cover = 0;
        for(int i = 0; i < 9; i++)
        {
            if(combi[i] & (0x1 << i))
                cover |= candidates[tuple[i]];
        }

        // count num of 1
        int bits = 0;
        int mask = 0;
        for(int k = 0; k < 9; k++)
        {
            // count bit and create mask
            if(cover & (0x1 << k))
            {
                bits++;
                mask |= 0x1 << k;
            }
        }

        if(bits == combiLength)
        {
            // apply mask to remaining elements
            for(int i = 0; i < 9; i++)
            {
                if(!(combi[i] & (0x1 << i)))
                    candidates[tuple[i]] &= ~mask;
            }
            //printf("candidates changed to %x\n", candidates[tuple[i]]);
        }
        else if(bits < combiLength)
        {
            // contradict
            return false;

        }
    }
    return true;
}



SudokuSolver::SudokuSolverResult SudokuSolver::solve_simple(const int *board, int *candidates)
{
    SudokuSolverResult ret;

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            //printf("search fixed board(%d %d) candidates %x\n", i, j, candidates[9*j+i]);
            if(board[9 * j + i] == 0)
            {
                // no candidate - contradict
                if(candidates[9 * j + i] == 0)
                {
                    ret.result = ResultType::CONTRADICT;
                    ret.x = ret.y = ret.num = -1;
                    return ret;
                }

                for(int k = 0; k < 9; k++)
                {
                    //printf(" compare %x %x\n", candidates[9 * j + i], (1 << k));
                    if(candidates[9 * j + i] == (1 << k))
                    {
                        // found
                        ret.result = ResultType::FOUND;
                        ret.x = i;
                        ret.y = j;
                        ret.num = k+1;
                        return ret;
                    }

                }
            }
        }
    }

    ret.result = ResultType::NOT_FOUND;
    ret.x = -1;
    ret.y = -1;
    ret.num = -1;

    return ret;
}

int SudokuSolver::createCombinationArray(int **array, unsigned int n, unsigned int r)
{
    int combination = 1;
    for(unsigned int i = 0; i < r; i++)
    {
        combination *= n-i;
    }
    for(unsigned int i = 0; i < r; i++)
    {
        combination = combination / (i+1);
    }

    *array = new int[combination];
    int iter = 0;

    unsigned int count;
    for(unsigned int i = 0; i < std::pow(2, n); i++)
    {
        count = 0;
        unsigned int mask = 0x1;
        for(unsigned int j = 0; j < n; j++)
        {
            if((i&mask) != 0)
                count++;

            mask = mask << 1;
        }

        if(count == r)
        {
            (*array)[iter] = i;
            iter++;
        }
    }

    return combination;
}

int SudokuSolver::countBit(unsigned int c, unsigned int digit)
{
    int count = 0;

    for(unsigned int i = 0; i < digit; i++)
    {
        if((c & (0x01 << i)) != 0)
            count++;
    }
    return count;
}


SudokuSolver::SudokuSolverResult SudokuSolver::solve_negation(const int *board, int *candidates)
{
    SudokuSolverResult ret;

    // keep candidates
    int *work_board = new int[9*9];
    int *work_candidates = new int[9*9];

    std::vector<int> assumption;


    // search item with n candidates
    for(unsigned int n = 2; n <= 9; n++)
    {
        for(int i = 0; i < 9*9; i++)
        {
            assumption.clear();
            for(unsigned int j = 0; j < 9; j++)
            {
                if((candidates[i] & (0x1 << j)) != 0)
                {
                    assumption.push_back(j);
                }
            }

            if(assumption.size() == n)
            {
                for(unsigned int j = 0; j < n; j++)
                {
                    //printf("try to negation...[%d %d] as %d\n", i%9, i/9, assumption[j]+1);

                    memcpy(work_board, board, sizeof(int) * 9 * 9);
                    memcpy(work_candidates, candidates, sizeof(int) * 9 * 9);
                    unsigned int mask = (0x1 << assumption[j]);
                    work_candidates[i] &= ~mask;
                    while(true)
                    {
                        ret = solve_simple(work_board, work_candidates);
                        if(ret.result == ResultType::FOUND)
                        {
                            work_board[9*ret.y+ret.x] = ret.num;
                            removeCandidate(ret.num, std::make_pair(ret.x, ret.y), work_candidates);
                            continue;
                        }
                        else if(ret.result == ResultType::CONTRADICT)
                        {
                            break;
                        }

                        ret = solve_heuristic(work_board, work_candidates);
                        if(ret.result == ResultType::FOUND)
                        {
                            work_board[9*ret.y+ret.x] = ret.num;
                            removeCandidate(ret.num, std::make_pair(ret.x, ret.y), work_candidates);
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }

                    if(ret.result == ResultType::CONTRADICT)
                    {
                        //printf("contradict found. [%d %d] can be guessed to %d\n", i%9, i/9, assumption[j]+1);
                        ret.result = ResultType::FOUND;
                        ret.x = i % 9;
                        ret.y = i / 9;
                        ret.num = assumption[j]+1;
                        delete [] work_board;
                        delete [] work_candidates;
                        return ret;
                    }
                }
            }
        }

    }

    ret.result = ResultType::NOT_FOUND;
    ret.x = -1;
    ret.y = -1;
    ret.num = -1;

    delete [] work_board;
    delete [] work_candidates;
    return ret;

}
