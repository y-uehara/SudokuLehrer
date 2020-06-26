#include <set>

#include "sudokuboard.h"

SudokuBoard::SudokuBoard()
{
    m_board = new int[9 * 9];

    for(int i = 0; i < 9 * 9; i++)
    {
        m_board[i] = 0;
    }

    m_solver = new SudokuSolver();
}


SudokuBoard::~SudokuBoard()
{
    delete [] m_board;

    delete m_solver;
}

void SudokuBoard::checkBoardStatus()
{
    int fixed = 0;
    bool illegal = false;

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(getNumber(i, j) != 0)
                fixed++;

            bool p_is_legal = isLegal(i, j);
            if(!p_is_legal)
                illegal = true;

            legalChangeRequest(i, j, p_is_legal);
        }
    }

    printf("check board... %d fixed\n", fixed);

    if(illegal)
        stateIllegal();
    else if(fixed == 9 * 9)
        stateSolved();
    else
        stateRegular();
}



void SudokuBoard::clearBoard()
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            m_board[9 * j + i] = 0;
            numberChangeRequest(i, j, 0);
            legalChangeRequest(i, j, true);
        }
    }
    stateRegular();
}


void SudokuBoard::calcNextHand()
{
    auto result = m_solver->solve(m_board);

    if(result.result == SudokuSolver::ResultType::FOUND)
    {
        changeNumber(result.x, result.y, result.num);
    }

    return;
}



void SudokuBoard::changeNumber(int x, int y, int num)
{
    printf("set [%d %d]\n", x, y);
    setNumber(num, x, y);
    numberChangeRequest(x, y, num);

    checkBoardStatus();
}

bool SudokuBoard::isLegal(int x, int y)
{
    // check column
    std::vector<int> columnList;
    for(int i = 0; i < 9; i++)
        columnList.push_back(getNumber(x, i));
    bool columnLegal = isLegal(columnList);

    // check row
    std::vector<int> rowList;
    for(int i = 0; i < 9; i++)
        rowList.push_back(getNumber(i, y));
    bool rowLegal = isLegal(rowList);

    // check box
    std::vector<int> boxList;
    int box_x = x / 3;
    int box_y = y / 3;

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            int num = getNumber(box_x * 3 + i, box_y * 3 + j);
            boxList.push_back(num);
        }
    }
    bool boxLegal = isLegal(boxList);

    return columnLegal && rowLegal && boxLegal;
}


bool SudokuBoard::isLegal(std::vector<int>numList)
{
    unsigned int count = 0;
    std::set<int> numSet;

    for(unsigned int i = 0; i < numList.size(); i++)
    {
        if(numList[i] != 0)
        {
            count++;
            numSet.insert(numList[i]);
        }
    }
    return (numSet.size() == count);
}

