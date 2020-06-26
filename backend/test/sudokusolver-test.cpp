#include <iostream>
#include <set>
#include <vector>

#include <gtest/gtest.h>

#include "../sudokusolver.h"

const int SudokuTuples[27][9] =
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

#include "sudokusolver-testCase.cpp"

bool check_legal_tuple(std::vector<int> tuple)
{
    int count = 0;
    std::set<int> numSet;

    //printf("check tuple [%d %d %d %d %d %d %d %d %d]\n",
    //    tuple[0],tuple[1],tuple[2],tuple[3],tuple[4],tuple[5],tuple[6],tuple[7],tuple[8]);


    for(unsigned int i = 0; i < tuple.size(); i++)
    {
        if(tuple[i] != 0)
        {
            count++;
            numSet.insert(tuple[i]);
        }
    }
    
    return (numSet.size() == count);
}

bool check_legal_board(int *board)
{
    std::vector<int> tuple;
    for(int i = 0; i < 27; i++)
    {
        tuple.clear();
        for(int j = 0; j < 9; j++)
            tuple.push_back(board[SudokuTuples[i][j]]);
        
        if(!check_legal_tuple(tuple))
            return false;
    }
    return true;
}


int count_fixed(int *board)
{
    int fixed = 0;
    for(int i = 0; i < 81; i++)
    {
        if(board[i] != 0)
            fixed++;
    }
    return fixed;
}

void dump(int *board)
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            std::cout << board[9*i + j] << " ";
            if(j % 3 == 2)
                std::cout << " ";
        }
        std::cout << std::endl;
        if(i % 3 == 2)
            std::cout << std::endl;
    }
}



class QuizList : public ::testing::TestWithParam<int> { };

TEST_P(QuizList, ResultTest)
{
    int i = GetParam();

    auto solver = new SudokuSolver();
    SudokuSolver::SudokuSolverResult result;
    int count_before, count_after;

    while(81 > count_fixed(test_case[i]))
    {
        //printf("count fixed:%d\n",count_fixed(test_case[i]));
        result = solver->solve(test_case[i]);
        //printf("ret:%s, x:%d y:%d num:%d\n", (result.result == SudokuSolver::ResultType::FOUND ? "FOUND" : (result.result == SudokuSolver::ResultType::NOT_FOUND ? "NOT FOUND" : "CONTRADICT")), result.x, result.y, result.num);
        // answer not found
        if(result.result != SudokuSolver::ResultType::FOUND)
        {
            // show final board status
            dump(test_case[i]);
        }

        ASSERT_TRUE(result.result == SudokuSolver::ResultType::FOUND);

        // apply change
        count_before = count_fixed(test_case[i]);
        //printf("counter before:%d\n", count_before);
        test_case[i][9*result.y + result.x] = result.num;
        count_after = count_fixed(test_case[i]);
        //printf("counter after:%d\n", count_after);

        // increase fixed number
        ASSERT_GT(count_after, count_before);

        // answer is legal
        //printf("legal check\n");
        ASSERT_TRUE(check_legal_board(test_case[i]));
        //printf("answer is legal\n");
    }
}

#define NUM_OF_TEST_CASE (static_cast<int>((sizeof(test_case) / sizeof(test_case[0]))))
INSTANTIATE_TEST_CASE_P(SudokuSolverTest, QuizList, ::testing::Range(0, NUM_OF_TEST_CASE));
