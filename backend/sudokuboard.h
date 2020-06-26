#ifndef SUDOKUBOARD_H
#define SUDOKUBOARD_H

#include <vector>

#include <qul/object.h>
#include <qul/singleton.h>
#include <qul/property.h>
#include <qul/signal.h>

#include "sudokusolver.h"

class SudokuBoard : public Qul::Singleton<SudokuBoard>
{
public:
    explicit SudokuBoard();
    ~SudokuBoard();

    // invokable
    void changeNumber(int x, int y, int num);
    void calcNextHand();
    void clearBoard();

    Qul::Signal<void(int x, int y, int num)> numberChangeRequest;
    Qul::Signal<void(int x, int y, bool legal)> legalChangeRequest;

    Qul::Signal<void()> stateIllegal;
    Qul::Signal<void()> stateSolved;
    Qul::Signal<void()> stateRegular;

private:
    SudokuSolver *m_solver;

    int *m_board;

    int getNumber(int x, int y) { return m_board[9 * y + x]; }
    void setNumber(int num, int x, int y) { m_board[9 * y + x] = num; }


    void checkBoardStatus();


    bool isLegal(int x, int y);
    bool isLegal(std::vector<int>numList);
};

#endif // SUDOKUBOARD_H
