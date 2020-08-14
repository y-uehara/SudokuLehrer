#ifndef SUDOKUBOARD_H
#define SUDOKUBOARD_H

#include <vector>

#include <QObject>

#include "sudokusolver.h"

class SudokuBoard : public QObject
{
    Q_OBJECT

public:
    explicit SudokuBoard();
    ~SudokuBoard();

    // invokable
    Q_INVOKABLE void changeNumber(int x, int y, int num);
    Q_INVOKABLE void calcNextHand();
    Q_INVOKABLE void clearBoard();

signals:
    void numberChangeRequest(int x, int y, int num);
    void legalChangeRequest(int x, int y, bool legal);

    void stateIllegal();
    void stateSolved();
    void stateRegular();

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
